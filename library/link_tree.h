#pragma once
#include <functional>
#include "link_linear.h"


namespace lib {

	template<class T>
	class LinkTree : public LinkLinear<T> {
	private:
		//====================================================================================================
		// メンバ変数

		std::weak_ptr<T> parent_;	// 親
		std::weak_ptr<T> child_;	// 先頭の子

		//====================================================================================================
	protected:
		LinkTree() {}

	public:
		virtual ~LinkTree() {}

		//====================================================================================================
		// メンバ関数

		// 根の取得
		inline std::shared_ptr<T> getRoot() {
			std::shared_ptr<T> root = getParent();
			if (!root) return SmartFactory::shared_from_this<T>();
			return root->getRoot();
		}

		// 親の取得
		inline std::shared_ptr<T> getParent() const {
			return (parent_.expired()) ? nullptr : parent_.lock();
		}

		// 先頭の子の取得
		inline std::shared_ptr<T> getChild() const {
			return (child_.expired()) ? nullptr : child_.lock();
		}

		// 子から数えてcount番目の子の取得
		inline std::shared_ptr<T> getChild(const int count) const {
			std::shared_ptr<T> child = getChild();
			for (int i = 0; i < count; ++i) {
				if (!child) break;
				child = child->getNext();
			}
			return child;
		}

		// 自身を子として前に追加
		inline void pushPrev(const std::shared_ptr<T>& obj) {
			obj->pop();
			obj->parent_ = SmartFactory::shared_from_this<T>();
			LinkLinear<T>::pushPrev(obj);
		}

		// 自身を子として後ろに追加
		inline void pushNext(const std::shared_ptr<T>& obj) {
			obj->pop();
			obj->parent_ = SmartFactory::shared_from_this<T>();
			LinkLinear<T>::pushNext(obj);
		}

		// 自身を子として最前に追加
		inline void pushFront(const std::shared_ptr<T>& obj) {
			obj->pop();
			obj->parent_ = SmartFactory::shared_from_this<T>();
			LinkLinear<T>::pushFront(obj);
		}

		// 自身を子として最後尾に追加
		inline void pushBack(const std::shared_ptr<T>& obj) {
			obj->pop();
			obj->parent_ = SmartFactory::shared_from_this<T>();
			LinkLinear<T>::pushBack(obj);
		}

		// 子の最後尾に追加
		inline void pushChild(const std::shared_ptr<T>& obj) {
			obj->pop();
			obj->parent_ = SmartFactory::shared_from_this<T>();
			std::shared_ptr<T> child = getChild();
			if (!child) {
				child_ = obj;
			}
			else {
				child->LinkLinear<T>::pushBack(obj);
			}
		}

		// 親から離脱
		inline void pop() {
			std::shared_ptr<T> parent = getParent();
			if (!parent) return;
			LinkLinear<T>::pop();
			parent_.reset();
		}

		// 自身から下のノードに対して関数を実行
		inline void roundup(const std::function<void(std::shared_ptr<T>)>& func) {
			std::shared_ptr<T> ptr = SmartFactory::shared_from_this<T>();
			func(ptr);
			ptr = getChild();
			while (ptr) {
				ptr->roundup(func);
				ptr = ptr->getNext();
			}
		}

		// 自分から下に存在するか
		inline bool isExist(const std::shared_ptr<T>& obj) {
			bool ret = false;
			roundup([obj, &ret](std::shared_ptr<T> node) {
				if (obj == node) ret = true;
			});
			return ret;
		}

		//====================================================================================================
	};

}
