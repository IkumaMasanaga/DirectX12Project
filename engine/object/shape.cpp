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

		//頂点バッファの作成
		resource_desc.Width = sizeof(Vertex3D) * vertex_num_;
		if (FAILED(mgr.device_->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vbo_)))) return false;

		//インデックスバッファの作成
		resource_desc.Width = sizeof(uint32_t) * index_num_;	// インデックス数
		if (FAILED(mgr.device_->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&ibo_)))) return false;

		return true;
	}

	bool Shape::copyBuffers(Vertex3D* vertexs, uint32_t* indexs) {

		// 頂点バッファに頂点データをコピー
		Vertex3D* vb{};
		if (FAILED(vbo_->Map(0, nullptr, (void**)&vb))) return false;
		memcpy(vb, vertexs, sizeof(Vertex3D) * vertex_num_);
		vbo_->Unmap(0, nullptr);

		// インデックスバッファにインデックスデータをコピー
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
				// 左上の三角形
				indexs[a + 0] = (i * (stacks + 1)) + k;
				indexs[a + 1] = indexs[a + 0] + 1;
				indexs[a + 2] = indexs[a + 0] + (stacks + 1);
				// 右下の三角形
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
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (desc.slices_ + 1); ++i) {	// x軸に平行に分割 (yが変化)
			for (int k = 0; k < (desc.stacks_ + 1); ++k) {	// y軸に平行に分割 (xが変化)
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

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}

	Shape::s_ptr Shape::createPlaneYZ(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
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

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createPlaneZX(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
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

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}

	Shape::s_ptr Shape::createTriangleIsosceles(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		float ww = desc.width_ * 0.5f;
		float hh = desc.height_ * 0.5f;
		lib::Vector3 c = (lib::Vector3(0, hh, 0) + lib::Vector3(-ww, -hh, 0) + lib::Vector3(ww, -hh, 0)) / 3.0f;

		// 頂点座標・UV・法線の計算
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

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createTriangleRight(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (desc.slices_ + 1); ++i) {
			//float x = -(desc.width_ * 0.5f) + (desc.stacks_ * (i * (1.0f / desc.slices_)));
			for (int k = 0; k < (desc.stacks_ + 1); ++k) {
				int a = (i * (desc.stacks_ + 1)) + k;
				//vtxs[a].position.x = x - ((desc.width_ / desc.stacks_ * k) * (i * (1.0f / desc.slices_)));
				vtxs[a].position.x = (-(desc.width_ * 0.5f) + (i * (desc.width_ / desc.stacks_))) - ((desc.width_ / desc.stacks_ * k) * (i * (1.0f / desc.slices_)));
				vtxs[a].position.y = (desc.height_ * 0.5f) - (desc.height_ / desc.slices_ * i);
				vtxs[a].position.z = 0;

				vtxs[a].uv.x = 1.0f - vtxs[a].position.x / desc.width_;
				vtxs[a].uv.y = 1.0f / (float)desc.slices_ * i;

				vtxs[a].normal = lib::Vector3::FORWARD;
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}

	Shape::s_ptr Shape::createTriangleLeft(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (desc.slices_ + 1); ++i) {
			for (int k = 0; k < (desc.stacks_ + 1); ++k) {
				int a = (i * (desc.stacks_ + 1)) + k;
				vtxs[a].position.x = (desc.width_ * 0.5f) - ((desc.width_ / desc.stacks_ * k) * (i * (1.0f / desc.slices_)));
				vtxs[a].position.y = (desc.height_ * 0.5f) - (desc.height_ / desc.slices_ * i);
				vtxs[a].position.z = 0;

				vtxs[a].uv.x = 1.0f - vtxs[a].position.x / desc.width_;
				vtxs[a].uv.y = 1.0f / (float)desc.slices_ * i;

				vtxs[a].normal = lib::Vector3::FORWARD;
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
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
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
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

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createDome(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
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

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createCone(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
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

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createDisk(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
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

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createDiskRing(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		float thickness = (desc.thickness_ > desc.radius_) ? desc.radius_ : desc.thickness_;
		float inner_radius = desc.radius_ - thickness;

		// 頂点座標・UV・法線の計算
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

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createHollowOutDiskPlane(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// 既に登録されている場合はそれを返す
		CreateDesc d = desc;
		d.slices_ = 1;
		ptr->vertex_num_ = (d.slices_ + 1) * (d.stacks_ + 1);
		ptr->index_num_ = d.slices_ * d.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (d.slices_ + 1); ++i) {
			for (int k = 0; k < (d.stacks_ + 1); ++k) {
				float u = k / ((float)d.stacks_);
				if (Angle::ANGLE_180 == d.angle_) u /= 2;
				else if (Angle::ANGLE_90 == d.angle_) u /= 4;
				else if (Angle::ANGLE_45 == d.angle_) u /= 8;
				lib::Vector3 vv;
				float rd = 2 * lib::Math::PI * u;
				vv.x = (cosf(rd) * d.radius_);
				vv.y = (sinf(rd) * d.radius_);
				if (i == d.slices_) {
					vv = lib::Vector3::normalize(vv) * d.radius_ * 2;
					vv.x = (vv.x > d.radius_) ? d.radius_ : vv.x;
					vv.y = (vv.y > d.radius_) ? d.radius_ : vv.y;
					vv.x = (vv.x < -d.radius_) ? -d.radius_ : vv.x;
					vv.y = (vv.y < -d.radius_) ? -d.radius_ : vv.y;
				}

				vv.x = -vv.x;

				int a = (i * (d.stacks_ + 1)) + k;
				vtxs[a].position = vv;

				vtxs[a].uv.x = 0.5f - (vv.x / d.radius_ * 0.5f);
				vtxs[a].uv.y = 0.5f - (vv.y / d.radius_ * 0.5f);

				vtxs[a].normal = lib::Vector3::FORWARD;
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, d.slices_, d.stacks_);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createCylinder(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name_)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices_ + 1) * (desc.stacks_ + 1);
		ptr->index_num_ = desc.slices_ * desc.stacks_ * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
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

		ptr->calcIndexs(idxs, desc.slices_, desc.stacks_);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
}
