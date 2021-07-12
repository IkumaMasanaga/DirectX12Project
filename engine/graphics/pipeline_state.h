#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "../../library/library.h"


namespace eng {

	class Shader;

	class PipelineState final : public lib::SmartFactory {
		friend class lib::SharedFlyweightMap<std::string, PipelineState>;
	private:
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
	public:
		using s_ptr = std::shared_ptr<PipelineState>;
		using w_ptr = std::weak_ptr<PipelineState>;

		struct CreateDesc {
			std::string regist_name_ = "default";
			std::shared_ptr<Shader> shader_ = nullptr;
			D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc_ = {};
		};

	private:
		//====================================================================================================
		// static変数

		inline static lib::SharedFlyweightMap<std::string, PipelineState> regist_map_;	// 登録マップ

		//====================================================================================================
		// メンバ変数

		ComPtr<ID3D12PipelineState> object_;		// パイプラインステートオブジェクト
		std::shared_ptr<Shader> shader_ = nullptr;	// シェーダー

		//====================================================================================================
		// static関数

		// SharedFlyweightMapでの生成
		static PipelineState::s_ptr loadOfFlyweight(const CreateDesc& desc);

		//====================================================================================================
	public:
		PipelineState() {}
		~PipelineState() {}

		//====================================================================================================
		// メンバ関数

		inline ComPtr<ID3D12PipelineState> getObject() { return object_; }
		inline std::shared_ptr<Shader> getShader() { return shader_; }

		//====================================================================================================
		// static関数

		// 生成
		static PipelineState::s_ptr create(const CreateDesc& desc);

		//====================================================================================================
	};

}
