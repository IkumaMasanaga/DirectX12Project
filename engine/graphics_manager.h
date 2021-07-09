#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include "../system/system.h"
#include "../library/library.h"


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

		// �t���[����
		// �\�ʂƗ���
		inline static constexpr UINT FRAME_COUNT = 2;

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

		// �����_�[�^�[�Q�b�g�r���[
		// �I�t�X�N���[�������_�����O���g�p����ʕ�������ۂɂ͕����K�v�H
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtv_heap_;
		Microsoft::WRL::ComPtr<ID3D12Resource> rtv_buffer_[FRAME_COUNT] = {};
		D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle_[FRAME_COUNT] = {};

		// �[�x�X�e���V���r���[
		// �I�t�X�N���[�������_�����O���g�p����ʕ�������ۂɂ͕����K�v�H
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsv_heap_;
		Microsoft::WRL::ComPtr<ID3D12Resource> dsv_buffer_;
		D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle_ = {};

		//----------------------------------------------------------------------------------------------------

		//// �����_�[�^�[�Q�b�g�r���[
		//std::shared_ptr<RenderTargetView> rtv_[FRAME_COUNT] = {};

		//// �[�x�X�e���V���r���[
		//std::shared_ptr<DepthStencilView> dsv_ = nullptr;

		//std::shared_ptr<DescriptorManager> rtv_heap_ = nullptr;	// �����_�[�^�[�Q�b�g�r���[�̃f�B�X�N���v�^�q�[�v
		//std::shared_ptr<DescriptorManager> dsv_heap_ = nullptr;	// �[�x�X�e���V���r���[�̃f�B�X�N���v�^�q�[�v
		//std::shared_ptr<DescriptorManager> srv_heap_ = nullptr;	// �V�F�[�_�[���\�[�X�r���[�̃f�B�X�N���v�^�q�[�v

		//====================================================================================================
	};

}
