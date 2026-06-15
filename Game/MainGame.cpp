#include "MainGame.h"
#include "UnlimitedLib/UnlimitedLib.h"
#include "Game.h"
#include "Player.h"
#if USE_IMGUI
#include "imgui/imgui.h"
#endif
#include "Stage.h"
#include "flameimp.h"
#include "flameslime.h"
#include "UnlimitedLib/UnlimitedLibExtension.h"
#include "IceSpear.h"
#include "IceBlock.h"
#include "UI.h"
#include "Item.h"
#include "MagicShard.h"
#include "Fireball.h"
#include <string.h>
#include "FlameDemon.h"
#include "Goal.h"
#include "GameSound.h"
#include "Pose.h"
#include "CheckHitBox.h"
#include "GameOver.h"
#include "Effect.h"
int gFrame;
bool gIsPaussed;
bool gameStop;
bool gameOverChange;

float camX = CameraX;
float camY = CameraY;
float camW = SCREEN_WIDTH;
float camH = SCREEN_HEIGHT;

void MAIN_Initialize(){
	gIsPaussed = false;
    gameStop=false;
	gameOverChange = false;

	EnableDDSReplace(true);
 
	PLAYER_Initialize();
	ENEMY_Initialize();
	ENEMY2_Initialize();
	FlameDemon_Initialize();
	Item_Initialize();
	MagicShard_Initialize();
	STAGE_Initialize();
	IceSpear_Initialize();
	IceBlock_Initialize();
	UI_Initialize();
	FIRE_Initialize();
	GOAL_Initialize();
	Pose_Initialize();
	GameOver_Initialize();
	EFFECT_Initialize();
	
}
#if USE_IMGUI
// デバッグメニュー描画(デバッグモード時のみ使用可能)
void DebugMenu(void)
{
	player.positionX;
	player.positionY;
}
#endif

void MAIN_Release(){
	STAGE_Release();
	PLAYER_Release();
	ENEMY_Release();
	ENEMY2_Release();
	Item_Release();
	MagicShard_Release();
	IceSpear_Release();
	IceBlock_Release();
	UI_Release();
	FIRE_Release();
	FlameDemon_Release();
	GOAL_Release();
    Pose_Release();
	GameOver_Release();
	EFFECT_Release();
	
}

void MAIN_Update(){
	GOAL_Update();
		if (!gameStop) {//ゴール
			Pose_Update();
			PLAYER_IsGoal();
			if (gameOverChange) {//ゲームオーバー
				//更新処理
				if (Select != 3) {
					ENEMY_Update();
					ENEMY2_Update();
				}else {
					FlameDemon_Update();
					UI_UpdateBoss();
				}
				FIRE_Update();
				IceSpear_Update();
				IceBlockUpdate();
				UI_Update();
				Item_Update();
				MagicShard_Update();
				PLAYER_Update();
				Pose_Update();
				EFFECT_Update();
				STAGE_Update();
			}else{
				if (!gIsPaussed) {//ポーズ
					if (Select != 3) {
						ENEMY_Update();
						ENEMY2_Update();
					
					}
					else {
						FlameDemon_Update();
						
					}
					IceSpear_Update();
					IceBlockUpdate();
					UI_Update();
					Item_Update();
					MagicShard_Update();
					FIRE_Update();
					PLAYER_Update();
					EFFECT_Update();
					STAGE_Update();
				}
			}
	    }
   //デバッグ用リスタートキー
   if (IsKeyPush('R')) {
	   Finalize();
	   //初期化
	   LoadingScene(SCENE_MAINGAME, true);
	   gFrame = 0;
	   return; // これ以上の処理をスキップ
   }
#if USE_IMGUI
   DebugMenu();
#endif
   gFrame++;
}

void MAIN_Render(){
	//描画処理
	STAGE_Render();
	HitBox_Render();
	ENEMY_Render();
	ENEMY2_Render();
	MagicShard_Render();
	Item_Render();
	GOAL_Render();
	FlameDemon_Render();
	FIRE_Render();
	UI_Render();
	GameOver_Render();
	IceBlock_Render();
	IceSpear_Render();
	PLAYER_Render();
	EFFECT_Render();
	Pose_Render();
	

}
