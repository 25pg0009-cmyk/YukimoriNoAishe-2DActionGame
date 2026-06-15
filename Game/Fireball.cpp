#include "UnlimitedLib/UnlimitedLib.h"
#include "Fireball.h"
#include "Enemy.h"
#include "flameimp.h"
#include "Stage.h"
#include "Game.h"
#include "IceSpear.h"
#include "Player.h"
#include "IceBlock.h"
#include "Animation.h"
#include <math.h>

#define  FIRE_BALL_SIZE 96
#define  FIRE_EFFECT_SIZE 1200
#define PI 3.14f

float fire_width = FIRE_BALL_SIZE;
float fire_size= FIRE_BALL_SIZE;
float fire_radius= 96.0f/2.0f;
int fire_ball[15];
int fire_hit[5];
int white;
bool hiticeblock;
FireParam fireParam[FIRE_BALL_MAX];

AnimationUniqueInfo fire_anim;
AnimationInfo fire_animptn = {
	//-1ループ,-2一度の再生で終了
	{15.0f,{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14, -1}},
};

AnimationUniqueInfo fire_effectAnim;
AnimationInfo fire_effectAnimptn = {
	//-1ループ,-2一度の再生で終了
	{5.0f,{0,1,2, -2}},
};

//ヒットエフェクトアニメーション
struct FireHitEffect {
	bool isAlived;
	float x, y;
	AnimationUniqueInfo fire_hitAnim;
};
FireHitEffect fire_hiteffect[FIRE_BALL_MAX];

AnimationInfo fire_hitAnimptn = {
	//-1ループ,-2一度の再生で終了
	{10.0f,{0,1,2,3,4 ,-2}},
};

int FireSprite = 0;

float CrossProduct(float x1, float y1, float x2, float y2);
//内積計算(A・B)
float DotProduct(float x1, float y1, float x2, float y2);

void hitBlockBall(int n);
void SetFireHitEffect(int n, float x, float y);

void FIRE_Initialize() {
    CreateSpriteMatrix("assets/enemy/atk/fireball_00.dds",
    96,96,
    15,1,15,
	fire_ball
    );
	CreateSpriteMatrix("assets/enemy/atk/fireball_hit_00.png",
		FIRE_EFFECT_SIZE, FIRE_EFFECT_SIZE,
		5, 1,5,
		fire_hit
	);
	for (int i = 0;i < 5;i++) {
		SetSpriteSize(fire_hit[i], FIRE_EFFECT_SIZE*0.14f, FIRE_EFFECT_SIZE*0.14f);
	}
    for (int b = 0;b < FIRE_BALL_MAX;b++) {
		fire_hiteffect[b].isAlived = false;
		fireParam[b].IsActive = false;
		fireParam[b].speed = 0.0f;
	}
}

void FIRE_Release() {
	ReleaseSpriteMatrix(fire_ball,15);
	
	ReleaseSpriteMatrix(fire_hit, 5);
}

