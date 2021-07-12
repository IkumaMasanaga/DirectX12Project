#include "../../system/dx12_manager.h"
#include "../graphics/graphics_manager.h"
#include "../graphics/shape.h"
#include "../graphics/texture.h"
#include "../graphics/pipeline_state.h"
#include "mesh.h"
#include "material.h"


namespace eng {

	bool Mesh::createCBV() {

		D3D12_HEAP_PROPERTIES heap_properties{};
		D3D12_RESOURCE_DESC   resource_desc{};

		heap_properties.Type = D3D12_HEAP_TYPE_UPLOAD;
		heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heap_properties.CreationNodeMask = 0;
		heap_properties.VisibleNodeMask = 0;

		resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resource_desc.Width = 256;
		resource_desc.Height = 1;
		resource_desc.DepthOrArraySize = 1;
		resource_desc.MipLevels = 1;
		resource_desc.Format = DXGI_FORMAT_UNKNOWN;
		resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resource_desc.SampleDesc.Count = 1;
		resource_desc.SampleDesc.Quality = 0;

		sys::Dx12Manager& mgr = sys::Dx12Manager::getInstance();

		//定数バッファの作成
		if (FAILED(mgr.device_->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&cbv_)))) return false;

		return true;
	}

	Mesh::s_ptr Mesh::createFromShape(std::shared_ptr<Shape> shape,const std::string& texture_file_path) {
		Mesh::s_ptr ptr = Mesh::createShared<Mesh>();
		ptr->createCBV();
		ptr->shape_ = shape;
		ptr->material_ = Material::createShared<Material>();
		ptr->material_->tex_diffuse_ = Texture::loadFromFile(texture_file_path);
		ptr->pso_ = GraphicsManager::getInstance().getDefaultPso();
		ptr->transform_ = Transform<Mesh>::create(ptr);
		return ptr;
	}

}
