#pragma once
#include <d3d12.h>


namespace eng {

	// ïœçXÇ∑ÇÈâ¬î\ê´Ç†ÇË
	
	class DescriptorHandle final {
	private:
		D3D12_CPU_DESCRIPTOR_HANDLE handle_cpu_;
		D3D12_GPU_DESCRIPTOR_HANDLE handle_gpu_;

	public:
		DescriptorHandle() : handle_cpu_(), handle_gpu_() {}
		DescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE handle_cpu, D3D12_GPU_DESCRIPTOR_HANDLE handle_gpu) : handle_cpu_(handle_cpu), handle_gpu_(handle_gpu) {}
		~DescriptorHandle() {}

		inline D3D12_CPU_DESCRIPTOR_HANDLE& getCpuHandle() { return handle_cpu_; }
		inline D3D12_GPU_DESCRIPTOR_HANDLE& getGpuHandle() { return handle_gpu_; }

	};

}
