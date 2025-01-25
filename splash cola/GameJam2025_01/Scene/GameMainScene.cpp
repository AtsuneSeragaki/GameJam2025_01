#include "GameMainScene.h"
#include "DxLib.h"

#include "../Object/CheckMouse/Bar.h"
#include "../Object/Player/Player.h"

#include "../Utility/InputManager.h"
#include "../Utility/ResourceManager.h"
#include <vector>

GameMainScene::GameMainScene()
{
	game_state = GameState::start;

	fps_count = 0;
	start_count = 3;
	timer = 10;			// 10秒でリザルト表示

	retry_button_color = 0xffffff;
	end_button_color = 0xffffff;

	end_flg = false;

	bar = new Bar;
	player = new Player();
	
	ResourceManager* resource = ResourceManager::GetInstance();
	std::vector<int> tmp;
	tmp = resource->GetImages("Resource/Images/GameMain/background.png");
	background_img = tmp[0];

	background_y = -1640.0f;

	in_game_bgm = rm->GetSounds("Resource/Sounds/BGM/MusMus-BGM.mp3");
	count_down_se = rm->GetSounds("Resource/Sounds/SE/count_down.mp3");
	start_se = rm->GetSounds("Resource/Sounds/SE/start.mp3");
	end_se = rm->GetSounds("Resource/Sounds/SE/end.mp3");

	// 音量調整
	ChangeVolumeSoundMem(120, in_game_bgm);
	ChangeVolumeSoundMem(255, count_down_se);
	ChangeVolumeSoundMem(255, start_se);
	ChangeVolumeSoundMem(255, end_se);

	play_count_down_se = true;
}

GameMainScene::~GameMainScene()
{
	delete bar;
	delete player;
}

// 初期化処理
void GameMainScene::Initialize()
{
	fps_count = 0;
	start_count = 3;
	timer = 10;						// 10秒でリザルト表示
	retry_button_color = 0xffffff;
	end_button_color = 0xffffff;
	SetMouseDispFlag(FALSE);		// マウスカーソル非表示
	bar->Initialize();
	background_y = -1640.0f;

	play_count_down_se = true;
}

void GameMainScene::Update()
{
	// ゲームメインbgmループ再生
	if (CheckSoundMem(in_game_bgm) == FALSE)
	{
		PlaySoundMem(in_game_bgm, DX_PLAYTYPE_LOOP);
	}

	switch (game_state)
	{
	case GameState::start:
		InStartUpdate();
		break;
	case GameState::in_game:
		InGameUpdate();
		break;
	case GameState::result:
		InGameResultUpdate();
		break;
	}
}

void GameMainScene::Draw() const
{
	DrawFormatString(0, 0, 0xffffff, "GameMain");
	switch (game_state)
	{
	case GameState::start:
		DrawBox(0, 0, 640, 480, 0xffffff, TRUE);

		// プレイヤー描画
		player->Draw();

		// カウントダウン描画
		DrawFormatString(0, 180, 0x000000, "Shake the cola!!!");
		if (start_count > 0)
		{
			DrawFormatString(0, 200, 0x000000, "count: %d", start_count);
		}
		else
		{
			DrawFormatString(0, 200, 0x000000, "start!!!");
		}
		break;
	case GameState::in_game:
		DrawBox(0, 0, 640, 480, 0xffffff,TRUE);
		DrawGraphF(0.0f, background_y, background_img, TRUE);
		DrawFormatString(0, 50, 0x000000, " background_y:%f", background_y);
		DrawFormatString(0, 20, 0x000000, "InGame");

		// 制限時間の描画
		DrawFormatString(0, 200, 0x000000, "timer: %d", timer);
		bar->Draw();
		player->Draw();
		break;
	case GameState::result:
		DrawFormatString(0, 20, 0xffffff, "Result");

		// リトライ・エンドボタンの描画
		DrawBox(150, 400, 250, 450, retry_button_color, TRUE);
		DrawFormatString(170, 420, 0x000000, "retry");
		DrawBox(390, 400, 490, 450, end_button_color, TRUE);
		DrawFormatString(410, 420, 0x000000, "end");
		break;
	}
}

AbstractScene* GameMainScene::Change()
{
	if (end_flg == true)
	{
		// ゲーム終了
		return nullptr;
	}

	return this;
}

void GameMainScene::InStartUpdate()
{
	// プレイヤー更新処理
	//player->Update();

	if (fps_count < 60)
	{
		fps_count++;
	}
	else
	{
		play_count_down_se = true;
		start_count--;
		fps_count = 0;
	}

	if (start_count <= 0)
	{
		// ゲームスタートSE再生
		if (CheckSoundMem(start_se) == FALSE)
		{
			PlaySoundMem(start_se, DX_PLAYTYPE_BACK);
		}
	}
	else
	{
		if (play_count_down_se == true)
		{
			// カウントダウンSE再生
			PlaySoundMem(count_down_se, DX_PLAYTYPE_BACK);
			play_count_down_se = false;
		}
	}

	if (start_count < 0)
	{
		// ゲーム開始
		game_state = GameState::in_game;
	}
}

void GameMainScene::InGameUpdate()
{
	if (fps_count < 60)
	{
		fps_count++;
	}
	else
	{
		timer--;
		fps_count = 0;
	}

	if (timer < 0)
	{
		// ゲーム終了SE再生
		PlaySoundMem(end_se, DX_PLAYTYPE_BACK);

		// ゲーム終了・リザルトへ
		game_state = GameState::result;
	}

	bar->Update();
	player->Update();

	if (background_y < 0)
	{
		background_y += 3.0f;
	}
}

void GameMainScene::InGameResultUpdate()
{
	// マウスカーソル表示
	SetMouseDispFlag(TRUE);

	// リトライボタンの更新処理
	RetryButtonUpdate();

	// エンドボタンの更新処理
	EndButtonUpdate();
}

// リトライボタンの更新処理
void GameMainScene::RetryButtonUpdate()
{
	// 入力制御インスタンス取得
	InputManager* input = InputManager::GetInstance();

	if (input->GetMouseLocation().x > 150.0f && input->GetMouseLocation().x < 250.0f
		&& input->GetMouseLocation().y > 400.0f && input->GetMouseLocation().y < 450.0f)
	{
		retry_button_color = 0xff0000;

		if (input->GetMouseInputState(MOUSE_INPUT_LEFT) == eInputState::ePress)
		{
			// 初期化処理
			Initialize();
			// ゲームスタート状態へ
			game_state = GameState::start;
		}
	}
	else
	{
		// ボタンの色変更
		retry_button_color = 0xffffff;
	}
}

// エンドボタンの更新処理
void GameMainScene::EndButtonUpdate()
{
	// 入力制御インスタンス取得
	InputManager* input = InputManager::GetInstance();

	if (input->GetMouseLocation().x > 390.0f && input->GetMouseLocation().x < 490.0f
		&& input->GetMouseLocation().y > 400.0f && input->GetMouseLocation().y < 450.0f)
	{
		end_button_color = 0xff0000;

		if (input->GetMouseInputState(MOUSE_INPUT_LEFT) == eInputState::ePress)
		{
			end_flg = true;
		}
	}
	else
	{
		// ボタンの色変更
		end_button_color = 0xffffff;
	}
}
