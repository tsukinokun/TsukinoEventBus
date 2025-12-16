//--------------------------------------------------------------
//! @file example5_raii.cpp
//! @brief TsukinoEventBusの使用例、RAIIによる購読解除の自動管理を確認
//! @author 山﨑愛 ( Qiita:tsukino_   github:tsukino)
//--------------------------------------------------------------
#include "TsukinoEventBus/TsukinoEventBus.hpp"
#include <iostream>

//--------------------------------------------------------------
//! @class ScopedEvent
//! @brief RAII動作確認用のイベントクラス
//--------------------------------------------------------------
class ScopedEvent : public TsukinoEventBus::BaseEvent {
private:
	std::string message_;   // メッセージ内容
public:
    //--------------------------------------------------------------
	//! @brief コンストラクタ
	//! @param msg [in] メッセージ内容
    //--------------------------------------------------------------
    explicit ScopedEvent(const std::string& msg) : message_(msg) {}

    //--------------------------------------------------------------
	//! @brief メッセージ内容を取得
	//! @return メッセージ文字列
    //--------------------------------------------------------------
    const std::string& getMessage() const { return message_; }
};

// エントリポイント
int main() {
	// イベントバスの作成
    TsukinoEventBus::EventBus bus;

    {
        // スコープ内で購読を登録
        auto scoped_handle = bus.subscribe<ScopedEvent>(
            [](const ScopedEvent& e) {
                std::cout << "[Scoped callback] " << e.getMessage() << std::endl;
            },
            5
        );

        // スコープ内でイベント発行 → コールバックが呼ばれる
        bus.publish(ScopedEvent("Inside scope"));
    } // scoped_handle がスコープを抜けると自動的に購読解除される

    // スコープ外でイベント発行 → すでに購読解除されているため呼ばれない
    bus.publish(ScopedEvent("Outside scope"));
	// プログラム終了
    return 0;
}
