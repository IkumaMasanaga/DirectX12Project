#include "../../system/d3dx12.h"
#include "../../system/dx12_manager.h"
#include "shape.h"


namespace eng {

	bool Shape::createBuffers() {
		ComPtr<ID3D12Device> device = sys::Dx12Manager::getInstance().getDevice();

		D3D12_HEAP_PROPERTIES heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC   resource_desc = {};

		//頂点バッファの作成
		resource_desc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex3D) * vertex_num_);
		if (FAILED(device->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vbo_)))) return false;

		//インデックスバッファの作成
		resource_desc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(uint32_t) * index_num_);
		if (FAILED(device->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&ibo_)))) return false;

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
		if (isCreated(ptr, desc.regist_name)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices + 1) * (desc.stacks + 1);
		ptr->index_num_ = desc.slices * desc.stacks * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (desc.slices + 1); ++i) {	// x軸に平行に分割 (yが変化)
			for (int k = 0; k < (desc.stacks + 1); ++k) {	// y軸に平行に分割 (xが変化)
				int a = (i * (desc.stacks + 1)) + k;
				vtxs[a].position.x = (desc.width * 0.5f) - (desc.width / desc.stacks * k);
				vtxs[a].position.y = (desc.height * 0.5f) - (desc.height / desc.slices * i);
				vtxs[a].position.z = 0;

				vtxs[a].uv.x = 1.0f / (float)desc.stacks * k;
				vtxs[a].uv.y = 1.0f / (float)desc.slices * i;

				vtxs[a].normal = lib::Vector3::FORWARD;
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices, desc.stacks);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}

	Shape::s_ptr Shape::createPlaneYZ(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices + 1) * (desc.stacks + 1);
		ptr->index_num_ = desc.slices * desc.stacks * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (desc.slices + 1); ++i) {
			for (int k = 0; k < (desc.stacks + 1); ++k) {
				int a = (i * (desc.stacks + 1)) + k;
				vtxs[a].position.x = 0;
				vtxs[a].position.y = (desc.height * 0.5f) - (desc.height / desc.slices * i);
				vtxs[a].position.z = -(desc.depth * 0.5f) + (desc.depth / desc.stacks * k);

				vtxs[a].uv.x = 1.0f / (float)desc.stacks * k;
				vtxs[a].uv.y = 1.0f / (float)desc.slices * i;

				vtxs[a].normal = lib::Vector3::RIGHT;
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices, desc.stacks);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createPlaneZX(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices + 1) * (desc.stacks + 1);
		ptr->index_num_ = desc.slices * desc.stacks * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (desc.slices + 1); ++i) {
			for (int k = 0; k < (desc.stacks + 1); ++k) {
				int a = (i * (desc.stacks + 1)) + k;
				vtxs[a].position.x = -(desc.width * 0.5f) + (desc.width / desc.stacks * k);
				vtxs[a].position.y = 0;
				vtxs[a].position.z = (desc.depth * 0.5f) - (desc.depth / desc.slices * i);

				vtxs[a].uv.x = 1.0f / (float)desc.stacks * k;
				vtxs[a].uv.y = 1.0f / (float)desc.slices * i;

				vtxs[a].normal = lib::Vector3::UP;
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices, desc.stacks);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}

	Shape::s_ptr Shape::createTriangleIsosceles(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices + 1) * (desc.stacks + 1);
		ptr->index_num_ = desc.slices * desc.stacks * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		float ww = desc.width * 0.5f;
		float hh = desc.height * 0.5f;
		lib::Vector3 c = (lib::Vector3(0, hh, 0) + lib::Vector3(-ww, -hh, 0) + lib::Vector3(ww, -hh, 0)) / 3.0f;

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (desc.slices + 1); ++i) {
			for (int k = 0; k < (desc.stacks + 1); ++k) {
				int a = (i * (desc.stacks + 1)) + k;
				vtxs[a].position.x = -c.x + ((desc.width * 0.5f) - (desc.width / desc.stacks * k)) * (1.0f / desc.slices * i);
				vtxs[a].position.y = -c.y + (desc.height * 0.5f) - (desc.height / desc.slices * i);
				vtxs[a].position.z = 0;

				vtxs[a].uv.x = 0.5f - (vtxs[(i * (desc.stacks + 1)) + k].position.x / desc.width);
				vtxs[a].uv.y = 1.0f / (float)desc.slices * i;

				vtxs[a].normal = lib::Vector3::FORWARD;
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices, desc.stacks);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createTriangleRight(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices + 1) * (desc.stacks + 1);
		ptr->index_num_ = desc.slices * desc.stacks * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (desc.slices + 1); ++i) {
			for (int k = 0; k < (desc.stacks + 1); ++k) {
				int a = (i * (desc.stacks + 1)) + k;
				vtxs[a].position.x = (-(desc.width * 0.5f) + (i * (desc.width / desc.stacks))) - ((desc.width / desc.stacks * k) * (i * (1.0f / desc.slices)));
				vtxs[a].position.y = (desc.height * 0.5f) - (desc.height / desc.slices * i);
				vtxs[a].position.z = 0;

				vtxs[a].uv.x = 0.5f - vtxs[a].position.x / desc.width;
				vtxs[a].uv.y = 1.0f / (float)desc.slices * i;

				vtxs[a].normal = lib::Vector3::FORWARD;
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices, desc.stacks);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}

	Shape::s_ptr Shape::createTriangleLeft(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices + 1) * (desc.stacks + 1);
		ptr->index_num_ = desc.slices * desc.stacks * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (desc.slices + 1); ++i) {
			for (int k = 0; k < (desc.stacks + 1); ++k) {
				int a = (i * (desc.stacks + 1)) + k;
				vtxs[a].position.x = (desc.width * 0.5f) - ((desc.width / desc.stacks * k) * (i * (1.0f / desc.slices)));
				vtxs[a].position.y = (desc.height * 0.5f) - (desc.height / desc.slices * i);
				vtxs[a].position.z = 0;

				vtxs[a].uv.x = 0.5f - vtxs[a].position.x / desc.width;
				vtxs[a].uv.y = 1.0f / (float)desc.slices * i;

				vtxs[a].normal = lib::Vector3::FORWARD;
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices, desc.stacks);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createTriangleEquilateral(const CreateDesc& desc) {
		float width = desc.egge_lenght;
		float height = desc.egge_lenght * sqrt(3.0f) * 0.5f;
		Shape::CreateDesc d;
		d.width = width;
		d.height = height;
		d.stacks = desc.stacks;
		d.slices = desc.slices;
		return createTriangleIsosceles(d);
	}
	
	Shape::s_ptr Shape::createSphere(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices + 1) * (desc.stacks + 1);
		ptr->index_num_ = desc.slices * desc.stacks * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (desc.slices + 1); ++i) {
			float s = i / ((float)desc.slices);
			float y = cosf(lib::Math::PI * s) * desc.radius;
			float r = sinf(lib::Math::PI * s) * desc.radius;
			for (int k = 0; k < (desc.stacks + 1); ++k) {
				float t = k / ((float)desc.stacks);
				lib::Vector3 v = lib::Vector3(cosf(2 * lib::Math::PI * t) * r, y, sinf(2 * lib::Math::PI * t) * r);
				int a = (i * (desc.stacks + 1)) + k;
				vtxs[a].position = v;

				vtxs[a].uv.x = 1.0f / (float)desc.stacks * k;
				vtxs[a].uv.y = 1.0f / (float)desc.slices * i;

				vtxs[a].normal = lib::Vector3::normalize(v);
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices, desc.stacks);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createDome(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices + 1) * (desc.stacks + 1);
		ptr->index_num_ = desc.slices * desc.stacks * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (desc.slices + 1); ++i) {
			float v = i / ((float)desc.slices) * 0.5f;
			float y = cosf(lib::Math::PI * v) * desc.radius;
			float r = sinf(lib::Math::PI * v) * desc.radius;
			float angle_ofs = 0;
			if (Angle::ANGLE_90 == desc.angle) angle_ofs = lib::Math::toRadian(45);
			else if (Angle::ANGLE_45 == desc.angle) angle_ofs = lib::Math::toRadian(67.5);
			for (int k = 0; k < (desc.stacks + 1); ++k) {
				float u = k / ((float)desc.stacks);
				if (Angle::ANGLE_180 == desc.angle) u /= 2;
				else if (Angle::ANGLE_90 == desc.angle) u /= 4;
				else if (Angle::ANGLE_45 == desc.angle) u /= 8;
				lib::Vector3 vv;
				vv.x = cosf(angle_ofs + 2 * lib::Math::PI * u) * r;
				vv.y = y;
				vv.z = sinf(angle_ofs + 2 * lib::Math::PI * u) * r;

				int a = (i * (desc.stacks + 1)) + k;
				vtxs[a].position = vv;

				vtxs[a].uv.x = 1.0f / (float)desc.stacks * k;
				vtxs[a].uv.y = 1.0f / (float)desc.slices * i;

				vtxs[a].normal = lib::Vector3::normalize(vv);
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices, desc.stacks);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createCone(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices + 1) * (desc.stacks + 1);
		ptr->index_num_ = desc.slices * desc.stacks * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (desc.slices + 1); ++i) {
			float r = (desc.radius / desc.slices) * i;
			float angle_ofs = 0;
			if (Angle::ANGLE_90 == desc.angle) angle_ofs = lib::Math::toRadian(45);
			else if (Angle::ANGLE_45 == desc.angle) angle_ofs = lib::Math::toRadian(67.5);
			for (int k = 0; k < (desc.stacks + 1); ++k) {
				float u = k / ((float)desc.stacks);
				if (Angle::ANGLE_180 == desc.angle) u /= 2;
				else if (Angle::ANGLE_90 == desc.angle) u /= 4;
				else if (Angle::ANGLE_45 == desc.angle) u /= 8;
				lib::Vector3 vv;
				vv.x = (cosf(angle_ofs + 2 * lib::Math::PI * u)) * r;
				vv.y = desc.height - (desc.height / desc.slices * i);
				vv.z = (sinf(angle_ofs + 2 * lib::Math::PI * u)) * r;

				int a = (i * (desc.stacks + 1)) + k;
				vtxs[a].position = vv;

				vtxs[a].uv.x = 1.0f / (float)desc.stacks * k;
				vtxs[a].uv.y = 1.0f / (float)desc.slices * i;

				vtxs[a].normal = lib::Vector3::normalize(vv);
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices, desc.stacks);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createDisk(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices + 1) * (desc.stacks + 1);
		ptr->index_num_ = desc.slices * desc.stacks * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (desc.slices + 1); ++i) {
			float v = i / ((float)desc.slices);
			if (Angle::ANGLE_180 == desc.angle) v *= 0.5f;
			else if (Angle::ANGLE_90 == desc.angle) v *= 0.5f;
			else if (Angle::ANGLE_45 == desc.angle) v *= 0.5f;
			float y = cosf(lib::Math::PI * v) * desc.radius;
			float r = sinf(lib::Math::PI * v) * desc.radius;
			for (int k = 0; k < (desc.stacks + 1); ++k) {
				float u = k / ((float)desc.stacks);
				if (Angle::ANGLE_90 == desc.angle) u *= 0.5f;
				else if (Angle::ANGLE_45 == desc.angle) u *= 0.5f;
				lib::Vector3 vv = lib::Vector3(cosf(lib::Math::PI * u) * r, y, 0);
				if ((lib::Math::PI * v) >= lib::Math::toRadian(45) && Angle::ANGLE_45 == desc.angle) {
					float x = cosf(lib::Math::PI * lib::Math::PI * 0.5f) * r;
					vv.x = 0;
					vv.y = desc.radius - ((float(i) / float(desc.slices)) * desc.radius) * 2;
				}
				int a = (i * (desc.stacks + 1)) + k;

				vtxs[a].position = vv;
				
				vtxs[a].uv.x = 0.5f - (vv.x / desc.radius * 0.5f);
				vtxs[a].uv.y = 0.5f - (vv.y / desc.radius * 0.5f);

				vtxs[a].normal = lib::Vector3::FORWARD;
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices, desc.stacks);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
	Shape::s_ptr Shape::createDiskRing(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices + 1) * (desc.stacks + 1);
		ptr->index_num_ = desc.slices * desc.stacks * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		float thickness = (desc.thickness > desc.radius) ? desc.radius : desc.thickness;
		float inner_radius = desc.radius - thickness;

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (desc.slices + 1); ++i) {
			float v = i / ((float)desc.slices) * 0.5f;
			float y = cosf(lib::Math::PI * v) * desc.radius;
			float r = (1.0f / (float)desc.slices) * (desc.radius - y) * desc.radius;
			r *= (thickness / desc.radius);
			float angle_ofs = 0;
			if (Angle::ANGLE_90 == desc.angle) angle_ofs = lib::Math::toRadian(45);
			else if (Angle::ANGLE_45 == desc.angle) angle_ofs = lib::Math::toRadian(67.5);
			for (int k = 0; k < (desc.stacks + 1); ++k) {
				float u = k / ((float)desc.stacks);
				if (Angle::ANGLE_180 == desc.angle) u /= 2;
				else if (Angle::ANGLE_90 == desc.angle) u /= 4;
				else if (Angle::ANGLE_45 == desc.angle) u /= 8;
				lib::Vector3 vv;
				float rd = angle_ofs + 2 * lib::Math::PI * u;
				vv.x = -(cosf(rd) * r) - (cosf(rd) * inner_radius);
				vv.y = (sinf(rd) * r) + (sinf(rd) * inner_radius);
				vv.z = 0;

				int a = (i * (desc.stacks + 1)) + k;
				vtxs[a].position = vv;

				vtxs[a].uv.x = 1.0f / (float)desc.stacks * k;
				vtxs[a].uv.y = 1.0f / (float)desc.slices * i;

				vtxs[a].normal = lib::Vector3::FORWARD;
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices, desc.stacks);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}

	Shape::s_ptr Shape::createCylinder(const CreateDesc& desc) {
		Shape::s_ptr ptr = nullptr;
		if (isCreated(ptr, desc.regist_name)) return ptr;		// 既に登録されている場合はそれを返す
		ptr->vertex_num_ = (desc.slices + 1) * (desc.stacks + 1);
		ptr->index_num_ = desc.slices * desc.stacks * 6;
		if (!ptr->createBuffers()) return nullptr;				// バッファの作成
		Vertex3D* vtxs = new Vertex3D[ptr->vertex_num_];
		uint32_t* idxs = new uint32_t[ptr->index_num_];

		//--------------------------------------------------

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (desc.slices + 1); ++i) {
			float r = desc.radius;
			float angle_ofs = 0;
			if (Angle::ANGLE_90 == desc.angle) angle_ofs = lib::Math::toRadian(45);
			else if (Angle::ANGLE_45 == desc.angle) angle_ofs = lib::Math::toRadian(67.5);
			for (int k = 0; k < (desc.stacks + 1); ++k) {
				float u = k / ((float)desc.stacks);
				if (Angle::ANGLE_180 == desc.angle) u /= 2;
				else if (Angle::ANGLE_90 == desc.angle) u /= 4;
				else if (Angle::ANGLE_45 == desc.angle) u /= 8;
				lib::Vector3 v;
				v.x = (cosf(angle_ofs + 2 * lib::Math::PI * u)) * r;
				v.y = (desc.height * 0.5f) - (desc.height / desc.slices * i);
				v.z = (sinf(angle_ofs + 2 * lib::Math::PI * u)) * r;

				int a = (i * (desc.stacks + 1)) + k;
				vtxs[a].position = v;

				vtxs[a].uv.x = 1.0f / (float)desc.stacks * k;
				vtxs[a].uv.y = 1.0f / (float)desc.slices * i;

				vtxs[a].normal = lib::Vector3::normalize(v);
			}
		}

		//--------------------------------------------------

		ptr->calcIndexs(idxs, desc.slices, desc.stacks);	// インデックスの計算
		bool is_complete = ptr->copyBuffers(vtxs, idxs);	// バッファへコピー
		delete[] vtxs;
		delete[] idxs;
		return (is_complete) ? ptr : nullptr;
	}
	
}
