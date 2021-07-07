#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include "../library/singleton.h"


namespace sys {

	class Dx12Manager final : public lib::Singleton<Dx12Manager> {
		// 基底クラスで生成するため
		friend class lib::Singleton<Dx12Manager>;
		// initialize, finalizeを呼ぶため
		friend class System;
	private:
		Dx12Manager() {}

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

		// ハードウェアアダプタ
		Microsoft::WRL::ComPtr<IDXGIFactory4> factory_;

		// デバイス
		Microsoft::WRL::ComPtr<ID3D12Device> device_;

		// D3D12CreateDeviceが失敗する場合はビデオカードがDirectX12に対応していないので is_use_warp_device_ をtrueにする
		bool is_use_warp_device_ = false;

		// フェンス関係
		HANDLE fence_event_ = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
		UINT64 fence_value_ = 0;

		//====================================================================================================
	};

}
