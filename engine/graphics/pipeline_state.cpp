#include "../../system/dx12_manager.h"
#include "pipeline_state.h"


namespace eng {

	PipelineState::s_ptr PipelineState::loadOfFlyweight(const CreateDesc& desc) {
		PipelineState::s_ptr ptr = PipelineState::createShared<PipelineState>();

		// PipelineState ( PSO ) の作成では D3D12_GRAPHICS_PIPELINE_STATE_DESC に設定した内容と
		// ルートシグネチャに設定した内容が使用するシェーダと整合性がとれていなければエラーになる
		// 例  頂点レイアウトの内容が実際のシェーダと異なる 等
		if (FAILED(sys::Dx12Manager::getInstance().getDevice()->CreateGraphicsPipelineState(&desc.pso_desc_, IID_PPV_ARGS(&ptr->object_)))) return nullptr;

		ptr->shader_ = desc.shader_;

		return ptr;
	}
	
	PipelineState::s_ptr PipelineState::create(const CreateDesc& desc) {
		return regist_map_.load<CreateDesc>(desc.regist_name_, desc);
	}

}
