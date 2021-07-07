#pragma once
#include "../game_object.h"


namespace eng {

	class Component : public Object {
		// 各種protected関数にアクセスするため
		friend class GameObject;
		//friend class lib::SmartFactory;
	public:
		using s_ptr = std::shared_ptr<Component>;
		using w_ptr = std::weak_ptr<Component>;
	private:
		//====================================================================================================
		// メンバ変数

		bool is_start_ = true;						// start関数呼び出しフラグ
		bool is_enable_ = true;						// onEnable, onDisable関数呼び出しフラグ
		GameObject::w_ptr game_object_;				// 自身がアタッチされているGameObject
		Transform<GameObject>::w_ptr transform_;	// GameObjectのTransform

		//====================================================================================================
	protected:
		//====================================================================================================
		// メンバ関数

		// 生成時
		virtual bool onCreated() { return true; }

		// 削除時
		virtual void onDestroy() {}

		// 有効になった時
		virtual void onEnable() {}

		// 無効になった時
		virtual void onDisable() {}

		// 開始時
		virtual void start() {}

		// 更新
		virtual void update() {}

		//====================================================================================================
	public:
		Component() {}
		virtual ~Component() {}

		//====================================================================================================
		// メンバ関数

		// アタッチされているGameObjectの取得
		inline GameObject::s_ptr getGameObject() const { return (game_object_.expired()) ? nullptr : game_object_.lock(); }

		// GameObjectのTransformの取得
		inline Transform<GameObject>::s_ptr getTransform() const { return (transform_.expired()) ? nullptr : transform_.lock(); }

		// Componentの追加
		template<class T>
		inline std::shared_ptr<T> addComponent() { return getGameObject()->addComponent<T>(); }

		// Componentの取得
		template<class T>
		inline std::shared_ptr<T> getComponent() { return getGameObject()->getComponent<T>(); }

		//====================================================================================================
	};

}
