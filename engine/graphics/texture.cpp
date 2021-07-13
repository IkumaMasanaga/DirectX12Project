#include "../../system/dx12_manager.h"
#include "../../system/d3dx12.h"
#include "../../library/bitmap.h"
#include "graphics_manager.h"
#include "descriptor_manager.h"
#include "texture.h"


namespace eng {

	Texture::s_ptr Texture::loadOfFlyweight(const std::string& file_path) {
		Texture::s_ptr ptr = Texture::createShared<Texture>();
		ComPtr<ID3D12Device> device = sys::Dx12Manager::getInstance().getDevice();

		// 画像を読み込んで ARGBテーブルを作成 ( αなし bmp )
		FILE* fp = nullptr;
		uint32_t* argb_tbl = nullptr;
		uint32_t tw = 0;
		uint32_t th = 0;

		fopen_s(&fp, file_path.c_str(), "rb");
		if (!fp) return nullptr;
		Picture* p = getBmp(fp);
		tw = p->x;
		th = p->y;

		argb_tbl = new unsigned int[tw * th];
		memset(argb_tbl, 0, sizeof(argb_tbl));
		for (int k = 0; k < (int)(tw * th); ++k) {
			argb_tbl[k] = 0xff000000 | (p->r[k] << 16) | (p->g[k] << 8) | (p->b[k] << 0);
		}
		free(p);
		fclose(fp);

		//テクスチャ用のリソースの作成
		D3D12_HEAP_PROPERTIES heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
		D3D12_RESOURCE_DESC   resource_desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_B8G8R8A8_UNORM, tw, th, 1, 1, 1, 0);
		if (FAILED(device->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&ptr->resource_)))) return nullptr;

		//シェーダリソースビューの作成
		D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
		srv_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MipLevels = 1;
		srv_desc.Texture2D.MostDetailedMip = 0;
		srv_desc.Texture2D.PlaneSlice = 0;
		srv_desc.Texture2D.ResourceMinLODClamp = 0.0F;
		srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		ptr->handle_ = GraphicsManager::getInstance().getSrvHeap()->alloc();
		device->CreateShaderResourceView(ptr->resource_.Get(), &srv_desc, ptr->handle_.getCpuHandle());

		//画像データの書き込み
		D3D12_BOX box = { 0, 0, 0, (UINT)tw, (UINT)th, 1 };
		if (FAILED(ptr->resource_->WriteToSubresource(0, &box, argb_tbl, 4 * tw, 4 * tw * th))) return nullptr;

		ptr->width_ = tw;
		ptr->height_ = th;

		delete[] argb_tbl;

		return ptr;
	}

	Texture::s_ptr Texture::loadFromFile(const std::string& file_path) {
		std::string path = (file_path.empty()) ? "engine/resources/default_texture.bmp" : file_path;
		return regist_map_.load<std::string>(path, path);
	}

	Texture::s_ptr Texture::createEmpty(const uint32_t width, const uint32_t height) {
		Texture::s_ptr ptr = Texture::createShared<Texture>();
		ComPtr<ID3D12Device> device = sys::Dx12Manager::getInstance().getDevice();

		//テクスチャ用のリソースの作成
		D3D12_HEAP_PROPERTIES heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
		D3D12_RESOURCE_DESC   resource_desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_B8G8R8A8_UNORM, width, height, 1, 1, 1, 0);
		if (FAILED(device->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&ptr->resource_)))) return nullptr;

		//シェーダリソースビューの作成
		D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
		srv_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MipLevels = 1;
		srv_desc.Texture2D.MostDetailedMip = 0;
		srv_desc.Texture2D.PlaneSlice = 0;
		srv_desc.Texture2D.ResourceMinLODClamp = 0.0F;
		srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		ptr->handle_ = GraphicsManager::getInstance().getSrvHeap()->alloc();
		device->CreateShaderResourceView(ptr->resource_.Get(), &srv_desc, ptr->handle_.getCpuHandle());

		ptr->width_ = width;
		ptr->height_ = height;

		return ptr;
	}

}
