#pragma once
#include "../../library/library.h"


namespace eng {

	class Object : public lib::SmartFactory {
	public:
		using s_ptr = std::shared_ptr<Object>;
		using w_ptr = std::weak_ptr<Object>;
	private:
		//====================================================================================================
		// メンバ変数

		std::string name_ = "unknown";	// 名前
		bool is_alive_ = true;			// 生存フラグ
		bool is_active_ = true;			// 有効フラグ
		UINT instance_id_ = 0;			// 唯一のID

		//====================================================================================================
		// static変数

		inline static UINT instance_count_ = 0;	// 今まで生成された数

		//====================================================================================================
	public:
		Object() {}
		virtual ~Object() {}

		//====================================================================================================
		// メンバ関数

		// セッター
		inline void setName(const std::string& name) { name_ = name; }
		inline void setActive(const bool is_active) { is_active_ = is_active; }
		inline void destroy() { is_alive_ = false; }

		// ゲッター
		inline std::string getName() const { return name_; }
		inline bool isAlive() const { return is_alive_; }
		inline bool isActive() const { return is_active_; }
		inline UINT getInstanceId() const { return instance_id_; }

		//====================================================================================================
		// static関数

		// 生成数の取得
		inline static UINT getInstanceCount() { return instance_count_; }

		//====================================================================================================
	};

}
