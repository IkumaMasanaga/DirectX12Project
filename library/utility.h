#pragma once
#include <stdint.h>
#include <vector>
#include <string>


namespace lib {

	class Utility final {
	private:
		// 生成の禁止
		Utility() = delete;
		Utility(const Utility&) = delete;
		Utility(const Utility&&) = delete;
		Utility& operator =(const Utility&) = delete;
		Utility& operator =(const Utility&&) = delete;

	public:
		//====================================================================================================
		// static関数

		// uint64_t型の10進数表記のデータを実際のバイナリデータに変換
		// 例：10進数 1101 → 10進数 13 (2進数 1011)
		template<class Tuint>
		inline static Tuint binaryIntTo(uint64_t binary) {
			Tuint ret = 0;
			int add = 1;
			for (int i = 0; i < (sizeof(Tuint) * 8); ++i) {
				ret += (binary & 1) ? add : 0;
				binary /= 10;
				if (binary == 0) break;
				add *= 2;
			}
			return ret;
		}

		// string型の10進数表記のデータを実際のバイナリデータに変換
		// 例：string型 "1101" → 10進数 13 (2進数 1011)
		template<class Tuint>
		inline static Tuint binaryStrTo(const std::string& binary) {
			return binaryIntTo<Tuint>(std::stoull(binary));
		}

		// バイナリデータをuint32_t型の10進数表記に変換
		// 例：2進数 1101 → 10進数 1101
		template<class Tuint>
		inline static uint64_t integerToBinaryInt(Tuint integer) {
			uint64_t ret = 0;
			int add = 1;
			for (int i = 0; i < (sizeof(Tuint) * 8); ++i) {
				ret += (integer & 1) ? add : 0;
				integer >>= 1;
				if (integer == 0) break;
				add *= 10;
			}
			return ret;
		}

		// バイナリデータをstring型の10進数表記に変換
		// 例：2進数 1101 → string型 "1101"
		template<class Tuint>
		inline static std::string integerToBinaryStr(Tuint integer) {
			std::string ret;
			for (int i = 0; i < (sizeof(Tuint) * 8); ++i) {
				std::string add = (integer & 1) ? "1" : "0";
				ret = add + ret;
				integer >>= 1;
			}
			return ret;
		}

		// 指定文字で分割
		static std::vector<std::string> split(const std::string& origin, const char div_ch);

		// CSVファイルのロード
		static std::vector<std::vector<std::string>> loadCSV(const std::string& file_path, const char div_ch = ',');

		//====================================================================================================
	};

}
