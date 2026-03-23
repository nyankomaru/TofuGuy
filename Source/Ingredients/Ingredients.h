#pragma once

//SpriteやTexture,Animationを使うためのヘッダファイル
#include "Fwk/Graphics.h"

//Vector2fクラスを使う為のヘッダファイル
#include "Lib/Math/Vector.h"
using namespace Lib::Math;

//コリジョンクラスを使うのでinclude
#include "Fwk/Collision/Collision.h"
//コリジョンクラスの名前空間を使う
using namespace Fwk::Collision;

//向きを使う
#include "GameDef.h"

//材料クラス
class Ingredients
{

	enum class Status
	{
		Idle,
		Moving,
		Falling,
	};

public :

	//材料の初期化
	virtual void Init();

	//材料の更新
	virtual void Update();

	//材料の描画
	void Render();

	//材料の後片付け
	 void Term();

	//アクティブか否かを取得
	bool IsActive();

	//アクティブ状態を設定
	void SetActive(bool bActive);

	//位置を返す
	Vector2f GetPosition();

	//位置を設定
	void SetPosition(Vector2f position);

	//ゴールの上か否か取得
	bool IsGoal();

protected:

	//コールバック関数
	void OnHit(const Collision& A,const Collision& B);

	//アイドル
	void UpdateIdle();

	//移動
	void UpdateMoving(Vector2f TargetPos);

	//落下
	void UpdateFalling();

protected:

	//表示用スプライト
	Sprite mSprite;
	//テクスチャ
	Texture mTexture;

	//オブジェクトの位置
	Vector2f mPosition;
	//コリジョン
	Collision mCollision;
	//ゴール用コリジョン
	Collision mGoalCollision;

	//アクティブ状態
	bool mActive;
	//現在の状態
	Status mStatus;
	//移動先の座標
	Vector2f TargetPos;
	//ゴールの上か
	bool mIsGoal;
};