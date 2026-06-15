#include"Item.h"
#include"Stage.h"
#include"Game.h"
#include"Player.h"
#include"hitblock.h"
#include <math.h>

Item_Param itemParam[ITEM_MAX];
float item_size= ITEM_SIZE;
float item_width= ITEM_SIZE;
float item_height=ITEM_SIZE;
int itemhandle[10];
float item_anim_frame = 0;
int item_anim_maxFrame = 10;
int itemSE;

void Item_Initialize() {
	CreateSpriteMatrix("assets/Item/potion_00.png",
		ITEM_SIZE, ITEM_SIZE,
		5, 2, 10,
		itemhandle
	);
	for (int i = 0;i < ITEM_MAX;i++) {
		itemParam[i].IsActive = false;
	}
	itemSE = CreateSEOgg("assets/SE/se_potion_00.ogg");
}

void Item_Release() {
	ReleaseSpriteMatrix(itemhandle, 10);
}

void Item_Update() {
	//アイテム取得時（回復処理）
	for (int i = 0;i < ITEM_MAX;i++){
		if (itemParam[i].IsActive == false)continue;
		float item_X = itemParam[i].x - item_size / 2 - CameraX;
		float item_Y = itemParam[i].y - item_size / 2 + screen_remainder;
		bool hititem = HitObject(&pl_x, &pl_y, &pl_width, &pl_height,
			&item_X, &item_Y, &item_size, &item_size, &itemParam[i].IsActive);
		if (hititem) {
			PLAYER_HEAL(HEAL_HP);
			PlaySE(itemSE);
		}
	}
	item_anim_frame+=DeltaTime;
}

void Item_Render() {
	for (int i = 0;i < ITEM_MAX;i++) {
	    if (itemParam[i].IsActive) {
			int item_animFrame = item_anim_frame * 60 / 8;
			float ys = itemParam[i].y + sinf(item_anim_frame * 2.0f) * 10.0f; // 10.0f が振幅（上下の幅)
			RenderSpritePos(itemhandle[item_animFrame % item_anim_maxFrame], itemParam[i].x - ITEM_SIZE / 2 - CameraX, ys - ITEM_SIZE / 2 + SCREEN_REMAINDER);
		}
	}
}

void Item_Set(float x, float y){	
	for (int i = 0;i < ITEM_MAX;i++){
		if (itemParam[i].IsActive == true)continue;
		//空き場所にセット
		itemParam[i].IsActive = true;
		itemParam[i].x = x;
		itemParam[i].y = y;
		break;//1体分セットでループ終了
	}
}
