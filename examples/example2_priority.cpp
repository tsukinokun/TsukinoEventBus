//--------------------------------------------------------------
//! @file example2_priority.cpp
//! @brief TsukinoEventBusの使用例、複数購読者と優先度による呼び出し順序
//! @author 山﨑愛 ( Qiita:tsukino_   github:tsukino)
//--------------------------------------------------------------
#include "TsukinoEventBus/TsukinoEventBus.hpp"
#include <iostream>
//--------------------------------------------------------------
//! @class PriorityEvent
//! @brief 優先度テスト用のイベントクラス
//--------------------------------------------------------------
class PriorityEvent : public TsukinoEventBus::BaseEvent {
public:
	std::string message;	// メッセージ内容を保持するメンバ変数
	
	//--------------------------------------------------------------
	//! @brief コンストラクタ
	//! @param msg [in] メッセージ内容
	//--------------------------------------------------------------
	explicit PriorityEvent(const std::string& msg) : message(msg) {}	
};

// エントリポイント
int main() {
	// イベントバスの作成
	TsukinoEventBus::EventBus bus;

	// 優先度の低い購読者 (priority = 1)
	auto low_handle = bus.subscribe<PriorityEvent>(
		[](const PriorityEvent& e) {
			std::cout << "[Low priority] " << e.message << std::endl;
		},
		1
	);

	// 優先度の高い購読者 (priority = 10)
	auto high_handle = bus.subscribe<PriorityEvent>(
		[](const PriorityEvent& e) {
			std::cout << "[High priority] " << e.message << std::endl;
		},
		10
	);

	// 同じ優先度の購読者 (priority = 10, 登録順で呼ばれる)
	auto high_handle2 = bus.subscribe<PriorityEvent>(
		[](const PriorityEvent& e) {
			std::cout << "[High priority, second registered] " << e.message << std::endl;
		},
		10
	);

	// イベント発行
	bus.publish(PriorityEvent("Priority test event"));
	// プログラム終了
	return 0;
}
