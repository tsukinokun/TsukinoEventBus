//--------------------------------------------------------------
//! @file   TsukinoEventBus.hpp
//! @brief  優先度付き・型安全なヘッダーオンリーイベントバス
//! @author 山﨑愛 ( Qiita:tsukino_   github:tsukino)
//--------------------------------------------------------------
#pragma once
#include <string>
#include <functional>
#include <vector>
#include <algorithm>
//名前空間 TsukinoEventBus
namespace TsukinoEventBus {
	//--------------------------------------------------------------
	//! @class BaseEvent
	//! @brief すべてのイベントの基底クラス
	//! @note  イベントはこのクラスを継承して作成してください
	//--------------------------------------------------------------
	class BaseEvent {
	public:
		//--------------------------------------------------------------
		//! @brief デストラクタ
		//--------------------------------------------------------------
		virtual ~BaseEvent() = default;
	};

	class SubscriptionHandle; // 前方宣言

	//--------------------------------------------------------------
	//! @class EventBus
	//! @brief イベントバスシステムの本体
	//! @note  優先度付き・型安全なイベントシステムを提供します
	//--------------------------------------------------------------
	class EventBus {
	public:
		//コールバックの型エイリアス
		using Callback = std::function<void(const BaseEvent&)>;
	public:
		//--------------------------------------------------------------
		//! @brief	   購読登録
		//! @tparam    T		 [in] イベントの型
		//! @param     callback [in] イベント発行時に呼び出されるコールバック関数
		//! @param     priority [in] コールバックの優先度 ( 数値が大きいほど優先度が高い )
		//! @return    SubscriptionHandle 購読解除用のRAIIハンドル
		//! @note 　   この関数内でソートを行います
		//! @note      同じ優先度の場合は登録順に呼び出されます
		//! @note      親クラスの型で購読登録することも可能、子クラスのイベントも受け取れます
		//--------------------------------------------------------------
		template<typename T>
		SubscriptionHandle subscribe(std::function<void(const T&)> callback, int priority);

		//--------------------------------------------------------------
		//! @brief 購読者のコールバックを更新する
		//! @param id			[in] 購読者ID
		//! @param new_callback [in] 新しいコールバック関数
		//! @return true: 更新成功 / false: 該当購読者なし
		//--------------------------------------------------------------
		bool updateCallback(size_t id, Callback new_callback);

		//--------------------------------------------------------------
		//! @brief 購読解除
		//! @param id [in] 購読者ID 
		//! @note RAIIハンドルのデストラクタから呼び出されます 
		//--------------------------------------------------------------
		void unsubscribe(size_t id);

		//--------------------------------------------------------------
		//! @brief   イベント発行
		//! @param   event [in] イベントオブジェクト
		//! @note    親クラス購読者は子クラスイベントも受け取ることができます。
		//! @note    購読者リストを走査し、dynamic_cast により適切な型だけ呼び出されます。
		//! @warning 該当する購読者が存在しない場合は処理は行われません。
		//--------------------------------------------------------------
		void publish(const BaseEvent& event);

	private:
		//--------------------------------------------------------------
		//! @struct Subscriber
		//! @brief  購読者情報を格納する構造体
		//--------------------------------------------------------------
		struct Subscriber {
			Callback callback;	//!< コールバック関数
			int priority;		//!< 優先度
			size_t id;			//!< 購読者ID
		};
	private:
		std::vector<Subscriber> subscribers_;	// 購読者リスト
		size_t nextId_ = 0;						// 次の購読者ID
	};

	//--------------------------------------------------------------
	// EventBusのコンストラクタ・メンバ関数実装
	//--------------------------------------------------------------

