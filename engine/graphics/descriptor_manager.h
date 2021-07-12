#pragma once
#include <wrl.h>
#include "../../system/d3dx12.h"
#include "../../library/library.h"
#include "descriptor_handle.h"


namespace eng {

	class DescriptorManager final : public lib::SmartFactory {
	private:
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
	public:
		using s_ptr = std::shared_ptr<DescriptorManager>;
		using w_ptr = std::weak_ptr<DescriptorManager>;
	private:
		//====================================================================================================
		// メンバ変数

		ComPtr<ID3D12DescriptorHeap> heap_;			// ディスクリプタヒープ
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle_cpu_;	// 確保してきたCPUハンドル
		CD3DX12_GPU_DESCRIPTOR_HANDLE handle_gpu_;	// 確保してきたGPUハンドル
		UINT index_ = 0;							// 確保した数
		UINT increment_size_ = 0;					// 管理するディスクリプタ1つののサイズ
		std::list<DescriptorHandle> free_list_;		// 開放したハンドル

		//====================================================================================================
	public:
		DescriptorManager() {}
		~DescriptorManager() {}

		//====================================================================================================
		// メンバ関数

		// 確保
		DescriptorHandle alloc();

		// 開放
		inline void free(const DescriptorHandle& handle) { free_list_.emplace_back(handle); }

		// ディスクリプタヒープの取得
		inline ComPtr<ID3D12DescriptorHeap> getHeap() const { return heap_; }

		//====================================================================================================
		// static関数

		// 生成
		static DescriptorManager::s_ptr create(const D3D12_DESCRIPTOR_HEAP_DESC& desc);

		//====================================================================================================
	};

}
