#include "camera.h"


namespace eng {

	lib::Matrix4x4 Camera::getViewMatrix4x4() const {
		lib::Matrix4x4 wm = getTransform()->getWorldMatrix4x4();
		lib::Vector3 pos = lib::Vector3(wm._41, wm._42, wm._43);
		lib::Vector3 z = lib::Vector3::normalize(lib::Vector3(wm._31, wm._32, wm._33));
		lib::Vector3 x = lib::Vector3::normalize(lib::Vector3::cross(lib::Vector3(wm._21, wm._22, wm._23), z));
		lib::Vector3 y = lib::Vector3::cross(z, x);
		return lib::Matrix4x4(
			x.x,                        y.x,                        z.x,                        0.0f,
			x.y,                        y.y,                        z.y,                        0.0f,
			x.z,                        y.z,                        z.z,                        0.0f,
			-lib::Vector3::dot(x, pos), -lib::Vector3::dot(y, pos), -lib::Vector3::dot(z, pos), 1.0f
		);
	}

	lib::Matrix4x4 Camera::getProjectionMatrix4x4() const {
		float zoom_y = 1 / tanf(lib::Math::toRadian(angle_) / 2);
		float zoom_x = zoom_y * (1 / aspect_);
		return lib::Matrix4x4(
			zoom_x, 0.0f,   0.0f,                             0.0f,
			0.0f,   zoom_y, 0.0f,                             0.0f,
			0.0f,   0.0f,   far_ * (1 / (far_ - near_)),      1.0f,
			0.0f,   0.0f,   -far_ * (near_ / (far_ - near_)), 0.0f
		);
	}

}
