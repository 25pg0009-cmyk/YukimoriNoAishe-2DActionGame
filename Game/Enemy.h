#pragma once
#include"Animation.h"
#define ENEMY_MAX 10
//敵パラメータ
struct EnemyParam {
	AnimationUniqueInfo anim_info;
	enum ENEMY_STATE
	{
		STATE_IDLE_E,
		STATE_DEATH,
		STATE_ATTACK,
	};
	bool IsActive;            //true: 有効　　false:無効
	float x, y;                  //座標
	float vy;                    //Y方向の速度
	float vx;                    //X方向の速度
	bool IsGround;         //接地状態？
	bool dir;                   //移動方向
	int startY;
	bool viewing_dir;     //左右の方向
	float atk;
	float hp;
	float if_frame;             //無敵時間
	float firecount;
	int state;                  //状態
	bool collision;          //当たり判定  on/off
	bool fireAttack;        //攻撃  on/off
	bool wasInScreen; // 前フレームで画面内にいたか
};



void ENEMY_Set(float x, float y, float vx);
void ENEMY_Set2(float x, float y, float vx);

extern float enemy_height;
extern float enemy_radius;
extern float enemy_width;
extern float enemy2_width;
extern float enemy2_height;
extern float enemy2_radius;
extern float fire_width;
extern float fire_size;
extern float fire_radius;
extern bool hiticespear_slime;
extern bool hiticespear_imp;

//スライムのステータス
#define FLAMESLIME_HP 30 
#define FLAMESLIME_ATK 10
//インプのステータス
#define FLAMEIMP_HP 30
#define FLAMEIMP_ATK 10
#define FIRE_ATK     10  

#define ENEMY_IF_FRAME 0.5
