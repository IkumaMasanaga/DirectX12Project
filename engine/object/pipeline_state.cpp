#include "../../system/dx12_manager.h"
#include "pipeline_state.h"
#include "shader.h"


namespace eng {

	PipelineState::s_ptr PipelineState::loadOfFlyweight(const CreateDesc& desc) {
		PipelineState::s_ptr ptr = PipelineState::createShared<PipelineState>();

		// �O���t�B�b�N�X�p�C�v���C���̏�ԃI�u�W�F�N�g���쐬
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline_state_desc = desc.create_desc_func_(ptr);

		// PipelineState ( PSO ) �̍쐬�ł� D3D12_GRAPHICS_PIPELINE_STATE_DESC �ɐݒ肵�����e��
		// ���[�g�V�O�l�`���ɐݒ肵�����e���g�p����V�F�[�_�Ɛ��������Ƃ�Ă��Ȃ���΃G���[�ɂȂ�
		// ��  ���_���C�A�E�g�̓��e�����ۂ̃V�F�[�_�ƈقȂ� ��
		if (FAILED(sys::Dx12Manager::getInstance().device_->CreateGraphicsPipelineState(&pipeline_state_desc, IID_PPV_ARGS(&ptr->pso_)))) return nullptr;

		ptr->shader_ = desc.shader_;

		return ptr;
	}
	
	PipelineState::s_ptr PipelineState::create(const CreateDesc& desc) {
		return regist_map_.load<CreateDesc>(desc.regist_name_, desc);
	}

}
