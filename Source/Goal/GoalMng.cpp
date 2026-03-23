#include "GoalMng.h"

//GetMapを使う為
#include "GameObjectMng/GameObjectMng.h"

//SaFE_DELETEを使う為に
#include "Lib/LibDef.h"

//扱うクラスの読み込み
#include "Goal/IngredientsGoal.h"
#include "Goal/PlayerGoal.h"


//初期化
void GoalMng::Init()
{
	//アクティブなポインタの配列を空に
	mActiveGoal.clear();
}

//更新
void GoalMng::Update()
{
	//アクティブなゴール全て更新
	for (int i = 0; i < mActiveGoal.size(); ++i)
	{
		mActiveGoal[i]->Update();
	}

	//更新後非アクティブとなったものがあるならば取り除く
	for (int i = 0; i < mActiveGoal.size(); ++i)
	{
		if (mActiveGoal[i]->IsActive())
		{
			//mActiveGoal[i]がアクティブのままであれば
			//次の要素を処理するのでiをインクリメント
			++i;
		}
		else
		{
			//mActiveGoal[i]が非アクティブとなっていたらその要素を取り除く
			mActiveGoal.erase(mActiveGoal.begin() + i);

			//※ここでは i をインクリメントしない
			//要素が取り除かれたら後の要素が前に詰められるらしい
		}
	}

}

//描画
void GoalMng::Render()
{
	//アクティブなゴール全て表示
	for (int i = 0; i < mActiveGoal.size(); ++i)
	{
		mActiveGoal[i]->Render();
	}
}

//後片付け
void GoalMng::Term()
{
	//アクティブなポインタの配列を空に
	mActiveGoal.clear();
	//全部のゴールプールを破棄
	DestroyPoolAll();
}

//オブジェクトプールの生成
void GoalMng::GeneratePool(string GoalNamne, int poolSize)
{
	//食材用ゴール
	if (GoalNamne == "IngredientsGoal")
	{
		mGoalPools[GoalNamne].Init<IngredientsGoal>(poolSize);
	}

	//プレイヤー用
	if (GoalNamne == "PlayerGoal")
	{
		mGoalPools[GoalNamne].Init<PlayerGoal>(poolSize);
	}
}

//全オブジェクトプールの破棄
void GoalMng::DestroyPoolAll()
{
	//バッファの破棄
	//食材用ゴール
	DestoryPool("IngredientsGoal");
	//プレイヤー用
	DestoryPool("PlayerGoal");
}

//指定したゴールのプール破棄
void GoalMng::DestoryPool(string GoalName)
{
	//ゴールプールの破棄
	if (mGoalPools.count(GoalName) > 0)
	{
		//ゴールクラスの片づけを呼んでいる
		mGoalPools[GoalName].Term();
		//指定された名前の要素を削除している？
		mGoalPools.erase(GoalName);
	}
}

//ゴールの生成
void GoalMng::CreateGoal(string GoalName, Vector2f position)
{
	//取得した非アクティブのオブジェクトのアドレスを保存するポインター型変数
	Goal* pGoal = nullptr;

	//特定の名前の要素数が0より大きかったら非アクティブのオブジェクトがある
	//そしたら変数にその名前に対応した値のうち一つを代入（？）
	if (mGoalPools.count(GoalName) > 0)
	{
		pGoal = mGoalPools[GoalName].Alloc();
	}

	//非アクティブの敵がいない（上のifが処理されずにnullのまま）
	if (pGoal == nullptr)
	{
		return;
	}

	//位置を設定
	pGoal->SetPosition(position);

	//アクティブなゴールの配列に追加
	mActiveGoal.push_back(pGoal);
}

//CSVからの生成
void GoalMng::CreateGoalCSV(CSVData* pCsvData)
{
	//引数がnullなら何もしない
	if (pCsvData == nullptr)
	{
		return;
	}

	//名前、X、Yの3つで1組
	//ゴールの数を判定
	int GoalNum = pCsvData->GetElementCount() / 3;
	//ゴールの数分繰り返し
	for (int i = 0; i < GoalNum; ++i)
	{
		//配置データの先頭（名前）の要素番号（列数）
		int index = i * 3;
		//名前の取得
		string  GoalName;
		pCsvData->GetString(index, &GoalName);
		//配置先の座標
		Vector2f position;
		float TileSize = GetMap()->GetTileSize();
		position.x = (float)pCsvData->GetInt(index + 1) * TileSize + TileSize / 2;
		position.y = (float)-(pCsvData->GetInt(index + 2) * TileSize + TileSize / 2);

		//取り出した値をもとに生成
		CreateGoal(GoalName, position);
	}

}