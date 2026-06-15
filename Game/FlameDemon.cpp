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
#include "UI.h"

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
int bigFireball[24];
float cooldown;
int demon_pattern;
bool isatk;
bool hitbigfire;
bool canshotfire;
int demonEffect[150];
int demonEffectState;
bool demonEffect_Active;
int demonEffect_size_h;
int demonEffect_size_w;
bool isStartdestroy_effect;
bool isPlay_demonanim;
bool demonFadeWait;
#define TACLECHARGEEFFECT 1080
#define DEADEFFECT 789
#define TACKLEEFFECT_W 916
#define TACKLEEFFECT_H 672
#define BIGFIREEFFECT 1200
int bigFireSize;
int hitbigFire[5];
bool hitbigfireActive;
int prev_demon_state;
int DelteFire_count;
bool bossFade;
bool notHitboss;

//炎の魔人行動
enum {
	IDLE,			  //待機
	FIRE_HORIZONTAL,  //ファイヤーボール（横）
	TACKLECHARGE,     //タックルチャージ
	TACKLE,           //タックル
	DEAD,			  //死亡
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
												 90,91,92,93,-2},
	                                  /*SE*/{0,15,0,0,0,0,0,0,0,0,
                                                  0,0,0,0,0,0,0,0,0,0,
                                                  0,0,0,1,0,0,0,0,0,0,
                                                  0,0,0,0,0,0,0,0,0,0,
                                                  0,0,0,0,0,0,0,0,0,0,
                                                  0,0,0,0,}},

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
								200,201,202,-2} ,
								/*SE*/     {0,16,0,0,0,0,0,0,0,0,
												  0,0,0,0,0,0,0,0,0,0,
												  0,0,0,0,0,0,0,0,0,0,
												  0,0,0,0,0,0,0,2,0,0,
												  0,0,0,0,0,0,0,0,0,0,
                                                  0,0,0,}},
		  /*04死亡*/{18.0f,{220,221,222,223,224,225,226,227,228,229,
		                    230,231,232,233,234,235,236,237,238,239,
							240,241,242,243,244,245,246,247,248,249,
							250,251,252,253,254,255,256,257,258,259,
							-2,},{0,14,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0}},
	}
};

int demon_state;

//ファイヤーボール（大）アニメーション
AnimationUniqueInfo bigfire_anim;
AnimationInfo bigfire_animptn = {
	//-1ループ,-2一度の再生で終了
	{
		/*00ファイヤーボール*/{10.0f,{0,1,2,3,4,5,6,7,8,9,10, 11,-1}},
	}
};

//ファイヤーボールヒットアニメーション
AnimationUniqueInfo bigfireHit_anim;
AnimationInfo bigfire_Hitanimptn = {
	//-1ループ,-2一度の再生で終了
	{
		/*01ファイヤーボールエフェクト*/{10.0f,{0,1,2,3,4,-2}},
	}
};
struct HitBigFireEffect{
	float x;
	float y;
};
HitBigFireEffect hitbigfireeffect;

//タックルエフェクトアニメーション
AnimationUniqueInfo demonEffect_anim;
AnimationInfo demonEffect_animptn = {
	//-1ループ,-2一度の再生で終了
	{
		/*チャージエフェクト*/{10.0f,{0,1,2,3,4,5,6,7,8,9,
		        10,11,12,13,14,15,16,17,18,19,
				20,21,22,23,-1}},
		/*死亡エフェクト*/{9.0f,{50,51,52,53,54,55,56,57,58,59,
								  60,61,62,63,-2}},
		/*タックルエフェクト*/{20.0f,{100,101,102,103,104,105,106,107,108,109,
		                              110,111,112,113,114,115,116,117,118,119,
									  120,121,122,123,124,125,126,127,128,-1}},
																
	}
};
enum {
	CHARGE_EFFECT,
	DEAD_EFFECT,
	TACKLE_EFFECT,
};


void FlameDemon_IDLE();
void FireHorizontal_Update();
void FlameDemon_Dead();
void FlameDemon_EffectAnimUpdate();