void FIRE_Update() {
	Animation(DeltaTime, fire_anim, fire_animptn);
	Animation(DeltaTime, fire_effectAnim, fire_effectAnimptn);
   for (int b = 0;b < FIRE_BALL_MAX;b++) {
	  	
		Animation(DeltaTime, fire_hiteffect[b].fire_hitAnim, fire_hitAnimptn);
		if (!fireParam[b].IsActive) { continue; }
		   fireParam[b].speed = 5.0f;

		   //向いている方向への移動値を算出
		   float moveX = cosf(fireParam[b].Dir);
		   float moveY = sinf(fireParam[b].Dir);
		   //弾の移動
		   fireParam[b].x += moveX * fireParam[b].speed * (DeltaTime * 50);
		   fireParam[b].y += moveY * fireParam[b].speed * (DeltaTime * 50);

		   //ホーミング機能の有無
		   if (fireParam[b].IsHoming) {
			   //弾からターゲットへの方向を算出
			   float targetX = player.positionX - fireParam[b].x;//プレイヤー－弾
			   float targetY = player.positionY - fireParam[b].y;

			   //プレイヤーのいる方向に角度を曲げていく
				//１はa(弾の向き)、２はb(弾からターゲットへの方向)
			   //Aベクトル
			   float x1 = moveX;
			   float y1 = moveY;
			   //Bベクトル
			   float x2 = targetX;
			   float y2 = targetY;

			   //内積の計算
			   float naiseki = DotProduct(x1, y1, x2, y2);
			   //前方の左右45°(90°)にいる場合飲み追跡する
			   float length1 = sqrtf(x1 * x2 + y1 * y2);//aの長さ
			   float length2 = sqrtf(x1 * x2 + y1 * y2);//bの長さ
			   float c = naiseki / (length1 * length2);
			   float limit = cosf((PI / 180.0f) * 90.0f);
			   if (c > limit) {
				   //外積を計算
				   float gaiseki = CrossProduct(x1, y1, x2, y2);
				   //回転速度
				   float rotSpeed = (PI / 45.0f) * 1.0f;
				   if (gaiseki < 0.0f) {
					   fireParam[b].Dir -= rotSpeed;
				   }
				   else {
					   fireParam[b].Dir += rotSpeed;
				   }
			   }
		   }

		   //FIRE_ACTIVE_FRAME超えた消す
		   fireParam[b].count += DeltaTime;
		   if (fireParam[b].count >= FIRE_ACTIVE_FRAME)
		   {
			   fireParam[b].IsActive = false;
			  
		   }

	 float radius = fire_width / 2.0f;
	 //アイスブロックとの当たり判定
	  for (int i = 0; i < ICEBLOCK_MAX; i++) {
	     if (IceBlock[i].isAlived == false) { continue; }//死んでいるアイスブロックはスキップ
		    hiticeblock = CircleVsRect(fireParam[b].x, fireParam[b].y, radius,
		    IceBlock[i].positionX, IceBlock[i].positionY, Ice_Size, Ice_Size);
		  if (hiticeblock) {
			 fireParam[b].IsActive = false;
		  }
	  }
	  hitBlockBall(b);  //ステージブロックとの当たり判定
	  if (!fireParam[b].IsActive) {
     	  SetFireHitEffect(b,fireParam[b].x,fireParam[b].y);
      }
	   //エフェクト更新
	   if (fire_hiteffect[b].isAlived) {
		   if (!fire_hiteffect[b].fire_hitAnim.isPlaying) {
			   fire_hiteffect[b].isAlived = false;
		   }
	   }
   }
}

void FIRE_Render() {
	for (int b = 0;b < FIRE_BALL_MAX;b++) {
		if (fireParam[b].IsActive) {
			if (fireParam[b].viewing_dir) {//右
				//弾の表示
				RenderSpritePos(fire_ball[fire_anim.sprite_index], fireParam[b].x - FIRE_BALL_SIZE/2 - CameraX, fireParam[b].y - FIRE_BALL_SIZE / 2 );
			}
			else {//左
				//弾の表示
				RenderSpritePosLR(fire_ball[fire_anim.sprite_index], fireParam[b].x - FIRE_BALL_SIZE/2 - CameraX, fireParam[b].y - FIRE_BALL_SIZE / 2 );
			}
		}
		if (fire_hiteffect[b].isAlived) {
           RenderSpritePos(fire_hit[fire_hiteffect[b].fire_hitAnim.sprite_index], fire_hiteffect[b].x- (FIRE_EFFECT_SIZE * 0.14f) /2- CameraX, fire_hiteffect[b].y - (FIRE_EFFECT_SIZE * 0.14f) / 2);
		}
    }
}

