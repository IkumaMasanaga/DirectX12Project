#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "object.h"


namespace eng {

	class Shader;

	class PipelineState final : public Object {
		friend class lib::SharedFlyweightMap<std::string, PipelineState>;
	public:
		using s_ptr = std::shared_ptr<PipelineState>;
		using w_ptr = std::weak_ptr<PipelineState>;

		struct CreateDesc {
			std::string regist_name_ = "default";
			std::shared_ptr<Shader> shader_ = nullptr;
			std::function<D3D12_GRAPHICS_PIPELINE_STATE_DESC(PipelineState::s_ptr&)> create_desc_func_;
		};

	private:
		//====================================================================================================
		// static�ϐ�

		inline static lib::SharedFlyweightMap<std::string, PipelineState> regist_map_;	// �o�^�}�b�v

		//====================================================================================================
		// static�֐�

		// SharedFlyweightMap�ł̐���
		static PipelineState::s_ptr loadOfFlyweight(const CreateDesc& desc);

		//====================================================================================================
	public:
		PipelineState() {}
		~PipelineState() {}

		//====================================================================================================
		// �����o�ϐ�
		
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pso_;	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
		std::shared_ptr<Shader> shader_ = nullptr;			// �V�F�[�_�[

		//====================================================================================================
		// static�֐�

		// ����
		static PipelineState::s_ptr create(const CreateDesc& desc);

		//====================================================================================================
	};

}
