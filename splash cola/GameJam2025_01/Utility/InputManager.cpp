#include "InputManager.h"
#include "DxLib.h"

InputManager* InputManager::instance = nullptr;

InputManager* InputManager::GetInstance()
{
	// 初回呼び出しの際に新しいオブジェクトを生成
	if (instance == nullptr)
	{
		instance = new InputManager();
	}

	// 常にインスタンスを返す
	return instance;
}

void InputManager::DeleteInstance()
{
	// インスタンス削除
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void InputManager::Update()
{
	// マウス入力値の更新
	old_mouse = now_mouse;
}

eInputState InputManager::GetMouseInputState(int mouse_button)
{
	//マウスの左ボタンが押されているかチェック
	if ((GetMouseInput() & mouse_button) != 0)
	{
		//押されてる
		now_mouse = 1;
	}
	else
	{
		//押されていない
		now_mouse = 0;
	}

	if (now_mouse == 1 && old_mouse == 0)
	{//クリックした瞬間を取る
		return eInputState::ePress;
	}
	else if (now_mouse == 1 && old_mouse == 1)
	{//ずっと押し続けているか
		return eInputState::eHeld;
	}
	else if (now_mouse == 0 && old_mouse == 1)
	{//放した瞬間
		return eInputState::eRelease;
	}

	return eInputState::eNone;
}

Vector2D InputManager::GetMouseLocation()
{
	int x, y;
	GetMousePoint(&x, &y);
	mouse_location.x = (float)x;
	mouse_location.y = (float)y;

	return mouse_location;
}

int InputManager::GetMouseWheelRotaVolume()
{
	// 前回 GetMouseWheelRotVol が呼ばれてから今回までの回転量を足す
	wheel += GetMouseWheelRotVol();

	return wheel;
}