	//--------------------------------------------------------------
	//! @brief 購読登録
	//--------------------------------------------------------------
	template<typename T>
	inline SubscriptionHandle EventBus::subscribe(std::function<void(const T&)> callback, int priority) {
		//ラッパー関数を作成
		auto wrapper = [callback](const BaseEvent& event) {
			// ダウンキャストを試みる
			if (auto derived = dynamic_cast<const T*>(&event)) {
				callback(*derived); // キャスト成功時のみ呼び出す 
			}
			};
		//購読者IDを発行
		size_t id = nextId_++;

		//購読者リストに追加
		subscribers_.emplace_back(wrapper, priority, id);
		// 優先度でソート
		std::stable_sort(subscribers_.begin(), subscribers_.end(),
			[](const Subscriber& lhs, const Subscriber& rhs) {
				return lhs.priority > rhs.priority;
			});
		//購読ハンドルを返す
		return SubscriptionHandle(*this, id);
	}

	//--------------------------------------------------------------
	//! @brief 購読者のコールバックを更新する
	//! @param id			[in] 購読者ID
	//! @param new_callback [in] 新しいコールバック関数
	//! @return true: 更新成功 / false: 該当購読者なし
	//--------------------------------------------------------------
	inline bool EventBus::updateCallback(size_t id, Callback new_callback) {
		for (auto& subscriber : subscribers_) {
			if (subscriber.id == id) {
				subscriber.callback = std::move(new_callback);
				return true; // 更新成功
			}
		}
		return false; // 該当購読者なし
	}

	//--------------------------------------------------------------
	//! @brief 購読解除
	//! @param id [in] 購読者ID 
	//! @note RAIIハンドルのデストラクタから呼び出されます 
	//--------------------------------------------------------------
	inline void EventBus::unsubscribe(size_t id) {
		subscribers_.erase(std::remove_if(subscribers_.begin(), subscribers_.end(),
			[&](const Subscriber& s) { return s.id == id; }),
			subscribers_.end());
	}

	//--------------------------------------------------------------
	//! @brief   イベント発行
	//! @param   event [in] イベントオブジェクト
	//! @note    親クラス購読者は子クラスイベントも受け取ることができます。
	//! @note    購読者リストを走査し、dynamic_cast により適切な型だけ呼び出されます。
	//! @warning 該当する購読者が存在しない場合は処理は行われません。
	//--------------------------------------------------------------
	inline void EventBus::publish(const BaseEvent& event) {
		//イベントの型情報を取得
		for (const auto& subscriber : subscribers_) {
			subscriber.callback(event); // dynamic_cast により適切な型だけ呼ばれる
		}
	}

	//--------------------------------------------------------------
	//! @class SubscriptionHandle
	//! @brief 購読解除をRAIIで管理するハンドル
	//! @details
	//! - EventBus::subscribe() から返されます
	//! - ハンドルがスコープを抜けると自動的に購読解除されます 
	//! - コピーは禁止、ムーブのみ可能です
	//-------------------------------------------------------------- 
	class SubscriptionHandle {
	public:
		//--------------------------------------------------------------
		// コンストラクタ
		//! @param bus [in] 購読登録されたイベントバスの参照
		//! @param id  [in] 購読登録ID
		//--------------------------------------------------------------
		SubscriptionHandle(EventBus& bus, size_t id);

		//--------------------------------------------------------------
		// デストラクタ
		//! @note 自動的に購読解除を行います
		//--------------------------------------------------------------
		~SubscriptionHandle()noexcept;

		SubscriptionHandle(const SubscriptionHandle&) = delete;				// コピー禁止
		SubscriptionHandle& operator=(const SubscriptionHandle&) = delete;  // コピー禁止
		
		//--------------------------------------------------------------
		// ムーブコンストラクタ
		//! @param other [in] 移動元のSubscriptionHandle
		//! @note  ムーブ後、ムーブ元を無効化します
		//--------------------------------------------------------------
		SubscriptionHandle(SubscriptionHandle&& other)noexcept;

		//--------------------------------------------------------------
		// ムーブ代入
		//! @param other [in] 移動元のSubscriptionHandle
		//! @note  ムーブ後、ムーブ元を無効化します
		//--------------------------------------------------------------
		SubscriptionHandle& operator=(SubscriptionHandle&& other)noexcept;	// ムーブ代入

