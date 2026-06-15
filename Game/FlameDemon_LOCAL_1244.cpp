#include "UnlimitedLib/UnlimitedLib.h"
#include "FlameDemon.h"
#include "Game.h"
#include "Stage.h"
#include "Player.h"
#include "IceSpear.h"
#include "IceBlock.h"
#include "hitblock.h"
#include "MainGame.h"
#include "GameSound.h"
#include "Animation.h"
#include <stdlib.h>
#include <time.h>
#include "Enemy.h"
#include "Fireball.h"


FlameDemon demon;
BigFire fire_holizontal;
BigFire fire_rain[BIGFIRE_MAX];
int demonhandle[300];
float demon_height=DEMON_HIGHT/1.2;
float demon_width=DEMON_WIDTH;
float bigfire_size = BIG_FIRE/1.2;
bool hiticespear_demon;
int demon_mode;
float tackle_count;
int BossClearSE;
int bigFireball[12];
float cooldown;
int r_firecount;
int demon_pattern;
bool isatk;
bool hitbigfire;
bool canshotfire;
int demonEffect[24];
int demonEffectState;
bool demonEffect_Active;
//炎の魔人行動
enum {
	IDLE,			  //待機
	FIRE_HORIZONTAL,  //ファイヤーボール（横）
	TACKLECHARGE,     //タックルチャージ
	TACKLE,           //タックル
};
//炎の魔人アニメーション
AnimationUniqueInfo demon_anim;
AnimationInfo demon_animptn = {
	//-1ループ,-2一度の再生で終了
	{

		/*00待機*/{20.0f,{0,1,2,3,4,5,6,7,8,9,
				          10,11,12,13,14,15,16,17,18,19,
		                  20,21,22,23,24,25,26,27,28,29,
		                  30,31,32,33,34,35,36,37,38,39, -1}},

		/*01攻撃ファイヤーボール（横）*/ {20.0f,{50,51,52,53,54,55,56,57,58,59,
												 60,61,62,63,64,65,66,67,68,69,
												 70,71,72,73,74,75,76,77,78,79,
												 80,81,82,83,84,85,86,87,88,89,
												 90,91,92,93,-2}},

		 /*02タックルチャージ*/{20.0f,{100,101,102,103,104,105,106,107,108,109,
									   110,111,112,113,114,115,116,117,118,119,
									   120,121,122,123,124,125,126,127,128,129,
									   130,131,132,133,134,135,136,137,138,139,
									   -1}},

		  /*03タックル*/{30.0f,{150,151,152,153,154,155,156,157,158,159,
								160,161,162,163,164,165,166,167,168,169,
								170,171,172,173,174,175,176,177,178,179,
								180,181,182,183,184,185,186,187,188,189,
								190,191,192,193,194,195,196,197,198,199, 
								200,201,202,-2}},
							 
	}
};

int demon_state;

//ファイヤーボール（大）アニメーション
AnimationUniqueInfo bigfire_anim;
AnimationInfo bigfire_animptn = {
	//-1ループ,-2一度の再生で終了
	{
		{10.0f,{0,1,2,3,4,5,6,7,8,9,10, 11,-1}},
	}
};

//タックルエフェクトアニメーション
AnimationUniqueInfo demonEffect_anim;
AnimationInfo demonEffect_animptn = {
	//-1ループ,-2一度の再生で終了
	{
		{10.0f,{0,1,2,3,4,5,6,7,8,9,
		        10,11,12,13,14,15,16,17,18,19,
				20,21,22,23,-1}},
	}
};
enum {
	CHARGE_EFFECT,
	TACKLE_EFFECT,
	DEAD_EFFECT,
};


void FlameDemon_IDLE();
void FireHorizontal_Update();

void FlameDemon_Initialize()
{

	isatk = false;
	demon_state = IDLE;
	demon_anim.isPlaying = true;
	BossClearSE = CreateSEOgg("assets/SE/se_clear_00.ogg");
	CreateSpriteMatrix("assets/enemy/boss_stop_00.png",
		DEMON_WIDTH, DEMON_HIGHT,
		8, 5, 40, demonhandle);
	CreateSpriteMatrix("assets/enemy/boss_fireball_horizontal_00.png",
		DEMON_WIDTH, DEMON_HIGHT,
		8, 7, 54, &demonhandle[50]);
	CreateSpriteMatrix("assets/enemy/boss_tackle_charge_00.png",
		DEMON_WIDTH, DEMON_HIGHT,
		8, 5, 40, &demonhandle[100]);
	CreateSpriteMatrix("assets/enemy/boss_tackle_attack_00.png",
		DEMON_WIDTH, DEMON_HIGHT,
		8, 7, 53, &demonhandle[150]);
	CreateSpriteMatrix("assets/enemy/atk/bigfireball_00.png",
		BIG_FIRE, BIG_FIRE,
		6, 2, 12, bigFireball);
	CreateSpriteMatrix("assets/enemy/boss_tackle_effect_00.png",
		1080, 1080,
		6, 4, 24, demonEffect);

	demon.isAlived = false;
	hiticespear_demon = false;
	tackle_count = 0;
	r_firecount = 0;
	fire_holizontal.isAlived = false;
	demonEffect_Active = false;

	for (int i = 0; i < BIGFIRE_MAX; i++)
	{
		fire_rain[i].isAlived = false;
	}

	

	cooldown = 0;
}
void FlameDemon_Release()
{
	ReleaseSpriteMatrix(demonhandle, 300);
	ReleaseSpriteMatrix(bigFireball, 12);
	ReleaseSpriteMatrix(demonEffect, 24);
}
 
