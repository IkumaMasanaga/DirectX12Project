#include "utility.h"


namespace lib {

	std::vector<std::string> Utility::split(const std::string& origin, const char div_ch) {
		std::vector<std::string> ret;
		std::string str = origin;
		while (true) {
			int count = str.find(div_ch);
			if (count == std::string::npos) {
				count = str.find('\n');
				if (count == std::string::npos) {
					count = str.length();
				}
			}
			ret.emplace_back(std::move(str.substr(0, count)));
			if (count == str.length()) break;
			str = str.substr(count+1, str.length() - count+1);
			if (str.empty() || str == "\n") break;
		}
		return std::move(ret);
	}

	std::vector<std::vector<std::string>> Utility::loadCSV(const std::string& file_path, const char div_ch) {
		std::vector<std::vector<std::string>> ret;
		FILE* fp = nullptr;
		fopen_s(&fp, file_path.c_str(), "r");
		if (!fp) return ret;
		char buff[1024] = { 0 };
		while (fgets(buff, sizeof(buff), fp)) {
			ret.emplace_back(std::move(split(buff, div_ch)));
			memset(buff, 0, sizeof(buff));
		}
		return std::move(ret);
	}

}
