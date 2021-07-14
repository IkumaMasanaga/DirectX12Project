#include "../../system/dx12_manager.h"
#include "descriptor_manager.h"
#include "depth_stencil_view.h"


namespace eng {

	DepthStencilView::~DepthStencilView() {
		GraphicsManager::getInstance().getDSVHeap()->free(handle_);
	}

	DepthStencilView::s_ptr DepthStencilView::create(const LONG width, const LONG height) {
		DepthStencilView::s_ptr ptr = DepthStencilView::createShared<DepthStencilView>();
		ComPtr<ID3D12Device> device = sys::Dx12Manager::getInstance().getDevice();

		//深度バッファの作成
		D3D12_HEAP_PROPERTIES heap_properties = {};
		D3D12_RESOURCE_DESC resource_desc = {};
		D3D12_CLEAR_VALUE clear_value = {};

		heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;
		heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heap_properties.CreationNodeMask = 0;
		heap_properties.VisibleNodeMask = 0;

		resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resource_desc.Width = width;
		resource_desc.Height = height;
		resource_desc.DepthOrArraySize = 1;
		resource_desc.MipLevels = 0;
		resource_desc.Format = DXGI_FORMAT_R32_TYPELESS;
		resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resource_desc.SampleDesc.Count = 1;
		resource_desc.SampleDesc.Quality = 0;
		resource_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		clear_value.Format = DXGI_FORMAT_D32_FLOAT;
		clear_value.DepthStencil.Depth = 1.0f;
		clear_value.DepthStencil.Stencil = 0;

		if (FAILED(device->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clear_value, IID_PPV_ARGS(&ptr->buffer_)))) return nullptr;

		//深度バッファのビューの作成
		D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
		dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsv_desc.Format = DXGI_FORMAT_D32_FLOAT;
		dsv_desc.Texture2D.MipSlice = 0;
		dsv_desc.Flags = D3D12_DSV_FLAG_NONE;

		ptr->handle_ = GraphicsManager::getInstance().getDSVHeap()->alloc();
		device->CreateDepthStencilView(ptr->buffer_.Get(), &dsv_desc, ptr->handle_.getCpuHandle());

		return ptr;
	}

}
