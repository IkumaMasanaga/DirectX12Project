#pragma once
#include "object.h"


namespace eng {

	class Texture;

	class Material final : public Object {
	public:
		using s_ptr = std::shared_ptr<Material>;
		using w_ptr = std::weak_ptr<Material>;
	private:

		// TODO: SharedFlyweightMapで管理する？

	public:
		Material() {}
		~Material() {}

		//====================================================================================================
		// メンバ変数

		float alpha_ = 1.0f;								// d  透過率
		float shininess_ = 0.0f;							// ni 輝き ( shininess )
		lib::Vector3 diffuse_ = { 0.8f, 0.8f, 0.8f };		// kd 拡散反射成分( diffuse )
		lib::Vector3 specular_ = { 1.0f, 1.0f, 1.0f };		// ks 鏡面反射成分( specular )
		lib::Vector3 ambient_ = { 0.2f, 0.2f, 0.2f };		// ka 環境光反射成分( ambient )
		std::shared_ptr<Texture> tex_diffuse_ = nullptr;	// kd テクスチャ( diffuse )
		std::shared_ptr<Texture> tex_specular_ = nullptr;	// ks テクスチャ( specular )
		std::shared_ptr<Texture> tex_ambient_ = nullptr;	// ka テクスチャ( ambient )
		std::shared_ptr<Texture> tex_bump_ = nullptr;		// bu テクスチャ( bump )

		//====================================================================================================
	};

}
