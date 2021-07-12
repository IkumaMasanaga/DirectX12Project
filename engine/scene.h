#pragma once
#include "../library/library.h"


namespace eng {

	class Camera;
	class GameObject;

	class Scene : public lib::SmartFactory {
		// update, renderを呼ぶため
		friend class Engine;
		// 登録するため
		friend class GameObject;
	public:
		using s_ptr = std::shared_ptr<Scene>;
		using w_ptr = std::weak_ptr<Scene>;
	private:
		//====================================================================================================
		// メンバ変数

		// GameObject一括管理リスト
		std::list<std::shared_ptr<GameObject>> game_objects_;

		//====================================================================================================
		// メンバ関数

		// 生成時
		bool onCreated();

		// 更新
		void update();

		// 描画
		void render();

		//====================================================================================================
	protected:
		//====================================================================================================
		// メンバ関数

		// 初期化
		virtual bool initialize() = 0;

		// GameObjectの更新後に呼ばれる更新
		virtual void lateUpdate() {}

		// 終了処理
		virtual void finalize() {}

		//====================================================================================================
	public:
		Scene() {}
		virtual ~Scene() {}

		//====================================================================================================
		// メンバ変数

		std::shared_ptr<Camera> camera_ = nullptr;	// カメラ

		//====================================================================================================
	};

}
