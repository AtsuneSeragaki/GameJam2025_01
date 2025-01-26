#include "GameMainScene.h"
#include "DxLib.h"

#include "../Object/CheckMouse/Bar.h"
#include "../Object/Player/Player.h"
#include "../Object/Obstacles/Bard.h"
#include "../Object/Cloud/Cloud.h"

#include "../Utility/ResourceManager.h"
#include "../Utility/InputManager.h"
#include "../Object/Ranking/RankingData.h"
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

	ranking_data = new RankingData();
	ranking_data->Initialize();

	cloud = new Cloud();
	
	ResourceManager* resource = ResourceManager::GetInstance();
	std::vector<int> tmp;
	tmp = resource->GetImages("Resource/Images/GameMain/background.png");
	background_img = tmp[0];
	tmp = resource->GetImages("Resource/Images/GameMain/pocketsound.png");
	pocket_sound_image = tmp[0];
	tmp = resource->GetImages("Resource/Images/GameMain/mouse.png");
	mouse_img = tmp[0];

	background_y = -1640.0f;

	in_game_bgm = resource->GetSounds("Resource/Sounds/BGM/MusMus-BGM.mp3");
	count_down_se = resource->GetSounds("Resource/Sounds/SE/count_down.mp3");
	start_se = resource->GetSounds("Resource/Sounds/SE/start.mp3");
	end_se = resource->GetSounds("Resource/Sounds/SE/open.mp3");
	result_se = resource->GetSounds("Resource/Sounds/SE/result.mp3");
	high_score_se = resource->GetSounds("Resource/Sounds/SE/high_score.mp3");
	decision_se = resource->GetSounds("Resource/Sounds/SE/decision.mp3");

	// 音量調整
	ChangeVolumeSoundMem(100, in_game_bgm);
	ChangeVolumeSoundMem(255, count_down_se);
	ChangeVolumeSoundMem(255, start_se);
	ChangeVolumeSoundMem(255, end_se);
	ChangeVolumeSoundMem(255, result_se);
	ChangeVolumeSoundMem(255, high_score_se);
	ChangeVolumeSoundMem(255, decision_se);

	play_count_down_se = true;

	// フォント作成
	explanation_font = CreateFontToHandle(NULL, 70, 9);
	start_font = CreateFontToHandle(NULL, 180, 9, DX_FONTTYPE_EDGE);
	count_down_font = CreateFontToHandle(NULL, 300, 9, DX_FONTTYPE_EDGE);
	timer_font = CreateFontToHandle(NULL, 20, 9);
	ranking_font = CreateFontToHandle(NULL, 40,9);
	shake_font = CreateFontToHandle(NULL, 27, 9);

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
	hit_num_max = 0;

	mouse_img_pos_y = 110;

	mouse_up = false;
}


GameMainScene::~GameMainScene()
{
	delete bar;
	delete player;
	delete bard;
	delete ranking_data;

	// 作成したフォントデータを削除する
	DeleteFontToHandle(explanation_font);
	DeleteFontToHandle(start_font);
	DeleteFontToHandle(count_down_font);
	DeleteFontToHandle(timer_font);
	DeleteFontToHandle(ranking_font);
	DeleteFontToHandle(shake_font);
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
	hit_num_max = 0;

	bubble_location.x = 280.0f;
	bubble_location.y = 180.0f;
	bubble_num = 0;
	bubble_cnt = 0;
	up_bubble = 0.0f;

	score = 0;

	bubble_height = 0.0f;

	mouse_img_pos_y = 110;

	mouse_up = false;
	cloud->Initialize();

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
		//DrawFormatString(0, 50, 0x000000, " background_y:%f", background_y);

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

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawBox(20, 60, 170, 210, 0xffffff, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		DrawStringToHandle(55, 75, "SHAKE!", 0x000000, shake_font);

		DrawGraph(75, mouse_img_pos_y, mouse_img, TRUE);

		if (timer > 0) {
			player->Draw();
		}
		else {

			DrawExtendGraphF(bubble_location.x, bubble_location.y, 360.0f, 180.0f, bubble_img[bubble_num], TRUE);
			player->ResultDraw();
		}

		break;
	case GameState::in_fly:
		DrawGraphF(0.0f, background_y, background_img, TRUE);
		//DrawFormatString(0, 50, 0x000000, " background_y:%d", amount_y);
		cloud->Draw();
		if (bard != NULL)
		{
			//DrawFormatString(0, 70, 0x000000, " bard_y:%f", bard->GetLocation().x);
			bard->Draw();
		}
		DrawExtendGraphF(bubble_location.x-30.0f,480.0f -up_bubble, 390.0f, 480.0f, bubble_img[bubble_num], TRUE);
		//DrawFormatString(0, 100, 0x000000, "score: %f", (bar->GetCntBarShake() * 2) + (bar->GetSecondBonus() * 2));

		break;
	case GameState::result:
		cloud->Draw();

		// リトライ・タイトルボタンの描画
		DrawBox(150, 400, 250, 450, left_button_color, TRUE);
		DrawFormatString(180, 420, 0x000000, "RETRY");
		DrawBox(390, 400, 490, 450, right_button_color, TRUE);
		DrawFormatString(420, 420, 0x000000, "TITLE");
		DrawExtendGraphF(bubble_location.x - 30.0f, 480.0f - up_bubble, 390.0f, 480.0f, bubble_img[bubble_num], TRUE);

		//ランキング
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawBox(30, 30, 610, 390, 0xffffff, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);

		// 制限時間の描画
		char buf_score[100];
		sprintf_s(buf_score, "%3d", (int)score);
		DrawStringToHandle(190, 70, "MyScore", 0x000000, ranking_font);
		DrawStringToHandle(350, 70, buf_score, 0x000000, ranking_font);

		DrawStringToHandle(190, 150, "ScoreRanking", 0x000000, ranking_font);

		DrawStringToHandle(230, 250, "No.1", 0x000000, ranking_font);
		DrawStringToHandle(230, 300, "No.2", 0x000000, ranking_font);
		DrawStringToHandle(230, 350, "No.3", 0x000000, ranking_font);

		for (int i = 0; i < RANKING_DATA; i++)
		{
			// 制限時間の描画
			char buf_rank[100];
			sprintf_s(buf_rank, "%3d", (int)ranking_data->GetScore(i));
			DrawStringToHandle(350, 250+(i*50), buf_rank, 0x000000, ranking_font);

			//DrawFormatString(300 + i * 30, 100, 0xffffff, "%d", ranking_data->GetScore(i));
		}


		/*DrawFormatString(300, 130, 0xffffff, "myscore : %f", score);

		DrawFormatString(340, 60, 0xffffff, "Shake%f", bar->GetCntBarShake());*/

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

	// バーの星エフェクト更新処理
	bar->StarUpdate();

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
		//スコア算出
		score = (bar->GetCntBarShake() * 2) + (bar->GetSecondBonus() * 2);
		player->ResultUpdate();
		ColaBubbleUpdate();
		return;
	}

	bar->Update();
	player->Update();
	MouseAnim();

}

