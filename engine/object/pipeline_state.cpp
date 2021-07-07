#include "../../system/dx12_manager.h"
#include "pipeline_state.h"
#include "shader.h"


namespace eng {

	PipelineState::s_ptr PipelineState::loadOfFlyweight(const CreateDesc& desc) {
		PipelineState::s_ptr ptr = PipelineState::createShared<PipelineState>();

		// グラフィックスパイプラインの状態オブジェクトを作成
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline_state_desc = desc.create_desc_func_(ptr);

		// PipelineState ( PSO ) の作成では D3D12_GRAPHICS_PIPELINE_STATE_DESC に設定した内容と
		// ルートシグネチャに設定した内容が使用するシェーダと整合性がとれていなければエラーになる
		// 例  頂点レイアウトの内容が実際のシェーダと異なる 等
		if (FAILED(sys::Dx12Manager::getInstance().device_->CreateGraphicsPipelineState(&pipeline_state_desc, IID_PPV_ARGS(&ptr->pso_)))) return nullptr;

		ptr->shader_ = desc.shader_;

		return ptr;
	}
	
	PipelineState::s_ptr PipelineState::create(const CreateDesc& desc) {
		return regist_map_.load<CreateDesc>(desc.regist_name_, desc);
	}

}
