#include "../system/dx12_manager.h"
#include "../system/window.h"
#include "graphics_manager.h"
#include "object/pipeline_state.h"
#include "object/shader.h"
#include "object/texture.h"


using namespace Microsoft::WRL;

namespace eng {

	bool GraphicsManager::initialize() {

		sys::Dx12Manager& mgr = sys::Dx12Manager::getInstance();

		/* コマンドキューの作成 */ {

			// 描画時にはコマンドキューに蓄えられたコマンドリストが実行される
			D3D12_COMMAND_QUEUE_DESC queue_desc = {};
			queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;	// ゲーム開発ならここのフラグは NONE でよいらしい
			queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;	// 他のフラグとしてはコンピュートシェーダ専用のフラグもある
			if (FAILED(mgr.device_->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&command_queue_)))) return false;
		}

		/* スワップチェインの作成 */ {

			DXGI_SWAP_CHAIN_DESC1	swap_chain_desc = {};
			swap_chain_desc.BufferCount = FRAME_COUNT;
			swap_chain_desc.Width = sys::Window::WIDTH;
			swap_chain_desc.Height = sys::Window::HEIGHT;
			swap_chain_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swap_chain_desc.SampleDesc.Count = 1;

			ComPtr<IDXGISwapChain1>	swap_chain;
			if (FAILED(mgr.factory_->CreateSwapChainForHwnd(command_queue_.Get(), sys::Window::handle_, &swap_chain_desc, nullptr, nullptr, &swap_chain))) return false;

			// フルスクリーンのサポートなし
			if (FAILED(mgr.factory_->MakeWindowAssociation(sys::Window::handle_, DXGI_MWA_NO_ALT_ENTER))) return false;

			if (FAILED(swap_chain.As(&swap_chain_))) return false;
			frame_index_ = swap_chain_->GetCurrentBackBufferIndex();
		}

		/* レンダーターゲットビューの作成 */ {

			// DirectX12では様々なバッファを DescriptorHeap として扱う 
			// ( dx11 でのRenderTargetView や ShaderResourceView 等もこれになる )

			// DescriptorHeap が何のバッファなのかを表す型
			D3D12_DESCRIPTOR_HEAP_DESC	rtv_heap_desc = {};

			// フレームバッファとバックバッファの２つ
			rtv_heap_desc.NumDescriptors = FRAME_COUNT;

			// レンダーターゲットビュー
			rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

			// シェーダの結果をリソースとして使用する場合は変更する？
			rtv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

			if (FAILED(mgr.device_->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&rtv_heap_)))) return false;
			UINT rtv_descriptor_size = mgr.device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

			// フレームバッファとバックバッファのレンダーターゲットビューを作成
			for (UINT i = 0; i < FRAME_COUNT; i++) {
				rtv_handle_[i] = rtv_heap_->GetCPUDescriptorHandleForHeapStart();
				rtv_handle_[i].ptr += rtv_descriptor_size * i;
				if (FAILED(swap_chain_->GetBuffer(i, IID_PPV_ARGS(&rtv_buffer_[i])))) return false;
				mgr.device_->CreateRenderTargetView(rtv_buffer_[i].Get(), nullptr, rtv_handle_[i]);
			}
		}

		/* 深度ステンシルビューの作成 */ {

			//深度バッファ用のデスクリプタヒープの作成
			D3D12_DESCRIPTOR_HEAP_DESC dsv_heap_desc = {};
			dsv_heap_desc.NumDescriptors = 1;
			dsv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			dsv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			dsv_heap_desc.NodeMask = 0;
			if (FAILED(mgr.device_->CreateDescriptorHeap(&dsv_heap_desc, IID_PPV_ARGS(&dsv_heap_)))) return false;

			//深度バッファの作成
			D3D12_HEAP_PROPERTIES heap_properties = {};
			D3D12_RESOURCE_DESC resource_desc = {};
			D3D12_CLEAR_VALUE clear_value = {};

			heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;
			heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			heap_properties.CreationNodeMask = 0;
			heap_properties.VisibleNodeMask = 0;

			resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			resource_desc.Width = sys::Window::WIDTH;
			resource_desc.Height = sys::Window::HEIGHT;
			resource_desc.DepthOrArraySize = 1;
			resource_desc.MipLevels = 0;
			resource_desc.Format = DXGI_FORMAT_R32_TYPELESS;
			resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			resource_desc.SampleDesc.Count = 1;
			resource_desc.SampleDesc.Quality = 0;
			resource_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

			clear_value.Format = DXGI_FORMAT_D32_FLOAT;
			clear_value.DepthStencil.Depth = 1.0f;
			clear_value.DepthStencil.Stencil = 0;

			if (FAILED(mgr.device_->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clear_value, IID_PPV_ARGS(&dsv_buffer_)))) return false;

			//深度バッファのビューの作成
			D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc{};

			dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			dsv_desc.Format = DXGI_FORMAT_D32_FLOAT;
			dsv_desc.Texture2D.MipSlice = 0;
			dsv_desc.Flags = D3D12_DSV_FLAG_NONE;

			dsv_handle_ = dsv_heap_->GetCPUDescriptorHandleForHeapStart();

			mgr.device_->CreateDepthStencilView(dsv_buffer_.Get(), &dsv_desc, dsv_handle_);
		}

		/* コマンドアロケーターの作成 */ {

			// コマンドアロケータはコマンドキューにコマンドリストの内容を積む為のコンテナのような役割をする
			// 引数の D3D12_COMMAND_LIST_TYPE_DIRECT はコマンドキューと合わせる必要がある
			if (FAILED(mgr.device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator_)))) return false;
		}

		/* ルートシグネチャとシェーダーの作成 */ {

			// ルートシグネチャは シェーダで使用される定数バッファ・テクスチャ・サンプラなどが
			// シェーダ内の何処にどんな形式で存在するか定義するようなもの
			// シェーダレジスタの b0 はこれで t0 はこれでといった細かい設定が可能なようだ
			eng::Shader::CreateDesc shader_desc;
			shader_desc.vertex_func_name_ = "VSMain";
			shader_desc.vertex_target_ = "vs_5_0";
			shader_desc.pixel_func_name_ = "PSMain";
			shader_desc.pixel_target_ = "ps_5_0";
			shader_desc.input_element_desc_ = new D3D12_INPUT_ELEMENT_DESC[]{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
				{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
			};
			shader_desc.input_element_size_ = 3;
			shader_desc.create_desc_func_ = [](eng::Shader::s_ptr& s) {

				D3D12_ROOT_SIGNATURE_DESC root_signature_desc = {};
				D3D12_DESCRIPTOR_RANGE range[1] = {};
				D3D12_ROOT_PARAMETER root_parameters[2] = {};
				D3D12_STATIC_SAMPLER_DESC sampler_desc[1] = {};

				//変換行列用の定数バッファ	
				root_parameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
				root_parameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
				root_parameters[0].Descriptor.ShaderRegister = 0;
				root_parameters[0].Descriptor.RegisterSpace = 0;

				//ライト用の定数バッファ
				//root_parameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
				//root_parameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
				//root_parameters[1].Descriptor.ShaderRegister = 1;
				//root_parameters[1].Descriptor.RegisterSpace = 0;

				//テクスチャ
				range[0].NumDescriptors = 1;
				range[0].BaseShaderRegister = 0;
				range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
				range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

				root_parameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
				root_parameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
				root_parameters[1].DescriptorTable.NumDescriptorRanges = 1;
				root_parameters[1].DescriptorTable.pDescriptorRanges = &range[0];

				//サンプラ
				sampler_desc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
				sampler_desc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				sampler_desc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				sampler_desc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				sampler_desc[0].MipLODBias = 0.0f;
				sampler_desc[0].MaxAnisotropy = 16;
				sampler_desc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
				sampler_desc[0].BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
				sampler_desc[0].MinLOD = 0.0f;
				sampler_desc[0].MaxLOD = D3D12_FLOAT32_MAX;
				sampler_desc[0].ShaderRegister = 0;
				sampler_desc[0].RegisterSpace = 0;
				sampler_desc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

				root_signature_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
				root_signature_desc.NumParameters = _countof(root_parameters);
				root_signature_desc.pParameters = root_parameters;
				root_signature_desc.NumStaticSamplers = _countof(sampler_desc);
				root_signature_desc.pStaticSamplers = sampler_desc;

				return std::move(root_signature_desc);
			};
			shader_desc.setting_func_ = [](ID3D12Resource* cbv, lib::Matrix4x4* world, lib::Matrix4x4* view_projection) {

				//全ての変換行列
				lib::Matrix4x4 mvp = lib::Matrix4x4::createTranspose(*world * *view_projection);

				// ワールド行列をシェーダの定数バッファにセット
				lib::Matrix4x4* buffer{};
				if (FAILED(cbv->Map(0, nullptr, (void**)&buffer))) return;

				//定数バッファをシェーダのレジスタにセット
				GraphicsManager::getInstance().command_list_->SetGraphicsRootConstantBufferView(0, cbv->GetGPUVirtualAddress());

				// 行列を定数バッファに書き込み
				buffer[0] = mvp;
				buffer[1] = *world;

				cbv->Unmap(0, nullptr);
			};

			default_shader_ = eng::Shader::create(shader_desc);
			if (!default_shader_) return false;
		}

		/* パイプラインステートオブジェクト(PSO)の作成 */ {

			// dx11 では個別にイミディエイトコンテキスト( dx12 では廃止 ) を使って設定していたパイプラインステートが dx12 では１つにまとめられた
			// 各種シェーダ・ラスタライザ・ブレンドステート・デプスステンシルステート等
			// そのためこれらの設定を変えた描画を行いたい場合は PSO を複数個作りどれを使って描画するか管理する必要がある
			eng::PipelineState::CreateDesc pso_desc;
			pso_desc.shader_ = default_shader_;
			pso_desc.create_desc_func_ = [pso_desc](eng::PipelineState::s_ptr& p) {

				D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline_state_desc = {};

				//シェーダーの設定
				pipeline_state_desc.VS.pShaderBytecode = pso_desc.shader_->vertex_shader_->GetBufferPointer();
				pipeline_state_desc.VS.BytecodeLength = pso_desc.shader_->vertex_shader_->GetBufferSize();
				pipeline_state_desc.PS.pShaderBytecode = pso_desc.shader_->pixel_shader_->GetBufferPointer();
				pipeline_state_desc.PS.BytecodeLength = pso_desc.shader_->pixel_shader_->GetBufferSize();

				//インプットレイアウトの設定
				pipeline_state_desc.InputLayout.pInputElementDescs = pso_desc.shader_->input_element_desc_;
				pipeline_state_desc.InputLayout.NumElements = pso_desc.shader_->input_element_size_;

				//サンプル系の設定
				pipeline_state_desc.SampleDesc.Count = 1;
				pipeline_state_desc.SampleDesc.Quality = 0;
				pipeline_state_desc.SampleMask = UINT_MAX;

				//レンダーターゲットの設定
				pipeline_state_desc.NumRenderTargets = 1;
				pipeline_state_desc.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;

				//三角形に設定
				pipeline_state_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

				//ルートシグネチャ
				pipeline_state_desc.pRootSignature = pso_desc.shader_->root_signature_.Get();

				//ラスタライザステートの設定
				pipeline_state_desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
				//pipeline_state_desc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
				//pipeline_state_desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
				//pipeline_state_desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
				pipeline_state_desc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
				pipeline_state_desc.RasterizerState.FrontCounterClockwise = FALSE;
				pipeline_state_desc.RasterizerState.DepthBias = 0;
				pipeline_state_desc.RasterizerState.DepthBiasClamp = 0;
				pipeline_state_desc.RasterizerState.SlopeScaledDepthBias = 0;
				pipeline_state_desc.RasterizerState.DepthClipEnable = TRUE;
				pipeline_state_desc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
				pipeline_state_desc.RasterizerState.AntialiasedLineEnable = FALSE;
				pipeline_state_desc.RasterizerState.MultisampleEnable = FALSE;

				//ブレンドステートの設定
				for (int i = 0; i < _countof(pipeline_state_desc.BlendState.RenderTarget); ++i) {
					pipeline_state_desc.BlendState.RenderTarget[i].BlendEnable = FALSE;
					pipeline_state_desc.BlendState.RenderTarget[i].SrcBlend = D3D12_BLEND_ONE;
					pipeline_state_desc.BlendState.RenderTarget[i].DestBlend = D3D12_BLEND_ZERO;
					pipeline_state_desc.BlendState.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
					pipeline_state_desc.BlendState.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_ONE;
					pipeline_state_desc.BlendState.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ZERO;
					pipeline_state_desc.BlendState.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
					pipeline_state_desc.BlendState.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
					pipeline_state_desc.BlendState.RenderTarget[i].LogicOpEnable = FALSE;
					pipeline_state_desc.BlendState.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_CLEAR;
				}
				pipeline_state_desc.BlendState.AlphaToCoverageEnable = FALSE;
				pipeline_state_desc.BlendState.IndependentBlendEnable = FALSE;

				//デプスステンシルステートの設定
				pipeline_state_desc.DepthStencilState.DepthEnable = TRUE;								//深度テストあり
				pipeline_state_desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
				pipeline_state_desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
				pipeline_state_desc.DepthStencilState.StencilEnable = FALSE;							//ステンシルテストなし
				pipeline_state_desc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
				pipeline_state_desc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

				pipeline_state_desc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
				pipeline_state_desc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
				pipeline_state_desc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
				pipeline_state_desc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

				pipeline_state_desc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
				pipeline_state_desc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
				pipeline_state_desc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
				pipeline_state_desc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

				pipeline_state_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

				return std::move(pipeline_state_desc);
			};

			default_pso_ = eng::PipelineState::create(pso_desc);
			if (!default_pso_) return false;
		}

		/* デフォルトテクスチャの作成 */ {
			default_texture_ = Texture::loadFromFile("engine/graphics/default_texture.bmp");
			if (!default_texture_) return false;
		}

		/* コマンドリストの作成 */ {

			if (FAILED(mgr.device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_allocator_.Get(), default_pso_->pso_.Get(), IID_PPV_ARGS(&command_list_)))) return false;
			if (FAILED(command_list_->Close())) return false;
		}

		// リソースがGPUにアップロードされるまで待機
		return waitForPreviousFrame();
	}

	void GraphicsManager::finalize() {
		waitForPreviousFrame();
	}

	bool GraphicsManager::waitForPreviousFrame() {

		sys::Dx12Manager& mgr = sys::Dx12Manager::getInstance();

		const UINT64 fence = mgr.fence_value_;
		if (FAILED(command_queue_->Signal(mgr.fence_.Get(), fence))) return false;
		mgr.fence_value_++;

		// 前のフレームが終了するまで待機
		if (mgr.fence_->GetCompletedValue() < fence) {
			if (FAILED(mgr.fence_->SetEventOnCompletion(fence, mgr.fence_event_))) return false;
			WaitForSingleObject(mgr.fence_event_, INFINITE);
		}

		frame_index_ = swap_chain_->GetCurrentBackBufferIndex();

		return true;
	}

}
