#include "../system/d3dx12.h"

#include "graphics/graphics_manager.h"
#include "graphics/render_target_view.h"
#include "graphics/depth_stencil_view.h"
#include "graphics/pipeline_state.h"
#include "scene.h"
#include "object/game_object.h"
#include "object/component/camera.h"
#include "object/component/renderer/renderer.h"


using namespace Microsoft::WRL;

namespace eng {

	bool Scene::onCreated() {

		GameObject::s_ptr obj = GameObject::createEmpty("main_camera");
		camera_ = obj->addComponent<Camera>();
		obj->transform_->local_position_.set(0.0f, 0.0f, -20.0f);

		initialize();

		return true;
	}

	void Scene::update() {
		
		std::list<std::shared_ptr<GameObject>>::iterator it = game_objects_.begin();
		while (it != game_objects_.end()) {
			if (!(*it)->isAlive()) {
				(*it)->onDestroy();
				it = game_objects_.erase(it);
				continue;
			}
			if ((*it)->isActive()) {
				if (!(*it)->is_enable_) {
					(*it)->onEnable();
					(*it)->is_enable_ = true;
				}
				(*it)->update();
			}
			else {
				if ((*it)->is_enable_) {
					(*it)->onDisable();
					(*it)->is_enable_ = false;
				}
			}
			++it;
		}

		lateUpdate();

	}

	void Scene::render() {

		GraphicsManager& mgr = GraphicsManager::getInstance();
		ComPtr<ID3D12GraphicsCommandList> com_list = mgr.getCommandList();

		if (FAILED(mgr.getCommandAllocator()->Reset())) return;
		if (FAILED(mgr.getCommandList()->Reset(mgr.getCommandAllocator().Get(), mgr.getDefaultPso()->getObject().Get()))) return;


		mgr.beforeRender(sys::Window::VIEWPORT, sys::Window::SCISSOR_RECT, mgr.getRtv(), mgr.getDsv());

		//==================================================


		std::list<std::shared_ptr<GameObject>>::iterator it = game_objects_.begin();
		while (it != game_objects_.end()) {
			if ((*it)->isActive()) {
				(*it)->render(camera_);
			}
			++it;
		}


		//==================================================

		mgr.afterRender(mgr.getRtv());

		mgr.executeCommandList();

	}

}
