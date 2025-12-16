//--------------------------------------------------------------
//! @file example4_update_callback.cpp
//! @brief TsukinoEventBusの使用例、購読中のコールバックを動的に更新する方法
//! @author 山﨑愛 ( Qiita:tsukino_   github:tsukino)
//--------------------------------------------------------------
#include "TsukinoEventBus/TsukinoEventBus.hpp"
#include <iostream>

//--------------------------------------------------------------
//! @class UpdateEvent
//! @brief コールバック更新テスト用のイベントクラス
//--------------------------------------------------------------
class UpdateEvent : public TsukinoEventBus::BaseEvent {
private:
    std::string message_;  // メッセージ内容
public:
    //--------------------------------------------------------------
    //! @brief コンストラクタ
    //! @param msg [in] メッセージ内容
    //--------------------------------------------------------------
    explicit UpdateEvent(const std::string& msg) : message_(msg) {}

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

    // 初期コールバックを登録
    auto handle = bus.subscribe<UpdateEvent>(
        [](const UpdateEvent& e) {
            std::cout << "[Initial callback] " << e.getMessage() << std::endl;
        },
        5
    );

    // イベント発行（初期コールバックが呼ばれる）
    bus.publish(UpdateEvent("First message"));

    // コールバックを更新
    handle.updateCallback<UpdateEvent>(
        [](const UpdateEvent& e) {
            std::cout << "[Updated callback] " << e.getMessage() << std::endl;
        }
    );

    // イベント発行（更新後のコールバックが呼ばれる）
    bus.publish(UpdateEvent("Second message"));

    // プログラム終了時に handle がスコープを抜けて自動解除される
    return 0;
}
