#include "DxLib.h"
#include "Utility/FPS.h"
#include "Scene/GameMainScene.h"
#include "Scene/SceneManager.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)

{
	// ウィンドウのタイトルを"splash cola"に変更
	SetMainWindowText("splash cola");
	ChangeWindowMode(TRUE);

	SetGraphMode(640, 480, 32);	//画面サイズ指定


	if (DxLib_Init() == -1)

	{

		return -1;

	}

	SetDrawScreen(DX_SCREEN_BACK);
	SceneManager SceneManager(dynamic_cast<AbstractScene*>(new GameMainScene()));

	FPS fp;
	//ループ前にFPS計測を初期化
	fp.Reset_FPS();


	while (ProcessMessage() != -1)

	{
		//fps固定
		fp.FPSUpdate();

		ClearDrawScreen();
		SceneManager.Update();
		SceneManager.Draw();

		if (SceneManager.Change() == nullptr) {
			break;
		}
		ScreenFlip();

	}

	DxLib_End();

	return 0;

}