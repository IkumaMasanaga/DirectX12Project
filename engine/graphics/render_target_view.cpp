#include "../../system/dx12_manager.h"
#include "graphics_manager.h"
#include "descriptor_manager.h"
#include "render_target_view.h"


namespace eng {

	RenderTargetView::~RenderTargetView() {
		GraphicsManager::getInstance().rtv_heap_->free(handle_);
	}

	RenderTargetView::s_ptr RenderTargetView::create(const lib::Color& clear_color, const LONG width, const LONG height) {
		RenderTargetView::s_ptr ptr = RenderTargetView::createShared<RenderTargetView>();

		// RGB •`‰æ—p
		CD3DX12_RESOURCE_DESC color_tex_desc = CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_B8G8R8A8_UNORM, width, height,
			1, 1, 1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
		);
		D3D12_CLEAR_VALUE clear_value = CD3DX12_CLEAR_VALUE(color_tex_desc.Format, clear_color.c);
		D3D12_HEAP_PROPERTIES heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		sys::Dx12Manager& mgr = sys::Dx12Manager::getInstance();
		if (FAILED(mgr.device_->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &color_tex_desc, D3D12_RESOURCE_STATE_RENDER_TARGET, &clear_value, IID_PPV_ARGS(&ptr->buffer_)))) return nullptr;
		ptr->handle_ = GraphicsManager::getInstance().rtv_heap_->alloc();
		mgr.device_->CreateRenderTargetView(ptr->buffer_.Get(), nullptr, ptr->handle_);
		ptr->clear_color_ = clear_color;

		return ptr;
	}

}
