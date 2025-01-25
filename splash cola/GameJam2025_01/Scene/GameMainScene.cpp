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

	start_flg = false;

	fps_count = 0;
	start_count = 3;
	timer = 10;			// 10秒でリザルト表示

	left_button_color = 0xffffff;
	right_button_color = 0xffffff;

	end_flg = false;

	bar = new Bar;
	player = new Player();
	
	ResourceManager* resource = ResourceManager::GetInstance();
	std::vector<int> tmp;
	tmp = resource->GetImages("Resource/Images/GameMain/background.png");
	background_img = tmp[0];

	background_y = -1640.0f;

	in_game_bgm = resource->GetSounds("Resource/Sounds/BGM/MusMus-BGM.mp3");
	count_down_se = resource->GetSounds("Resource/Sounds/SE/count_down.mp3");
	start_se = resource->GetSounds("Resource/Sounds/SE/start.mp3");
	end_se = resource->GetSounds("Resource/Sounds/SE/end.mp3");

	// 音量調整
	ChangeVolumeSoundMem(100, in_game_bgm);
	ChangeVolumeSoundMem(255, count_down_se);
	ChangeVolumeSoundMem(255, start_se);
	ChangeVolumeSoundMem(255, end_se);

	play_count_down_se = true;

	// フォント作成
	explanation_font = CreateFontToHandle(NULL, 70, 9);
	start_font = CreateFontToHandle(NULL, 180, 9, DX_FONTTYPE_EDGE);
	count_down_font = CreateFontToHandle(NULL, 300, 9, DX_FONTTYPE_EDGE);
}

GameMainScene::~GameMainScene()
{
	delete bar;
	delete player;

	// 作成したフォントデータを削除する
	DeleteFontToHandle(explanation_font);
	DeleteFontToHandle(start_font);
	DeleteFontToHandle(count_down_font);
}

// 初期化処理
void GameMainScene::Initialize()
{
	fps_count = 0;
	start_count = 3;
	timer = 10;						// 10秒でリザルト表示
	left_button_color = 0xffffff;
	right_button_color = 0xffffff;
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
	// 背景描画
	DrawGraphF(0.0f, background_y, background_img, TRUE);

	switch (game_state)
	{
	case GameState::start:
		// プレイヤー描画
		player->Draw();

		//DrawStringToHandle(20, 20, "Shake the cola!!!", 0x000000, explanation_font);
		DrawStringToHandle(20, 20, "SHAKE THE SODA!!!", 0x000000, explanation_font);

		if (start_flg == false)
		{
			// プレイ・エンドボタンの描画
			DrawBox(150, 400, 250, 450, left_button_color, TRUE);
			DrawFormatString(170, 420, 0x000000, "PLAY");
			DrawBox(390, 400, 490, 450, right_button_color, TRUE);
			DrawFormatString(410, 420, 0x000000, "END");
		}
		else
		{
			// カウントダウン描画
			if (start_count > 0)
			{
				// 文字列に変換
				char buf[100];
				sprintf_s(buf, "%d", start_count);
				DrawStringToHandle(250, 100, buf, 0x000000, count_down_font, 0xffffff);
			}
			else
			{
				DrawStringToHandle(50, 150, "START!", 0x000000, start_font, 0xffffff);
			}
		}
		break;

	case GameState::in_game:
		DrawFormatString(0, 50, 0x000000, " background_y:%f", background_y);

		// 制限時間の描画
		DrawFormatString(0, 200, 0x000000, "timer: %d", timer);
		bar->Draw();
		player->Draw();
		break;

	case GameState::result:
		// リトライ・タイトルボタンの描画
		DrawBox(150, 400, 250, 450, left_button_color, TRUE);
		DrawFormatString(170, 420, 0x000000, "RETRY");
		DrawBox(390, 400, 490, 450, right_button_color, TRUE);
		DrawFormatString(410, 420, 0x000000, "TITLE");

		// クレジット
		// ポケットサウンド
		DrawFormatString(0, 460, 0xffffff, "BGM/SE");
		DrawFormatString(100, 460, 0xffffff, "MusMus");
		DrawFormatString(300, 460, 0xffffff, "pokettosaundo");

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
	if (start_flg == false)
	{
		// プレイボタン更新処理
		PlayButtonUpdate();
		// エンドボタン更新処理
		EndButtonUpdate();
	}
	else
	{
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
}

void GameMainScene::InGameUpdate()
{
	SetMouseDispFlag(FALSE);		// マウスカーソル非表示

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

	// タイトルボタンの更新処理
	TitleButtonUpdate();
}

// リトライボタンの更新処理
void GameMainScene::RetryButtonUpdate()
{
	// 入力制御インスタンス取得
	InputManager* input = InputManager::GetInstance();
	input->Update();

	if (input->GetMouseLocation().x > 150.0f && input->GetMouseLocation().x < 250.0f
		&& input->GetMouseLocation().y > 400.0f && input->GetMouseLocation().y < 450.0f)
	{
		left_button_color = 0xff0000;

		if (input->GetMouseInputState(MOUSE_INPUT_LEFT) == eInputState::ePress)
		{
			// 初期化処理
			Initialize();
			// ゲーム開始
			game_state = GameState::start;
		}
	}
	else
	{
		// ボタンの色変更
		left_button_color = 0xffffff;
	}
}

// タイトルボタンの更新処理
void GameMainScene::TitleButtonUpdate()
{
	// 入力制御インスタンス取得
	InputManager* input = InputManager::GetInstance();

	if (input->GetMouseLocation().x > 390.0f && input->GetMouseLocation().x < 490.0f
		&& input->GetMouseLocation().y > 400.0f && input->GetMouseLocation().y < 450.0f)
	{
		right_button_color = 0xff0000;

		if (input->GetMouseInputState(MOUSE_INPUT_LEFT) == eInputState::ePress)
		{
			//end_flg = true;

			// 初期化処理
			Initialize();
			start_flg = false;
			// ゲームスタート状態へ
			game_state = GameState::start;
		}
	}
	else
	{
		// ボタンの色変更
		right_button_color = 0xffffff;
	}
}

// プレイボタンの更新処理
void GameMainScene::PlayButtonUpdate()
{
	// 入力制御インスタンス取得
	InputManager* input = InputManager::GetInstance();
	input->Update();

	if (input->GetMouseLocation().x > 150.0f && input->GetMouseLocation().x < 250.0f
		&& input->GetMouseLocation().y > 400.0f && input->GetMouseLocation().y < 450.0f)
	{
		left_button_color = 0xff0000;

		if (input->GetMouseInputState(MOUSE_INPUT_LEFT) == eInputState::ePress)
		{
			start_flg = true;
		}
	}
	else
	{
		// ボタンの色変更
		left_button_color = 0xffffff;
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
		right_button_color = 0xff0000;

		if (input->GetMouseInputState(MOUSE_INPUT_LEFT) == eInputState::ePress)
		{
			// ゲーム終了
			end_flg = true;
		}
	}
	else
	{
		// ボタンの色変更
		right_button_color = 0xffffff;
	}
}
