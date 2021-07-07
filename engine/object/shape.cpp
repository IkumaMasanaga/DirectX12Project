#include "../../system/dx12_manager.h"
#include "shape.h"


namespace eng {

	bool Shape::createBuffers() {
		sys::Dx12Manager& mgr = sys::Dx12Manager::getInstance();

		D3D12_HEAP_PROPERTIES heap_properties{};
		D3D12_RESOURCE_DESC   resource_desc{};

		heap_properties.Type = D3D12_HEAP_TYPE_UPLOAD;
		heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heap_properties.CreationNodeMask = 0;
		heap_properties.VisibleNodeMask = 0;

		resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resource_desc.Height = 1;
		resource_desc.DepthOrArraySize = 1;
		resource_desc.MipLevels = 1;
		resource_desc.Format = DXGI_FORMAT_UNKNOWN;
		resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resource_desc.SampleDesc.Count = 1;
		resource_desc.SampleDesc.Quality = 0;

		//���_�o�b�t�@�̍쐬
		resource_desc.Width = sizeof(Vertex3D) * vertex_num_;
		if (FAILED(mgr.device_->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vbo_)))) return false;

		//�C���f�b�N�X�o�b�t�@�̍쐬
		resource_desc.Width = sizeof(uint32_t) * index_num_;	// �C���f�b�N�X��
		if (FAILED(mgr.device_->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&ibo_)))) return false;

		return true;
	}

	bool Shape::copyBuffers(Vertex3D* vertexs, uint32_t* indexs) {

		// ���_�o�b�t�@�ɒ��_�f�[�^���R�s�[
		Vertex3D* vb{};
		if (FAILED(vbo_->Map(0, nullptr, (void**)&vb))) return false;
		memcpy(vb, vertexs, sizeof(Vertex3D) * vertex_num_);
		vbo_->Unmap(0, nullptr);

		// �C���f�b�N�X�o�b�t�@�ɃC���f�b�N�X�f�[�^���R�s�[
		uint32_t* ib{};
		if (FAILED(ibo_->Map(0, nullptr, (void**)&ib))) return false;
		memcpy(ib, indexs, sizeof(uint32_t) * index_num_);
		ibo_->Unmap(0, nullptr);

		return true;
	}
	
	bool Shape::isCreated(Shape::s_ptr& ret_obj, const std::string& regist_name) {
		if (!regist_name.empty()) {
			ret_obj = regist_map_.load<void*>(regist_name, nullptr);
			if (ret_obj->vbo_) return true;
		}
		else {
			ret_obj = Shape::createShared<Shape>();
		}
		return false;
	}
	
	Shape::s_ptr Shape::loadOfFlyweight(void* none) {
		return Shape::createShared<Shape>();
	}

	Shape::s_ptr Shape::createPlaneXY(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;

		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;

		if (!ptr->createBuffers()) return nullptr;

		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		// ���_���W�EUV�E�@���̌v�Z
		for (int i = 0; i < (desc.slices_ + 1); ++i) {	// x���ɕ��s�ɕ��� (y���ω�)
			for (int k = 0; k < (desc.stacks_ + 1); ++k) {	// y���ɕ��s�ɕ��� (x���ω�)
				int a = (i * (desc.stacks_ + 1)) + k;
				vtxs[a].position.x = (desc.width_ * 0.5f) - (desc.width_ / desc.stacks_ * k);
				vtxs[a].position.y = (desc.height_ * 0.5f) - (desc.height_ / desc.slices_ * i);
				vtxs[a].position.z = 0;

				vtxs[a].uv.x = 1.0f / (float)desc.stacks_ * k;
				vtxs[a].uv.y = 1.0f / (float)desc.slices_ * i;

				vtxs[a].normal.x = 0;
				vtxs[a].normal.y = 0;
				vtxs[a].normal.z = 1.0f;	// �����ɒ���
			}
		}

		// ���_�C���f�b�N�X�̌v�Z
		for (int i = 0; i < desc.slices_; ++i) {
			for (int k = 0; k < desc.stacks_; ++k) {
				int a = (i * desc.stacks_ + k) * 6;
				// ����̎O�p�`
				idxs[a + 0] = (i * (desc.stacks_ + 1)) + k;
				idxs[a + 1] = idxs[a + 0] + 1;
				idxs[a + 2] = idxs[a + 0] + (desc.stacks_ + 1);
				// �E���̎O�p�`
				idxs[a + 3] = idxs[a + 1];
				idxs[a + 4] = idxs[a + 2] + 1;
				idxs[a + 5] = idxs[a + 2];
			}
		}

		bool is_complete = ptr->copyBuffers(vtxs, idxs);

		delete[] vtxs;
		delete[] idxs;

		return (is_complete) ? ptr : nullptr;
	}

	Shape::s_ptr Shape::createPlaneYZ(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;

		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;

		if (!ptr->createBuffers()) return nullptr;

		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		// ���_���W�EUV�E�@���̌v�Z
		for (int i = 0; i < (desc.slices_ + 1); ++i) {
			for (int k = 0; k < (desc.stacks_ + 1); ++k) {
				int a = (i * (desc.stacks_ + 1)) + k;
				vtxs[a].position.x = 0;
				vtxs[a].position.y = (desc.height_ * 0.5f) - (desc.height_ / desc.slices_ * i);
				vtxs[a].position.z = -(desc.depth_ * 0.5f) + (desc.depth_ / desc.stacks_ * k);

				vtxs[a].uv.x = 1.0f / (float)desc.stacks_ * k;
				vtxs[a].uv.y = 1.0f / (float)desc.slices_ * i;

				vtxs[a].normal.x = 1.0f;
				vtxs[a].normal.y = 0;
				vtxs[a].normal.z = 0;
			}
		}

		// ���_�C���f�b�N�X�̌v�Z
		for (int i = 0; i < desc.slices_; ++i) {
			for (int k = 0; k < desc.stacks_; ++k) {
				int a = (i * desc.stacks_ + k) * 6;
				// ����̎O�p�`
				idxs[a + 0] = (i * (desc.stacks_ + 1)) + k;
				idxs[a + 1] = idxs[a + 0] + 1;
				idxs[a + 2] = idxs[a + 0] + (desc.stacks_ + 1);
				// �E���̎O�p�`
				idxs[a + 3] = idxs[a + 1];
				idxs[a + 4] = idxs[a + 2] + 1;
				idxs[a + 5] = idxs[a + 2];
			}
		}

		bool is_complete = ptr->copyBuffers(vtxs, idxs);

		delete[] vtxs;
		delete[] idxs;

		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createPlaneZX(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;

		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;

		if (!ptr->createBuffers()) return nullptr;

		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		// ���_���W�EUV�E�@���̌v�Z
		for (int i = 0; i < (desc.slices_ + 1); ++i) {
			for (int k = 0; k < (desc.stacks_ + 1); ++k) {
				int a = (i * (desc.stacks_ + 1)) + k;
				vtxs[a].position.x = -(desc.width_ * 0.5f) + (desc.width_ / desc.stacks_ * k);
				vtxs[a].position.y = 0;
				vtxs[a].position.z = (desc.depth_ * 0.5f) - (desc.depth_ / desc.slices_ * i);

				vtxs[a].uv.x = 1.0f / (float)desc.stacks_ * k;
				vtxs[a].uv.y = 1.0f / (float)desc.slices_ * i;

				vtxs[a].normal.x = 0;
				vtxs[a].normal.y = 1.0f;
				vtxs[a].normal.z = 0;
			}
		}

		// ���_�C���f�b�N�X�̌v�Z
		for (int i = 0; i < desc.slices_; ++i) {
			for (int k = 0; k < desc.stacks_; ++k) {
				int a = (i * desc.stacks_ + k) * 6;
				// ����̎O�p�`
				idxs[a + 0] = (i * (desc.stacks_ + 1)) + k;
				idxs[a + 1] = idxs[a + 0] + 1;
				idxs[a + 2] = idxs[a + 0] + (desc.stacks_ + 1);
				// �E���̎O�p�`
				idxs[a + 3] = idxs[a + 1];
				idxs[a + 4] = idxs[a + 2] + 1;
				idxs[a + 5] = idxs[a + 2];
			}
		}

		bool is_complete = ptr->copyBuffers(vtxs, idxs);

		delete[] vtxs;
		delete[] idxs;

		return (is_complete) ? ptr : nullptr;
	}

}
