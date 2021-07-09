#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include "../../system/system.h"
#include "../../library/library.h"


namespace eng {

	class DescriptorManager;
	class RenderTargetView;
	class DepthStencilView;
	class PipelineState;
	class Shader;
	class Texture;

	class GraphicsManager final : public lib::Singleton<GraphicsManager> {
		// ���N���X�Ő������邽��
		friend class lib::Singleton<GraphicsManager>;
		// initialize, finalize���ĂԂ���
		friend class sys::System;
	private:
		GraphicsManager() {}

		//====================================================================================================
		// static�萔

		inline static constexpr UINT FRAME_COUNT = 2;				// �t���[����
		inline static constexpr UINT DESCRIPTOR_RTV_NUM = 100;		// �����_�[�^�[�Q�b�g�r���[
		inline static constexpr UINT DESCRIPTOR_DSV_NUM = 100;		// �[�x�X�e���V���r���[
		inline static constexpr UINT DESCRIPTOR_SRV_NUM = 2048;		// �V�F�[�_�[���\�[�X�r���[

		//====================================================================================================
		// �����o�֐�

		// ������
		bool initialize();

		// �I������
		void finalize();

		// �`�抮���̓��������
		bool waitForPreviousFrame();

		//====================================================================================================
	public:
		~GraphicsManager() {}
		
		//====================================================================================================
		// �����o�ϐ�

		UINT frame_index_ = 0;												// ���݂̕`��t���[��
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> command_queue_;			// �R�}���h�L���[
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> command_allocator_;	// �R�}���h�A���P�[�^�[
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> command_list_;	// �R�}���h���X�g
		Microsoft::WRL::ComPtr<IDXGISwapChain3> swap_chain_;				// �X���b�v�`�F�C��

		std::shared_ptr<PipelineState> default_pso_ = nullptr;	// �f�t�H���g��PSO
		std::shared_ptr<Shader> default_shader_ = nullptr;		// �f�t�H���g�̃V�F�[�_
		std::shared_ptr<Texture> default_texture_ = nullptr;	// �f�t�H���g�̃e�N�X�`��

		//--------------------------------------------------
		// Layer�ɓ�������H
		
		std::shared_ptr<RenderTargetView> rtv_[FRAME_COUNT] = {};	// �����_�[�^�[�Q�b�g�r���[
		std::shared_ptr<DepthStencilView> dsv_ = nullptr;			// �[�x�X�e���V���r���[

		//--------------------------------------------------

		std::shared_ptr<DescriptorManager> rtv_heap_ = nullptr;	// �����_�[�^�[�Q�b�g�r���[�̃f�B�X�N���v�^�q�[�v
		std::shared_ptr<DescriptorManager> dsv_heap_ = nullptr;	// �[�x�X�e���V���r���[�̃f�B�X�N���v�^�q�[�v
		std::shared_ptr<DescriptorManager> srv_heap_ = nullptr;	// �V�F�[�_�[���\�[�X�r���[�̃f�B�X�N���v�^�q�[�v

		//====================================================================================================
	};

}
