#pragma once
#include <memory>
#include <any>


namespace lib {

	//====================================================================================================
	// 
	// スマートポインタでの生成を強制するクラス
	// 生のポインタからnewでの生成を禁止
	// 生のポインタから配列オブジェクトの生成を禁止
	// コンストラクタを使用する際は constructor 仮想関数をオーバーライドする
	// 
	//====================================================================================================
	class SmartFactory : public std::enable_shared_from_this<SmartFactory> {
	private:
		//====================================================================================================

		// newでの生成を禁止
		// _mm_malloc ... アライメントを第二引数の値ごと調整してメモリ確保する
		inline static void* operator new(std::size_t size) { return _mm_malloc(size, 16); }

		// 配列オブジェクトの生成を禁止
		static void* operator new[](std::size_t) = delete;
		static void operator delete[](void*) = delete;

		//====================================================================================================
	protected:
		SmartFactory() {}

		//====================================================================================================
		// メンバ関数

		// 引数なしコンストラクタ
		virtual bool constructor() { return true; }

		// 引数ありコンストラクタ
		virtual bool constructor(std::any args) { return true; }

		//====================================================================================================
	public:
		virtual ~SmartFactory() {}

		// _mm_free ... _mm_mallocで動的確保したメモリを解放する際に使用する
		inline static void operator delete(void* p) { _mm_free(p); }

		//====================================================================================================
		// static関数

		template<class T>
		inline std::shared_ptr<T> shared_from_this() {
			return (!std::is_base_of<SmartFactory, T>::value) ? nullptr : std::static_pointer_cast<T>(std::enable_shared_from_this<SmartFactory>::shared_from_this());
		}

		// shared_ptr<T>の生成（引数なし）
		template<class T>
		inline static std::shared_ptr<T> createShared() {
			if (!std::is_base_of<SmartFactory, T>::value) return nullptr;
			std::shared_ptr<T> ptr = std::make_shared<T>();
			if (!ptr->constructor()) return nullptr;
			return ptr;
		}

		// shared_ptr<T>の生成（引数あり）
		template<class T, class Desc>
		inline static std::shared_ptr<T> createShared(Desc desc) {
			if (!std::is_base_of<SmartFactory, T>::value) return nullptr;
			std::shared_ptr<T> ptr = std::make_shared<T>();
			if (!ptr->constructor(desc)) return nullptr;
			return ptr;
		}

		// unique_ptr<T>の生成（引数なし）
		template<class T>
		inline static std::unique_ptr<T> createUnique() {
			if (!std::is_base_of<SmartFactory, T>::value) return nullptr;
			std::unique_ptr<T> ptr = std::make_unique<T>();
			if (!ptr->constructor()) return nullptr;
			return ptr;
		}

		// unique_ptr<T>の生成（引数あり）
		template<class T, class Desc>
		inline static std::unique_ptr<T> createUnique(Desc desc) {
			if (!std::is_base_of<SmartFactory, T>::value) return nullptr;
			std::unique_ptr<T> ptr = std::make_unique<T>();
			if (!ptr->constructor(desc)) return nullptr;
			return ptr;
		}

		//====================================================================================================
	};

}
