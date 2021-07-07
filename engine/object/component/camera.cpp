#include "camera.h"


namespace eng {

	lib::Matrix4x4 Camera::getViewMatrix4x4() const {
		lib::Matrix4x4 wm = getTransform()->getWorldMatrix4x4();
		lib::Vector3 pos = lib::Vector3(wm._41, wm._42, wm._43);
		lib::Vector3 look = lib::Vector3(wm._31, wm._32, wm._33);
		lib::Vector3 up = lib::Vector3::normalize(lib::Vector3(wm._21, wm._22, wm._23));
		return DirectX::XMMatrixLookAtLH(pos, look, up);
	}

	lib::Matrix4x4 Camera::getProjectionMatrix4x4() const {
		return DirectX::XMMatrixPerspectiveFovLH(lib::Math::toRadian(angle_), aspect_, near_, far_);
	}

}
