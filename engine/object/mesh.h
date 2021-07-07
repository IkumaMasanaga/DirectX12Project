#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "object.h"
#include "../transform.h"


namespace eng {

	class Shape;
	class Material;
	class PipelineState;

	class Mesh final : public Object {
	public:
		using s_ptr = std::shared_ptr<Mesh>;
		using w_ptr = std::weak_ptr<Mesh>;
	private:
		//====================================================================================================
		// メンバ関数

		// 定数バッファの作成
		bool createCBV();

		//====================================================================================================
	public:
		Mesh() {}
		~Mesh() {}

		//====================================================================================================
		// メンバ変数

		Microsoft::WRL::ComPtr<ID3D12Resource> cbv_;	// 定数バッファ
		std::shared_ptr<Shape> shape_ = nullptr;		// 形状データ
		std::shared_ptr<Material> material_ = nullptr;	// マテリアル
		std::shared_ptr<PipelineState> pso_ = nullptr;	// パイプラインステートオブジェクト
		Transform<Mesh>::s_ptr transform_ = nullptr;	// 親子関係

		//====================================================================================================
		// static関数

		// 生成
		static Mesh::s_ptr createFromShape(std::shared_ptr<Shape> shape, const std::string& texture_file_path = "");

		//====================================================================================================
	};

}
