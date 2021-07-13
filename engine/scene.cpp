#include "graphics/graphics_manager.h"
#include "graphics/texture.h"
#include "scene.h"
#include "object/game_object.h"
#include "object/component/camera.h"
#include "object/component/renderer/renderer.h"	// addComponentでのエラー防止
#include "object/component/renderer/skybox.h"


using namespace Microsoft::WRL;

namespace eng {

	bool Scene::onCreated() {

		// メインカメラの作成
		GameObject::s_ptr obj = GameObject::createEmpty("main_camera");
		camera_ = obj->addComponent<Camera>();
		obj->transform_->local_position_.set(0.0f, 0.0f, -20.0f);

		// スカイボックスの作成
		Skybox::SetDesc desc;
		desc.camera = camera_;
		desc.texture_right = Texture::loadFromFile("engine/resources/skybox/right.bmp");		// 右
		desc.texture_left = Texture::loadFromFile("engine/resources/skybox/left.bmp");			// 左
		desc.texture_up = Texture::loadFromFile("engine/resources/skybox/up.bmp");				// 上
		desc.texture_down = Texture::loadFromFile("engine/resources/skybox/down.bmp");			// 下
		desc.texture_forward = Texture::loadFromFile("engine/resources/skybox/forward.bmp");	// 前
		desc.texture_back = Texture::loadFromFile("engine/resources/skybox/back.bmp");			// 後ろ
		camera_->addComponent<Skybox>()->set(desc);

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
			if ((*it)->isActiveParent()) {
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

		// コマンドアロケーターとコマンドリストのリセット
		if (!mgr.resetCommandList()) {
			lib_DebugLog("ERROR: GraphicsManager::resetCommandList");
			return;
		}

		// 描画前の処理
		// ビューポートの設定
		// PRESENT→RENDER_TERGETへ遷移のバリアをはる
		// レンダーターゲットの設定
		// レンダーターゲットと深度バッファのクリア
		mgr.renderBefore(sys::Window::VIEWPORT, sys::Window::SCISSOR_RECT, mgr.getMainRtv(), mgr.getMainDsv());

		// コマンドリストへ描画処理をため込む
		std::list<std::shared_ptr<GameObject>>::iterator it = game_objects_.begin();
		while (it != game_objects_.end()) {
			if ((*it)->isActiveParent()) {
				(*it)->render(camera_);
			}
			++it;
		}

		// RENDER_TERGET→PRESENTへ遷移のバリアをはる
		mgr.renderAfter(mgr.getMainRtv());

		// コマンドリストにため込まれた描画処理を最終出力
		if (!mgr.executeCommandList()) {
			lib_DebugLog("ERROR: GraphicsManager::executeCommandList");
			return;
		}

	}

}
