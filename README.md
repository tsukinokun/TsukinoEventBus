<p align="center">
  <img src="Image/logo.png" width="150" alt="TsukinoEventBus Logo">
</p>

<h1 align="center">TsukinoEventBus</h1>

<p align="center">
  <img src="https://img.shields.io/badge/License-MIT-yellow.svg" alt="License: MIT">
  <img src="https://img.shields.io/badge/C++-17-blue.svg" alt="C++17">
</p>

TsukinoEventBus は C++17 向けの軽量イベントバスライブラリです。  
RAII による安全な購読管理、優先度制御、継承対応、コールバック更新などをサポートしています。  
初心者にも安心して使えるよう、シンプルで明快な API 設計を心がけています。

## 🚀 Quick Start
最小限のコードで TsukinoEventBus を使う例です。

```cpp
#include "TsukinoEventBus/TsukinoEventBus.hpp"
#include <iostream>

// イベントクラスの定義
class HelloEvent : public TsukinoEventBus::BaseEvent {
public:
   std::string message;
   explicit HelloEvent(const std::string\& msg) : message(msg) {}
};



int main() {
   TsukinoEventBus::EventBus bus;
   // 購読者を登録
   auto handle = bus.subscribe<HelloEvent>(
      \[](const HelloEvent\& e) {
         std::cout << "Received: " << e.message << std::endl;
      },
       1
   );

   // イベントを発行
   bus.publish(HelloEvent("Hello EventBus!"));
}
```

```bash
Received: Hello EventBus!
```

## 🔧 ビルド方法
### g++ を使う場合
単一ファイルを直接ビルドできます。
```bash
g++ -std=c++17 -Iinclude examples/example1\_basic.cpp -o example1\_basic./example1\_basic
```

### CMake を使う場合
複数の example をまとめてビルドする場合は CMakeLists.txt を利用してください。
```bash
mkdir build \&\& cd build
cmake ..
cmake --build .
./example1\_basic
```

### Visual Studio を使う場合
``.sln``ファイルを開いてビルドすることも可能です。

## 📂 Examples
`examples/` ディレクトリに、基本から応用までのサンプルを用意しています。

| Example | ファイル名 | 内容 |
|---------|------------|------|
| 1 | `example1\_basic.cpp` | 基本的な購読と発行 |
| 2 | `example2\_priority.cpp` | 複数購読者と優先度制御 |
| 3 | `example3\_inheritance.cpp` | 親クラス購読者が子クラスイベントを受け取れる |
| 4 | `example4\_update\_callback.cpp` | 購読中のコールバックを動的に更新 |
| 5 | `example5\_raii.cpp` | RAIIによる自動解除 |

## 🧩 特徴
- RAII による安全な購読解除  
- 優先度付きイベント処理  
- 継承対応（親クラス購読）  
- コールバックの動的更新  
- シンプルで初心者にも安心な API  

## 📦 インストール方法



TsukinoEventBus は **ヘッダオンリーライブラリ**です。  

特別なビルドやインストールは不要で、`include` ディレクトリをプロジェクトにコピーするだけで利用できます。

### 手順

1. リポジトリを取得 
```bash
git clone https://github.com/tsukinokun/TsukinoEventBus.git
```

2. ``include``ディレクトリを自分のプロジェクトにコピー、またはインクルードパスに追加
```bash
g++ -std=c++17 -I/path/to/TsukinoEventBus/include your\_code.cpp -o your\_program
```
3.``#include "TsukinoEventBus/TsukinoEventBus.hpp"``とすれば使用可能



## 🛡 ライセンス
このライブラリは **MIT License** の下で公開されています。  
自由に利用・改変・再配布が可能ですが、ライセンス文書のコピーを必ず含めてください。

### MIT License 
このライブラリは [MIT License](./LICENSE) の下で公開されています。

## Documentation
- [解説記事(Qiita)](https://qiita.com/tsukino_/items/58d449d52fb9acaac2d7)

# 👩‍💻 作者
**山﨑 愛**
- [Qiita: tsukino_](https://qiita.com/tsukino_) 
- [GitHub: tsukino](https://github.com/tsukinokun)
