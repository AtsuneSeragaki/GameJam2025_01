#include "GameMainScene.h"
#include "DxLib.h"

#include "../Object/CheckMouse/Bar.h"
#include "../Object/Player/Player.h"
#include "../Object/Obstacles/Bard.h"

#include "../Utility/ResourceManager.h"
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
	bard = NULL;
	
	ResourceManager* resource = ResourceManager::GetInstance();
	std::vector<int> tmp;
	tmp = resource->GetImages("Resource/Images/GameMain/background.png");
	background_img = tmp[0];
	tmp = resource->GetImages("Resource/Images/GameMain/pocketsound.png");
	pocket_sound_image = tmp[0];

	background_y = -1640.0f;

	in_game_bgm = resource->GetSounds("Resource/Sounds/BGM/MusMus-BGM.mp3");
	count_down_se = resource->GetSounds("Resource/Sounds/SE/count_down.mp3");
	start_se = resource->GetSounds("Resource/Sounds/SE/start.mp3");
	end_se = resource->GetSounds("Resource/Sounds/SE/open.mp3");

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
	timer_font = CreateFontToHandle(NULL, 20, 9);

	tmp = resource->GetImages("Resource/Cola/Geyser.png", 2, 2, 1, 64, 128);
	for (int i = 0; i <= 1; i++)
	{
		bubble_img[i] = tmp[i];
	}

	//320.0f, 150.f
	bubble_location.x = 280.0f;
	bubble_location.y = 180.0f;
	bubble_num = 0;
	bubble_cnt = 0;
	up_bubble = 0.0f;

	score = 0;

	bubble_height = 0.0f;
	amount_y = 0;

	total_fps_count = 0;
	
	fade_mode = 1;		// 明るくなる
	fade_alpha = 255;

	hit_num = 0;
}


GameMainScene::~GameMainScene()
{
	delete bar;
	delete player;
	delete bard;

	// 作成したフォントデータを削除する
	DeleteFontToHandle(explanation_font);
	DeleteFontToHandle(start_font);
	DeleteFontToHandle(count_down_font);
	DeleteFontToHandle(timer_font);
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
	player->Initialize();
	background_y = -1640.0f;

	play_count_down_se = true;
	total_fps_count = 0;

	hit_num = 0;
}

