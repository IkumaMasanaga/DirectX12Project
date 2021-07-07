#pragma once
#include "../library/library.h"
#include "../system/system.h"

#include "graphics_manager.h"
#include "scene.h"
#include "layer.h"
#include "transform.h"

#include "object/object.h"
#include "object/game_object.h"
#include "object/shape.h"
#include "object/texture.h"
#include "object/shader.h"
#include "object/material.h"
#include "object/pipeline_state.h"
#include "object/mesh.h"

#include "object/component/component.h"
#include "object/component/camera.h"

#include "object/component/renderer/renderer.h"
#include "object/component/renderer/mesh_renderer.h"


namespace eng {

	class Engine final : public lib::Singleton<Engine> {
		// ���N���X�Ő������邽��
		friend class lib::Singleton<Engine>;
		// initialize, update, finalize���ĂԂ���
		friend class sys::System;
	private:
		Engine() {}

		//====================================================================================================
		// �����o�ϐ�

		Scene::s_ptr now_ = nullptr;	// ���݂̃V�[��
		Scene::s_ptr next_ = nullptr;	// ���̃V�[��

		//====================================================================================================
		// �����o�֐�

		// ������
		bool initialize();

		// �X�V
		void update();

		// �I������
		void finalize();

		//====================================================================================================
	public:
		~Engine() {}
		
		//====================================================================================================
		// �����o�֐�

		// ���݂̃V�[���̎擾
		inline Scene::s_ptr getNowScene() const { return now_; }

		// �V�[���̐؂�ւ�
		inline void changeScene(const Scene::s_ptr next) { next_ = next; }

		//====================================================================================================
	};

}
