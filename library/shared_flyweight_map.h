#pragma once
#include <unordered_map>
#include <memory>


namespace lib {

	//====================================================================================================
	// 
	// std::shared_ptr でのフライウェイトな管理マップ
	// 管理したいクラスの static なメンバ変数にし、
	// loadOfFlyweight(const Desc& desc) を実装すると
	// load(const Key& key, const Desc& desc) を使用できる
	// 
	//====================================================================================================
	template<class Key, class T>
	class SharedFlyweightMap final {
	private:
		//====================================================================================================
		// メンバ変数

		std::unordered_map<Key, std::shared_ptr<T>> management_map_;	// 管理マップ

		//====================================================================================================
	public:
		SharedFlyweightMap() {}
		~SharedFlyweightMap() {}

		//====================================================================================================
		// メンバ関数

		// サイズ
		inline int size() { return management_map_.size(); }

		// 登録されているか
		inline bool isExist(const Key& key) {
			return management_map_.find(key) != management_map_.end();
		}

		// 削除
		inline void erase(const Key& key) {
			auto it = management_map_.find(key);
			if (management_map_.end() == it) return;
			management_map_.erase(it);
		}

		// 取得
		inline std::shared_ptr<T> get(const Key& key) {
			auto it = management_map_.find(key);
			if (management_map_.end() == it) return nullptr;
			return it->second;
		}

		// 全開放
		inline void clear() {
			management_map_.clear();
		}

		// このマップのみで管理しているデータの解放
		inline void releaseUnique() {
			auto it = management_map_.begin();
			while (it != management_map_.end()) {
				if (1 < it->second.use_count()) {
					++it;
					continue;
				}
				management_map_.erase(it);
			}
		}

		// ロード
		// この関数の中で T::loadOfFlyweight(const Desc& desc) を呼んでいる
		template<class Desc>
		inline std::shared_ptr<T> load(const Key& key, const Desc& desc) {
			std::shared_ptr<T> ptr = get(key);
			if (!ptr) {
				ptr = T::loadOfFlyweight(desc);
				management_map_.try_emplace(key, ptr);
			}
			return ptr;
		}

		//====================================================================================================
	};

}
