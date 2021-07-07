#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "object.h"


namespace eng {

	class Texture final : public Object {
		friend class lib::SharedFlyweightMap<std::string, Texture>;
	public:
		using s_ptr = std::shared_ptr<Texture>;
		using w_ptr = std::weak_ptr<Texture>;
	private:
		//====================================================================================================
		// static�ϐ�

		inline static lib::SharedFlyweightMap<std::string, Texture> regist_map_;	// �o�^�}�b�v

		//====================================================================================================
		// static�֐�

		// SharedFlyweightMap�ł̐���
		static Texture::s_ptr loadOfFlyweight(const std::string& file_path);

		//====================================================================================================
	public:
		Texture() {}
		~Texture() {}

		//====================================================================================================
		// �����o�ϐ�

		UINT width_ = 0;	// ��
		UINT height_ = 0;	// ����
		Microsoft::WRL::ComPtr<ID3D12Resource> texture_;				// �e�N�X�`��
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptor_heap_;	// GPU��ւ̃A�h���X�̂悤�Ȃ���

		//====================================================================================================
		// static�֐�

		// ���[�h
		static Texture::s_ptr loadFromFile(const std::string& file_path);

		//====================================================================================================
	};

}