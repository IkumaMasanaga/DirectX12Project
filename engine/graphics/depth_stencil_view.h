#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "../../library/library.h"
#include "descriptor_handle.h"


namespace eng {

	class DepthStencilView final : public lib::SmartFactory {
	public:
		using s_ptr = std::shared_ptr<DepthStencilView>;
		using w_ptr = std::weak_ptr<DepthStencilView>;
	public:
		DepthStencilView() {}
		~DepthStencilView();

		//====================================================================================================
		// メンバ変数

		Microsoft::WRL::ComPtr<ID3D12Resource> buffer_;	// バッファ
		DescriptorHandle handle_;						// ハンドル

		//====================================================================================================
		// static関数

		// 生成
		static DepthStencilView::s_ptr create(const LONG width, const LONG height);

		//====================================================================================================
	};

}
