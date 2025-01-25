#pragma once

#include "Vector2D.h"

enum class eInputState : unsigned char
{
	eNone,
	ePress,
	eRelease,
	eHeld,
};

// Singleton:クラスが1つのインスタンスのみを保証する＆このインスタンスへの大域アクセス・ポイントを提供する
class InputManager
{
private:
	static InputManager* instance;

	int now_mouse;                 // 現在のフレーム入力値
	int old_mouse;                 // 過去のフレーム入力値
	Vector2D mouse_location;       // マウス座標
	int wheel;                     // マウスのホイール回転量

private:
	// コンストラクタをprivateにする
	InputManager() = default;
	// コピーガードの作成
	// コピーコンストラクタ禁止
	InputManager(const InputManager&) = delete;
	// 代入演算子禁止
	InputManager& operator = (const InputManager&) = delete;

public:
	~InputManager() = default;

public:
	// コンストラクタとデストラクタとして機能する静的メンバ関数を作成する
	// 初回呼び出しの際に新しいオブジェクトを生成し、関数は常にそのインスタンスを返すようにする
	static InputManager* GetInstance();
	static void DeleteInstance();

public:
	// 更新処理
	void Update();

public:
	// マウスの入力取得処理
	eInputState GetMouseInputState(int mouse_button);
	// マウスの座標取得処理
	Vector2D GetMouseLocation();
	// マウスのホイール回転量取得処理
	int GetMouseWheelRotaVolume();
};