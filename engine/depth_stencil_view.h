#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "../library/library.h"


namespace eng {

	class DepthStencilView final : lib::SmartFactory {
	public:
		DepthStencilView() {}
		~DepthStencilView() {}

		Microsoft::WRL::ComPtr<ID3D12Resource> buffer_;
		D3D12_CPU_DESCRIPTOR_HANDLE handle_ = {};
	};

}
