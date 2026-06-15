#include "UnlimitedLib/UnlimitedLib.h"
#include "IceBlock.h"
#include "Player.h"
#include "Stage.h"
#include "Game.h"
#include "hitblock.h"
#include "flameslime.h"
#include "Fireball.h"
#include "FlameDemon.h"
Player IceBlock[ICEBLOCK_MAX];

#define D_ICEBLOCKSIZE 180

float Ice_Size = ICEBLOCKSIZE;

int iceblockhandle[150];
int iceblockeEffectHandles[8];
int blockEffectState;
int blockState[ICEBLOCK_MAX];
int blockVsFireballSE;

void IceBlock_AnimUpdate(int n);
AnimationInfo iceblock_animptn{
	{
		/*00生成*/{25.0f,{0,1,2,3,4,5,6,7,-2}},
		/*01アイスブロック*/{7.0f,{50,50,50,50,50,50,51,52,53,54,55,56,57,58,59,
								   60,61,61,61,61,-2}},
		/*02砕ける*/{25.0f,{100,101,102,103,104,105,-2}},
    }
};


enum {
	STATE_ICEBLOCK_CREATE,
	STATE_ICEBLOCK,
	STATE_ICEBLOCK_BREAK,
	STATE_MAX,
};

void IceBlock_Initialize() {
	//SE
	blockVsFireballSE = CreateSEOgg("assets/SE/se_fireball_block_collision_00.ogg");
	//生成
	CreateSpriteMatrix("assets/Player/iceblock_create_00.png",
		ICEBLOCKSIZE, ICEBLOCKSIZE,
		4, 2, 8,
		iceblockhandle);
	//本体
	CreateSpriteMatrix("assets/Player/iceblock_countdown_00.png",
		ICEBLOCKSIZE, ICEBLOCKSIZE,
		4,3,12,
		&iceblockhandle[50]);
	//砕ける
	CreateSpriteMatrix("assets/Player/iceblock_destroy_00.png",
		D_ICEBLOCKSIZE, D_ICEBLOCKSIZE,
		3, 2, 6,
		&iceblockhandle[100]);
	
	//アイスブロックの初期化
	for (int i = 0;i < ICEBLOCK_MAX;i++) {
		IceBlock[i].isAlived = false;//現在弾死んでる
		IceBlock[i].anim_Active = false;
	
	}
	for (int j = 0; j < 150; j++) {
		SetSpriteSize(iceblockhandle[j], ICEBLOCKSIZE, ICEBLOCKSIZE);
	}
}

void IceBlock_Release() {
	ReleaseSpriteMatrix(iceblockhandle,150);
}

void  IceBlockUpdate() {
	for (int i = 0;i < ICEBLOCK_MAX;i++) {
		Animation(DeltaTime, IceBlock[i].pl_anim_info, iceblock_animptn);
		IceBlock_AnimUpdate(i);
		//生存しているアイスブロックのみ更新する
		//ステージとの当たり判定処理
		if (IceBlock[i].isAlived) {
			for (int y = 0; y < STAGE_HEIGHT; y++) {
				for (int x = 0; x < STAGE_WIDTH; x++) {
					if (stage[y][x] == 0) continue;
					float blockX = x * Block_Size + Block_Size / 2;
					float blockY = y * Block_Size + Block_Size / 2;
					HitIceBlock(&IceBlock[i].positionX, &IceBlock[i].positionY, &Ice_Size, &Ice_Size, &IceBlock[i].isAlived,
						&blockX, &blockY, &Block_Size);
				}
			}

			IceBlock[i].count+=DeltaTime;
			if (IceBlock[i].positionY >= SCREEN_HEIGHT + ICEBLOCKSIZE) {
				IceBlock[i].isAlived = false;
			}
			if (IceBlock[i].count >= ICEBLOCK_FRAME) {
				IceBlock[i].count = 0;
				IceBlock[i].isAlived = false;
				PlaySE(blockVsFireballSE);
			}
		}
	}
}
void IceBlock_Render() {
	for (int i = 0;i < ICEBLOCK_MAX;i++) {
		//生存しているアイスブロックのみ更新する
		if (IceBlock[i].anim_Active) {
			//プレイヤーとアイスブロック
			if (IceBlock[i].Dir) {
				RenderSpritePos(iceblockhandle[IceBlock[i].pl_anim_info.sprite_index], IceBlock[i].positionX - CameraX, IceBlock[i].positionY+ SCREEN_REMAINDER);
			}
			else {
				RenderSpritePosLR(iceblockhandle[IceBlock[i].pl_anim_info.sprite_index], IceBlock[i].positionX- CameraX, IceBlock[i].positionY + SCREEN_REMAINDER);
			}
		}
	}
}

void Create(float x, float y, bool Dir) {
	int n = -1;
	//使われていないアイスブロックを探し出す
	for (int i = 0;i < ICEBLOCK_MAX;i++) {
		if (IceBlock[i].isAlived == false) {//i番目が生存しているか確認する
			//生存していない場合はｎに空いてる番号を代入する
			n = i;
			break;
		}
	}
	//空きがなければ終了
	if (n == -1) {
		return;
	}
	//アイスブロックの初期化
	

	IceBlock[n].Dir = Dir;
	//右の場合
	if (IceBlock[n].Dir) {
		IceBlock[n].positionX = x + ICEBLOCKSIZE;
		IceBlock[n].positionY = y - ICEBLOCKSIZE;
	}
	//左の場合
	else {
		IceBlock[n].positionX = x - ICEBLOCKSIZE * 2;
		IceBlock[n].positionY = y - ICEBLOCKSIZE;
	}
	IceBlock[n].isAlived = true;
	IceBlock[n].count = 0;
	IceBlock[n].pl_anim_info.frame = 0;
	IceBlock[n].pl_anim_info.ID = -1;
	IceBlock[n].pl_anim_info.sprite_index = iceblock_animptn.param[0].ptn[(int)IceBlock[n].pl_anim_info.frame];
	IceBlock[n].pl_anim_info.isPlaying = true;
	blockState[n] = STATE_ICEBLOCK_CREATE;
	IceBlock[n].anim_Active = true;
}

void IceBlock_AnimUpdate(int n) {
		switch (blockState[n])
		{
		case STATE_ICEBLOCK_CREATE:
			if (!IceBlock[n].pl_anim_info.isPlaying) {
				blockState[n] = STATE_ICEBLOCK;
			}
			break;
		case STATE_ICEBLOCK:
			if (!IceBlock[n].isAlived) {
				blockState[n] = STATE_ICEBLOCK_BREAK;
			}
			break;
		case STATE_ICEBLOCK_BREAK:
			if (!IceBlock[n].pl_anim_info.isPlaying) {
				IceBlock[n].anim_Active = false;
			}
			break;
		default:
			break;
		}
		ChangeAnimation(blockState[n], IceBlock[n].pl_anim_info, iceblock_animptn);
}
