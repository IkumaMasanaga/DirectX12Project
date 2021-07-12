#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include "../library/singleton.h"
#include "../engine/graphics/graphics_manager.h"


namespace sys {

	class Dx12Manager final : public lib::Singleton<Dx12Manager> {
		// 基底クラスで生成するため
		friend class lib::Singleton<Dx12Manager>;
		// initialize, finalizeを呼ぶため
		friend class System;
		// 各種privateメンバにアクセスするため
		friend class eng::GraphicsManager;
	private:
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
	private:
		Dx12Manager() {}

		//====================================================================================================
		// メンバ変数

		ComPtr<IDXGIFactory4> factory_;	// ハードウェアアダプタ
		ComPtr<ID3D12Device> device_;	// デバイス
		HANDLE fence_event_ = nullptr;	// フェンスイベント
		ComPtr<ID3D12Fence> fence_;		// フェンス
		UINT64 fence_value_ = 0;

		//====================================================================================================
		// メンバ関数

		// 初期化
		bool initialize();

		// 終了処理
		void finalize();

		//====================================================================================================
	public:
		~Dx12Manager() {}

		//====================================================================================================
		// メンバ変数

		inline ComPtr<ID3D12Device> getDevice() const { return device_; }

		//====================================================================================================
	};

}
