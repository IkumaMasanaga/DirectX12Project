#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "../../library/library.h"


namespace eng {

	class Shader final : public lib::SmartFactory {
		friend class lib::SharedFlyweightMap<LPCWSTR, Shader>;
	private:
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
	public:
		using s_ptr = std::shared_ptr<Shader>;
		using w_ptr = std::weak_ptr<Shader>;

		struct CreateDesc {
			LPCWSTR file_path_ = L"engine/resources/default_shader.fx";
			std::string vertex_func_name_ = "";
			std::string vertex_target_ = "";
			std::string pixel_func_name_ = "";
			std::string pixel_target_ = "";
			D3D12_INPUT_ELEMENT_DESC* input_element_desc_ = nullptr;
			UINT input_element_size_ = 0;
			D3D12_ROOT_SIGNATURE_DESC root_signature_desc_ = {};
			std::function<void(ID3D12Resource*, lib::Matrix4x4*, lib::Matrix4x4*)> setting_func_;
		};

	private:
		//====================================================================================================
		// メンバ変数

		D3D12_INPUT_ELEMENT_DESC* input_element_desc_ = nullptr;	// 頂点入力レイアウト
		UINT input_element_size_ = 0;								// 頂点入力レイアウトの要素数
		ComPtr<ID3DBlob> vertex_shader_;							// 頂点シェーダー
		ComPtr<ID3DBlob> pixel_shader_;								// ピクセルシェーダー
		ComPtr<ID3D12RootSignature> root_signature_;				// ルートシグネチャ

		//====================================================================================================
		// static変数

		inline static lib::SharedFlyweightMap<LPCWSTR, Shader> regist_map_;	// 登録マップ

		//====================================================================================================
		// static関数

		// SharedFlyweightMapでの生成
		static Shader::s_ptr loadOfFlyweight(const CreateDesc& desc);

		//====================================================================================================
	public:
		Shader() {}
		~Shader() {
			if (!input_element_desc_) return;
			delete[] input_element_desc_;
		}

		//====================================================================================================
		// メンバ関数

		// ゲッター
		inline D3D12_INPUT_ELEMENT_DESC* getInputElementDesc() const { return input_element_desc_; }
		inline UINT getInputElementSize() const { return input_element_size_; }
		inline ComPtr<ID3DBlob> getVertexShader() const { return vertex_shader_; }
		inline ComPtr<ID3DBlob> getPixelShader() const { return pixel_shader_; }
		inline ComPtr<ID3D12RootSignature> getRootSignature() const { return root_signature_; }

		std::function<void(ID3D12Resource*, lib::Matrix4x4*, lib::Matrix4x4*)> setting_func_;	// 設定関数 消す

		//====================================================================================================
		// static関数

		// 生成
		static Shader::s_ptr create(const CreateDesc& desc);

		//====================================================================================================
	};

}
