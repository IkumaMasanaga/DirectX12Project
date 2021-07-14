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
		// resetCommandList, renderBefore, renderAfter, executeCommandListを呼ぶため
		friend class Scene;
	private:
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
	private:
		GraphicsManager() {}

		//====================================================================================================
		// static定数

		inline static constexpr UINT FRAME_COUNT = 2;				// フレーム数
		inline static constexpr UINT DESCRIPTOR_RTV_NUM = 100;		// レンダーターゲットビュー
		inline static constexpr UINT DESCRIPTOR_DSV_NUM = 100;		// 深度ステンシルビュー
		inline static constexpr UINT DESCRIPTOR_SRV_NUM = 2048;		// シェーダーリソースビュー

		//====================================================================================================
		// メンバ変数

		UINT frame_index_ = 0;								// 現在の描画フレーム
		ComPtr<ID3D12CommandQueue> command_queue_;			// コマンドキュー
		ComPtr<ID3D12CommandAllocator> command_allocator_;	// コマンドアロケーター
		ComPtr<ID3D12GraphicsCommandList> command_list_;	// コマンドリスト
		ComPtr<IDXGISwapChain3> swap_chain_;				// スワップチェイン

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
		// メンバ関数

		// 初期化
		bool initialize();

		// 終了処理
		void finalize();

		// 描画完了の同期を取る
		bool waitForPreviousFrame();

		// コマンドリストとコマンドアロケーターをリセット
		bool resetCommandList();

		// バリアを貼る
		void setResourceBarrier(ID3D12Resource* rtv, const D3D12_RESOURCE_STATES state_before, const D3D12_RESOURCE_STATES state_after);

		// 描画前の処理
		void renderBefore(const D3D12_VIEWPORT& viewport, const D3D12_RECT& scissor_rect, const std::shared_ptr<RenderTargetView>& rtv, const std::shared_ptr<DepthStencilView>& dsv);

		// 描画後の処理
		void renderAfter(const std::shared_ptr<RenderTargetView>& rtv);

		// コマンドリストを実行し描画
		bool executeCommandList();

		// フレームを最終出力
		bool presentSwapChain();

		//====================================================================================================
	public:
		~GraphicsManager() {}
		
		//====================================================================================================
		// メンバ変数

		// ゲッター
		inline ComPtr<ID3D12GraphicsCommandList> getCommandList() const { return command_list_; }

		inline std::shared_ptr<PipelineState> getDefaultPSO() const { return default_pso_; }
		inline std::shared_ptr<Shader> getDefaultShader() const { return default_shader_; }
		inline std::shared_ptr<Texture> getDefaultTexture() const { return default_texture_; }
		
		inline std::shared_ptr<RenderTargetView> getMainRTV() const { return rtv_[frame_index_]; }
		inline std::shared_ptr<DepthStencilView> getMainDSV() const { return dsv_; }

		inline std::shared_ptr<DescriptorManager> getRTVHeap() const { return rtv_heap_; }
		inline std::shared_ptr<DescriptorManager> getDSVHeap() const { return dsv_heap_; }
		inline std::shared_ptr<DescriptorManager> getSRVHeap() const { return srv_heap_; }

		//====================================================================================================
	};

}
