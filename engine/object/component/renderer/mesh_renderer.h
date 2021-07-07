#pragma once
#include "renderer.h"


namespace eng {

	class Mesh;

	class MeshRenderer final : public Renderer {
	public:
		using s_ptr = std::shared_ptr<MeshRenderer>;
		using w_ptr = std::weak_ptr<MeshRenderer>;
	private:
		//====================================================================================================
		// メンバ関数

		// 描画
		void render(std::shared_ptr<Camera> camera) final override;

		//====================================================================================================
	public:
		MeshRenderer() {}
		~MeshRenderer() {}

		//====================================================================================================
		// メンバ変数

		std::vector<std::shared_ptr<Mesh>> meshs_;	// 描画するメッシュ

		//====================================================================================================
	};

}