		//--------------------------------------------------------------
		// 型安全に購読中のコールバックを更新する
		//! @tparam T [in] イベント型
		//! @param newCallback [in] 新しいコールバック関数
		//! @return true: 更新成功 / false: 解除済み or 該当なし
		//--------------------------------------------------------------
		template<typename T>
		bool updateCallback(std::function<void(const T&)> newCallback);

		//--------------------------------------------------------------
		// 明示的に購読解除を行う関数
		//! @return true: 解除成功 / false: すでに解除済み
		//! @note  解除済みの場合は何もしません 
		//--------------------------------------------------------------
		bool release();
	private:
		EventBus* bus_;	//!< 購読登録されたイベントバスの参照(解除でnullptrにする)
		size_t id_;		//!< 購読登録ID
		bool released_; //!< 解除済みフラグ
	};

	//--------------------------------------------------------------
	// SubscriptionHandleのコンストラクタ・デストラクタ・メンバ関数実装
	//--------------------------------------------------------------

	//--------------------------------------------------------------
	//! @brief コンストラクタ
	//--------------------------------------------------------------
	inline SubscriptionHandle::SubscriptionHandle(EventBus& bus, size_t id)
		: bus_(&bus), id_(id), released_(false) {
	}

	//--------------------------------------------------------------
	//! @brief デストラクタ
	//--------------------------------------------------------------
	inline SubscriptionHandle::~SubscriptionHandle()noexcept {
		// 自動的に購読解除を行う(二重解放を予防)
		release();
	}

	//--------------------------------------------------------------
	//! @brief ムーブコンストラクタ
	//--------------------------------------------------------------
	inline SubscriptionHandle::SubscriptionHandle(SubscriptionHandle&& other) noexcept
		: bus_(other.bus_), id_(other.id_), released_(other.released_) {
		other.bus_ = nullptr;		// ムーブ元を無効化
		other.released_ = true;		// ムーブ元を解除済みに設定
	}

	//--------------------------------------------------------------
	//! @brief ムーブ代入
	//--------------------------------------------------------------
	inline SubscriptionHandle& SubscriptionHandle::operator=(SubscriptionHandle&& other) noexcept {
		if (this != &other) {
			// 既存の購読を解除
			release();
			// ムーブ元からデータを移動
			bus_ = other.bus_;
			id_ = other.id_;
			released_ = other.released_;
			// ムーブ元を無効化
			other.bus_ = nullptr;
			other.released_ = true;
		}
		return *this;
	}

	//--------------------------------------------------------------
	//! @brief 型安全に購読中のコールバックを更新する
	//--------------------------------------------------------------
	template<typename T>
	inline bool SubscriptionHandle::updateCallback(std::function<void(const T&)> newCallback) {
		if (released_ || !bus_) {
			return false; // 解除済みまたは無効なバス
		}
		// 型安全なラッパー関数を作成
		auto wrapper = [newCallback](const BaseEvent& event) {
			// ダウンキャストを試みる
			if (auto derived = dynamic_cast<const T*>(&event)) {
				newCallback(*derived); // キャスト成功時のみ呼び出す 
			}
			};
		// イベントバスに更新を依頼
		return bus_->updateCallback(id_, wrapper);
	}

	//--------------------------------------------------------------
	//! @brief 明示的に購読解除を行う関数
	//--------------------------------------------------------------
	inline bool SubscriptionHandle::release() {
		// まだ解除されていなければ解除を行う
		if (!released_ && bus_) {
			bus_->unsubscribe(id_);	// 購読解除
			released_ = true;		// 解除済みフラグを立てる
			bus_ = nullptr;			// bus_をnullptrにして再度解除されないようにする
			return true;			// 解除成功
		}
		return false;				// 既に解除済み
	}
} //namespace TsukinoEventBus