void FlameDemon_Initialize()
{
	notHitboss = false;
	DelteFire_count=0;
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
	CreateSpriteMatrix("assets/enemy/boss_dead_00.png",
		DEMON_WIDTH, DEMON_HIGHT,
		8, 5, 40, &demonhandle[220]);
	CreateSpriteMatrix("assets/enemy/atk/bigfireball_00.png",
		BIG_FIRE, BIG_FIRE,
		6, 2, 12, bigFireball);
	CreateSpriteMatrix("assets/enemy/atk/fireball_hit_00.png",
		BIGFIREEFFECT, BIGFIREEFFECT,
		5, 1, 5, hitbigFire);
	for (int n = 0; n < 5; n++) {
		SetSpriteSize(hitbigFire[n], BIGFIREEFFECT * 0.4, BIGFIREEFFECT * 0.4);
	}
	CreateSpriteMatrix("assets/enemy/boss_tackle_effect_00.png",
		TACLECHARGEEFFECT, TACLECHARGEEFFECT,
		6, 4, 24, demonEffect);
	for (int i = 0; i < 24; i++) {
		SetSpriteSize(demonEffect[i], TACLECHARGEEFFECT / 1.5, TACLECHARGEEFFECT / 1.5);
	}
	CreateSpriteMatrix("assets/enemy/boss_destroy_effect_00.png",
		DEADEFFECT, DEADEFFECT,
		7, 2, 14, &demonEffect[50]);
	CreateSpriteMatrix("assets/enemy/boss_tackle_atk_effect_00.png",
		TACKLEEFFECT_W, TACKLEEFFECT_H,
		10, 3, 29, &demonEffect[100]);

	demon.isAlived = false;
	hiticespear_demon = false;
	tackle_count = 0;
	fire_holizontal.isAlived = false;
	demonEffect_Active = false;
	isStartdestroy_effect = false;
	demonFadeWait = false;
	for (int i = 0; i < BIGFIRE_MAX; i++)
	{
		fire_rain[i].isAlived = false;
	}
	cooldown = 0;
}

void FlameDemon_Release()
{
	ReleaseSpriteMatrix(demonhandle, 300);
	ReleaseSpriteMatrix(bigFireball, 24);
    ReleaseSpriteMatrix(demonEffect, 150);
	ReleaseSpriteMatrix(hitbigFire, 5);
}
 
