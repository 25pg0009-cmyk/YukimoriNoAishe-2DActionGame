#include "UnlimitedLib/UnlimitedLib.h"
#include "flameslime.h"
#include "Game.h"
#include "MainGame.h"
#include "Enemy.h"
#include "Stage.h"
#include "IceSpear.h"
#include "hitblock.h"
#include "Player.h"
#include "IceBlock.h"
#include "Animation.h"
#include "GameSound.h"

#define ENEMY_SPEED 3.0f

int Enemy2[35];//ファイヤースライム画像
bool hiticespear_slime;
float enemy2_width = ENEMY_FS_WIDTH/1.3;
float enemy2_height = ENEMY_FS_HEIGHT/1.3;
float enemy2_radius = ENEMY_FS_WIDTH / 2.0f;

EnemyParam enemyParam2[ENEMY_MAX];
//アニメーション
AnimationUniqueInfo enemy_slime;
AnimationInfo enemy_slimeptn = {
	//-1ループ,-2一度の再生で終了
	{
		/*00待機*/{20.0f,{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14, -1},{0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,}},
		/*01死亡*/{20.0f,{15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34, -2}},
     }
};

void EnemyVsIceBlock(float* charX, float* charY, float* VelocityY, float* charWidth, float* charHeight, bool* Dir,
	float* BlockX, float* BlockY, float* BlockSize, bool* IsGround);

void ENEMY2_Initialize(){
	SetSEVolume(0.5f);
	 CreateSpriteMatrix("assets/enemy/slime/enemy2_move_00.dds",
		 ENEMY_FS_WIDTH, ENEMY_FS_HEIGHT,
		 8,2,15,
	    Enemy2
	);

	 CreateSpriteMatrix("assets/enemy/slime/enemy2_dead_00.dds",
		 ENEMY_FS_WIDTH, 172,
		 5, 4, 20,
		 &Enemy2[15]
	 );
	for (int i = 0;i < ENEMY_MAX;i++) {
		enemyParam2[i].IsActive = false;
		enemyParam2[i].state = enemyParam2[i].STATE_IDLE_E;
		enemyParam2[i].collision = true;
		enemyParam2[i].wasInScreen = false;
	}
}

void ENEMY2_Release(){
	ReleaseSpriteMatrix(Enemy2,35);
}

