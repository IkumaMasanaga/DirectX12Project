#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "../../library/library.h"
#include "descriptor_handle.h"


namespace eng {

	class Texture final : public lib::SmartFactory {
		// 登録するため
		friend class lib::SharedFlyweightMap<std::string, Texture>;
	private:
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
	public:
		using s_ptr = std::shared_ptr<Texture>;
		using w_ptr = std::weak_ptr<Texture>;
	private:
		//====================================================================================================
		// メンバ変数

		uint32_t width_ = 0;				// 幅
		uint32_t height_ = 0;				// 高さ
		ComPtr<ID3D12Resource> texture_;	// テクスチャ
		DescriptorHandle handle_;			// ハンドル

		//====================================================================================================
		// static変数

		inline static lib::SharedFlyweightMap<std::string, Texture> regist_map_;	// 登録マップ

		//====================================================================================================
		// static関数

		// SharedFlyweightMapでの生成
		static Texture::s_ptr loadOfFlyweight(const std::string& file_path);

		//====================================================================================================
	public:
		Texture() {}
		~Texture() {}

		//====================================================================================================
		// メンバ関数

		// ゲッター
		inline uint32_t getWidth() const { return width_; }
		inline uint32_t getHeight() const { return height_; }
		inline ComPtr<ID3D12Resource> const getTexture() { return texture_; }
		inline DescriptorHandle getHandle() const { return handle_; }

		//====================================================================================================
		// static関数

		// ロード
		static Texture::s_ptr loadFromFile(const std::string& file_path);

		// 空の描画テクスチャの生成
		static Texture::s_ptr createEmpty(const uint32_t width, const uint32_t height);

		//====================================================================================================
	};

}
