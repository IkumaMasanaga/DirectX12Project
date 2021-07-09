#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "../../library/library.h"


namespace eng {

	class Texture final : public lib::SmartFactory {
		// 登録するため
		friend class lib::SharedFlyweightMap<std::string, Texture>;
	public:
		using s_ptr = std::shared_ptr<Texture>;
		using w_ptr = std::weak_ptr<Texture>;
	private:
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
		// メンバ変数

		UINT width_ = 0;	// 幅
		UINT height_ = 0;	// 高さ
		Microsoft::WRL::ComPtr<ID3D12Resource> texture_;				// テクスチャ
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptor_heap_;	// GPU上へのアドレスのようなもの のちにハンドルへ変更する

		//====================================================================================================
		// static関数

		// ロード
		static Texture::s_ptr loadFromFile(const std::string& file_path);

		//====================================================================================================
	};

}
