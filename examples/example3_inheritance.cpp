//--------------------------------------------------------------
//! @file example3_inheritance.cpp
//! @brief TsukinoEventBusの使用例、親クラス購読者が子クラスイベントを受け取れることを確認
//! @author 山﨑愛 ( Qiita:tsukino_   github:tsukino)
//--------------------------------------------------------------
#include "TsukinoEventBus/TsukinoEventBus.hpp"
#include <iostream>

//--------------------------------------------------------------
//! @class BaseGameEvent
//! @brief ゲーム関連イベントの基底クラス
//--------------------------------------------------------------
class BaseGameEvent : public TsukinoEventBus::BaseEvent {
public:
	//--------------------------------------------------------------
	//! @brief デストラクタ
	//--------------------------------------------------------------
	virtual ~BaseGameEvent() = default;
};

//--------------------------------------------------------------
//! @class PlayerJoinedEvent
//! @brief プレイヤー参加イベント
//--------------------------------------------------------------
class PlayerJoinedEvent : public BaseGameEvent {
private:
	std::string name_;  // プレイヤー名
public:
	//--------------------------------------------------------------
	//! @brief コンストラクタ
	//! @param name [in] 参加したプレイヤーの名前
	//--------------------------------------------------------------
	explicit PlayerJoinedEvent(const std::string& name)
		: name_(name) {
	}

	//--------------------------------------------------------------
	//! @brief 参加したプレイヤーの名前を取得
	//! @return プレイヤー名の参照
	//--------------------------------------------------------------
	const std::string& getName() const { return name_; }
};

//--------------------------------------------------------------
//! @class ScoreUpdatedEvent
//! @brief スコア更新イベント
//--------------------------------------------------------------
class ScoreUpdatedEvent : public BaseGameEvent {
private:
	int score_; // 新しいスコア
public:

	//--------------------------------------------------------------
	//! @brief コンストラクタ
	//! @param score [in] 新しいスコア値
	//--------------------------------------------------------------
	explicit ScoreUpdatedEvent(int score)
		: score_(score) {
	}

	//--------------------------------------------------------------
	//! @brief 新しいスコア値を取得
	//! @return スコア値
	//--------------------------------------------------------------
	int getScore() const { return score_; }
};

// エントリポイント
int main() {
	TsukinoEventBus::EventBus bus;

	// 親クラス購読者（BaseGameEventを購読）
	auto base_handle = bus.subscribe<BaseGameEvent>(
		[](const BaseGameEvent& e) {
			std::cout << "[BaseGameEvent subscriber] 子クラスイベントを受け取りました" << std::endl;
		},
		5
	);

	// 子クラス専用購読者（PlayerJoinedEvent）
	auto joined_handle = bus.subscribe<PlayerJoinedEvent>(
		[](const PlayerJoinedEvent& e) {
			std::cout << "[PlayerJoinedEvent subscriber] Player joined: " << e.getName() << std::endl;
		},
		10
	);

	// 子クラス専用購読者（ScoreUpdatedEvent）
	auto score_handle = bus.subscribe<ScoreUpdatedEvent>(
		[](const ScoreUpdatedEvent& e) {
			std::cout << "[ScoreUpdatedEvent subscriber] Score updated: " << e.getScore() << std::endl;
		},
		8
	);

	// イベント発行
	bus.publish(PlayerJoinedEvent("Alice"));
	bus.publish(ScoreUpdatedEvent(42));
	// プログラム終了
	return 0;
}
