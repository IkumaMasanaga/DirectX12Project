#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include "../system/system.h"
#include "../library/library.h"


namespace eng {

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

		// フレーム数
		// 表面と裏面
		inline static constexpr UINT FRAME_COUNT = 2;

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

		// 現在の描画フレーム
		UINT frame_index_ = 0;

		// コマンドキュー
		// 1つ？
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> command_queue_;
		// コマンドアロケーター
		// 1つ？
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> command_allocator_;
		// コマンドリスト
		// 1つ？
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> command_list_;

		// スワップチェイン
		// 1つ？
		Microsoft::WRL::ComPtr<IDXGISwapChain3> swap_chain_;

		// レンダーターゲットビュー
		// オフスクリーンレンダリングを使用し画面分割する際には複数必要？
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtv_heap_;
		Microsoft::WRL::ComPtr<ID3D12Resource> rtv_buffer_[FRAME_COUNT] = {};
		D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle_[FRAME_COUNT] = {};

		// 深度ステンシルビュー
		// オフスクリーンレンダリングを使用し画面分割する際には複数必要？
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsv_heap_;
		Microsoft::WRL::ComPtr<ID3D12Resource> dsv_buffer_;
		D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle_ = {};

		std::shared_ptr<PipelineState> default_pso_ = nullptr;	// デフォルトのPSO
		std::shared_ptr<Shader> default_shader_ = nullptr;		// デフォルトのシェーダ
		std::shared_ptr<Texture> default_texture_ = nullptr;	// デフォルトのテクスチャ

		//====================================================================================================
	};

}
