#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "../../library/library.h"


namespace eng {

	class RenderTargetView : public lib::SmartFactory {
	public:
		RenderTargetView() {}
		~RenderTargetView() {}

		Microsoft::WRL::ComPtr<ID3D12Resource> buffer_;
		D3D12_CPU_DESCRIPTOR_HANDLE handle_ = {};
		lib::Color clear_;
	};

}
