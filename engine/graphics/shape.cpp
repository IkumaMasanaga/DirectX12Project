#include "../../system/d3dx12.h"
#include "../../system/dx12_manager.h"
#include "shape.h"


namespace eng {

	bool Shape::createBuffers() {
		ComPtr<ID3D12Device> device = sys::Dx12Manager::getInstance().getDevice();

		D3D12_HEAP_PROPERTIES heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC   resource_desc = {};

		//���_�o�b�t�@�̍쐬
		resource_desc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex3D) * vertex_num_);
		if (FAILED(device->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vbo_)))) return false;

		//�C���f�b�N�X�o�b�t�@�̍쐬
		resource_desc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(uint32_t) * index_num_);
		if (FAILED(device->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&ibo_)))) return false;

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

	void Shape::calcIndexs(uint32_t*& indexs, const int slices, const int stacks) {
		for (int i = 0; i < slices; ++i) {
			for (int k = 0; k < stacks; ++k) {
				int a = (i * stacks + k) * 6;
				// ����̎O�p�`
				indexs[a + 0] = (i * (stacks + 1)) + k;
				indexs[a + 1] = indexs[a + 0] + 1;
				indexs[a + 2] = indexs[a + 0] + (stacks + 1);
				// �E���̎O�p�`
				indexs[a + 3] = indexs[a + 1];
				indexs[a + 4] = indexs[a + 2] + 1;
				indexs[a + 5] = indexs[a + 2];
			}
		}
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
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// ���ɓo�^����Ă���ꍇ�͂����Ԃ�
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// �o�b�t�@�̍쐬
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// ���_���W�EUV�E�@���̌v�Z
		for (int i = 0; i < (desc.slices_ + 1); ++i) {	// x���ɕ��s�ɕ��� (y���ω�)
			for (int k = 0; k < (desc.stacks_ + 1); ++k) {	// y���ɕ��s�ɕ��� (x���ω�)
				int a = (i * (desc.stacks_ + 1)) + k;
				vtxs[a].position.x = (desc.width_ * 0.5f) - (desc.width_ / desc.stacks_ * k);
				vtxs[a].position.y = (desc.height_ * 0.5f) - (desc.height_ / desc.slices_ * i);
				vtxs[a].position.z = 0;

				vtxs[a].uv.x = 1.0f / (float)desc.stacks_ * k;
				vtxs[a].uv.y = 1.0f / (float)desc.slices_ * i;

				vtxs[a].normal = lib::Vector3::FORWARD;
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// �C���f�b�N�X�̌v�Z
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// �o�b�t�@�փR�s�[
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}

	Shape::s_ptr Shape::createPlaneYZ(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// ���ɓo�^����Ă���ꍇ�͂����Ԃ�
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// �o�b�t�@�̍쐬
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// ���_���W�EUV�E�@���̌v�Z
		for (int i = 0; i < (desc.slices_ + 1); ++i) {
			for (int k = 0; k < (desc.stacks_ + 1); ++k) {
				int a = (i * (desc.stacks_ + 1)) + k;
				vtxs[a].position.x = 0;
				vtxs[a].position.y = (desc.height_ * 0.5f) - (desc.height_ / desc.slices_ * i);
				vtxs[a].position.z = -(desc.depth_ * 0.5f) + (desc.depth_ / desc.stacks_ * k);

				vtxs[a].uv.x = 1.0f / (float)desc.stacks_ * k;
				vtxs[a].uv.y = 1.0f / (float)desc.slices_ * i;

				vtxs[a].normal = lib::Vector3::RIGHT;
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// �C���f�b�N�X�̌v�Z
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// �o�b�t�@�փR�s�[
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createPlaneZX(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// ���ɓo�^����Ă���ꍇ�͂����Ԃ�
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// �o�b�t�@�̍쐬
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// ���_���W�EUV�E�@���̌v�Z
		for (int i = 0; i < (desc.slices_ + 1); ++i) {
			for (int k = 0; k < (desc.stacks_ + 1); ++k) {
				int a = (i * (desc.stacks_ + 1)) + k;
				vtxs[a].position.x = -(desc.width_ * 0.5f) + (desc.width_ / desc.stacks_ * k);
				vtxs[a].position.y = 0;
				vtxs[a].position.z = (desc.depth_ * 0.5f) - (desc.depth_ / desc.slices_ * i);

				vtxs[a].uv.x = 1.0f / (float)desc.stacks_ * k;
				vtxs[a].uv.y = 1.0f / (float)desc.slices_ * i;

				vtxs[a].normal = lib::Vector3::UP;
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// �C���f�b�N�X�̌v�Z
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// �o�b�t�@�փR�s�[
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}

	Shape::s_ptr Shape::createTriangleIsosceles(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// ���ɓo�^����Ă���ꍇ�͂����Ԃ�
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// �o�b�t�@�̍쐬
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		float ww = desc.width_ * 0.5f;
		float hh = desc.height_ * 0.5f;
		lib::Vector3 c = (lib::Vector3(0, hh, 0) + lib::Vector3(-ww, -hh, 0) + lib::Vector3(ww, -hh, 0)) / 3.0f;

		// ���_���W�EUV�E�@���̌v�Z
		for (int i = 0; i < (desc.slices_ + 1); ++i) {
			for (int k = 0; k < (desc.stacks_ + 1); ++k) {
				int a = (i * (desc.stacks_ + 1)) + k;
				vtxs[a].position.x = -c.x + ((desc.width_ * 0.5f) - (desc.width_ / desc.stacks_ * k)) * (1.0f / desc.slices_ * i);
				vtxs[a].position.y = -c.y + (desc.height_ * 0.5f) - (desc.height_ / desc.slices_ * i);
				vtxs[a].position.z = 0;

				vtxs[a].uv.x = 0.5f - (vtxs[(i * (desc.stacks_ + 1)) + k].position.x / desc.width_);
				vtxs[a].uv.y = 1.0f / (float)desc.slices_ * i;

				vtxs[a].normal = lib::Vector3::FORWARD;
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// �C���f�b�N�X�̌v�Z
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// �o�b�t�@�փR�s�[
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createTriangleRight(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// ���ɓo�^����Ă���ꍇ�͂����Ԃ�
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// �o�b�t�@�̍쐬
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// ���_���W�EUV�E�@���̌v�Z
		for (int i = 0; i < (desc.slices_ + 1); ++i) {
			for (int k = 0; k < (desc.stacks_ + 1); ++k) {
				int a = (i * (desc.stacks_ + 1)) + k;
				vtxs[a].position.x = (-(desc.width_ * 0.5f) + (i * (desc.width_ / desc.stacks_))) - ((desc.width_ / desc.stacks_ * k) * (i * (1.0f / desc.slices_)));
				vtxs[a].position.y = (desc.height_ * 0.5f) - (desc.height_ / desc.slices_ * i);
				vtxs[a].position.z = 0;

				vtxs[a].uv.x = 0.5f - vtxs[a].position.x / desc.width_;
				vtxs[a].uv.y = 1.0f / (float)desc.slices_ * i;

				vtxs[a].normal = lib::Vector3::FORWARD;
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// �C���f�b�N�X�̌v�Z
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// �o�b�t�@�փR�s�[
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}

	Shape::s_ptr Shape::createTriangleLeft(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// ���ɓo�^����Ă���ꍇ�͂����Ԃ�
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// �o�b�t�@�̍쐬
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// ���_���W�EUV�E�@���̌v�Z
		for (int i = 0; i < (desc.slices_ + 1); ++i) {
			for (int k = 0; k < (desc.stacks_ + 1); ++k) {
				int a = (i * (desc.stacks_ + 1)) + k;
				vtxs[a].position.x = (desc.width_ * 0.5f) - ((desc.width_ / desc.stacks_ * k) * (i * (1.0f / desc.slices_)));
				vtxs[a].position.y = (desc.height_ * 0.5f) - (desc.height_ / desc.slices_ * i);
				vtxs[a].position.z = 0;

				vtxs[a].uv.x = 0.5f - vtxs[a].position.x / desc.width_;
				vtxs[a].uv.y = 1.0f / (float)desc.slices_ * i;

				vtxs[a].normal = lib::Vector3::FORWARD;
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// �C���f�b�N�X�̌v�Z
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// �o�b�t�@�փR�s�[
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createTriangleEquilateral(const CreateDesc& desc) {
		float width = desc.egge_lenght_;
		float height = desc.egge_lenght_ * sqrt(3.0f) * 0.5f;
		Shape::CreateDesc d;
		d.width_ = width;
		d.height_ = height;
		d.stacks_ = desc.stacks_;
		d.slices_ = desc.slices_;
		return createTriangleIsosceles(d);
	}
	
	Shape::s_ptr Shape::createSphere(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// ���ɓo�^����Ă���ꍇ�͂����Ԃ�
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// �o�b�t�@�̍쐬
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// ���_���W�EUV�E�@���̌v�Z
		for (int i = 0; i < (desc.slices_ + 1); ++i) {
			float s = i / ((float)desc.slices_);
			float y = cosf(lib::Math::PI * s) * desc.radius_;
			float r = sinf(lib::Math::PI * s) * desc.radius_;
			for (int k = 0; k < (desc.stacks_ + 1); ++k) {
				float t = k / ((float)desc.stacks_);
				lib::Vector3 v = lib::Vector3(cosf(2 * lib::Math::PI * t) * r, y, sinf(2 * lib::Math::PI * t) * r);
				int a = (i * (desc.stacks_ + 1)) + k;
				vtxs[a].position = v;

				vtxs[a].uv.x = 1.0f / (float)desc.stacks_ * k;
				vtxs[a].uv.y = 1.0f / (float)desc.slices_ * i;

				vtxs[a].normal = lib::Vector3::normalize(v);
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// �C���f�b�N�X�̌v�Z
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// �o�b�t�@�փR�s�[
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createDome(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// ���ɓo�^����Ă���ꍇ�͂����Ԃ�
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// �o�b�t�@�̍쐬
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// ���_���W�EUV�E�@���̌v�Z
		for (int i = 0; i < (desc.slices_ + 1); ++i) {
			float v = i / ((float)desc.slices_) * 0.5f;
			float y = cosf(lib::Math::PI * v) * desc.radius_;
			float r = sinf(lib::Math::PI * v) * desc.radius_;
			float angle_ofs = 0;
			if (Angle::ANGLE_90 == desc.angle_) angle_ofs = lib::Math::toRadian(45);
			else if (Angle::ANGLE_45 == desc.angle_) angle_ofs = lib::Math::toRadian(67.5);
			for (int k = 0; k < (desc.stacks_ + 1); ++k) {
				float u = k / ((float)desc.stacks_);
				if (Angle::ANGLE_180 == desc.angle_) u /= 2;
				else if (Angle::ANGLE_90 == desc.angle_) u /= 4;
				else if (Angle::ANGLE_45 == desc.angle_) u /= 8;
				lib::Vector3 vv;
				vv.x = cosf(angle_ofs + 2 * lib::Math::PI * u) * r;
				vv.y = y;
				vv.z = sinf(angle_ofs + 2 * lib::Math::PI * u) * r;

				int a = (i * (desc.stacks_ + 1)) + k;
				vtxs[a].position = vv;

				vtxs[a].uv.x = 1.0f / (float)desc.stacks_ * k;
				vtxs[a].uv.y = 1.0f / (float)desc.slices_ * i;

				vtxs[a].normal = lib::Vector3::normalize(vv);
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// �C���f�b�N�X�̌v�Z
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// �o�b�t�@�փR�s�[
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createCone(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// ���ɓo�^����Ă���ꍇ�͂����Ԃ�
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// �o�b�t�@�̍쐬
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// ���_���W�EUV�E�@���̌v�Z
		for (int i = 0; i < (desc.slices_ + 1); ++i) {
			float r = (desc.radius_ / desc.slices_) * i;
			float angle_ofs = 0;
			if (Angle::ANGLE_90 == desc.angle_) angle_ofs = lib::Math::toRadian(45);
			else if (Angle::ANGLE_45 == desc.angle_) angle_ofs = lib::Math::toRadian(67.5);
			for (int k = 0; k < (desc.stacks_ + 1); ++k) {
				float u = k / ((float)desc.stacks_);
				if (Angle::ANGLE_180 == desc.angle_) u /= 2;
				else if (Angle::ANGLE_90 == desc.angle_) u /= 4;
				else if (Angle::ANGLE_45 == desc.angle_) u /= 8;
				lib::Vector3 vv;
				vv.x = (cosf(angle_ofs + 2 * lib::Math::PI * u)) * r;
				vv.y = desc.height_ - (desc.height_ / desc.slices_ * i);
				vv.z = (sinf(angle_ofs + 2 * lib::Math::PI * u)) * r;

				int a = (i * (desc.stacks_ + 1)) + k;
				vtxs[a].position = vv;

				vtxs[a].uv.x = 1.0f / (float)desc.stacks_ * k;
				vtxs[a].uv.y = 1.0f / (float)desc.slices_ * i;

				vtxs[a].normal = lib::Vector3::normalize(vv);
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// �C���f�b�N�X�̌v�Z
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// �o�b�t�@�փR�s�[
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createDisk(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// ���ɓo�^����Ă���ꍇ�͂����Ԃ�
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// �o�b�t�@�̍쐬
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// ���_���W�EUV�E�@���̌v�Z
		for (int i = 0; i < (desc.slices_ + 1); ++i) {
			float v = i / ((float)desc.slices_);
			if (Angle::ANGLE_180 == desc.angle_) v *= 0.5f;
			else if (Angle::ANGLE_90 == desc.angle_) v *= 0.5f;
			else if (Angle::ANGLE_45 == desc.angle_) v *= 0.5f;
			float y = cosf(lib::Math::PI * v) * desc.radius_;
			float r = sinf(lib::Math::PI * v) * desc.radius_;
			for (int k = 0; k < (desc.stacks_ + 1); ++k) {
				float u = k / ((float)desc.stacks_);
				if (Angle::ANGLE_90 == desc.angle_) u *= 0.5f;
				else if (Angle::ANGLE_45 == desc.angle_) u *= 0.5f;
				lib::Vector3 vv = lib::Vector3(cosf(lib::Math::PI * u) * r, y, 0);
				if ((lib::Math::PI * v) >= lib::Math::toRadian(45) && Angle::ANGLE_45 == desc.angle_) {
					float x = cosf(lib::Math::PI * lib::Math::PI * 0.5f) * r;
					vv.x = 0;
					vv.y = desc.radius_ - ((float(i) / float(desc.slices_)) * desc.radius_) * 2;
				}
				int a = (i * (desc.stacks_ + 1)) + k;

				vtxs[a].position = vv;
				
				vtxs[a].uv.x = 0.5f - (vv.x / desc.radius_ * 0.5f);
				vtxs[a].uv.y = 0.5f - (vv.y / desc.radius_ * 0.5f);

				vtxs[a].normal = lib::Vector3::FORWARD;
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// �C���f�b�N�X�̌v�Z
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// �o�b�t�@�փR�s�[
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createDiskRing(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// ���ɓo�^����Ă���ꍇ�͂����Ԃ�
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// �o�b�t�@�̍쐬
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		float thickness = (desc.thickness_ > desc.radius_) ? desc.radius_ : desc.thickness_;
		float inner_radius = desc.radius_ - thickness;

		// ���_���W�EUV�E�@���̌v�Z
		for (int i = 0; i < (desc.slices_ + 1); ++i) {
			float v = i / ((float)desc.slices_) * 0.5f;
			float y = cosf(lib::Math::PI * v) * desc.radius_;
			float r = (1.0f / (float)desc.slices_) * (desc.radius_ - y) * desc.radius_;
			r *= (thickness / desc.radius_);
			float angle_ofs = 0;
			if (Angle::ANGLE_90 == desc.angle_) angle_ofs = lib::Math::toRadian(45);
			else if (Angle::ANGLE_45 == desc.angle_) angle_ofs = lib::Math::toRadian(67.5);
			for (int k = 0; k < (desc.stacks_ + 1); ++k) {
				float u = k / ((float)desc.stacks_);
				if (Angle::ANGLE_180 == desc.angle_) u /= 2;
				else if (Angle::ANGLE_90 == desc.angle_) u /= 4;
				else if (Angle::ANGLE_45 == desc.angle_) u /= 8;
				lib::Vector3 vv;
				float rd = angle_ofs + 2 * lib::Math::PI * u;
				vv.x = -(cosf(rd) * r) - (cosf(rd) * inner_radius);
				vv.y = (sinf(rd) * r) + (sinf(rd) * inner_radius);
				vv.z = 0;

				int a = (i * (desc.stacks_ + 1)) + k;
				vtxs[a].position = vv;

				vtxs[a].uv.x = 1.0f / (float)desc.stacks_ * k;
				vtxs[a].uv.y = 1.0f / (float)desc.slices_ * i;

				vtxs[a].normal = lib::Vector3::FORWARD;
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// �C���f�b�N�X�̌v�Z
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// �o�b�t�@�փR�s�[
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}

	Shape::s_ptr Shape::createCylinder(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// ���ɓo�^����Ă���ꍇ�͂����Ԃ�
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// �o�b�t�@�̍쐬
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// ���_���W�EUV�E�@���̌v�Z
		for (int i = 0; i < (desc.slices_ + 1); ++i) {
			float r = desc.radius_;
			float angle_ofs = 0;
			if (Angle::ANGLE_90 == desc.angle_) angle_ofs = lib::Math::toRadian(45);
			else if (Angle::ANGLE_45 == desc.angle_) angle_ofs = lib::Math::toRadian(67.5);
			for (int k = 0; k < (desc.stacks_ + 1); ++k) {
				float u = k / ((float)desc.stacks_);
				if (Angle::ANGLE_180 == desc.angle_) u /= 2;
				else if (Angle::ANGLE_90 == desc.angle_) u /= 4;
				else if (Angle::ANGLE_45 == desc.angle_) u /= 8;
				lib::Vector3 v;
				v.x = (cosf(angle_ofs + 2 * lib::Math::PI * u)) * r;
				v.y = (desc.height_ * 0.5f) - (desc.height_ / desc.slices_ * i);
				v.z = (sinf(angle_ofs + 2 * lib::Math::PI * u)) * r;

				int a = (i * (desc.stacks_ + 1)) + k;
				vtxs[a].position = v;

				vtxs[a].uv.x = 1.0f / (float)desc.stacks_ * k;
				vtxs[a].uv.y = 1.0f / (float)desc.slices_ * i;

				vtxs[a].normal = lib::Vector3::normalize(v);
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// �C���f�b�N�X�̌v�Z
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// �o�b�t�@�փR�s�[
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
}
