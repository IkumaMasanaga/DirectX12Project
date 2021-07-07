#pragma once
#include "../../../system/window.h"
#include "component.h"


namespace eng {

	class Camera : public Component {
	public:
		using s_ptr = std::shared_ptr<Camera>;
		using w_ptr = std::weak_ptr<Camera>;
	private:


	public:
		Camera() {}
		virtual ~Camera() {}

		//====================================================================================================
		// メンバ変数

		float angle_ = 60.0f;						// 視野角
		float aspect_ = sys::Window::ASPECT_RATIO;	// アスペクト比
		float near_ = 1.0f;							// 近距離の映る範囲
		float far_ = 5000.0f;						// 遠距離の映る範囲

		//====================================================================================================
		// メンバ関数

		// ビュー行列の取得
		lib::Matrix4x4 getViewMatrix4x4() const;

		// プロジェクション行列の取得
		lib::Matrix4x4 getProjectionMatrix4x4() const;

		//====================================================================================================
	};

}
