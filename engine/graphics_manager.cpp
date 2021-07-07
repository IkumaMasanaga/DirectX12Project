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

		/* �R�}���h�L���[�̍쐬 */ {

			// �`�掞�ɂ̓R�}���h�L���[�ɒ~����ꂽ�R�}���h���X�g�����s�����
			D3D12_COMMAND_QUEUE_DESC queue_desc = {};
			queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;	// �Q�[���J���Ȃ炱���̃t���O�� NONE �ł悢�炵��
			queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;	// ���̃t���O�Ƃ��Ă̓R���s���[�g�V�F�[�_��p�̃t���O������
			if (FAILED(mgr.device_->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&command_queue_)))) return false;
		}

		/* �X���b�v�`�F�C���̍쐬 */ {

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

			// �t���X�N���[���̃T�|�[�g�Ȃ�
			if (FAILED(mgr.factory_->MakeWindowAssociation(sys::Window::handle_, DXGI_MWA_NO_ALT_ENTER))) return false;

			if (FAILED(swap_chain.As(&swap_chain_))) return false;
			frame_index_ = swap_chain_->GetCurrentBackBufferIndex();
		}

		/* �����_�[�^�[�Q�b�g�r���[�̍쐬 */ {

			// DirectX12�ł͗l�X�ȃo�b�t�@�� DescriptorHeap �Ƃ��Ĉ��� 
			// ( dx11 �ł�RenderTargetView �� ShaderResourceView ��������ɂȂ� )

			// DescriptorHeap �����̃o�b�t�@�Ȃ̂���\���^
			D3D12_DESCRIPTOR_HEAP_DESC	rtv_heap_desc = {};

			// �t���[���o�b�t�@�ƃo�b�N�o�b�t�@�̂Q��
			rtv_heap_desc.NumDescriptors = FRAME_COUNT;

			// �����_�[�^�[�Q�b�g�r���[
			rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

			// �V�F�[�_�̌��ʂ����\�[�X�Ƃ��Ďg�p����ꍇ�͕ύX����H
			rtv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

			if (FAILED(mgr.device_->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&rtv_heap_)))) return false;
			UINT rtv_descriptor_size = mgr.device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

			// �t���[���o�b�t�@�ƃo�b�N�o�b�t�@�̃����_�[�^�[�Q�b�g�r���[���쐬
			for (UINT i = 0; i < FRAME_COUNT; i++) {
				rtv_handle_[i] = rtv_heap_->GetCPUDescriptorHandleForHeapStart();
				rtv_handle_[i].ptr += rtv_descriptor_size * i;
				if (FAILED(swap_chain_->GetBuffer(i, IID_PPV_ARGS(&rtv_buffer_[i])))) return false;
				mgr.device_->CreateRenderTargetView(rtv_buffer_[i].Get(), nullptr, rtv_handle_[i]);
			}
		}

		/* �[�x�X�e���V���r���[�̍쐬 */ {

			//�[�x�o�b�t�@�p�̃f�X�N���v�^�q�[�v�̍쐬
			D3D12_DESCRIPTOR_HEAP_DESC dsv_heap_desc = {};
			dsv_heap_desc.NumDescriptors = 1;
			dsv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			dsv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			dsv_heap_desc.NodeMask = 0;
			if (FAILED(mgr.device_->CreateDescriptorHeap(&dsv_heap_desc, IID_PPV_ARGS(&dsv_heap_)))) return false;

			//�[�x�o�b�t�@�̍쐬
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

			//�[�x�o�b�t�@�̃r���[�̍쐬
			D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc{};

			dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			dsv_desc.Format = DXGI_FORMAT_D32_FLOAT;
			dsv_desc.Texture2D.MipSlice = 0;
			dsv_desc.Flags = D3D12_DSV_FLAG_NONE;

			dsv_handle_ = dsv_heap_->GetCPUDescriptorHandleForHeapStart();

			mgr.device_->CreateDepthStencilView(dsv_buffer_.Get(), &dsv_desc, dsv_handle_);
		}

		/* �R�}���h�A���P�[�^�[�̍쐬 */ {

			// �R�}���h�A���P�[�^�̓R�}���h�L���[�ɃR�}���h���X�g�̓��e��ςވׂ̃R���e�i�̂悤�Ȗ���������
			// ������ D3D12_COMMAND_LIST_TYPE_DIRECT �̓R�}���h�L���[�ƍ��킹��K�v������
			if (FAILED(mgr.device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator_)))) return false;
		}

		/* ���[�g�V�O�l�`���ƃV�F�[�_�[�̍쐬 */ {

			// ���[�g�V�O�l�`���� �V�F�[�_�Ŏg�p�����萔�o�b�t�@�E�e�N�X�`���E�T���v���Ȃǂ�
			// �V�F�[�_���̉����ɂǂ�Ȍ`���ő��݂��邩��`����悤�Ȃ���
			// �V�F�[�_���W�X�^�� b0 �͂���� t0 �͂���łƂ������ׂ����ݒ肪�\�Ȃ悤��
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

				//�ϊ��s��p�̒萔�o�b�t�@	
				root_parameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
				root_parameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
				root_parameters[0].Descriptor.ShaderRegister = 0;
				root_parameters[0].Descriptor.RegisterSpace = 0;

				//���C�g�p�̒萔�o�b�t�@
				//root_parameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
				//root_parameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
				//root_parameters[1].Descriptor.ShaderRegister = 1;
				//root_parameters[1].Descriptor.RegisterSpace = 0;

				//�e�N�X�`��
				range[0].NumDescriptors = 1;
				range[0].BaseShaderRegister = 0;
				range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
				range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

				root_parameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
				root_parameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
				root_parameters[1].DescriptorTable.NumDescriptorRanges = 1;
				root_parameters[1].DescriptorTable.pDescriptorRanges = &range[0];

				//�T���v��
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

				//�S�Ă̕ϊ��s��
				lib::Matrix4x4 mvp = lib::Matrix4x4::createTranspose(*world * *view_projection);

				// ���[���h�s����V�F�[�_�̒萔�o�b�t�@�ɃZ�b�g
				lib::Matrix4x4* buffer{};
				if (FAILED(cbv->Map(0, nullptr, (void**)&buffer))) return;

				//�萔�o�b�t�@���V�F�[�_�̃��W�X�^�ɃZ�b�g
				GraphicsManager::getInstance().command_list_->SetGraphicsRootConstantBufferView(0, cbv->GetGPUVirtualAddress());

				// �s���萔�o�b�t�@�ɏ�������
				buffer[0] = mvp;
				buffer[1] = *world;

				cbv->Unmap(0, nullptr);
			};

			default_shader_ = eng::Shader::create(shader_desc);
			if (!default_shader_) return false;
		}

		/* �p�C�v���C���X�e�[�g�I�u�W�F�N�g(PSO)�̍쐬 */ {

			// dx11 �ł͌ʂɃC�~�f�B�G�C�g�R���e�L�X�g( dx12 �ł͔p�~ ) ���g���Đݒ肵�Ă����p�C�v���C���X�e�[�g�� dx12 �ł͂P�ɂ܂Ƃ߂�ꂽ
			// �e��V�F�[�_�E���X�^���C�U�E�u�����h�X�e�[�g�E�f�v�X�X�e���V���X�e�[�g��
			// ���̂��߂����̐ݒ��ς����`����s�������ꍇ�� PSO �𕡐����ǂ���g���ĕ`�悷�邩�Ǘ�����K�v������
			eng::PipelineState::CreateDesc pso_desc;
			pso_desc.shader_ = default_shader_;
			pso_desc.create_desc_func_ = [pso_desc](eng::PipelineState::s_ptr& p) {

				D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline_state_desc = {};

				//�V�F�[�_�[�̐ݒ�
				pipeline_state_desc.VS.pShaderBytecode = pso_desc.shader_->vertex_shader_->GetBufferPointer();
				pipeline_state_desc.VS.BytecodeLength = pso_desc.shader_->vertex_shader_->GetBufferSize();
				pipeline_state_desc.PS.pShaderBytecode = pso_desc.shader_->pixel_shader_->GetBufferPointer();
				pipeline_state_desc.PS.BytecodeLength = pso_desc.shader_->pixel_shader_->GetBufferSize();

				//�C���v�b�g���C�A�E�g�̐ݒ�
				pipeline_state_desc.InputLayout.pInputElementDescs = pso_desc.shader_->input_element_desc_;
				pipeline_state_desc.InputLayout.NumElements = pso_desc.shader_->input_element_size_;

				//�T���v���n�̐ݒ�
				pipeline_state_desc.SampleDesc.Count = 1;
				pipeline_state_desc.SampleDesc.Quality = 0;
				pipeline_state_desc.SampleMask = UINT_MAX;

				//�����_�[�^�[�Q�b�g�̐ݒ�
				pipeline_state_desc.NumRenderTargets = 1;
				pipeline_state_desc.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;

				//�O�p�`�ɐݒ�
				pipeline_state_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

				//���[�g�V�O�l�`��
				pipeline_state_desc.pRootSignature = pso_desc.shader_->root_signature_.Get();

				//���X�^���C�U�X�e�[�g�̐ݒ�
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

				//�u�����h�X�e�[�g�̐ݒ�
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

				//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
				pipeline_state_desc.DepthStencilState.DepthEnable = TRUE;								//�[�x�e�X�g����
				pipeline_state_desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
				pipeline_state_desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
				pipeline_state_desc.DepthStencilState.StencilEnable = FALSE;							//�X�e���V���e�X�g�Ȃ�
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

		/* �f�t�H���g�e�N�X�`���̍쐬 */ {
			default_texture_ = Texture::loadFromFile("engine/graphics/default_texture.bmp");
			if (!default_texture_) return false;
		}

		/* �R�}���h���X�g�̍쐬 */ {

			if (FAILED(mgr.device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_allocator_.Get(), default_pso_->pso_.Get(), IID_PPV_ARGS(&command_list_)))) return false;
			if (FAILED(command_list_->Close())) return false;
		}

		// ���\�[�X��GPU�ɃA�b�v���[�h�����܂őҋ@
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

		// �O�̃t���[�����I������܂őҋ@
		if (mgr.fence_->GetCompletedValue() < fence) {
			if (FAILED(mgr.fence_->SetEventOnCompletion(fence, mgr.fence_event_))) return false;
			WaitForSingleObject(mgr.fence_event_, INFINITE);
		}

		frame_index_ = swap_chain_->GetCurrentBackBufferIndex();

		return true;
	}

}