void GameMainScene::Update()
{
	// ゲームメインbgmループ再生
	if (CheckSoundMem(in_game_bgm) == FALSE)
	{
		PlaySoundMem(in_game_bgm, DX_PLAYTYPE_LOOP);
	}

	if (fade_mode == 0)
	{
		switch (game_state)
		{
		case GameState::start:
			InStartUpdate();
			break;
		case GameState::in_game:
			InGameUpdate();
			break;
		case GameState::in_fly:
			InFlyUpdate();
			break;
		case GameState::result:
			InGameResultUpdate();
			break;
		}
	}
	else
	{
		// フェード更新処理
		fadeUpdate();
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
			DrawFormatString(180, 420, 0x000000, "PLAY");
			DrawBox(390, 400, 490, 450, right_button_color, TRUE);
			DrawFormatString(425, 420, 0x000000, "END");

			// クレジット
			DrawFormatString(380, 460, 0x000000, "BGM/SE  MusMus /");
			DrawRotaGraph(580, 467, 0.2, 0.0, pocket_sound_image, TRUE);
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

		// 制限時間のバー
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawBox(30, 460, 640, 480, 0xffffff, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawBox(0, 460, 30, 480, 0xffffff, TRUE);
		DrawBox(30, 460, 640 - total_fps_count, 480, 0x00bb00, TRUE);

		// 制限時間の描画
		char buf[100];
		sprintf_s(buf, "%2d", timer);
		DrawStringToHandle(5, 461, buf, 0x000000, timer_font);

		bar->Draw();
		if (timer > 0) {
			player->Draw();
		}
		else {
			int score_height = (bar->GetCntBarShake() * 2) + (bar->GetSecondBonus() * 2) * -100;
			DrawFormatString(0, 100, 0x000000, "score: %d",score_height);

			DrawExtendGraph(bubble_location.x, bubble_location.y, 360.0f, 180.0f, bubble_img[bubble_num], TRUE);
			player->ResultDraw();
		}
		break;
	case GameState::in_fly:
		DrawGraphF(0.0f, background_y, background_img, TRUE);
		DrawFormatString(0, 50, 0x000000, " background_y:%d", amount_y);
		
		if (bard != NULL)
		{
			DrawFormatString(0, 70, 0x000000, " bard_y:%f", bard->GetLocation().x);
			bard->Draw();
		}
		DrawExtendGraph(bubble_location.x-30.0f,480.0f -up_bubble, 390.0f, 480.0f, bubble_img[bubble_num], TRUE);
		DrawFormatString(0, 100, 0x000000, "score: %f", (bar->GetCntBarShake() * 2) + (bar->GetSecondBonus() * 2));

		break;
	case GameState::result:
		// リトライ・タイトルボタンの描画
		DrawBox(150, 400, 250, 450, left_button_color, TRUE);
		DrawFormatString(180, 420, 0x000000, "RETRY");
		DrawBox(390, 400, 490, 450, right_button_color, TRUE);
		DrawFormatString(420, 420, 0x000000, "TITLE");

		DrawFormatString(340, 60, 0xffffff, "Shake%f", bar->GetCntBarShake());

		break;
	}

	if (fade_mode != 0)
	{
		// フェードをつける
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_alpha);
		DrawBox(0, 0, 640, 480, 0x000000, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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

	if (timer > 0)
	{
		total_fps_count++;
		if (fps_count < 60)
		{
			fps_count++;
		}
		else
		{
			timer--;
			fps_count = 0;
		}
	}
	else
	{
		//// ゲーム終了SE再生
		//PlaySoundMem(end_se, DX_PLAYTYPE_BACK);
		//スコア算出
		score = (bar->GetCntBarShake() * 2) + (bar->GetSecondBonus() * 2);
		player->ResultUpdate();
		ColaBubbleUpdate();
		return;
	}

	bar->Update();
	player->Update();


}

void GameMainScene::InFlyUpdate()
{
	if (up_bubble < 350)
	{
		up_bubble += 50;
	}

	if (bubble_cnt++ > 10)
	{
		bubble_num = 1;
		bubble_cnt = 0;
	}
	else if(bubble_cnt > 5)
	{
		bubble_num = 0;
	}

	if (background_y < -(1640.0f - bubble_height))
	{
		if (background_y < -149.0f)
		{
			background_y += 10.0f;
		}
		
		amount_y += 50;
	}

	if (amount_y % 100 == 0 && bard == NULL && hit_num < 5)
	{
		bard = new Bard(hit_num);

		if (hit_num < 4)
		{
			hit_num++;
		}
	}
	else if(bard != NULL)
	{
		bard->Update();

		if (bard->GetLocation().x >= 700.0f)
		{
			delete bard;
			bard = NULL;

			if (background_y >= -(1640 - bubble_height))
			{
				game_state = GameState::result;
			}
		}
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

void GameMainScene::ColaBubbleUpdate()
{
	//高さ　（振った回数×２）＋（bonus＊２）*-10
	int score_height = (bar->GetCntBarShake() * 2) + (bar->GetSecondBonus() * 2)*-10;


	if (player->GetColaNum() >= 2)
	{
		if (bubble_location.y>-100)
		{
			bubble_location.y -= 50;
		}
		else
		{
			/*ResourceManager* resource = ResourceManager::GetInstance();
			std::vector<int> tmp;
			tmp = resource->GetImages("Resource/Images/GameMain/background2.png");
			background_img = tmp[0];*/
			bubble_height = bar->GetCntBarShake() * 50.0f;
			game_state = GameState::in_fly;
		}
	}

	if (bubble_cnt++ > 10)
	{
		bubble_num = 1;
		bubble_cnt = 0;
	}
	else if (bubble_cnt > 5)
	{
		bubble_num = 0;
	}

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
			// フェードで暗くする
			fade_mode = 2;
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
			start_flg = false;
			// フェードで暗くする
			fade_mode = 2;
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

// フェード更新処理
void GameMainScene::fadeUpdate()
{
	if (fade_mode == 1)
	{
		if (fade_alpha > 0)
		{
			// 明るくなる
			fade_alpha -= 10;
		}
		else
		{
			fade_alpha = 0;
			fade_mode = 0;
		}
	}
	else if (fade_mode == 2)
	{
		if (fade_alpha < 255)
		{
			// 暗くなる
			fade_alpha += 10;
		}
		else
		{
			// 初期化処理
			Initialize();
			// ゲームスタート状態へ
			game_state = GameState::start;

			fade_alpha = 255;
			fade_mode = 1;
		}
	}
}
