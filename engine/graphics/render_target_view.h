#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "../../library/library.h"
#include "descriptor_handle.h"


namespace eng {

	class RenderTargetView : public lib::SmartFactory {
	private:
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
	public:
		using s_ptr = std::shared_ptr<RenderTargetView>;
		using w_ptr = std::weak_ptr<RenderTargetView>;
	private:
		//====================================================================================================
		// メンバ変数

		ComPtr<ID3D12Resource> buffer_;	// バッファ
		DescriptorHandle handle_;		// ハンドル
		lib::Color clear_color_;		// クリアする色

		//====================================================================================================
	public:
		RenderTargetView() {}
		~RenderTargetView();

		//====================================================================================================
		// メンバ関数

		// ゲッター
		inline ComPtr<ID3D12Resource>& getBuffer() { return buffer_; }
		inline DescriptorHandle getHandle() const { return handle_; }
		inline lib::Color getClearColor() const { return clear_color_; }

		//====================================================================================================
		// static関数

		// 生成
		static RenderTargetView::s_ptr create(const lib::Color& clear_color, const LONG width, const LONG height);

		//====================================================================================================
	};

}
