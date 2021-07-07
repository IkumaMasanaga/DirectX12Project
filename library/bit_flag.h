#pragma once
#include <string>


namespace lib {

	class BitFlag final {
	private:
		//====================================================================================================
		// メンバ変数

		uint8_t* array_ = nullptr;	// ビットフラグ管理する配列
		uint32_t array_size_ = 0;	// 配列の要素数
		uint32_t size_ = 0;			// 管理しているビットの数

		//====================================================================================================
	public:
		explicit BitFlag(const uint32_t size = 0);
		explicit BitFlag(const std::string& bit_flag);
		BitFlag(const BitFlag& other);
		~BitFlag();

		//====================================================================================================
		// メンバ関数

		// 各種演算子
		BitFlag& operator =(const BitFlag& other);
		BitFlag operator &(const BitFlag& other) const;
		BitFlag& operator &=(const BitFlag& other);
		BitFlag operator |(const BitFlag& other) const;
		BitFlag& operator |=(const BitFlag& other);
		BitFlag operator ^(const BitFlag& other) const;
		BitFlag& operator ^=(const BitFlag& other);
		BitFlag operator ~() const;
		operator bool();

		// 指定位置のフラグを設定
		void set(const uint32_t index, const bool value);

		// 指定位置のフラグを取得
		bool get(const uint32_t index) const;

		// サイズの変更
		void resize(const uint32_t size);

		// サイズの変更
		void resize(const std::string& bit_flag);

		// 文字列に変換
		std::string toString() const;

		// 削除
		void clear();

		// サイズの取得
		inline uint32_t size() const { return size_; }

		//====================================================================================================
	};

}
