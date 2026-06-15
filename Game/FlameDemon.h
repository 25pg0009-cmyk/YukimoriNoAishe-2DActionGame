#pragma once

void FlameDemon_Initialize();
void FlameDemon_Release();
void FlameDemon_Update();
void FlameDemon_Render();
void FlameDemon_Tackle();
void FlameDemon_IsHitBullet();
void FlameDemon_Set(float x,float y);
void Set_FireHorizontal(float x, float y);
void FlameDemon_FireHorizontal();
void FlameDemon_AnimUpdate();
void FlameDemon_ChangePattern();
void HitBigFire_Set(float x, float y);

#define DEMON_HIGHT 576
#define DEMON_WIDTH 576
#define DEMON_ATK 20 //炎の魔人攻撃力
#define DEMON_HP 350 //炎の魔人体力
#define TACKLECOUNT 4 //タックルが始まるまでのカウント
#define DEMON_SPEED 20 //タックルスピード
#define H_BIGFIRE_SPEED 10//ファイヤーボールスピード（横）
#define R_BIGFIRE_SPEED 10//ファイヤーボールスピード（上から下）
#define BIGFIRE_MAX 3
#define COOLDOWN_COUNT 3 //次の技までのクールダウン
#define BIG_FIRE 384

struct FlameDemon {
	int handle;
	float positionX, positionY;
	float speed;
	float hp;
	float atk;
	float if_frame;//無敵時間
	bool Dir;
	bool isAlived;
};
extern FlameDemon demon;

struct BigFire {
	float positionX, positionY;
	float speed;
	float atk;
	bool isAlived;
	bool isAscending;
};
extern  BigFire fire_holizontal;
extern  BigFire fire_rain[BIGFIRE_MAX];


extern float demon_height;
extern float demon_width;
extern float bigfire_size;
extern bool  hiticespear_demon;
extern bool notHitboss;

