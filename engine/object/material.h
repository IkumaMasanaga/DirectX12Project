#pragma once
#include "object.h"


namespace eng {

	class Texture;

	class Material final : public Object {
	public:
		using s_ptr = std::shared_ptr<Material>;
		using w_ptr = std::weak_ptr<Material>;
	private:

		// TODO: SharedFlyweightMapΕΗ·ιH

	public:
		Material() {}
		~Material() {}

		//====================================================================================================
		// oΟ

		float alpha_ = 1.0f;								// d  §ί¦
		float shininess_ = 0.0f;							// ni P« ( shininess )
		lib::Vector3 diffuse_ = { 0.8f, 0.8f, 0.8f };		// kd gU½Λ¬ͺ( diffuse )
		lib::Vector3 specular_ = { 1.0f, 1.0f, 1.0f };		// ks ΎΚ½Λ¬ͺ( specular )
		lib::Vector3 ambient_ = { 0.2f, 0.2f, 0.2f };		// ka Β«υ½Λ¬ͺ( ambient )
		std::shared_ptr<Texture> tex_diffuse_ = nullptr;	// kd eNX`( diffuse )
		std::shared_ptr<Texture> tex_specular_ = nullptr;	// ks eNX`( specular )
		std::shared_ptr<Texture> tex_ambient_ = nullptr;	// ka eNX`( ambient )
		std::shared_ptr<Texture> tex_bump_ = nullptr;		// bu eNX`( bump )

		//====================================================================================================
	};

}