void GameMainScene::InFlyUpdate()
{
	cloud->Update();

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


	if (background_y < 0.0f)
	{
		background_y += 10.0f;

		if (background_y > 0.0f)
		{
			background_y = 0.0f;
		}
	}

	if (hit_num_max == 0 && background_y > -1300.0f)
	{
		game_state = GameState::result;
	}

	if (hit_num_max != 0 && bard == NULL)
	{
		bard = new Bard(hit_num);
		hit_num++;
	}
	else if(bard != NULL)
	{
		bard->Update();

		if (bard->GetLocation().x >= 700.0f || bard->GetLocation().x <= -60.0f)
		{
			delete bard;
			bard = NULL;

			if (hit_num >= hit_num_max)
			{
				game_state = GameState::result;

				if ((int)score < ranking_data->GetScore(2))
				{
					// リザルト発表SE再生
					PlaySoundMem(result_se, DX_PLAYTYPE_BACK);
				}
				else
				{
					// ランキング入りSE再生
					PlaySoundMem(high_score_se, DX_PLAYTYPE_BACK);
				}
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

void GameMainScene::ColaBubbleUpdate()
{
	//高さ　（振った回数×２）＋（bonus＊２）*-10
	//int score_height = (bar->GetCntBarShake() * 2) + (bar->GetSecondBonus() * 2)*-10;


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
			SetHitNumMax();
			ranking_data->SetRankingData((int)score);//スコアをランキングに
			game_state = GameState::in_fly;
			bar->Finalize();
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
			// 決定SEの再生
			PlaySoundMem(decision_se, DX_PLAYTYPE_BACK);

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
			// 決定SEの再生
			PlaySoundMem(decision_se, DX_PLAYTYPE_BACK);

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
			// 決定SEの再生
			PlaySoundMem(decision_se, DX_PLAYTYPE_BACK);

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

void GameMainScene::SetHitNumMax()
{
	if (bar->GetCntBarShake() >= 45.0f)
	{
		hit_num_max = 5;
	}
	else if (bar->GetCntBarShake() >= 35.0f)
	{
		hit_num_max = 4;
	}
	else if (bar->GetCntBarShake() >= 25.0f)
	{
		hit_num_max = 3;
	}
	else if (bar->GetCntBarShake() >= 15.0f)
	{
		hit_num_max = 2;
	}
	else if (bar->GetCntBarShake() >= 5.0f)
	{
		hit_num_max = 1;
	}
	else
	{
		hit_num_max = 0;
	}
}

void GameMainScene::MouseAnim()
{
	if (mouse_up == true)
	{
		mouse_img_pos_y -= 2;

		if (mouse_img_pos_y <= 110)
		{
			mouse_img_pos_y = 110;
			mouse_up = false;
		}
	}
	else
	{
		mouse_img_pos_y += 2;

		if (mouse_img_pos_y >= 135)
		{
			mouse_img_pos_y = 135;
			mouse_up = true;
		}
	}
}
