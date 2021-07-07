#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "object.h"


namespace eng {

	class Shader final : public Object {
		friend class lib::SharedFlyweightMap<LPCWSTR, Shader>;
	public:
		using s_ptr = std::shared_ptr<Shader>;
		using w_ptr = std::weak_ptr<Shader>;

		struct CreateDesc {
			LPCWSTR file_path_ = L"engine/graphics/default_shader.fx";
			std::string vertex_func_name_ = "";
			std::string vertex_target_ = "";
			std::string pixel_func_name_ = "";
			std::string pixel_target_ = "";
			D3D12_INPUT_ELEMENT_DESC* input_element_desc_ = nullptr;
			UINT input_element_size_ = 0;
			std::function<D3D12_ROOT_SIGNATURE_DESC(Shader::s_ptr&)> create_desc_func_;
			std::function<void(ID3D12Resource*, lib::Matrix4x4*, lib::Matrix4x4*)> setting_func_;
		};

	private:
		//====================================================================================================
		// static�ϐ�

		inline static lib::SharedFlyweightMap<LPCWSTR, Shader> regist_map_;	// �o�^�}�b�v

		//====================================================================================================
		// static�֐�

		// SharedFlyweightMap�ł̐���
		static Shader::s_ptr loadOfFlyweight(const CreateDesc& desc);

		//====================================================================================================
	public:
		Shader() {}
		~Shader() {
			if (input_element_desc_) {
				delete[] input_element_desc_;
			}
		}

		//====================================================================================================
		// �����o�ϐ�

		D3D12_INPUT_ELEMENT_DESC* input_element_desc_ = nullptr;		// ���_���̓��C�A�E�g
		UINT input_element_size_ = 0;									// ���_���̓��C�A�E�g�̗v�f��
		Microsoft::WRL::ComPtr<ID3DBlob> vertex_shader_;				// ���_�V�F�[�_�[
		Microsoft::WRL::ComPtr<ID3DBlob> pixel_shader_;					// �s�N�Z���V�F�[�_�[
		Microsoft::WRL::ComPtr<ID3D12RootSignature> root_signature_;	// ���[�g�V�O�l�`��
		std::function<void(ID3D12Resource*, lib::Matrix4x4*, lib::Matrix4x4*)> setting_func_;	// �ݒ�֐�

		//====================================================================================================
		// static�֐�

		// ����
		static Shader::s_ptr create(const CreateDesc& desc);

		//====================================================================================================
	};

}