void FlameDemon_Update() {
	FlameDemon_AnimUpdate();
	Animation(DeltaTime, demon_anim, demon_animptn);
	Animation(DeltaTime, bigfire_anim, bigfire_animptn);
	Animation(DeltaTime, demonEffect_anim, demonEffect_animptn);
	FlameDemon_IsHitBullet();
	FireHorizontal_Update();
		switch (demon_state) {
		case IDLE:
			FlameDemon_IDLE();
			break;
		case FIRE_HORIZONTAL:
			FlameDemon_FireHorizontal();
			break;
		case TACKLECHARGE:
			FlameDemon_Tackle();
			break;
		case TACKLE:
			FlameDemon_Tackle();
			break;
		default:
			break;
		}
	
}

 void FlameDemon_Render()
 {
	 if (demon.isAlived)
	 {
		 //炎の魔人描画
		 if (demon.if_frame % 2 == 0)
		 {
			 if (demon.Dir)
			 {
				 RenderSpritePos(demonhandle[demon_anim.sprite_index], demon.positionX - DEMON_WIDTH / 2, demon.positionY - DEMON_HIGHT + SCREEN_REMAINDER);
			 }
			 else
			 {
				 RenderSpritePosLR(demonhandle[demon_anim.sprite_index], demon.positionX- DEMON_WIDTH / 2, demon.positionY - DEMON_HIGHT + SCREEN_REMAINDER);
			 }
		 }

	     //ファイヤーボール描画（横)
	     if (fire_holizontal.isAlived)
	     {
	 	     if (demon.Dir)
	 	     {
	 	       	 RenderSpritePos(bigFireball[bigfire_anim.sprite_index], fire_holizontal.positionX -BIG_FIRE/2, fire_holizontal.positionY - BIG_FIRE / 2);
	 	     }
	 	     else
	 	     {
	 		     RenderSpritePosLR(bigFireball[bigfire_anim.sprite_index], fire_holizontal.positionX - BIG_FIRE / 2, fire_holizontal.positionY - BIG_FIRE / 2);
	 	     }
	     }
		 if (demonEffect_Active) {
			 RenderSpritePos(demonEffect[demonEffect_anim.sprite_index], demon.positionX - DEMON_WIDTH / 2, demon.positionY - DEMON_HIGHT + SCREEN_REMAINDER);
		 }
	 }
 }

 void FlameDemon_IDLE() {
	 if (!fire_holizontal.isAlived) {
		 cooldown += DeltaTime;
		 canshotfire = true;
	 }
	 if (cooldown >= COOLDOWN_COUNT) {
		 cooldown = COOLDOWN_COUNT;
	 }
	 if (cooldown == COOLDOWN_COUNT) {
		 FlameDemon_ChangePattern();
		 cooldown = 0;
	 }
 }


 //タックル処理
 void FlameDemon_Tackle()
 {

	 tackle_count+=DeltaTime;
	 if (tackle_count >= TACKLECOUNT) {
		 tackle_count = TACKLECOUNT;
	 }
	 if (demon_state == TACKLE && !demon_anim.isPlaying) {
		 demon_state = TACKLECHARGE;
	 }
	 if (tackle_count >= TACKLECOUNT)
	 {
		 demon_state = TACKLE;
		 if (demon.Dir)
		 {
			 //左向きの場合
			 demon.positionX -= DEMON_SPEED;
			 //画面端処理
			 if (demon.positionX < 0 + DEMON_WIDTH / 2 )
			 {
				 demon.positionX = DEMON_WIDTH / 2;
				 tackle_count = 0;
			 }
		 }
		 else
		 {
			 //右の場合
			 demon.positionX += DEMON_SPEED;
			 //画面端処理
			 if (demon.positionX > SCREEN_WIDTH - DEMON_WIDTH / 2)
			 {
				 demon.positionX = SCREEN_WIDTH - DEMON_WIDTH / 2;
				 tackle_count = 0;
			 }
		 }

	 }
}


 void FireHorizontal_Update()
 {
	 if (fire_holizontal.isAlived) {
		 if (demon.Dir)
		 {
			 fire_holizontal.positionX -= fire_holizontal.speed;
		 }
		 else
		 {
			 fire_holizontal.positionX += fire_holizontal.speed;
		 }
	 }

	 //画面端処理
	 if (fire_holizontal.positionX < 0 || fire_holizontal.positionX>SCREEN_WIDTH)
	 {
		 fire_holizontal.isAlived = false;
	 }

 }

 //ファイヤーボール処理（横）
 void FlameDemon_FireHorizontal()
 {
	 if (!fire_holizontal.isAlived&& canshotfire)
	 {
		 // isatk = true;
		 if (demon_anim.sprite_index >= 72) {
			 Set_FireHorizontal(demon.positionX, demon.positionY);
			 canshotfire = false;
		 }

	 }

	
 }

 //アイススピアと炎の魔人の当たり判定
 void FlameDemon_IsHitBullet()
 {
	 if (demon.if_frame > 0)
	 {
		 demon.if_frame--;
	 }
	 for (int i = 0; i < BULLET_MAX; i++)
	 {
		 if (icespear[i].isAlived == false) { continue; }
			 if (demon.isAlived)
			 {
				 hiticespear_demon = HitBullet(&icespear[i].positionX, &icespear[i].positionY, &IceSpear_radius, &IceSpear_height,
					 &demon.positionX, &demon.positionY, &demon_width, &demon_height, &demon.hp, &player.atk, &icespear[i].isAlived);
				 if (hiticespear_demon)
				 {
					 demon.if_frame = ENEMY_IF_FRAME;
				 }
				 if (demon.hp <= 0)
				 {
					 demon.isAlived = false; MAIN_Release();
					 PlaySE(BossClearSE, false, 0.3f); BGM_Stop(); 	SetFadeOut(1, 1, 1, 2.0f);InitializeScene(SCENE_RESULT);
					 return;
				 }
			 }
	 }
	 for (int i = 0; i < ICEBLOCK_MAX; i++) {
		 if (fire_holizontal.isAlived) {
			 if (IceBlock[i].isAlived == false) { continue; }
			 hitbigfire = CircleVsRect(fire_holizontal.positionX, fire_holizontal.positionY, bigfire_size / 2,
				 IceBlock[i].positionX, IceBlock[i].positionY, Ice_Size, Ice_Size);
			 if (hitbigfire) {
				 fire_holizontal.isAlived = false;
			 }
		 }
	 }
 }

 //炎の魔人アニメーション切り替え
 void FlameDemon_AnimUpdate()
 {
	 //アニメーション終了時、待機(IDLE)に戻す
	 if (!demon_anim.isPlaying) {

		 //タックル画面端到達時、炎の魔人の向き変更
		 if (demon_state == TACKLE|| demon_state==TACKLECHARGE) {
			 demon.Dir = !demon.Dir;
		 }

		 demon_state = IDLE;
	 }
	 ChangeAnimation(demon_state, demon_anim, demon_animptn);
 }

 void FlameDemon_EffectAnimUpdate() {
	 switch (demon_state)
	 {
	 case TACKLECHARGE:
		 demonEffectState = CHARGE_EFFECT;
		 break;
	 case TACKLE:
		 demonEffectState = TACKLE_EFFECT;
		 break;
	 default:
		 break;
	 }

 }

 //行動パターン変化
 void FlameDemon_ChangePattern()
 {
	 srand(time(NULL));
	 demon_state = rand()%2 + 1;

 }
 void FlameDemon_Set(float x,float y)
 {
	 demon.isAlived = true;
	 demon.positionX = x;
	 demon.positionY = y;
	 demon.hp = DEMON_HP;
	 demon.atk = DEMON_ATK;
	 demon.if_frame = 0;
	 demon.Dir = true;//最初は左向き
	 demon.speed = 0;
 }
 void Set_FireHorizontal(float x, float y)
 {
	 if (demon.Dir) {
		 fire_holizontal.positionX = x - DEMON_WIDTH/1.4;
	 }
	 else {
		 fire_holizontal.positionX = x + DEMON_WIDTH/1.4;
	 }
	 fire_holizontal.positionY = y-DEMON_HIGHT/3+SCREEN_REMAINDER;
	 fire_holizontal.speed = H_BIGFIRE_SPEED;
	 fire_holizontal.isAlived = true;
	
 }

