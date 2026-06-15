#include "UnlimitedLib/UnlimitedLib.h"
#include "flameimp.h"
#include "Game.h"
#include "Enemy.h"
#include "Stage.h"
#include "Player.h"
#include "IceSpear.h"
#include "hitblock.h"
#include "Fireball.h"
#include "Animation.h"
#include <math.h>
EnemyParam enemyParam[ENEMY_MAX];
#define ENEMY_MOVE_RANGE 90  // 上下移動範囲
#define ENEMY_MOVE_SPEED 2.0f

int Enemy1[68];//ファイヤーインプ画像
float enemy_height = ENEMY_FI_HEIGHT;
float enemy_width = ENEMY_FI_WIDTH;
float enemy_radius = enemy_width / 2.0f;
bool hiticespear_imp;

int enemySE;
void ENEMY_Move(int n);
void ENEMY_Collison(int n);

//アニメーション
AnimationInfo enemy_impptn = {
	//-1ループ,-2一度の再生で終了
	{
		/*00待機*/{20.0f,
					     {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,-1}},
	    /*01死亡*/{20.0f,
					     {27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,-2}},
		/*01攻撃*/{20.0f,
						 {47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,-2},
                         {0,0,0,0,0,0,17,0,0,0,0,0,0,0,0,0,0,0,0,0}},
    }
};


void ENEMY_Initialize()
{
	enemySE= CreateSEOgg("assets/SE/se_enemy1_attack_00.ogg");

	 CreateSpriteMatrix("assets/enemy/imp/enemy1_stop_00.dds",
		 ENEMY_FI_WIDTH,ENEMY_FI_HEIGHT,
		 8,4,27,
		 Enemy1
		);
	 CreateSpriteMatrix("assets/enemy/imp/enemy1_dead_00.dds",
		 ENEMY_FI_WIDTH, ENEMY_FI_HEIGHT,
		 8, 3, 21,
		 &Enemy1[27]
	 );
	 CreateSpriteMatrix("assets/enemy/imp/enemy1_attack_00.dds",
		 ENEMY_FI_WIDTH, ENEMY_FI_HEIGHT,
		 8, 3, 20,
		 &Enemy1[47]
	 );

	for (int i = 0;i < ENEMY_MAX;i++) {
		enemyParam[i].IsActive = false;
		enemyParam[i].state = enemyParam[i].STATE_IDLE_E;
		enemyParam[i].collision = true;
		enemyParam[i].fireAttack = true;
		enemyParam[i].firecount = 0.0f;
	}
}

void ENEMY_Release()
{
	ReleaseSpriteMatrix(Enemy1,69);
	ReleaseSE(enemySE);
}


