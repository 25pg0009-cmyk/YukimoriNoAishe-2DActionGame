#include "UnlimitedLib/UnlimitedLib.h"
#include "Result.h"
#include "Game.h"
#include "GameOver.h"
#include "MainGame.h"
#include "MagicShard.h"
#include "GameSound.h"
#include "Stage.h"
#include "Player.h"

int gameclearBG;
int GameClearText;
int Gameclear[5];//画像
int button_on2[2];//画像
int button_off2[2];//画像
int collection2[3];//画像
int arrowice2;
extern float Angle;
extern int arrowiceX;
extern int arrowiceY;
int arrowPosX[3] = { 590,1180,1770 };
int resultX;
int ButtonID2;
extern int Select;

void Result_Initialize() {
	gameclearBG = CreateSprite("assets/GameScene/Result/gameclear_00.dds");
    GameClearText=CreateSprite("assets/GameScene/Result/text_clear_00.png");
	Gameclear[0] = CreateSprite("assets/GameScene/Result/clear_back_00.png");
	Gameclear[1] = CreateSprite("assets/GameScene/Result/clear_enemy_00.png");
	Gameclear[2] = CreateSprite("assets/GameScene/Result/text_result_00.png");
	Gameclear[3] = CreateSprite("assets/GameScene/Result/clear_aishe_00.png");
	Gameclear[4] = CreateSprite("assets/GameScene/Result/clear_front_00.png");

	button_on2[1] = CreateSprite("assets/GameScene/Result/retry_button_00.dds");
	button_on2[0] = CreateSprite("assets/GameScene/Result/stage_select_button_00.dds");

	button_off2[1] = CreateSprite("assets/GameScene/Result/retry_button_01.dds");
	button_off2[0] = CreateSprite("assets/GameScene/Result/stage_select_button_01.dds");

	collection2[0] = CreateSprite("assets/GameScene/Result/collection_00.png");
	collection2[1] = CreateSprite("assets/GameScene/Result/collection_01.png");

	arrowice2 = CreateSprite("assets/GameScene/Result/select_cursor_pose_00.dds");

	for (int i = 0;i < 2;i++) {
		SetSpriteSize(collection2[i], 166, 288);
	}

	SelectSE[0] = CreateSEOgg("assets/SE/se_select_00.ogg");
	SelectSE[1] = CreateSEOgg("assets/SE/se_decision_00.ogg");

	Angle = 0.0f;
	arrowiceY = 910;
	arrowiceX = arrowPosX[0]+180; // 初期位置
	ButtonID2 = 0;
	resultX = 0;
}

void Result_Release() {
	ReleaseSprite(gameclearBG);
	ReleaseSprite(GameClearText);
	ReleaseSprite(Gameclear[0]);
	ReleaseSprite(Gameclear[1]);
	ReleaseSprite(Gameclear[2]);
	ReleaseSprite(Gameclear[3]);
	ReleaseSprite(Gameclear[4]);

	ReleaseSprite(button_on2[0]);
	ReleaseSprite(button_on2[1]);

	ReleaseSprite(button_off2[0]);
	ReleaseSprite(button_off2[1]);

	ReleaseSprite(collection2[0]);
	ReleaseSprite(collection2[1]);

	ReleaseSprite(arrowice2);

	ReleaseSE(SelectSE[0]);
	ReleaseSE(SelectSE[1]);
}

void Result_Update() {
	if (Select == 3) {
		if (IsKeyPush(VK_RETURN) || IsPadPush(0, XINPUT_GAMEPAD_A)) {
			InitializeScene(SCENE_TITLE);
		}
	}else if (Select == 1 || Select == 2) {
		if (IsKeyPush(VK_RETURN) || IsPadPush(0, XINPUT_GAMEPAD_A)) {
			switch (ButtonID2) {
			case 0:Result_Release();InitializeScene(SCENE_STAGESELECT);//ステージセレクト
				break;
			case 1:Result_Release();	LoadingScene(SCENE_MAINGAME, true); // リトライ
				noAction = true;
				break;
			}
			PlaySE(SelectSE[1]);//SE再生
		}
		if (IsKeyPush('D') || IsPadPush(0, XINPUT_GAMEPAD_DPAD_RIGHT)) {
			ButtonID2++;
			if (ButtonID2 > 1) {
				ButtonID2 = 1;//一番上で停止
			}
			arrowiceX = arrowPosX[ButtonID2] + 435;
			PlaySE(SelectSE[0]);//SE再生
		}
		if (IsKeyPush('A') || IsPadPush(0, XINPUT_GAMEPAD_DPAD_LEFT)) {
			ButtonID2--;
			if (ButtonID2 < 0) {
				ButtonID2 = 0;//一番下で停止
			}
			arrowiceX = arrowPosX[ButtonID2] + 180;
			PlaySE(SelectSE[0]);//SE再生
		}
	}
}

void Result_Render() {
	if (Select == 1 || Select == 2) {
		RenderSprite(Gameclear[0]);
		RenderSprite(Gameclear[2]);
		RenderSprite(Gameclear[3]);
		RenderSpritePos(Gameclear[1], SCREEN_WIDTH / 2 + 330, SCREEN_HEIGHT / 2 + 100);
		for (int i = 0;i < 2;i++) {
			if (ButtonID2 == i) {
				RenderSpritePos(button_on2[i], (SCREEN_WIDTH / 3 - BUTTON_WIDTH / 2 + 500) * i + BUTTON_WIDTH / 3 + 50, SCREEN_HEIGHT / 1.3 + 40);
			}
			else {
				RenderSpritePos(button_off2[i], (SCREEN_WIDTH / 3 - BUTTON_WIDTH / 2 + 500) * i + BUTTON_WIDTH / 3 + 50, SCREEN_HEIGHT / 1.3 + 40);
			}
		}
		Angle += 0.05f *(DeltaTime * 50);
		
		RenderSpriteRot(arrowice2, 96 / 2, 96 / 2, arrowiceX, arrowiceY, Angle, 0.6f);
		RenderSprite(Gameclear[4]);
		for (int i = 0;i < 3;i++) {
			RenderSpritePos(collection2[1], 1100 - COLLECTION_WIDTH / 2 + COLLECTION_WIDTH + 230 * i - 100, SCREEN_HEIGHT / 2 - COLLECTION_HEIGHT / 2 + 50);
		}
		if (Select == 1) {
			if (get_ms_count > 0) {
				for (int i = 0;i < get_ms_count;i++) {
					RenderSpritePos(collection2[0], 1100 - COLLECTION_WIDTH / 2 + COLLECTION_WIDTH + 230 * i - 100, SCREEN_HEIGHT / 2 - COLLECTION_HEIGHT / 2 + 50);
				}
			}
			RenderSpritePos(GameClearText, SCREEN_WIDTH / 2 - TEXT_WIDTH / 2 - 450, SCREEN_HEIGHT / 2 - TEXT_HEIGHT / 2 - 400);
		}
		if (Select == 2) {
			if (get_ms_count2 > 0) {
				for (int i = 0;i < get_ms_count2;i++) {
					RenderSpritePos(collection2[0], 1100 - COLLECTION_WIDTH / 2 + COLLECTION_WIDTH + 230 * i - 100, SCREEN_HEIGHT / 2 - COLLECTION_HEIGHT / 2 + 50);
				}
			}
			RenderSpritePos(GameClearText, SCREEN_WIDTH / 2 - TEXT_WIDTH / 2 - 450, SCREEN_HEIGHT / 2 - TEXT_HEIGHT / 2 - 400);
		}



	}
	if (Select == 3) {
		RenderSprite(gameclearBG);
	}
}