void FlameDemon_Update() {
	UI_SetLifeBoss(demon.hp, DEMON_HP);
	prev_demon_state = demon_state;
	FlameDemon_AnimUpdate();
	Animation(DeltaTime, demon_anim, demon_animptn);
	Animation(DeltaTime, bigfire_anim, bigfire_animptn);
	Animation(DeltaTime, bigfireHit_anim, bigfire_Hitanimptn);
	Animation(DeltaTime, demonEffect_anim, demonEffect_animptn);
	FlameDemon_IsHitBullet();
	FireHorizontal_Update();
	FlameDemon_EffectAnimUpdate();
	//ステート切り替え
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
		case DEAD:
			notHitboss = true;
			FlameDemon_Dead();
			break;
		default:
			break;
		}

}

 void FlameDemon_Render()
 {
	 if (demon.isAlived)
	 {
		 int demonIF_frame = demon.if_frame * 30;
		 //炎の魔人描画
		 if (demonIF_frame % 2 == 0)
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

		 //ファイヤーボールヒットエフェクト描画
		 if (hitbigfireActive) {
			 if (demon.Dir)
			 {
				 RenderSpritePos(hitbigFire[bigfireHit_anim.sprite_index], hitbigfireeffect.x - (BIGFIREEFFECT * 0.4f)/ 2, hitbigfireeffect.y -BIGFIREEFFECT * 0.4/ 2);
			 }
			 else
			 {
				 RenderSpritePosLR(hitbigFire[bigfireHit_anim.sprite_index], hitbigfireeffect.x -(BIGFIREEFFECT * 0.4f) / 2, hitbigfireeffect.y -BIGFIREEFFECT * 0.4 / 2);
			 }
		 }

		 if (demonEffect_Active) {
			 if (demon.Dir)
			 {
				 RenderSpritePos(demonEffect[demonEffect_anim.sprite_index], demon.positionX - demonEffect_size_w / 2, demon.positionY - demonEffect_size_h + SCREEN_REMAINDER);
			 }
			 else
			 {
				 RenderSpritePosLR(demonEffect[demonEffect_anim.sprite_index], demon.positionX  -demonEffect_size_w / 2, demon.positionY - demonEffect_size_h+ SCREEN_REMAINDER);
			 }
		 }
	 }
 }

 //待機処理＆次の行動までの待ち時間処理
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
			 demon.positionX -= DEMON_SPEED * (DeltaTime * 50);
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
			 demon.positionX += DEMON_SPEED * (DeltaTime * 50);
			 //画面端処理
			 if (demon.positionX > SCREEN_WIDTH - DEMON_WIDTH / 2)
			 {
				 demon.positionX = SCREEN_WIDTH - DEMON_WIDTH / 2;
				 tackle_count = 0;
			 }
		 }

	 }
}

 //ファイヤーボール処理（横）
 void FireHorizontal_Update()
 {
	 if (fire_holizontal.isAlived) {
		 if (demon.Dir)
		 {
			 fire_holizontal.positionX -= fire_holizontal.speed * (DeltaTime * 50);
		 }
		 else
		 {
			 fire_holizontal.positionX += fire_holizontal.speed * (DeltaTime * 50);
		 }
	 }

	 //画面端処理
	 if (fire_holizontal.positionX < 0 || fire_holizontal.positionX>SCREEN_WIDTH)
	 {
		 fire_holizontal.isAlived = false;
	 }
 }
 
 void FlameDemon_FireHorizontal()
 {
	 if (!fire_holizontal.isAlived&& canshotfire)
	 {
		 // isatk = true;
		 if (demon_anim.sprite_index >= 72) {//炎の魔人アニメーション７２番目になったタイミングでファイヤーボールセット
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
		 demon.if_frame-=DeltaTime;
		 if (demon.if_frame <= 0) {
			 demon.if_frame = 0;
		 }
	 }
	 //アイススピア
	 for (int i = 0; i < BULLET_MAX; i++)
	 {
		 float demon_X = demon.positionX - demon_width / 2;
		 float demon_Y = demon.positionY - demon_height + screen_remainder;
		 float is_X = icespear[i].positionX - CameraX;
		 if (icespear[i].isAlived == false) { continue; }
			 if (demon.isAlived)
			 {
				 hiticespear_demon = HitBullet(&is_X, &icespear[i].positionY, &IceSpear_radius, &IceSpear_height,
					 &demon_X, &demon_Y, &demon_width, &demon_height, &demon.hp, &player.atk, &icespear[i].isAlived);
				 if (hiticespear_demon)
				 {
					 demon.if_frame = ENEMY_IF_FRAME;
				 }
				 if (demon.hp <= 0)
				 {
					 hiticespear_demon = false;
				 }
			 }
	 }
	 //アイスブロック
	 for (int i = 0; i < ICEBLOCK_MAX; i++) {
		 if (fire_holizontal.isAlived) {
			 if (IceBlock[i].isAlived == false) { continue; }
			 hitbigfire = CircleVsRect(fire_holizontal.positionX, fire_holizontal.positionY, bigfire_size / 2,
				 IceBlock[i].positionX, IceBlock[i].positionY, Ice_Size, Ice_Size);
			 if (hitbigfire) {
				 DelteFire_count++;
				
			 }
			 if (DelteFire_count >= 2) {
				 fire_holizontal.isAlived = false;
				 HitBigFire_Set(fire_holizontal.positionX, fire_holizontal.positionY);
				 DelteFire_count = 0;
			 }
		 }
	 }

	 if (hitbigfireActive && !bigfireHit_anim.isPlaying) {
		 hitbigfireActive = false;
	}
 }

 //炎の魔人死亡
 void FlameDemon_Dead()
 {
	 static bool fadeStart = false; // フェード開始したかどうか
	 //爆発エフェクト終了でフェードアウト
	 if (demonEffectState == DEAD_EFFECT && !demonEffect_anim.isPlaying&&!fadeStart) {
		 SetFadeIn(1, 1, 1, 1.5f);
		 bossFade = true;
		 fadeStart = true;
	 }
	 if (bossFade && IsFadeFinished()) {
		 PlaySE(BossClearSE, false, 0.3f);
		 MAIN_Release();
		 BGM_Stop();
		 SetFadeOut(1, 1, 1, 2.0f);
		 InitializeScene(SCENE_RESULT);
		 fadeStart = false;
		 bossFade = true;
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
	 if (demon.hp <= 0) {
		 demon_state = DEAD;
	 }
	 ChangeAnimation(demon_state, demon_anim, demon_animptn);
 }

 

 //エフェクト
 void FlameDemon_EffectAnimUpdate() {
	 switch (demon_state)
	 {
	 case TACKLECHARGE:
		 demonEffect_size_h = TACLECHARGEEFFECT/1.5f;//エフェクトサイズ変更
		 demonEffect_size_w = TACLECHARGEEFFECT / 1.5f;
		 demonEffect_Active = true;
		 demonEffectState = CHARGE_EFFECT;
		 break;
	 case TACKLE:
		 demonEffect_size_h = TACKLEEFFECT_H-SCREEN_REMAINDER*3.5 ;
		 demonEffect_size_w = TACKLEEFFECT_W;
		 demonEffect_Active = true;
		 demonEffectState = TACKLE_EFFECT;
		 break;
	 case DEAD:
		 demonEffect_size_h = DEADEFFECT;//エフェクトサイズ変更
		 demonEffect_size_w = DEADEFFECT;
		 if (demon_anim.sprite_index==240){//死亡アニメーション２４０番目になった際死亡エフェクト発生
			 isStartdestroy_effect = true;
		 }
		 if (isStartdestroy_effect) {
			 demonEffect_Active = true;
			 demonEffectState = DEAD_EFFECT;
		 }
		 else {
			 demonEffect_Active = false;
		 }
		 break;
	 default:
		 demonEffect_Active = false;
		 demonEffect_anim.frame = 0;
		 demonEffect_anim.sprite_index = demonEffect_animptn.param[0].ptn[0];
		 demonEffect_anim.isPlaying = true;
		 demonEffect_anim.ID = -1;
		 break;
	 }

	 ChangeAnimation(demonEffectState, demonEffect_anim, demonEffect_animptn);

 }

 void HitBigFire_Set(float x,float y) {
	 hitbigfireeffect.x = x;
	 hitbigfireeffect.y = y;
	 bigfireHit_anim.frame = 0;
	 bigfireHit_anim.sprite_index = bigfire_Hitanimptn.param[0].ptn[0];
	 bigfireHit_anim.isPlaying = true;
	 bigfireHit_anim.ID = 0;
	 hitbigfireActive = true; 
 }


 int pattern_change_count = 0;
 //行動パターン変化(2パターン）
 void FlameDemon_ChangePattern()
 {
	 if (pattern_change_count<3) {
		 demon_state = FIRE_HORIZONTAL;
		 pattern_change_count++;

	 }
	 if (pattern_change_count >= 3) {
		 demon_state = TACKLECHARGE;
		 pattern_change_count = 0;
	 }

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

	 //アニメーション初期化
	 isStartdestroy_effect = false;
	 demonEffect_Active = false;
	 demonEffectState = -1;
	 demonEffect_anim.isPlaying = true; 
	 demonEffect_anim.frame = 0;
	 demonEffect_anim.sprite_index = demonEffect_animptn.param[0].ptn[0];
	 demonEffect_anim.ID = -1;

	 bigfireHit_anim.isPlaying = false;
	 bigfireHit_anim.frame = 0;
	 bigfireHit_anim.sprite_index = bigfire_Hitanimptn.param[0].ptn[0];
	 bigfireHit_anim.ID = -1;

	 hitbigfireActive = false;
	 DelteFire_count = 0;

	 bossFade = false;
	 //行動パターン初期化
	 pattern_change_count = 0;

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

