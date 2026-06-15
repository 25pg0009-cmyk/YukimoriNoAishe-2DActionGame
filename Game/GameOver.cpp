#include "UnlimitedLib/UnlimitedLib.h"
#include"Game.h"
#include "GameOver.h"
#include "StageSelect.h"
#include "Title.h"
#include "MainGame.h"
#include "GameSound.h"
#include "MagicShard.h"
#include "Result.h"
#include "Player.h"

int gameoverText;//画像
int button_on[3];//画像
int button_off[3];//画像
int arrowice3;
float laserAngle2;
int arrowiceX2;
int arrowiceY2;
int arrowPosX2[2] = { 590,1180 };
int ButtonID;
int GameoverX;
int gameOverSE;
bool isGameOverSEPlayed; 
//ゲームオーバー画面（仮）
void GameOver_Initialize()
{
	SetFadeOut(0, 0 , 0, 2.0f);

	gameoverText= CreateSprite("assets/GameScene/Result/gameover_text_00.png");

	button_on[1] = CreateSprite("assets/GameScene/Result/retry_button_00.dds");
	button_on[0] = CreateSprite("assets/GameScene/Result/stage_select_button_00.dds");

	button_off[1] = CreateSprite("assets/GameScene/Result/retry_button_01.dds");
	button_off[0] = CreateSprite("assets/GameScene/Result/stage_select_button_01.dds");
	arrowice3 = CreateSprite("assets/GameScene/Result/select_cursor_pose_00.dds");

	SelectSE[0] = CreateSEOgg("assets/SE/se_select_00.ogg");
	SelectSE[1] = CreateSEOgg("assets/SE/se_decision_00.ogg");
	gameOverSE = CreateSEOgg("assets/SE/se_gameover_00.ogg");

	laserAngle2 = 0.0f;
	arrowiceY2 = 910;
	arrowiceX2 = arrowPosX[1]+435; // 初期位置
	ButtonID =1;
	GameoverX = 0;
	isGameOverSEPlayed = false;
	if (currentCollection > 3) currentCollection = 3;
	if (currentCollection2 > 3) currentCollection2 = 3;
	if (get_ms_count > 3) get_ms_count = 3;
	if (get_ms_count2 > 3) get_ms_count2 = 3;
}

void GameOver_Release() {
	ReleaseSprite(gameoverText);
	ReleaseSprite(button_on[0]);
	ReleaseSprite(button_on[1]);
	ReleaseSprite(button_off[0]);
	ReleaseSprite(button_off[1]);
    ReleaseSprite(arrowice3);
	ReleaseSE(SelectSE[0]);
	ReleaseSE(SelectSE[1]);
	ReleaseSE(gameOverSE);
}

void GameOver_Update() {
	// ゲームオーバー突入時に一度だけ再生
	if (gameOverChange && !isGameOverSEPlayed) {
		PlaySE(gameOverSE, false);
		isGameOverSEPlayed = true;
		if (player.positionY > SCREEN_HEIGHT + pl_height / 2) {
			SE_Play(7);
		}
	}

	if (IsKeyPush(VK_RETURN) || IsPadPush(0, XINPUT_GAMEPAD_A)) {
        switch (ButtonID){
        case 0: MAIN_Release();
			GameOver_Release();
			InitializeScene(SCENE_STAGESELECT);//ステージセレクト
        	break;
        case 1:MAIN_Release();
			GameOver_Release();LoadingScene(SCENE_MAINGAME, true); // リトライ
			noAction = true;
			break;
		}
		PlaySE(SelectSE[1]);
	}

	if (IsKeyPush('D') || IsPadPush(0, XINPUT_GAMEPAD_DPAD_RIGHT)) {
		ButtonID++;
		if (ButtonID > 1) ButtonID = 1;
		arrowiceX2 = arrowPosX[ButtonID] + 435;
		PlaySE(SelectSE[0]);
	}
	if (IsKeyPush('A') || IsPadPush(0, XINPUT_GAMEPAD_DPAD_LEFT)) {
		ButtonID--;
		if (ButtonID < 0) ButtonID = 0;
		arrowiceX2 = arrowPosX[ButtonID] + 180;
		PlaySE(SelectSE[0]);
	}
}

void GameOver_Render() {
	if (gameOverChange) {
		RenderSpritePos(gameoverText, SCREEN_WIDTH / 2 - TEXT_WIDTH / 2, SCREEN_HEIGHT / 2 - TEXT_HEIGHT / 2);
		for (int i = 0;i < 2;i++) {
			if (ButtonID == i) {
				RenderSpritePos(button_on[i],(SCREEN_WIDTH / 3 - BUTTON_WIDTH / 2+500) * i + BUTTON_WIDTH / 3 + 50, SCREEN_HEIGHT / 1.3 + 40);
			}
			else {
				RenderSpritePos(button_off[i],(SCREEN_WIDTH / 3 - BUTTON_WIDTH / 2+500) * i + BUTTON_WIDTH / 3 + 50, SCREEN_HEIGHT / 1.3 + 40);
			}
		}
		laserAngle2 += 0.05f * (DeltaTime * 50);
		RenderSpriteRot(arrowice3, 96 / 2, 96 / 2, arrowiceX2, arrowiceY2, laserAngle2, 0.6f);
	}
}
