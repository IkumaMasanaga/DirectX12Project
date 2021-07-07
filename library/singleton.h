#pragma once
#include <memory>


namespace lib {

	template<class T>
	class Singleton {
	private:
		// コピー、代入の禁止
		Singleton(const Singleton&) = delete;
		Singleton(const Singleton&&) = delete;
		Singleton& operator = (const Singleton&) = delete;
		Singleton& operator = (const Singleton&&) = delete;

		//====================================================================================================
		// static変数
		
		inline static T* instance_ = nullptr;	// 唯一のインスタンス

		//====================================================================================================
	protected:
		Singleton() {}

	public:
		virtual ~Singleton() {}

		//====================================================================================================
		// static関数

		// インスタンスの取得
		inline static T& getInstance() {
			if (!instance_) {
				instance_ = new T();
			}
			return *instance_;
		}

		// インスタンスの削除
		inline static void destroy() {
			if (!instance_) return;
			delete instance_;
			instance_ = nullptr;
		}

		//====================================================================================================
	};

}