void ENEMY_Update(){
	for (int i = 0;i < ENEMY_MAX;i++) {
		if (enemyParam[i].IsActive == false) continue;//無効の敵はスキップ
		//アニメーション更新
		Animation(DeltaTime, enemyParam[i].anim_info, enemy_impptn);

		//死亡アニメーション
		if (enemyParam[i].hp <= 0) {
			//アニメーション未再生なら切り替え
			if (enemyParam[i].state != enemyParam[i].STATE_DEATH) {
				enemyParam[i].state = enemyParam[i].STATE_DEATH;
				ChangeAnimation(enemyParam[i].state, enemyParam[i].anim_info, enemy_impptn);
				// 死亡遷移時に停止＆無効化
				enemyParam[i].vx = 0;
				enemyParam[i].vy = 0;
				enemyParam[i].collision = false; // 当たり判定OFF
				enemyParam[i].fireAttack = false;    // 攻撃OFF
			}
			//アニメーション後に削除
			if (!enemyParam[i].anim_info.isPlaying) {
				enemyParam[i].IsActive = false;
			}
			//continue;
		}

		if (enemyParam[i].if_frame > 0) {
			enemyParam[i].if_frame-=DeltaTime;
			if (enemyParam[i].if_frame <= 0) {
				enemyParam[i].if_frame = 0;
			}
		}
		
		//生存中のみ攻撃
		enemyParam[i].firecount+=DeltaTime;
		if (enemyParam[i].fireAttack) {
			//攻撃アニメーション
			if (enemyParam[i].state != enemyParam[i].STATE_ATTACK) {
				//60フレーム毎に弾を撃つ
				if (enemyParam[i].firecount>=4.0f) {
					float speed = 2.0f;
					float radian = atan2f((player.positionY - enemyParam[i].y), (player.positionX - enemyParam[i].x));
					float angle = radian;

					Fire2(&enemyParam[i].x, &enemyParam[i].y, &speed, &angle);
					enemyParam[i].state = enemyParam[i].STATE_ATTACK;
					ChangeAnimation(enemyParam[i].state, enemyParam[i].anim_info, enemy_impptn);
					PlaySE(enemySE, false, 0.5f);
					enemyParam[i].firecount = 0;
				}
			}else {
				if (!enemyParam[i].anim_info.isPlaying) {
					enemyParam[i].state = enemyParam[i].STATE_IDLE_E;
					ChangeAnimation(enemyParam[i].state, enemyParam[i].anim_info, enemy_impptn);
				}
			}
        }
		//死亡時は動きを止める
		if (enemyParam[i].state != enemyParam[i].STATE_DEATH) {
			ENEMY_Move(i);//移動処理
			if (enemyParam[i].collision) {
				ENEMY_Collison(i);
			}
		}
		if (enemyParam[i].collision) {
			for (int k = 0; k < BULLET_MAX; k++) {
				float imp_X = enemyParam[i].x - enemy_width / 2 - CameraX;
				float imp_Y = enemyParam[i].y - enemy_height + screen_remainder;
				float is_X = icespear[k].positionX - CameraX;
				if (icespear[k].isAlived == false) { continue; }
				//敵とスピアの判定
				hiticespear_imp = HitBullet(&is_X, &icespear[k].positionY, &IceSpear_radius, &IceSpear_height,
					&imp_X, &imp_Y, &enemy_width, &enemy_height, &enemyParam[i].hp, &player.atk, &icespear[k].isAlived);
				if (hiticespear_imp) {
					enemyParam[i].if_frame = ENEMY_IF_FRAME;
				}
			}
		}
	}	
}

void ENEMY_Move(int n) {
	//プレイヤー　＞　敵　＝右を向く
	if (player.positionX > enemyParam[n].x) {
		enemyParam[n].viewing_dir = 0;
	}
	else {
		enemyParam[n].viewing_dir = 1;
	}

	// === 上下移動処理 ===
	if (enemyParam[n].dir == 0) { // 上
		enemyParam[n].vy = -ENEMY_MOVE_SPEED;
		enemyParam[n].y += enemyParam[n].vy*(DeltaTime * 50);

		// 移動範囲を超えたら下に変更
		if (enemyParam[n].y < enemyParam[n].startY - ENEMY_MOVE_RANGE) {
			enemyParam[n].dir = 1;
		}
	}
	else if (enemyParam[n].dir == 1) { // 下
		enemyParam[n].vy = ENEMY_MOVE_SPEED;
		enemyParam[n].y += enemyParam[n].vy*(DeltaTime * 50);

		// 移動範囲を超えたら上に変更
		if (enemyParam[n].y > enemyParam[n].startY + ENEMY_MOVE_RANGE) {
			enemyParam[n].dir = 0;
		}
	}
}