void ENEMY2_Update(){
	for (int i = 0;i < ENEMY_MAX;i++) {
		if (enemyParam2[i].IsActive == false)continue;//無効の敵はスキップ
		//アニメーション更新
		Animation(DeltaTime, enemyParam2[i].anim_info, enemy_slimeptn);

		//死亡アニメーション
		if (enemyParam2[i].hp <= 0) {
			//アニメーション未再生なら切り替え
			if (enemyParam2[i].state != enemyParam2[i].STATE_DEATH) {
				enemyParam2[i].state = enemyParam2[i].STATE_DEATH;
				ChangeAnimation(enemyParam2[i].state, enemyParam2[i].anim_info, enemy_slimeptn);
				// 死亡遷移時に停止＆無効化
				enemyParam2[i].vx = 0;
				enemyParam2[i].vy = 0;
				enemyParam2[i].y= enemyParam2[i].y-28;
				enemyParam2[i].collision = false; // 当たり判定OFF
				enemyParam2[i].fireAttack = false;    // 攻撃OFF
			}
			//アニメーション後に削除
			if (!enemyParam2[i].anim_info.isPlaying) {
				enemyParam2[i].IsActive = false;
			}
		}
		

		//死亡時は動きを止める
		if (enemyParam2[i].state != enemyParam2[i].STATE_DEATH) {
			if (enemyParam2[i].dir == true) {
				enemyParam2[i].vx = -ENEMY_SPEED;
			}
			else {
				enemyParam2[i].vx = ENEMY_SPEED;
			}

			enemyParam2[i].x += enemyParam2[i].vx * (DeltaTime * 50);
		
			//重力＋Y移動
			enemyParam2[i].vy += 1.0f * (DeltaTime * 50);
			enemyParam2[i].y += enemyParam2[i].vy * DeltaTime ;
			enemyParam2[i].IsGround = false;
		}
		enemyParam2[i].IsGround = false;
		if (enemyParam2[i].if_frame > 0) {
			enemyParam2[i].if_frame -= DeltaTime;
			if (enemyParam2[i].if_frame <= 0) {
				enemyParam2[i].if_frame = 0;
			}
		}
		//当たり判定処理
		//右当たり判定
		//右ブロックチェック
		int RightX = enemyParam2[i].x + enemy2_radius;
		int tileR = STAGE_GetTileNo(RightX, enemyParam2[i].y - 1);
		int tileUR = STAGE_GetTileNo(enemyParam2[i].x + 24, enemyParam2[i].y);
		//頭上の右もチェック
		int tileHeadR = STAGE_GetTileNo(RightX, enemyParam2[i].y - enemy2_height);
		//中央のブロックもチェック
		int tileBodyR = STAGE_GetTileNo(RightX, enemyParam2[i].y - BlockSIZE);
		if (tileR > 0 || tileHeadR > 0 || tileBodyR > 0) {
			int BlockLeftX = RightX / BlockSIZE;
			int bottomX = BlockLeftX * BlockSIZE;
			enemyParam2[i].x = bottomX - enemy2_radius - 1;
			enemyParam2[i].vx = 0;
		}

		//左当たり判定
		//左ブロックチェック
		int LeftX = enemyParam2[i].x - enemy2_radius;
		int tileL = STAGE_GetTileNo(LeftX, enemyParam2[i].y - 1);
		int tileUL = STAGE_GetTileNo(enemyParam2[i].x - 24, enemyParam2[i].y);
		//頭上の左もチェック
		int tileHeadL = STAGE_GetTileNo(LeftX, enemyParam2[i].y - enemy2_height);
		//中央の左もチェック
		int tileBodyL = STAGE_GetTileNo(LeftX, enemyParam2[i].y - BlockSIZE);
		if (tileL > 0 || tileHeadL > 0 || tileBodyL > 0) {
			int BlockRightX = LeftX / BlockSIZE + 1;
			int bottomX = BlockRightX * BlockSIZE;
			enemyParam2[i].x = bottomX + enemy2_radius + 1;
		}

		//壁判定
		if ((tileL > 0) && (tileR == 0)) {
			enemyParam2[i].vx = ENEMY_SPEED;
			enemyParam2[i].dir = false;
		}
		if ((tileL == 0) && (tileR > 0)) {
			enemyParam2[i].vx = -ENEMY_SPEED;
			enemyParam2[i].dir = true;
		}
		
		//頭上チェック
		if (enemyParam2[i].vy < 0) {
			int topY = enemyParam2[i].y - enemy2_height;
			//頭上のブロック
			int tileU = STAGE_GetTileNo(enemyParam2[i].x, topY);
			int tileUR = STAGE_GetTileNo(enemyParam2[i].x + enemy2_radius, enemyParam2[i].y - enemy2_height);
			int tileUL = STAGE_GetTileNo(enemyParam2[i].x - enemy2_radius, enemyParam2[i].y - enemy2_height);
			//頭上がブロックだった場合
			if (tileU > 0 || tileUR > 0 || tileUL > 0) {
				enemyParam2[i].vy = 0;
				int ty = topY / BlockSIZE + 1;
				int bottomY = ty * BlockSIZE;
				enemyParam2[i].y = bottomY + enemy2_height;

			}
		}

		if (enemyParam2[i].vy >= 0) {//落下中
			int tile = STAGE_GetTileNo(enemyParam2[i].x, enemyParam2[i].y);//足元のタイルをゲット
			int tileR = STAGE_GetTileNo(enemyParam2[i].x + enemy2_radius, enemyParam2[i].y);
			int tileL = STAGE_GetTileNo(enemyParam2[i].x - enemy2_radius, enemyParam2[i].y);
			if (tile > 0 || tileR > 0 || tileL > 0) {
				enemyParam2[i].IsGround = true;

				int ty = enemyParam2[i].y / BlockSIZE;
				enemyParam2[i].y = ty * BlockSIZE - 1;
				enemyParam2[i].vy = 0.0f;//重力を止める
				// 地面にいるときの崖チェック
				if (enemyParam2[i].IsGround) {
					int forwardX = enemyParam2[i].x + (enemyParam2[i].dir == 0 ? enemy2_radius : -enemy2_radius);
					int forwardY = enemyParam2[i].y +1; // 足元少し下
					int tileForwardDown = STAGE_GetTileNo(forwardX, forwardY);
					if (tileForwardDown == 0) {
						// 崖なので反転する
						enemyParam2[i].vx = -enemyParam2[i].vx;
						enemyParam2[i].dir = (enemyParam2[i].dir == 0) ? 1 : 0;
					}
				}
			}
			else {
				enemyParam2[i].IsGround = false;
			}
		}
		if (enemyParam2[i].collision) {
			for (int k = 0;k < BULLET_MAX;k++) {
				float slime_X = enemyParam2[i].x - enemy2_width / 2 - CameraX;
				float slime_Y = enemyParam2[i].y - enemy2_height + screen_remainder;
				float is_X = icespear[k].positionX - CameraX;
				if (icespear[k].isAlived == false) { continue; }
				//敵とスピアの判定
				hiticespear_slime = HitBullet(&is_X, &icespear[k].positionY, &IceSpear_radius, &IceSpear_height,
					&slime_X, &slime_Y, &enemy2_width, &enemy2_height, &enemyParam2[i].hp, &player.atk, &icespear[k].isAlived);
				if (hiticespear_slime) {
					enemyParam2[i].if_frame = ENEMY_IF_FRAME;
				}
			}
		}
		//アイスブロック
		for (int ib = 0; ib < ICEBLOCK_MAX; ib++) {
			if (IceBlock[ib].isAlived == false) { continue; }//死んでいるアイスブロックはスキップ
			float iceX = IceBlock[ib].positionX / Ice_Size;
			float iceY = IceBlock[ib].positionY / Ice_Size;
			EnemyVsIceBlock(&enemyParam2[i].x, &enemyParam2[i].y, &enemyParam2[i].vy, &enemy2_width, &enemy2_height, &enemyParam2[i].dir,
				&iceX, &iceY, &Ice_Size, &enemyParam2[i].IsGround);
		}
		ChangeAnimation(enemyParam2[i].state, enemyParam2[i].anim_info, enemy_slimeptn);
	}
}
int slimeIf_frame[ENEMY_MAX];
void ENEMY2_Render(){
	for (int i = 0;i < ENEMY_MAX;i++) {
		slimeIf_frame[i] = enemyParam2[i].if_frame * 30;
		if (slimeIf_frame[i] % 2 == 0) {
	    	if (enemyParam2[i].IsActive) {
			   if (enemyParam2[i].dir) {
				   RenderSpritePos(Enemy2[enemyParam2[i].anim_info.sprite_index], enemyParam2[i].x - ENEMY_FS_WIDTH / 2 - CameraX, enemyParam2[i].y - ENEMY_FS_HEIGHT + SCREEN_REMAINDER);
		       }else {
				   RenderSpritePosLR(Enemy2[enemyParam2[i].anim_info.sprite_index], enemyParam2[i].x - ENEMY_FS_WIDTH / 2 - CameraX, enemyParam2[i].y - ENEMY_FS_HEIGHT + SCREEN_REMAINDER);
			   }
		    }
	    }
	}
}

