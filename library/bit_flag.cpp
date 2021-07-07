#include "bit_flag.h"
#include "utility.h"


namespace lib {

	BitFlag::BitFlag(const uint32_t size) {
		resize(size);
	}

	BitFlag::BitFlag(const std::string& bit_flag) {
		resize(bit_flag);
	}

	BitFlag::BitFlag(const BitFlag& other) : size_(other.size_), array_size_(other.array_size_) {
		if (array_size_ <= 0) return;
		array_ = new uint8_t[array_size_]{ 0 };
		for (uint32_t i = 0; i < array_size_; ++i) {
			array_[i] = other.array_[i];
		}
	}

	BitFlag::~BitFlag() {
		if (!array_) return;
		delete[] array_;
		array_ = nullptr;
	}

	BitFlag& BitFlag::operator =(const BitFlag& other) {
		size_ = other.size_;
		array_size_ = other.array_size_;
		if (array_) {
			delete[] array_;
			array_ = nullptr;
		}
		if (array_size_ <= 0) return *this;
		array_ = new uint8_t[array_size_]{ 0 };
		for (uint32_t i = 0; i < array_size_; ++i) {
			array_[i] = other.array_[i];
		}
		return *this;
	}

	BitFlag BitFlag::operator &(const BitFlag& other) const {
		BitFlag ret = (other.size_ < size_) ? *this : other;
		for (uint32_t i = 0; i < std::min(array_size_, other.array_size_); ++i) {
			ret.array_[i] = array_[i] & other.array_[i];
		}
		return ret;
	}

	BitFlag& BitFlag::operator &=(const BitFlag& other) {
		*this = *this & other;
		return *this;
	}

	BitFlag BitFlag::operator |(const BitFlag& other) const {
		BitFlag ret = (other.size_ < size_) ? *this : other;
		for (uint32_t i = 0; i < std::min(array_size_, other.array_size_); ++i) {
			ret.array_[i] = array_[i] | other.array_[i];
		}
		return ret;
	}

	BitFlag& BitFlag::operator |=(const BitFlag& other) {
		*this = *this | other;
		return *this;
	}

	BitFlag BitFlag::operator ^(const BitFlag& other) const {
		BitFlag ret = (other.size_ < size_) ? *this : other;
		for (uint32_t i = 0; i < std::min(array_size_, other.array_size_); ++i) {
			ret.array_[i] = array_[i] ^ other.array_[i];
		}
		return ret;
	}

	BitFlag& BitFlag::operator ^=(const BitFlag& other) {
		*this = *this ^ other;
		return *this;
	}

	BitFlag BitFlag::operator ~() const {
		BitFlag ret = *this;
		for (uint32_t i = 0; i < array_size_; ++i) {
			ret.array_[i] = ~ret.array_[i];
		}
		return ret;
	}

	BitFlag::operator bool() {
		for (uint32_t i = 0; i < array_size_; ++i) {
			if (array_[i] != 0) return true;
		}
		return false;
	}

	void BitFlag::set(const uint32_t index, const bool value) {
		if (size_ <= index) resize(index);
		uint32_t vector_index = index / 8;
		uint8_t mask_bit = (1 << (index % 8));
		if (value) {
			array_[vector_index] |= mask_bit;
		}
		else {
			mask_bit = ~mask_bit;
			array_[vector_index] &= mask_bit;
		}
	}

	bool BitFlag::get(const uint32_t index) const {
		if (size_ <= index) return false;
		uint32_t vector_index = index / 8;
		uint8_t mask_bit = (1 << (index % 8));
		return array_[vector_index] & mask_bit;
	}

	void BitFlag::resize(const uint32_t size) {
		size_ = size;
		uint32_t prev_array_size = array_size_;
		array_size_ = (size + 7) / 8;
		if (!array_) {
			if (array_size_ <= 0) return;
			array_ = new uint8_t[array_size_]{ 0 };
			return;
		}
		if (array_size_ <= 0) {
			delete[] array_;
			array_ = nullptr;
			return;
		}
		uint8_t* prev_array = array_;
		array_ = new uint8_t[array_size_]{ 0 };
		for (uint32_t i = 0; i < array_size_; ++i) {
			if (prev_array_size <= i) break;
			array_[i] = prev_array[i];
		}
		delete[] prev_array;
	}

	void BitFlag::resize(const std::string& bit_flag) {
		size_ = bit_flag.length();
		array_size_ = (size_ + 7) / 8;
		if (array_) {
			delete[] array_;
			array_ = nullptr;
		}
		if (array_size_ <= 0) return;
		array_ = new uint8_t[array_size_]{ 0 };
		std::string bitstr = bit_flag;
		for (uint32_t i = 0; i < array_size_; ++i) {
			std::string str = (bitstr.length() < 8) ? bitstr : bitstr.substr(bitstr.length() - 8, bitstr.length());
			array_[i] = Utility::binaryStrTo<uint8_t>(str);
			if (str == bitstr) return;
			bitstr = bitstr.substr(0, bitstr.length() - 8);
		}
	}

	std::string BitFlag::toString() const {
		std::string ret;
		for (uint32_t i = 0; i < array_size_; ++i) {
			std::string add = Utility::integerToBinaryStr<uint8_t>(array_[i]);
			ret = add + ret;
		}
		ret = ret.substr(ret.length() - size_, ret.length());
		return ret;
	}

	void BitFlag::clear() {
		size_ = 0;
		array_size_ = 0;
		if (!array_) return;
		delete[] array_;
		array_ = nullptr;
	}

}
