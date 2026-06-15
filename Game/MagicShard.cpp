#include "UnlimitedLib/UnlimitedLib.h"
#include "MagicShard.h"
#include "Stage.h"
#include "Game.h"
#include "hitblock.h"
#include "Player.h"
#include "Fireball.h"
#include "Animation.h"
#include "StageSelect.h"

int  MagicShard[4];
float ms_anim_frame = 0;
int ms_anim_maxFrame = 4;
float ms_width = MS_W;
float ms_height = MS_H;
 int get_ms_count=0;//ステージ１用
 int get_ms_count2=0; // ステージ２用
int msSE;


MS_Param msParam[MS_MAX];

void MagicShard_Initialize() {
	msSE = CreateSEOgg("assets/SE/se_magic_item_00.ogg");
	CreateSpriteMatrix("assets/MagicShard/collection_00.dds",
		96, 192,
		4, 1, 4,
		MagicShard
	);
	for (int i = 0;i < MS_MAX;i++) {
		msParam[i].IsActive = false;
	}
}

void MagicShard_Release() {
	ReleaseSpriteMatrix(MagicShard,4);
}

void MagicShard_Update() {
	
	ms_anim_frame+=DeltaTime;
	for (int i = 0;i < MS_MAX;i++) {
		if (msParam[i].IsActive == false)continue;
		float ms_X = msParam[i].x - MS_W / 2-CameraX;
		float ms_y = msParam[i].y - MS_H / 2 + screen_remainder;
		bool hitMagicShard = HitObject(&pl_x, &pl_y, &pl_width, &pl_height,
			&ms_X, &ms_y, &ms_width, &ms_height, &msParam[i].IsActive);
		if (hitMagicShard) {
			PlaySE(msSE);
			switch (Select){
			case 1:get_ms_count++;currentCollection++;break;
			case 2:get_ms_count2++;currentCollection2++;break;
			}
		}
	}
}

void MagicShard_Render() {
	for (int i = 0;i < MS_MAX;i++) {
		if (msParam[i].IsActive == false)continue;//無効の敵はスキップ
		int ms_animFrame = ms_anim_frame * 60 / 8;
		RenderSpritePos(MagicShard[ms_animFrame % ms_anim_maxFrame], msParam[i].x - MS_W/2 - CameraX, msParam[i].y - MS_H /2 + SCREEN_REMAINDER);
		
	}
}

void MS_Set(float x, float y){
	for (int i = 0;i < MS_MAX;i++){
		//空きを探す
		if (msParam[i].IsActive == true) continue;
		//空き場所にセット
		msParam[i].IsActive = true;//すでに有効な敵はスキップ
		msParam[i].x = x;
		msParam[i].y = y;
		break;//1体分セットでループ終了
	}
}
