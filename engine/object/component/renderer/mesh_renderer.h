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
		// �����o�֐�

		// �`��
		void render(std::shared_ptr<Camera> camera) final override;

		//====================================================================================================
	public:
		MeshRenderer() {}
		~MeshRenderer() {}

		//====================================================================================================
		// �����o�ϐ�

		std::vector<std::shared_ptr<Mesh>> meshs_;	// �`�悷�郁�b�V��

		//====================================================================================================
	};

}
