#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include "../../system/system.h"
#include "../../library/library.h"


namespace eng {

	class DescriptorManager;
	class RenderTargetView;
	class DepthStencilView;
	class PipelineState;
	class Shader;
	class Texture;

	class GraphicsManager final : public lib::Singleton<GraphicsManager> {
		// 基底クラスで生成するため
		friend class lib::Singleton<GraphicsManager>;
		// initialize, finalizeを呼ぶため
		friend class sys::System;
	private:
		GraphicsManager() {}

		//====================================================================================================
		// static定数

		inline static constexpr UINT FRAME_COUNT = 2;				// フレーム数
		inline static constexpr UINT DESCRIPTOR_RTV_NUM = 100;		// レンダーターゲットビュー
		inline static constexpr UINT DESCRIPTOR_DSV_NUM = 100;		// 深度ステンシルビュー
		inline static constexpr UINT DESCRIPTOR_SRV_NUM = 2048;		// シェーダーリソースビュー

		//====================================================================================================
		// メンバ関数

		// 初期化
		bool initialize();

		// 終了処理
		void finalize();

		// 描画完了の同期を取る
		bool waitForPreviousFrame();

		//====================================================================================================
	public:
		~GraphicsManager() {}
		
		//====================================================================================================
		// メンバ変数

		UINT frame_index_ = 0;												// 現在の描画フレーム
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> command_queue_;			// コマンドキュー
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> command_allocator_;	// コマンドアロケーター
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> command_list_;	// コマンドリスト
		Microsoft::WRL::ComPtr<IDXGISwapChain3> swap_chain_;				// スワップチェイン

		std::shared_ptr<PipelineState> default_pso_ = nullptr;	// デフォルトのPSO
		std::shared_ptr<Shader> default_shader_ = nullptr;		// デフォルトのシェーダ
		std::shared_ptr<Texture> default_texture_ = nullptr;	// デフォルトのテクスチャ

		//--------------------------------------------------
		// Layerに統合する？
		
		std::shared_ptr<RenderTargetView> rtv_[FRAME_COUNT] = {};	// レンダーターゲットビュー
		std::shared_ptr<DepthStencilView> dsv_ = nullptr;			// 深度ステンシルビュー

		//--------------------------------------------------

		std::shared_ptr<DescriptorManager> rtv_heap_ = nullptr;	// レンダーターゲットビューのディスクリプタヒープ
		std::shared_ptr<DescriptorManager> dsv_heap_ = nullptr;	// 深度ステンシルビューのディスクリプタヒープ
		std::shared_ptr<DescriptorManager> srv_heap_ = nullptr;	// シェーダーリソースビューのディスクリプタヒープ

		//====================================================================================================
	};

}