void ENEMY_Collison(int n) {
	//当たり判定処理
	//右当たり判定
	//右ブロックチェック
	int RightX = enemyParam[n].x + enemy_radius;
	int tileR = STAGE_GetTileNo(RightX, enemyParam[n].y - 1);
	//頭上の右もチェック
	int tileHeadR = STAGE_GetTileNo(RightX, enemyParam[n].y - enemy_height);
	//中央のブロックもチェック
	int tileBodyR = STAGE_GetTileNo(RightX, enemyParam[n].y - BlockSIZE - 1);
	if (tileR > 0 || tileHeadR > 0 || tileBodyR > 0) {
		int BlockLeftX = RightX / BlockSIZE;
		int bottomX = BlockLeftX * BlockSIZE;
		enemyParam[n].x = bottomX - enemy_radius - 1;
		enemyParam[n].vx = 0;
	}

	//左当たり判定
	//左ブロックチェック
	int LeftX = enemyParam[n].x - enemy_radius;
	int tileL = STAGE_GetTileNo(LeftX, enemyParam[n].y - 1);
	//頭上の左もチェック
	int tileHeadL = STAGE_GetTileNo(LeftX, enemyParam[n].y - enemy_height);
	//中央の左もチェック
	int tileBodyL = STAGE_GetTileNo(LeftX, enemyParam[n].y - BlockSIZE - 1);
	if (tileL > 0 || tileHeadL > 0 || tileBodyL > 0) {
		int BlockRightX = LeftX / BlockSIZE + 1;
		int bottomX = BlockRightX * BlockSIZE;
		enemyParam[n].x = bottomX + enemy_radius + 1;
		enemyParam[n].vx = 0;
	}

	//頭上チェック
	if (enemyParam[n].y < 0) {
		int topY = enemyParam[n].y - enemy_height;
		//頭上のブロック
		int tileU = STAGE_GetTileNo(enemyParam[n].x, topY);
		int tileUR = STAGE_GetTileNo(enemyParam[n].x + enemy_radius, enemyParam[n].y - enemy_height);
		int tileUL = STAGE_GetTileNo(enemyParam[n].x - enemy_radius, enemyParam[n].y - enemy_height);
		//頭上がブロックだった場合
		if (tileU > 0 || tileUR > 0 || tileUL > 0) {
			enemyParam[n].y = 0;
			int ty = topY / BlockSIZE + 1;
			int bottomY = ty * BlockSIZE;
			enemyParam[n].y = bottomY + enemy_height;
		}
	}
	if (enemyParam[n].y >= 0) {//落下中
		int tile = STAGE_GetTileNo(enemyParam[n].x, enemyParam[n].y);//足元のタイルをゲット
		int tileR = STAGE_GetTileNo(enemyParam[n].x + enemy_radius, enemyParam[n].y);
		int tileL = STAGE_GetTileNo(enemyParam[n].x - enemy_radius, enemyParam[n].y);
		if (tile > 0 || tileR > 0 || tileL > 0) {
			int ty = enemyParam[n].y / BlockSIZE;
			enemyParam[n].y = ty * BlockSIZE;
			enemyParam[n].y = 0.0f;//速度を止める
			enemyParam[n].IsGround = true;
		}else {
			enemyParam[n].IsGround = false;
		}
	}
}
int impIf_frame[ENEMY_MAX];
void ENEMY_Render(){
	for (int i = 0;i < ENEMY_MAX;i++) {
		impIf_frame[i] = enemyParam[i].if_frame * 30;
		if (impIf_frame[i] % 2 == 0) {
			if (enemyParam[i].IsActive == false) { continue;}//無効の敵はスキップ
			if (enemyParam[i].viewing_dir) {
				RenderSpritePos(Enemy1[enemyParam[i].anim_info.sprite_index], enemyParam[i].x - ENEMY_FI_WIDTH / 2 - CameraX, enemyParam[i].y - ENEMY_FI_HEIGHT + SCREEN_REMAINDER);
			}
			else {
				RenderSpritePosLR(Enemy1[enemyParam[i].anim_info.sprite_index], enemyParam[i].x - ENEMY_FI_WIDTH / 2 - CameraX, enemyParam[i].y - ENEMY_FI_HEIGHT + SCREEN_REMAINDER);
			}
		}
	}
}

void ENEMY_Set(float x, float y, float vx)
{
	for (int i = 0;i < ENEMY_MAX;i++)
	{
		//空きを探す
		if (enemyParam[i].IsActive == true) continue;
		//空き場所にセット
		enemyParam[i].IsActive = true;//すでに有効な敵はスキップ
		enemyParam[i].x = x;
		enemyParam[i].y = y;
		enemyParam[i].vx = 0.0f;
		enemyParam[i].vy = 0.0f;
		enemyParam[i].dir = 1;
		enemyParam[i].viewing_dir = 0;
		enemyParam[i].IsGround = false;
		enemyParam[i].startY = enemyParam[i].y;
		enemyParam[i].atk = FLAMEIMP_ATK;
		enemyParam[i].hp = FLAMEIMP_HP;
		hiticespear_imp = false;
		enemyParam[i].if_frame = 0.0f;
		enemyParam[i].firecount = 0.0f;
		enemyParam[i].state = enemyParam[i].STATE_IDLE_E;
		enemyParam[i].collision = true;
		enemyParam[i].fireAttack = true;
		enemyParam[i].anim_info.ID = 0;
		enemyParam[i].anim_info.frame = 0;//最初から
		enemyParam[i].anim_info.sprite_index = enemy_impptn.param[0].ptn[(int)enemyParam[i].anim_info.frame];
		enemyParam[i].anim_info.isPlaying = true;//再生中
		break;//1体分セットでループ終了
	}
}

