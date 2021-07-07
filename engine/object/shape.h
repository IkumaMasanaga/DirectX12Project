#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "object.h"


namespace eng {

	class Vertex3D {
	public:
		lib::Vector3 position;
		lib::Vector3 normal;
		lib::Vector2 uv;
	};

	class Shape final : public Object {
		friend class lib::SharedFlyweightMap<std::string, Shape>;
	public:
		using s_ptr = std::shared_ptr<Shape>;
		using w_ptr = std::weak_ptr<Shape>;

		struct CreateDesc {
			std::string regist_name_ = "";
			float width_ = 10.0f;
			float height_ = 10.0f;
			float depth_ = 10.0f;
			int stacks_ = 10;
			int slices_ = 10;
		};

	private:
		//====================================================================================================
		// static変数

		inline static lib::SharedFlyweightMap<std::string, Shape> regist_map_;	// 登録マップ

		//====================================================================================================
		// メンバ関数

		// 頂点バッファ、インデックスバッファの作成
		bool createBuffers();

		// 頂点バッファ、インデックスバッファへコピー
		bool copyBuffers(Vertex3D* vertexs, uint32_t* indexs);

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
		// メンバ変数

		Microsoft::WRL::ComPtr<ID3D12Resource> vbo_;	// 頂点バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> ibo_;	// インデックスバッファ
		int vertex_num_ = 0;	// 頂点数
		int index_num_ = 0;		// インデックス数

		//====================================================================================================
		// static関数

		// 四角形の生成(X,Y軸)
		static Shape::s_ptr createPlaneXY(const CreateDesc& desc);
		// 四角形の生成(Y,Z軸)
		static Shape::s_ptr createPlaneYZ(const CreateDesc& desc);
		// 四角形の生成(Z,X軸)
		static Shape::s_ptr createPlaneZX(const CreateDesc& desc);

		//====================================================================================================
	};

}
