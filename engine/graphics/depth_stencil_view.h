#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "../../library/library.h"
#include "descriptor_handle.h"


namespace eng {

	class DepthStencilView final : public lib::SmartFactory {
	private:
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
	public:
		using s_ptr = std::shared_ptr<DepthStencilView>;
		using w_ptr = std::weak_ptr<DepthStencilView>;
	private:
		//====================================================================================================
		// メンバ変数

		ComPtr<ID3D12Resource> buffer_;	// バッファ
		DescriptorHandle handle_;		// ハンドル

		//====================================================================================================
	public:
		DepthStencilView() {}
		~DepthStencilView();

		//====================================================================================================
		// メンバ関数

		// ゲッター
		inline ComPtr<ID3D12Resource> getBuffer() { return buffer_; }
		inline DescriptorHandle getHandle() const { return handle_; }

		//====================================================================================================
		// static関数

		// 生成
		static DepthStencilView::s_ptr create(const LONG width, const LONG height);

		//====================================================================================================
	};

}
