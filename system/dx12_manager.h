#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include "../library/singleton.h"


namespace sys {

	class Dx12Manager final : public lib::Singleton<Dx12Manager> {
		// ���N���X�Ő������邽��
		friend class lib::Singleton<Dx12Manager>;
		// initialize, finalize���ĂԂ���
		friend class System;
	private:
		Dx12Manager() {}

		//====================================================================================================
		// �����o�֐�

		// ������
		bool initialize();

		// �I������
		void finalize();

		//====================================================================================================
	public:
		~Dx12Manager() {}

		//====================================================================================================
		// �����o�ϐ�

		// �n�[�h�E�F�A�A�_�v�^
		Microsoft::WRL::ComPtr<IDXGIFactory4> factory_;

		// �f�o�C�X
		Microsoft::WRL::ComPtr<ID3D12Device> device_;

		// D3D12CreateDevice�����s����ꍇ�̓r�f�I�J�[�h��DirectX12�ɑΉ����Ă��Ȃ��̂� is_use_warp_device_ ��true�ɂ���
		bool is_use_warp_device_ = false;

		// �t�F���X�֌W
		HANDLE fence_event_ = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
		UINT64 fence_value_ = 0;

		//====================================================================================================
	};

}
