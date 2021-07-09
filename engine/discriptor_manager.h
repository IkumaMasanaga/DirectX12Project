#pragma once
#include <wrl.h>
#include "../system/d3dx12.h"


namespace eng {

	class DescriptorHandle final {
		friend class DescriptorManager;
	private:
		DescriptorHandle() : handle_cpu_(), handle_gpu_() {}
		DescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE handle_cpu, D3D12_GPU_DESCRIPTOR_HANDLE handle_gpu) : handle_cpu_(handle_cpu), handle_gpu_(handle_gpu) {}

		D3D12_CPU_DESCRIPTOR_HANDLE handle_cpu_;
		D3D12_GPU_DESCRIPTOR_HANDLE handle_gpu_;

	public:

		operator D3D12_CPU_DESCRIPTOR_HANDLE() const { return handle_cpu_; }
		operator D3D12_GPU_DESCRIPTOR_HANDLE() const { return handle_gpu_; }

	};

	class DescriptorManager final {
		friend class GraphicsManager;
	private:
		DescriptorManager(const D3D12_DESCRIPTOR_HEAP_DESC& desc);
		~DescriptorManager() {}

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle_cpu_;
		CD3DX12_GPU_DESCRIPTOR_HANDLE handle_gpu_;
		UINT index_ = 0;
		UINT increment_size_ = 0;
		std::list<DescriptorHandle> free_list_;
	public:
		inline Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> getHeap() const { return heap_; }

		inline DescriptorHandle alloc() {
			if (!free_list_.empty()) {
				DescriptorHandle ret = free_list_.front();
				free_list_.pop_front();
				return ret;
			}

			UINT use = index_++;
			DescriptorHandle ret = DescriptorHandle(handle_cpu_.Offset(use, increment_size_), handle_gpu_.Offset(use, increment_size_));

			return ret;
		}

		inline void free(const DescriptorHandle& handle) { free_list_.emplace_back(handle); }

	};

}
