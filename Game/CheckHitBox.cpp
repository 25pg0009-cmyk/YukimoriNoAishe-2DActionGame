#include "UnlimitedLib/UnlimitedLib.h"
#include "CheckHitBox.h"
#include "Player.h"
#include "Fireball.h"
#include "Stage.h"
#include "Enemy.h"
#include "FlameDemon.h"
#include "flameimp.h"
#include "flameslime.h"
#include "MagicShard.h"
#include "Item.h"
#include "IceSpear.h"
#include "Goal.h"


bool isCheckHitBox = true;
void HitBox_Render() {
	
	if (IsKeyPush('U')) {
		isCheckHitBox = !isCheckHitBox;
	}
	if (!isCheckHitBox) {
		//ファイヤーボール
		for (int b = 0; b < FIRE_BALL_MAX; b++) {
			if (fireParam[b].IsActive) {
				RenderPoly(fireParam[b].x - fire_size / 2 - CameraX, fireParam[b].y - fire_size / 2, fire_size, fire_size, 0xffffffff);
			}
			
		}
		if (fire_holizontal.isAlived) {
			RenderPoly(fire_holizontal.positionX - bigfire_size / 2, fire_holizontal.positionY - bigfire_size / 2, bigfire_size, bigfire_size, 0xffffffff);
		}
		//敵との衝突判定
		RenderPoly(player.positionX - pl_width / 2 - CameraX, player.positionY - pl_height + screen_remainder, pl_width, pl_height, 0xffffffff);
		for (int n = 0; n < ENEMY_MAX; n++) {
			if (enemyParam2[n].IsActive) {
				RenderPoly(enemyParam2[n].x - enemy2_width / 2 - CameraX, enemyParam2[n].y - enemy2_height + screen_remainder, enemy2_width, enemy2_height, 0xffffffff);
			}
			if (enemyParam[n].IsActive) {
				RenderPoly(enemyParam[n].x - enemy_width / 2 - CameraX, enemyParam[n].y - enemy_height + screen_remainder, enemy_width, enemy_height, 0xffffffff);
			}
		}
		if (demon.isAlived) {
			RenderPoly(demon.positionX - demon_width / 2, demon.positionY - demon_height + screen_remainder, demon_width, demon_height, 0xffffffff);
		}

		for (int i = 0; i < MS_MAX; i++) {
			if (msParam[i].IsActive) {
				RenderPoly(msParam[i].x - ms_width / 2 - CameraX, msParam[i].y - ms_height / 2 + screen_remainder, ms_width, ms_height, 0xffffffff);
			}
		}
		for (int i = 0; i < ITEM_MAX; i++)
		{
			if (itemParam[i].IsActive) {
				RenderPoly(itemParam[i].x - item_size / 2 - CameraX, itemParam[i].y - item_size / 2 + screen_remainder, item_size, item_size, 0xffffffff);
			}
		}
		RenderPoly(goal_x - Goal_width / 2 - CameraX, goal_y - Goal_height / 2, Goal_width, Goal_height, 0xffffffff);
		for (int i = 0; i < BULLET_MAX; i++) {
			if (icespear[i].isAlived) {
				RenderPoly(icespear[i].positionX - CameraX, icespear[i].positionY, IceSpear_radius, IceSpear_height, 0xffffffff);
			}
		}
	}
}
