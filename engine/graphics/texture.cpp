#include "../../system/dx12_manager.h"
#include "../../library/bitmap.h"
#include "texture.h"


namespace eng {

	Texture::s_ptr Texture::loadOfFlyweight(const std::string& file_path) {
		Texture::s_ptr ptr = Texture::createShared<Texture>();

		sys::Dx12Manager& mgr = sys::Dx12Manager::getInstance();

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

		D3D12_HEAP_PROPERTIES heap_properties{};
		D3D12_RESOURCE_DESC   resource_desc{};

		//テクスチャ用のリソースの作成
		heap_properties.Type = D3D12_HEAP_TYPE_CUSTOM;
		heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

		resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resource_desc.Width = tw;
		resource_desc.Height = th;
		resource_desc.DepthOrArraySize = 1;
		resource_desc.MipLevels = 1;
		resource_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resource_desc.SampleDesc.Count = 1;
		resource_desc.SampleDesc.Quality = 0;
		if (FAILED(mgr.device_->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&ptr->texture_)))) return nullptr;

		//テクスチャ用のデスクリプタヒープの作成
		D3D12_DESCRIPTOR_HEAP_DESC descriptor_heap_desc{};
		descriptor_heap_desc.NumDescriptors = 1;
		descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		descriptor_heap_desc.NodeMask = 0;
		if (FAILED(mgr.device_->CreateDescriptorHeap(&descriptor_heap_desc, IID_PPV_ARGS(&ptr->descriptor_heap_)))) return nullptr;

		//シェーダリソースビューの作成
		D3D12_CPU_DESCRIPTOR_HANDLE handle_srv{};
		D3D12_SHADER_RESOURCE_VIEW_DESC resourct_view_desc{};

		resourct_view_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		resourct_view_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		resourct_view_desc.Texture2D.MipLevels = 1;
		resourct_view_desc.Texture2D.MostDetailedMip = 0;
		resourct_view_desc.Texture2D.PlaneSlice = 0;
		resourct_view_desc.Texture2D.ResourceMinLODClamp = 0.0F;
		resourct_view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		handle_srv = ptr->descriptor_heap_->GetCPUDescriptorHandleForHeapStart();
		mgr.device_->CreateShaderResourceView(ptr->texture_.Get(), &resourct_view_desc, handle_srv);

		//resourct_view_desc.Format = DXGI_FORMAT_R32_FLOAT;
		//handle_srv.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//device->CreateShaderResourceView(sm, &resourct_view_desc, handle_srv);


		//画像データの書き込み
		D3D12_BOX box = { 0, 0, 0, (UINT)tw, (UINT)th, 1 };
		if (FAILED(ptr->texture_->WriteToSubresource(0, &box, argb_tbl, 4 * tw, 4 * tw * th))) return nullptr;

		ptr->width_ = tw;
		ptr->height_ = th;

		delete[] argb_tbl;

		return ptr;
	}

	Texture::s_ptr Texture::loadFromFile(const std::string& file_path) {
		std::string path = (file_path.empty()) ? "engine/resources/default_texture.bmp" : file_path;
		return regist_map_.load<std::string>(path, path);
	}

}
