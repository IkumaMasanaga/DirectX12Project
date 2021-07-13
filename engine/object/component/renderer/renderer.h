#pragma once
#include "../component.h"


namespace eng {

	class Camera;

	class Renderer : public Component {
		// render‚ğŒÄ‚Ô‚½‚ß
		friend class GameObject;
	public:
		using s_ptr = std::shared_ptr<Renderer>;
		using w_ptr = std::weak_ptr<Renderer>;
	private:


	protected:
		//====================================================================================================
		// ƒƒ“ƒoŠÖ”

		// •`‰æ
		// TODO: Layer‚ğÀ‘•‚µ‚½‚çLayer‚É•Ï‚¦‚é
		virtual void render(const std::shared_ptr<Camera>& camera) {}

		//====================================================================================================
	public:
		Renderer() {}
		virtual ~Renderer() {}


	};

}
