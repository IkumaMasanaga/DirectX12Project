#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "../../library/library.h"
#include "descriptor_handle.h"


namespace eng {

	class RenderTargetView : public lib::SmartFactory {
	public:
		using s_ptr = std::shared_ptr<RenderTargetView>;
		using w_ptr = std::weak_ptr<RenderTargetView>;
	public:
		RenderTargetView() {}
		~RenderTargetView();

		//====================================================================================================
		// �����o�ϐ�

		Microsoft::WRL::ComPtr<ID3D12Resource> buffer_;	// �o�b�t�@
		DescriptorHandle handle_;						// �n���h��
		lib::Color clear_color_;						// �N���A����F

		//====================================================================================================
		// static�֐�

		// ����
		static RenderTargetView::s_ptr create(const lib::Color& clear_color, const LONG width, const LONG height);

		//====================================================================================================
	};

}
