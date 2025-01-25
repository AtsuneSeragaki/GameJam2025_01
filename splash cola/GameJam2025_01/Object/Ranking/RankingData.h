#pragma once

#define RANKING_DATA 3

class RankingData
{
private:
	int score[RANKING_DATA];  // 泡の高さ
	int rank[RANKING_DATA];   // 順位

public:

	// コンストラクタ
	RankingData();

	// デストラクタ
	~RankingData();

	// 初期処理
	void Initialize();

	// 終了処理
	void Finalize();

	// ランキングデータの設定
	void SetRankingData(int score);

	// データ入れ替え処理
	void SortData();
};