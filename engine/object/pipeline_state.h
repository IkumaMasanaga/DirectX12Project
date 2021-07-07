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
		// static変数

		inline static lib::SharedFlyweightMap<std::string, PipelineState> regist_map_;	// 登録マップ

		//====================================================================================================
		// static関数

		// SharedFlyweightMapでの生成
		static PipelineState::s_ptr loadOfFlyweight(const CreateDesc& desc);

		//====================================================================================================
	public:
		PipelineState() {}
		~PipelineState() {}

		//====================================================================================================
		// メンバ変数
		
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pso_;	// パイプラインステートオブジェクト
		std::shared_ptr<Shader> shader_ = nullptr;			// シェーダー

		//====================================================================================================
		// static関数

		// 生成
		static PipelineState::s_ptr create(const CreateDesc& desc);

		//====================================================================================================
	};

}
