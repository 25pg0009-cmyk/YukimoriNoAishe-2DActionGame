#include "UnlimitedLib/UnlimitedLib.h"
#include "Effect.h"
#include "Animation.h"
#include "MagicShard.h"
#include "Stage.h"
#include "Game.h"
#include "Item.h"
#include "Player.h"
#include "IceSpear.h"

//画像用
int MagicShardEffect[6];
int ItemEffect[6];
int magic_circle[5];

// ヒットエフェクトアニメーション
struct Effect {
	bool effectExistence; // 有無
	float x, y;
	float time;     // 現在の生存時間(秒)
	float lifetime; // 寿命(？秒で消滅)
	bool prevActive;	// 前フレームの状態を保存
	AnimationUniqueInfo msEffect;
	AnimationUniqueInfo itemEffect;
	AnimationUniqueInfo magicCircleEffect;
};

Effect magicShard[MS_MAX];
Effect item[ITEM_MAX];
Effect magicCircle[BULLET_MAX];

AnimationInfo msEffect_ptn = {
	// -1ループ, -2一度の再生で終了
	{10.0f,{0,1,2,3,4,5,-1}},
};
AnimationInfo itemEffect_ptn = {
	// -1ループ, -2一度の再生で終了
	{10.0f,{0,1,2,3,4,5,-1}},
};

AnimationInfo magicCircleEffect_ptn = {
	// -1ループ, -2一度の再生で終了
	{15.0f,{0,1,2,3,4,-2}},
};

void EFFECT_Initialize(){
	
	CreateSpriteMatrix("assets/MagicShard/magicfragment_effect_00.dds",
		500, 500,
		6, 1, 6,
		MagicShardEffect
	);

	CreateSpriteMatrix("assets/Item/potion_effect_00.dds",
		500, 500,
		6, 1, 6,
		ItemEffect
	);
	CreateSpriteMatrix("assets/player/icespear_atk_00.dds",
		104,192,
		5,1,5,
		magic_circle
		);

	for (int i = 0; i < MS_MAX; i++) {
		magicShard[i].effectExistence = false;
		magicShard[i].time = 0.0f;
		magicShard[i].lifetime = 0.7f;
		magicShard[i].prevActive = false; // 初期状態
		magicShard[i].msEffect.ID = 0;
		magicShard[i].msEffect.frame = 0;//最初から
		magicShard[i].msEffect.sprite_index = msEffect_ptn.param[0].ptn[(int)magicShard[i].msEffect.frame];
		magicShard[i].msEffect.isPlaying = true;//再生中
	}
	for (int j = 0; j < ITEM_MAX; j++) {
		item[j].effectExistence = false;
		item[j].time = 0.0f;
		item[j].lifetime = 0.7f;
		item[j].prevActive = false; // 初期状態
		item[j].itemEffect.ID = 0;
		item[j].itemEffect.frame = 0;//最初から
		item[j].itemEffect.sprite_index = itemEffect_ptn.param[0].ptn[(int)item[j].itemEffect.frame];
		item[j].itemEffect.isPlaying = true;//再生中
	}
	for (int i = 0; i < BULLET_MAX; i++) {
		magicCircle[i].effectExistence = false;
		magicCircle[i].time = 0.0f;
		magicCircle[i].prevActive = false; // 初期は生存していない前提
		magicCircle[i].magicCircleEffect.ID = 0;
		magicCircle[i].magicCircleEffect.frame = 0;
		magicCircle[i].magicCircleEffect.sprite_index = magicCircleEffect_ptn.param[0].ptn[0];
		magicCircle[i].magicCircleEffect.isPlaying = false; // 最初は再生していない
	}
}

void EFFECT_Release(){
	ReleaseSpriteMatrix(MagicShardEffect, 6);
	ReleaseSpriteMatrix(ItemEffect, 6);
	ReleaseSpriteMatrix(magic_circle, 5);
}

void EFFECT_Update(){
	for (int i = 0;i < BULLET_MAX;i++) {
		if (!magicCircle[i].prevActive&&icespear[i].isAlived) {
			magicCircle[i].effectExistence = true;
			magicCircle[i].magicCircleEffect.frame = 0;
			magicCircle[i].magicCircleEffect.sprite_index =magicCircleEffect_ptn.param[0].ptn[0];
			magicCircle[i].magicCircleEffect.isPlaying = true;
		}
		// アニメーション更新
		if (magicCircle[i].effectExistence) {
			Animation(DeltaTime, magicCircle[i].magicCircleEffect, magicCircleEffect_ptn);
			if (!magicCircle[i].magicCircleEffect.isPlaying) {
				magicCircle[i].effectExistence = false;
			}
		}
		// 今の状態を保存
		magicCircle[i].prevActive = icespear[i].isAlived;
	}
	
	//魔法のかけら処理
	for (int i = 0; i < MS_MAX; i++) {
		// 消えた瞬間を検知（前フレームはActive、今フレームは非Active）
		if (magicShard[i].prevActive && !msParam[i].IsActive) {
			magicShard[i].effectExistence = true;
			magicShard[i].x = msParam[i].x;
			magicShard[i].y = msParam[i].y;
			magicShard[i].time = 0.0f; // リセット
		}
		// アニメーション更新
		if (magicShard[i].effectExistence) {
			Animation(DeltaTime, magicShard[i].msEffect, msEffect_ptn);
			magicShard[i].time += DeltaTime;
			if (magicShard[i].time >= magicShard[i].lifetime) {
				magicShard[i].effectExistence = false;
			}
		}
		// 今の状態を保存
		magicShard[i].prevActive = msParam[i].IsActive;
	}

	//アイテム処理
	for (int j = 0;j < ITEM_MAX;j++) {
		// 消えた瞬間を検知（前フレームはActive、今フレームは非Active）
		if (item[j].prevActive && !itemParam->IsActive) {
			item[j].effectExistence = true;
			item[j].x = itemParam->x;
			item[j].y = itemParam->y;
			item[j].time = 0.0f; // リセット
		}
		// アニメーション更新
		if (item[j].effectExistence) {
			Animation(DeltaTime, item[j].itemEffect, itemEffect_ptn);
			item[j].time += DeltaTime;
			if (item[j].time >= item[j].lifetime) {
				item[j].effectExistence = false;
			}
		}
		// 今の状態を保存
		item[j].prevActive = itemParam->IsActive;
	}
}

void EFFECT_Render(){
	for (int i = 0; i < MS_MAX; i++) {
		if (magicShard[i].effectExistence) {
			RenderSpritePos(MagicShardEffect[magicShard[i].msEffect.sprite_index],magicShard[i].x - 500 / 2 - CameraX,magicShard[i].y - 500 / 2);
		}
	}
	for (int j = 0;j < ITEM_MAX;j++) {
		if (item[j].effectExistence) {
			RenderSpritePos(ItemEffect[item[j].itemEffect.sprite_index], player.positionX - 500 / 2 - CameraX, player.positionY- 500 / 2-80);
		}
	}
	for (int i = 0;i < BULLET_MAX;i++) {
		if (magicCircle[i].effectExistence) {
			if (icespear[i].Dir) {
				RenderSpritePos(magic_circle[magicCircle[i].magicCircleEffect.sprite_index], player.positionX + 50 - CameraX, (player.positionY - 192 / 2) - PLAYER_HEIGHT / 3);
			}else {
				RenderSpritePosLR(magic_circle[magicCircle[i].magicCircleEffect.sprite_index], player.positionX - 50 - PLAYER_WIDTH/2 - CameraX, (player.positionY - 192 / 2) - PLAYER_HEIGHT / 3);
			}
		}
	}
}
