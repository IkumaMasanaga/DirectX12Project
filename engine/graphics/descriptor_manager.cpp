#include "../../system/dx12_manager.h"
#include "descriptor_manager.h"


namespace eng {

	DescriptorManager::DescriptorManager(const D3D12_DESCRIPTOR_HEAP_DESC& desc) {
		sys::Dx12Manager& mgr = sys::Dx12Manager::getInstance();
		if (FAILED(mgr.device_->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap_)))) return;
		handle_cpu_ = heap_->GetCPUDescriptorHandleForHeapStart();
		handle_gpu_ = heap_->GetGPUDescriptorHandleForHeapStart();
		increment_size_ = mgr.device_->GetDescriptorHandleIncrementSize(desc.Type);
	}

}
