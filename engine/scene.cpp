#include "graphics_manager.h"
#include "scene.h"
#include "object/game_object.h"
#include "object/pipeline_state.h"
#include "object/component/camera.h"
#include "object/component/renderer/renderer.h"


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

		if (FAILED(mgr.command_allocator_->Reset())) return;
		if (FAILED(mgr.command_list_->Reset(mgr.command_allocator_.Get(), mgr.default_pso_->pso_.Get()))) return;

		mgr.command_list_->RSSetViewports(1, &sys::Window::VIEWPORT);
		mgr.command_list_->RSSetScissorRects(1, &sys::Window::SCISSOR_RECT);

		// 【 バックバッファが描画ターゲットとして使用できるようになるまで待つ 】
		// 複数個の GPUコアが 並列作業をする dx12 では
		// GPUコア で使用されるリソースにバリアを張るらしい
		// 例えば とある GPUコア で使用中のリソースを他の GPUコア が勝手に触れられないようにする処置だろうか
		// この場合はレンダーターゲット( 描画対象のバックバッファ )にバリアを張っている
		{
			D3D12_RESOURCE_BARRIER	resourceBarrier = {};
			resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;				// リソースの状態遷移に対して設置
			resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			resourceBarrier.Transition.pResource = mgr.rtv_buffer_[mgr.frame_index_].Get();		// リソースは描画ターゲット
			resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;		// 遷移前はPresent
			resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;	// 遷移後は描画ターゲット
			resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			mgr.command_list_->ResourceBarrier(1, &resourceBarrier);
		}

		// レンダーターゲットの設定
		// カレントバッファを使用する
		mgr.command_list_->OMSetRenderTargets(1, &mgr.rtv_handle_[mgr.frame_index_], FALSE, &mgr.dsv_handle_);


		// 深度ステンシルビューとレンダーターゲットビューのクリア
		const float	clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		mgr.command_list_->ClearDepthStencilView(mgr.dsv_handle_, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		mgr.command_list_->ClearRenderTargetView(mgr.rtv_handle_[mgr.frame_index_], clearColor, 0, nullptr);


		//==================================================


		std::list<std::shared_ptr<GameObject>>::iterator it = game_objects_.begin();
		while (it != game_objects_.end()) {
			if ((*it)->isActive()) {
				(*it)->render(camera_);
			}
			++it;
		}


		//==================================================


		// バックバッファの描画完了を待つためのバリアを設置
		{
			D3D12_RESOURCE_BARRIER	resourceBarrier = {};
			resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;					// リソースの状態遷移に対して設置
			resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			resourceBarrier.Transition.pResource = mgr.rtv_buffer_[mgr.frame_index_].Get();			// リソースは描画ターゲット
			resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	// 遷移前は描画ターゲット
			resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			// 遷移後はPresent
			resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			mgr.command_list_->ResourceBarrier(1, &resourceBarrier);
		}

		if (FAILED(mgr.command_list_->Close())) return;

		// コマンドリストを実行
		ID3D12CommandList* ppCommandLists[] = { mgr.command_list_.Get() };
		mgr.command_queue_->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		// フレームを最終出力
		if (FAILED(mgr.swap_chain_->Present(1, 0))) return;

	}

}
