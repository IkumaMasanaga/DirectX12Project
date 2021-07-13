#pragma once
#include "renderer.h"


namespace eng {

	class Mesh;

	class DefaultMeshRenderer : public Renderer {
	public:
		using s_ptr = std::shared_ptr<DefaultMeshRenderer>;
		using w_ptr = std::weak_ptr<DefaultMeshRenderer>;
	private:
		//====================================================================================================
		// メンバ関数

		// 描画
		void render(const std::shared_ptr<Camera>& camera) final override;

		//====================================================================================================
	public:
		DefaultMeshRenderer() {}
		~DefaultMeshRenderer() {}

		//====================================================================================================
		// メンバ変数

		std::vector<std::shared_ptr<Mesh>> meshs_;	// 描画するメッシュ

		//====================================================================================================
	};

}
