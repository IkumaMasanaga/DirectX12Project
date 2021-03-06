#pragma once
#include "../component.h"


namespace eng {

	class Camera;

	class Renderer : public Component {
		// renderを呼ぶため
		friend class GameObject;
	public:
		using s_ptr = std::shared_ptr<Renderer>;
		using w_ptr = std::weak_ptr<Renderer>;
	private:


	protected:
		//====================================================================================================
		// メンバ関数

		// 描画
		// TODO: Layerを実装したらLayerに変える
		virtual void render(const std::shared_ptr<Camera>& camera) {}

		//====================================================================================================
	public:
		Renderer() {}
		virtual ~Renderer() {}


	};

}
