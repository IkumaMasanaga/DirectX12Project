#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "../../library/library.h"


namespace eng {

	struct Vertex3D {
		lib::Vector3 position;
		lib::Vector3 normal;
		lib::Vector2 uv;
	};

	class Shape final : public lib::SmartFactory {
		// 登録するため
		friend class lib::SharedFlyweightMap<std::string, Shape>;
	private:
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
	public:
		using s_ptr = std::shared_ptr<Shape>;
		using w_ptr = std::weak_ptr<Shape>;

		enum class Angle {
			ANGLE_45,
			ANGLE_90,
			ANGLE_180,
			ANGLE_360,
		};

		struct CreateDesc {
			std::string regist_name_ = "";
			float width_ = 10.0f;
			float height_ = 10.0f;
			float depth_ = 10.0f;
			float radius_ = 5.0f;
			float thickness_ = 3.0f;
			float egge_lenght_ = 10.0f;
			int stacks_ = 10;
			int slices_ = 10;
			Angle angle_ = Angle::ANGLE_360;
		};

	private:
		//====================================================================================================
		// メンバ変数

		ComPtr<ID3D12Resource> vbo_;	// 頂点バッファ
		ComPtr<ID3D12Resource> ibo_;	// インデックスバッファ
		int vertex_num_ = 0;			// 頂点数
		int index_num_ = 0;				// インデックス数

		//====================================================================================================
		// static変数

		inline static lib::SharedFlyweightMap<std::string, Shape> regist_map_;	// 登録マップ

		//====================================================================================================
		// メンバ関数

		// 頂点バッファ、インデックスバッファの作成
		bool createBuffers();

		// 頂点バッファ、インデックスバッファへコピー
		bool copyBuffers(Vertex3D* vertexs, uint32_t* indexs);

		// インデックスの計算
		void calcIndexs(uint32_t*& indexs, const int slices, const int stacks);

		//====================================================================================================
		// static関数

		// 既に生成されているか
		static bool isCreated(Shape::s_ptr& ret_obj, const std::string& regist_name);

		// SharedFlyweightMapでの生成
		static Shape::s_ptr loadOfFlyweight(void* none);

		//====================================================================================================
	public:
		Shape() {}
		~Shape() {}

		//====================================================================================================
		// メンバ関数

		// ゲッター
		inline ComPtr<ID3D12Resource> const getVertexBuffer() { return vbo_; }
		inline ComPtr<ID3D12Resource> const getIndexBuffer() { return ibo_; }
		inline int getVertexNum() const { return vertex_num_; }
		inline int getIndexNum() const { return index_num_; }
		inline D3D12_VERTEX_BUFFER_VIEW getVertexView() const { return { vbo_->GetGPUVirtualAddress(), sizeof(Vertex3D) * vertex_num_, sizeof(Vertex3D) }; }
		inline D3D12_INDEX_BUFFER_VIEW getIndexView() const { return { ibo_->GetGPUVirtualAddress(), sizeof(uint32_t) * index_num_, DXGI_FORMAT_R32_UINT }; }

		//====================================================================================================
		// static関数

		// 四角形(X,Y軸)
		static Shape::s_ptr createPlaneXY(const CreateDesc& desc);
		// 四角形(Y,Z軸)
		static Shape::s_ptr createPlaneYZ(const CreateDesc& desc);
		// 四角形(Z,X軸)
		static Shape::s_ptr createPlaneZX(const CreateDesc& desc);
		// 二等辺三角形
		static Shape::s_ptr createTriangleIsosceles(const CreateDesc& desc);
		// 直角三角形(右下が直角)
		static Shape::s_ptr createTriangleRight(const CreateDesc& desc);
		// 直角三角形(左下が直角)
		static Shape::s_ptr createTriangleLeft(const CreateDesc& desc);
		// 正三角形
		static Shape::s_ptr createTriangleEquilateral(const CreateDesc& desc);
		// 球体
		static Shape::s_ptr createSphere(const CreateDesc& desc);
		// ドーム
		static Shape::s_ptr createDome(const CreateDesc& desc);
		// 底面なしの円錐
		static Shape::s_ptr createCone(const CreateDesc& desc);
		// 円盤
		static Shape::s_ptr createDisk(const CreateDesc& desc);
		// 円盤状のリング 
		static Shape::s_ptr createDiskRing(const CreateDesc& desc);
		// 円筒
		static Shape::s_ptr createCylinder(const CreateDesc& desc);

		//====================================================================================================
	};

}
