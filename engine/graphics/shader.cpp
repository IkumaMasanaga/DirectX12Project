#include <D3Dcompiler.h>
#include "../../system/dx12_manager.h"
#include "shader.h"


namespace eng {

	Shader::s_ptr Shader::loadOfFlyweight(const CreateDesc& desc) {
		Shader::s_ptr ptr = Shader::createShared<Shader>();

		Microsoft::WRL::ComPtr<ID3DBlob> blob;

		if (FAILED(D3D12SerializeRootSignature(&desc.root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, nullptr))) return nullptr;		
		if (FAILED(sys::Dx12Manager::getInstance().getDevice()->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&ptr->root_signature_)))) return nullptr;

#if defined(_DEBUG)
		// グラフィックデバッグツールによるシェーダーのデバッグを有効にする
		UINT	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT	compileFlags = 0;
#endif
		if (FAILED(D3DCompileFromFile(desc.file_path, nullptr, nullptr, desc.vertex_shader_name.c_str(), desc.vertex_target.c_str(), compileFlags, 0, &ptr->vertex_shader_, nullptr))) return nullptr;
		if (FAILED(D3DCompileFromFile(desc.file_path, nullptr, nullptr, desc.pixel_shader_name.c_str(), desc.pixel_target.c_str(), compileFlags, 0, &ptr->pixel_shader_, nullptr))) return nullptr;

		ptr->input_element_desc_ = desc.input_element_desc;
		ptr->input_element_size_ = desc.input_element_size;

		return ptr;
	}

	Shader::s_ptr Shader::create(const CreateDesc& desc) {
		return regist_map_.load<CreateDesc>(desc.file_path, desc);
	}

}
