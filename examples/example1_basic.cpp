//--------------------------------------------------------------
//! @file example1_basic.cpp
//! @brief TsukinoEventBusの基本的な使用例、イベントの購読と発行の方法
//! @author 山﨑愛 ( Qiita:tsukino_   github:tsukino)
//--------------------------------------------------------------
#include "TsukinoEventBus/TsukinoEventBus.hpp"
#include <iostream>
//--------------------------------------------------------------
//! @class HelloEvent  
//! @brief サンプルイベントクラス
//--------------------------------------------------------------
class HelloEvent : public TsukinoEventBus::BaseEvent {
public:
	std::string message;    // メッセージ内容を保持するメンバ変数

	//--------------------------------------------------------------
	//! @brief コンストラクタ
	//! @param msg [in] メッセージ内容
	//--------------------------------------------------------------
	HelloEvent(const std::string& msg) : message(msg) {}
};

// エントリポイント
int main() {
	// イベントバスの作成
	TsukinoEventBus::EventBus bus;

	// HelloEvent用のコールバック関数を定義
	auto hello_event_callback = [](const HelloEvent& e) {
		std::cout << "HelloEvent Callback: " << e.message << std::endl;
		};

	// イベント購読（RAIIハンドルを保持）
	auto handle = bus.subscribe<HelloEvent>(
		hello_event_callback,   // コールバック関数
		1                       // 優先度
	);

	// イベント発行
	bus.publish(HelloEvent("Hello EventBus!"));
	// handle がスコープを抜けると自動的に購読解除される
	// プログラム終了
	return 0;
}
