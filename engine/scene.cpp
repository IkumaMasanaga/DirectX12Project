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

		// �y �o�b�N�o�b�t�@���`��^�[�Q�b�g�Ƃ��Ďg�p�ł���悤�ɂȂ�܂ő҂� �z
		// ������ GPU�R�A�� �����Ƃ����� dx12 �ł�
		// GPU�R�A �Ŏg�p����郊�\�[�X�Ƀo���A�𒣂�炵��
		// �Ⴆ�� �Ƃ��� GPU�R�A �Ŏg�p���̃��\�[�X�𑼂� GPU�R�A ������ɐG����Ȃ��悤�ɂ��鏈�u���낤��
		// ���̏ꍇ�̓����_�[�^�[�Q�b�g( �`��Ώۂ̃o�b�N�o�b�t�@ )�Ƀo���A�𒣂��Ă���
		{
			D3D12_RESOURCE_BARRIER	resourceBarrier = {};
			resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;				// ���\�[�X�̏�ԑJ�ڂɑ΂��Đݒu
			resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			resourceBarrier.Transition.pResource = mgr.rtv_buffer_[mgr.frame_index_].Get();		// ���\�[�X�͕`��^�[�Q�b�g
			resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;		// �J�ڑO��Present
			resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;	// �J�ڌ�͕`��^�[�Q�b�g
			resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			mgr.command_list_->ResourceBarrier(1, &resourceBarrier);
		}

		// �����_�[�^�[�Q�b�g�̐ݒ�
		// �J�����g�o�b�t�@���g�p����
		mgr.command_list_->OMSetRenderTargets(1, &mgr.rtv_handle_[mgr.frame_index_], FALSE, &mgr.dsv_handle_);


		// �[�x�X�e���V���r���[�ƃ����_�[�^�[�Q�b�g�r���[�̃N���A
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


		// �o�b�N�o�b�t�@�̕`�抮����҂��߂̃o���A��ݒu
		{
			D3D12_RESOURCE_BARRIER	resourceBarrier = {};
			resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;					// ���\�[�X�̏�ԑJ�ڂɑ΂��Đݒu
			resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			resourceBarrier.Transition.pResource = mgr.rtv_buffer_[mgr.frame_index_].Get();			// ���\�[�X�͕`��^�[�Q�b�g
			resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	// �J�ڑO�͕`��^�[�Q�b�g
			resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			// �J�ڌ��Present
			resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			mgr.command_list_->ResourceBarrier(1, &resourceBarrier);
		}

		if (FAILED(mgr.command_list_->Close())) return;

		// �R�}���h���X�g�����s
		ID3D12CommandList* ppCommandLists[] = { mgr.command_list_.Get() };
		mgr.command_queue_->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		// �t���[�����ŏI�o��
		if (FAILED(mgr.swap_chain_->Present(1, 0))) return;

	}

}
