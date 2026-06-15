#pragma once
#include "Animation.h"

void PLAYER_Initialize();//プレイヤー初期化
void PLAYER_Release();//プレイヤー解放
void PLAYER_Update();//プレイヤー更新
void PLAYER_Render();//プレイヤー描画
void PLAYER_Move();//プレイヤー移動
void PLAYER_Jump();//プレイヤージャンプ
void PLAYERVsEnemy();//プレイヤーと敵の判定
void PLAYERVsStage();//プレイヤーとステージの判定
void PLAYERVsIceBlock();//プレイヤーとアイスブロックの当たり判定
void PLAYER_CollisionUpdate();
void PLAYER_UseMagic();//魔法使用処理
void PLAYER_HealMp();//MP回復処理
void PLAYER_SetUI();//UIのセット
void PLAYER_HEAL(float val);
void PLAYER_AnimUpdate();
void PLAYER_IsGoal();
#define PLAYER_WIDTH  192		//プレイヤー幅
#define PLAYER_HEIGHT 288		//プレイヤー高さ
#define PLAYER_RADIUS 60 		//プレイヤーめり込み量を変化可能
#define PLAYER_JUMP_FORCE 18    //プレイヤージャンプ力

#define PLAYER_SPEED   9	//プレイヤー速さ
#define PLAYER_GRAVITY 1.0 //重力

//プレイヤーのステータス
#define MP_MAX 80			//MPの最大量(ゲージの描画上：最大＊１０にしてます）
#define HP_MAX 80			//HPの最大量 
#define ICEBLOCK_MP 20		//アイスブロックの使用MP
#define ICESPEAR_MP 10		//アイススピアの使用MP
#define PLAYER_ATTACK 10	//プレイヤーの攻撃力
#define HEAL_MP 10			//１秒間に回復するMP
#define IF_FRAME 1    //プレイヤーの無敵時間

#define FRAME 1
extern float pl_width;
extern float pl_height;

extern float pl_hp;
extern float pl_mp;

extern float pl_x;
extern float pl_y;

extern bool noAction;

struct Player{
	AnimationUniqueInfo pl_anim_info;

	int handle = -1; // 画像ハンドル
	float positionX, positionY; // プレイヤーの動く座標
	float VelocityY;
	float speed;//プレイヤーの速さ
	bool isGround; // 地面にいるかどうか
	bool isAlived;//true:生存 false:死亡
	bool Dir;//向き true:右　false:左
	float hp;
	float mp;
	float atk;
	float count;
	bool anim_Active;
};
extern Player player;

extern float screen_remainder;

extern bool hitbigfireball;
