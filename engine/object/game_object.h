#pragma once
#include "object.h"
#include "shape.h"
#include "../transform.h"


namespace eng {

	class Camera;
	class Component;
	class Renderer;

	class GameObject final : public Object {
		// 各種privateメンバにアクセスするため
		friend class Scene;
		// 基底クラスで生成するため
		friend class lib::SmartFactory;
	public:
		using s_ptr = std::shared_ptr<GameObject>;
		using w_ptr = std::weak_ptr<GameObject>;
	private:
		//====================================================================================================
		// メンバ変数

		bool is_enable_ = true;		// 関数呼び出しフラグ
		std::list<std::shared_ptr<Component>> components_;	// すべてのComponent管理リスト
		std::list<std::shared_ptr<Renderer>> renderers_;	// 描画Component管理リスト

		//====================================================================================================
		// メンバ関数

		// コンストラクタ
		bool constructor() final override;

		// 削除時
		void onDestroy();

		// 有効になった時
		void onEnable();

		// 無効になった時
		void onDisable();

		// 更新
		void update();

		// 描画
		void render(std::shared_ptr<Camera> camera);

		//====================================================================================================
	public:
		GameObject() {}
		~GameObject() {}

		//====================================================================================================
		// メンバ変数

		Transform<GameObject>::s_ptr transform_ = nullptr;	// 親子関係

		//====================================================================================================
		// メンバ関数

		// Componentの追加
		template<class T>
		inline std::shared_ptr<T> addComponent() {
			if (!std::is_base_of<Component, T>::value) return nullptr;
			std::shared_ptr<T> add = SmartFactory::createShared<T>();
			std::shared_ptr<Component> com = std::static_pointer_cast<Component>(add);
			com->game_object_ = shared_from_this<GameObject>();
			com->transform_ = transform_;
			if (!com->onCreated()) return nullptr;
			components_.emplace_back(com);
			if (std::is_base_of<Renderer, T>::value) renderers_.emplace_back(std::static_pointer_cast<Renderer>(com));
			return add;
		}

		// Componentの取得
		template<class T>
		inline std::shared_ptr<T> getComponent(const int find_count = 0) {
			int count = 0;
			std::list<std::shared_ptr<Component>>::iterator it = components_.begin();
			while (it != components_.end()) {
				if (typeid(*(*it)) == typeid(T)) {
					if (count == find_count) return (*it);
					++count;
				}
				++it;
			}
			return nullptr;
		}

		//====================================================================================================
		// static関数

		// 空のGameObjectの生成
		static GameObject::s_ptr createEmpty(const std::string& name);

		// 四角形
		static GameObject::s_ptr createPlaneXY(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path = "");
		static GameObject::s_ptr createPlaneYZ(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path = "");
		static GameObject::s_ptr createPlaneZX(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path = "");
		// キューブ
		static GameObject::s_ptr createCube(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path = "");
		// 二等辺三角形
		static GameObject::s_ptr createTriangleIsosceles(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path = "");
		// 直角三角形
		static GameObject::s_ptr createTriangleRight(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path = "");
		static GameObject::s_ptr createTriangleLeft(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path = "");
		// 正三角形
		static GameObject::s_ptr createTriangleEquilateral(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path = "");
		// 球体
		static GameObject::s_ptr createSphere(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path = "");
		// ドーム
		static GameObject::s_ptr createDome(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path = "");
		// 底面なしの円錐
		static GameObject::s_ptr createCone(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path = "");
		// 円盤
		static GameObject::s_ptr createDisk(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path = "");
		// 円盤状のリング 
		static GameObject::s_ptr createDiskRing(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path = "");
		// 円状にくり抜いた四角形
		static GameObject::s_ptr createHollowOutDiskPlane(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path = "");
		// 円筒
		static GameObject::s_ptr createCylinder(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path = "");

		//====================================================================================================
	};

}
