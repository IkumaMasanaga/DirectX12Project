#include "../../system/dx12_manager.h"
#include "descriptor_manager.h"


namespace eng {

	DescriptorHandle DescriptorManager::alloc() {
		if (!free_list_.empty()) {
			DescriptorHandle ret = free_list_.front();
			free_list_.pop_front();
			return ret;
		}
		UINT use = index_++;
		DescriptorHandle ret = DescriptorHandle(handle_cpu_.Offset(use, increment_size_), handle_gpu_.Offset(use, increment_size_));
		return ret;
	}

	DescriptorManager::s_ptr DescriptorManager::create(const D3D12_DESCRIPTOR_HEAP_DESC& desc) {
		DescriptorManager::s_ptr ptr = DescriptorManager::createShared<DescriptorManager>();
		ComPtr<ID3D12Device> device = sys::Dx12Manager::getInstance().getDevice();
		if (FAILED(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&ptr->heap_)))) return nullptr;
		ptr->handle_cpu_ = ptr->heap_->GetCPUDescriptorHandleForHeapStart();
		ptr->handle_gpu_ = ptr->heap_->GetGPUDescriptorHandleForHeapStart();
		ptr->increment_size_ = device->GetDescriptorHandleIncrementSize(desc.Type);
		return ptr;
	}

}