void Fire2(float* x, float* y, float* speed, float* Dir) {
	int n = -1;
	//使われていない弾を探し出す
	for (int b = 0;b < FIRE_BALL_MAX;b++) {
		if (fireParam[b].IsActive == false) {//i番目が生存しているか確認する
			//生存していない場合はｎに空いてる番号を代入する
			n = b;
			break;
		}
	}
	//空きがなければ終了
	if (n == -1) {
		return;
	}
	fireParam[n].x =* x;
	fireParam[n].y =* y - ENEMY_FI_HEIGHT / 4;
	fireParam[n].Dir =* Dir;
	fireParam[n].speed =* speed;
	fireParam[n].count = 0;
	fireParam[n].IsActive = true;
	fireParam[n].IsHoming = false;
	fireParam[n].atk = FIRE_ATK;
	// 🔹発射時にだけ向きを決める
	if (player.positionX > fireParam[n].x) {
		fireParam[n].viewing_dir = false; // 右
	}else {
		fireParam[n].viewing_dir = true;  // 左
	}
}

void hitBlockBall(int n) {
	//右当たり判定
	//右ブロックチェック
	int RightX = fireParam[n].x + FIRE_BALL_SIZE/2;
	int tileR = STAGE_GetTileNo(RightX, fireParam[n].y-1);
	//頭上の右もチェック
	int tileHeadR = STAGE_GetTileNo(RightX, fireParam[n].y - FIRE_BALL_SIZE / 2);
	//中央のブロックもチェック
	int tileBodyR = STAGE_GetTileNo(RightX, fireParam[n].y - FIRE_BALL_SIZE / 2);
	if (tileR > 0 || tileHeadR >0 || tileBodyR > 0) {
		fireParam[n].IsActive = false;
		return;
	}
	//左当たり判定
	//左ブロックチェック
	int LeftX = fireParam[n].x - FIRE_BALL_SIZE/2;
	int tileL = STAGE_GetTileNo(LeftX, fireParam[n].y);
	//頭上の左もチェック
	int tileHeadL = STAGE_GetTileNo(LeftX, fireParam[n].y - FIRE_BALL_SIZE / 2);
	//中央の左もチェック
	int tileBodyL = STAGE_GetTileNo(LeftX, fireParam[n].y - FIRE_BALL_SIZE / 2);
	if (tileL > 0 || tileHeadL > 0 || tileBodyL > 0) {
		fireParam[n].IsActive = false;
	}
}

// clamp(クランプ)関数（値をmin～maxの範囲に収める）
float Clamp(float value, float min, float max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}



// 円と矩形の当たり判定
bool CircleVsRect(float cX, float cY, float cR,
	float rX, float rY, float rW, float rH)
{
	// 矩形内で円の中心に一番近い点を求める
	float closestX = Clamp(cX, rX, rX + rW);
	float closestY = Clamp(cY, rY , rY + rH);

	// 円の中心とその点の距離を比較
	float dx = cX - closestX;
	float dy = cY - closestY;
	
	return (dx * dx + dy * dy) <= (cR * cR);
}

//外積計算(A×B)
float CrossProduct(float x1, float y1, float x2, float y2) {
	//|A||B|sinθ
	return x1 * y2 - y1 * x2;
}
//内積計算(A・B)
float DotProduct(float x1, float y1, float x2, float y2) {
	//|A||B|cosθ
	return x1 * x2 + y1 * y2;
}

void SetFireHitEffect(int n, float x, float y) {
	fire_hiteffect[n].x = x;
	fire_hiteffect[n].y = y;
	//アニメーション初期化
	fire_hiteffect[n].fire_hitAnim.isPlaying = true;
	fire_hiteffect[n].isAlived = true;
	fire_hiteffect[n].fire_hitAnim.frame = 0;
	fire_hiteffect[n].fire_hitAnim.ID = 0;
	fire_hiteffect[n].fire_hitAnim.sprite_index = fire_hitAnimptn.param[0].ptn[(int)fire_hiteffect[n].fire_hitAnim.frame];
}