void ENEMY_Set2(float x, float y, float vx)
{
	for (int i = 0;i < ENEMY_MAX;i++)
	{
		//空きを探す
		if (enemyParam2[i].IsActive == true) continue;
		//空き場所にセット
		enemyParam2[i].IsActive = true;
		enemyParam2[i].x = x;
		enemyParam2[i].y = y;
		enemyParam2[i].vx = ENEMY_SPEED;
		enemyParam2[i].vy = 0;
		enemyParam2[i].dir = 0;
		enemyParam2[i].atk = FLAMESLIME_ATK;
		enemyParam2[i].hp = FLAMESLIME_HP;
		hiticespear_slime = false;
		enemyParam2[i].if_frame = 0;
		enemyParam2[i].state = enemyParam2[i].STATE_IDLE_E;
		break;//1体分セットでループ終了
	}
}

void EnemyVsIceBlock(float* charX, float* charY, float* VelocityY, float* charWidth, float* charHeight, bool* Dir,
	float* BlockX, float* BlockY, float* BlockSize, bool* IsGround) {

	float  CharTop = *charY - *charHeight;
	float  CharBottom = *charY;
	float  CharRight = *charX + *charWidth / 2;
	float  CharLeft = *charX - *charWidth / 2;

	float BlockTop = *BlockY * (*BlockSize);
	float BlockBottom = BlockTop + *BlockSize;
	float BlockLeft = *BlockX * (*BlockSize);
	float BlockRight = BlockLeft + *BlockSize;

	float wallheightSize = BlockSIZE / 2;
	float wallwidthSize = BlockSIZE / 3;
	//左右の当たり判定
	if (CharBottom >= BlockTop + wallheightSize && CharTop <= BlockBottom - wallheightSize) {

		//ブロックの右壁の当たり判定(座標補正かえる
		if (CharRight >= BlockLeft - 1 && CharRight <= BlockLeft + wallwidthSize) {
			*Dir = true;
			*charX = BlockLeft - *charWidth / 2 - 1;
			CharRight = *charX - *charWidth / 2;
			CharLeft = *charX + *charWidth / 2;

		}
		//ブロックの左壁の当たり判定
		if (CharLeft >= BlockRight - wallwidthSize && CharLeft <= BlockRight + 1) {
			*Dir = false;
			*charX = BlockRight + *charWidth / 2 + 1;
			CharRight = *charX - *charWidth / 2;
			CharLeft = *charX + *charWidth / 2;
		}
	}

	//天井と床の当たり判定
	if (CharRight >= BlockLeft + 1 && CharLeft <= BlockRight - 1) {
		//床
		if (CharBottom >= BlockTop - 1 && CharBottom <= BlockTop + wallheightSize) {
			*charY = BlockTop - 1;
			*VelocityY = 0;
			CharTop = *charY - *charHeight;
			CharBottom = *charY;
			*IsGround = true;
		}
		//天井
		if (CharTop >= BlockBottom - wallheightSize && CharTop <= BlockBottom) {
			*charY = BlockBottom + *charHeight + 1;
			*VelocityY = 0;
			CharTop = *charY - *charHeight;
			CharBottom = *charY;
		}
	}
}
