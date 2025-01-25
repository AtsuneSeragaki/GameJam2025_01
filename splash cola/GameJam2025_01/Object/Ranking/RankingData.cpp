#include "RankingData.h"
#include <stdio.h>
#include <string.h>

RankingData::RankingData()
{
	for (int i = 0; i < RANKING_DATA; i++)
	{
		score[i] = NULL;
		rank[i] = NULL;
	}
}

RankingData::~RankingData()
{
}

void RankingData::Initialize()
{
	// ランキングデータの読み込み
	FILE* fp = nullptr;

	// ファイルオープン
	errno_t result = fopen_s(&fp, "Resource/dat/ranking_data.txt", "r");

	// エラーチェック
	if (result != 0)
	{
		throw("Resource/dat/ranking_data.txtが開けませんでした\n");
	}

	// 対象ファイルから読み込む
	for (int i = 0; i < RANKING_DATA; i++)
	{
		fscanf_s(fp, "%d %d", &rank[i],&score[i]);
	}

	// ファイルクローズ
	fclose(fp);
}

void RankingData::Finalize()
{

}

void RankingData::SetRankingData(int score)
{
	this->score[2] = score;

	SortData();
}

void RankingData::SortData()
{
	// 選択法ソートを使用し、降順で入れ替える
	for (int i = 0; i < RANKING_DATA - 1; i++)
	{
		for (int j = i + 1; j < RANKING_DATA; j++)
		{
			if (score[i] < score[j])
			{
				int tmp = score[i];
				score[i] = score[j];
				score[j] = tmp;
			}
		}
	}

	// 順位を整列させる
	for (int i = 0; i < RANKING_DATA; i++)
	{
		rank[i] = 1;
	}
	for (int i = 0; i < RANKING_DATA - 1; i++)
	{
		for (int j = i + 1; j < RANKING_DATA; j++)
		{
			if (score[i] > score[j])
			{
				rank[j]++;
			}
		}
	}

	// ランキングデータの書き込み
	FILE* fp = nullptr;

	// ファイルオープン
	errno_t result = fopen_s(&fp, "Resource/dat/ranking_data.txt", "w");

	// エラーチェック
	if (result != 0)
	{
		throw("Resource/dat/ranking_data.txtが開けませんでした\n");
	}

	// 対象ファイルに書き込み
	for (int i = 0; i < RANKING_DATA; i++)
	{
		fprintf(fp, "%d %d\n", rank[i],score[i]);
	}

	// ファイルクローズ
	fclose(fp);
}
