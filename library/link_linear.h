#pragma once
#include "smart_factory.h"


namespace lib {

	template<class T>
	class LinkLinear : public SmartFactory {
	private:
		//====================================================================================================
		// メンバ変数

		std::weak_ptr<T> prev_;	// 前
		std::weak_ptr<T> next_;	// 後ろ

		//====================================================================================================
	protected:
		LinkLinear() {}

	public:
		virtual ~LinkLinear() {}

		//====================================================================================================
		// メンバ関数

		// 前の取得
		inline std::shared_ptr<T> getPrev() const {
			return (prev_.expired()) ? nullptr : prev_.lock();
		}

		// 後ろの取得
		inline std::shared_ptr<T> getNext() const {
			return (next_.expired()) ? nullptr : next_.lock();
		}

		// 最前の取得
		inline std::shared_ptr<T> getFront() {
			std::shared_ptr<T> prev = getPrev();
			if (!prev) return shared_from_this<T>();
			return prev->getFront();
		}

		// 最後尾の取得
		inline std::shared_ptr<T> getBack() {
			std::shared_ptr<T> next = getNext();
			if (!next) return shared_from_this<T>();
			return next->getBack();
		}

		// 前に追加
		inline void pushPrev(const std::shared_ptr<T>& obj) {
			obj->pop();
			std::shared_ptr<T> s_this = shared_from_this<T>();
			std::shared_ptr<T> prev = getPrev();
			if (prev) {
				obj->prev_ = prev;
				prev->next_ = obj;
			}
			obj->next_ = s_this;
			s_this->prev_ = obj;
		}

		// 後ろに追加
		inline void pushNext(const std::shared_ptr<T>& obj) {
			obj->pop();
			std::shared_ptr<T> s_this = shared_from_this<T>();
			std::shared_ptr<T> next = getNext();
			if (next) {
				obj->next_ = next;
				next->prev_ = obj;
			}
			obj->prev_ = s_this;
			s_this->next_ = obj;
		}

		// 最前に追加
		inline void pushFront(const std::shared_ptr<T>& obj) {
			std::shared_ptr<T> front = getFront();
			front->pushPrev(obj);
		}

		// 最後尾に追加
		inline void pushBack(const std::shared_ptr<T>& obj) {
			std::shared_ptr<T> back = getBack();
			back->pushNext(obj);
		}

		// 離脱
		inline void pop() {
			std::shared_ptr<T> prev = getPrev();
			std::shared_ptr<T> next = getNext();
			if (prev) prev->next_ = next;
			if (next) next->prev_ = prev;
			prev_.reset();
			next_.reset();
		}

		//====================================================================================================
	};

}
