#include <D3Dcompiler.h>
#include "../../system/dx12_manager.h"
#include "shader.h"


namespace eng {

	Shader::s_ptr Shader::loadOfFlyweight(const CreateDesc& desc) {
		Shader::s_ptr ptr = Shader::createShared<Shader>();

		D3D12_ROOT_SIGNATURE_DESC root_signature_desc = desc.create_desc_func_(ptr);
		Microsoft::WRL::ComPtr<ID3DBlob> blob;

		if (FAILED(D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, nullptr))) return nullptr;		
		if (FAILED(sys::Dx12Manager::getInstance().device_->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&ptr->root_signature_)))) return nullptr;

#if defined(_DEBUG)
		// グラフィックデバッグツールによるシェーダーのデバッグを有効にする
		UINT	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT	compileFlags = 0;
#endif
		if (FAILED(D3DCompileFromFile(desc.file_path_, nullptr, nullptr, desc.vertex_func_name_.c_str(), desc.vertex_target_.c_str(), compileFlags, 0, &ptr->vertex_shader_, nullptr))) return nullptr;
		if (FAILED(D3DCompileFromFile(desc.file_path_, nullptr, nullptr, desc.pixel_func_name_.c_str(), desc.pixel_target_.c_str(), compileFlags, 0, &ptr->pixel_shader_, nullptr))) return nullptr;

		ptr->input_element_desc_ = desc.input_element_desc_;
		ptr->input_element_size_ = desc.input_element_size_;
		ptr->setting_func_ = desc.setting_func_;

		return ptr;
	}

	Shader::s_ptr Shader::create(const CreateDesc& desc) {
		return regist_map_.load<CreateDesc>(desc.file_path_, desc);
	}

}
