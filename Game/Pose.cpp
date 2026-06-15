#include "UnlimitedLib/UnlimitedLib.h"
#include "Pose.h"
#include "MainGame.h"
#include "GameSound.h"
#include "Game.h"
#include "GameOver.h"
#include "Animation.h"
#include "IceSpear.h"
#include "IceBlock.h"

int iceBlockP[30];
int pauseBook;
int pauseText;
int pause_button_on[2];
int pause_button_off[2];
int pause_ButtonID;
int icespearP[6];
AnimationUniqueInfo icespearPause;
AnimationInfo  icespearPause_animptn{
	{
		/*00弾*/{5.0f,{0,1,2,3,4,5,-1}},
	}
};

AnimationUniqueInfo iceBlockPause;
AnimationInfo  iceBlockPause_animptn{
	{
		/*00ブロック*/{10.0f,{12,13,14,15,16,17,18,19,0,1,2,3,4,5,6,7,8,9,10,11, -1}},
	}
};

void Pose_Initialize(){
	pauseBook=CreateSprite("assets/GameScene/pose_book_00.dds");
	pauseText = CreateSprite("assets/GameScene/manual_text_00.png.dds");
	pause_button_on[0]= CreateSprite("assets/GameScene/resume_game_button_00.png");
	pause_button_on[1] = CreateSprite("assets/GameScene/back_title_button_pose_00.png");
	pause_button_off[0] = CreateSprite("assets/GameScene/resume_game_button_01.png");
	pause_button_off[1] = CreateSprite("assets/GameScene/back_title_button_pose_01.png");
	SelectSE[0] = CreateSEOgg("assets/SE/se_select_00.ogg");
	SelectSE[1] = CreateSEOgg("assets/SE/se_decision_00.ogg");
	pause_ButtonID = 0;

	//アイスブロックモーション
	CreateSpriteMatrix("assets/player/iceblock_countdown_00.dds",
		ICEBLOCKSIZE, ICEBLOCKSIZE,
		4, 3, 12,
		iceBlockP);

	CreateSpriteMatrix("assets/player/iceblock_create_00.dds",
		ICEBLOCKSIZE, ICEBLOCKSIZE,
		4, 2, 8,
		&iceBlockP[12]);

	CreateSpriteMatrix("assets/player/iceblock_destroy_00.dds",
		180, 180,
		5, 2, 10,
		&iceBlockP[20]);
	for (int i = 0;i < 20;i++) {
		SetSpriteSize(iceBlockP[i], 144.0f*1.5f, 144.0f*1.5f);
	}
	for (int i = 21;i < 30;i++) {
		SetSpriteSize(iceBlockP[i], 144.0f * 1.5f, 144.0f * 1.5f);
	}
	//弾モーション
	CreateSpriteMatrix("assets/player/icespear_00.png",
		BULLET_WIDTH, BULLET_HEIGHT,
		6, 1, 6,
		icespearP);
	for (int i = 0;i < 6;i++) {
	   SetSpriteSize(icespearP[i], 480, 240);
	}
}

void Pose_Release(){
	ReleaseSprite(pauseBook);
	ReleaseSprite(pauseText);
	ReleaseSprite(pause_button_on[0]);
	ReleaseSprite(pause_button_on[1]);
	ReleaseSprite(pause_button_off[0]);
	ReleaseSprite(pause_button_off[1]);
	ReleaseSpriteMatrix(iceBlockP, 30);
	ReleaseSpriteMatrix(icespearP, 6);
	ReleaseSE(SelectSE[0]);
	ReleaseSE(SelectSE[1]);
}

void Pose_Update(){
	
	if (IsKeyPush(VK_ESCAPE)||IsPadPush(0, XINPUT_GAMEPAD_START)) {
		gIsPaussed = !gIsPaussed;
		if (!gIsPaussed) {
			iceBlockPause.ID = 0;
			iceBlockPause.frame = 0;//最初から
			iceBlockPause.sprite_index = iceBlockPause_animptn.param[0].ptn[(int)iceBlockPause.frame];
			iceBlockPause.isPlaying = true;//再生中
		}
	}
	if (gIsPaussed) {
		Animation(DeltaTime, icespearPause, icespearPause_animptn);
		Animation(DeltaTime, iceBlockPause, iceBlockPause_animptn);
		if (IsKeyPush(VK_RETURN) || IsPadPush(0, XINPUT_GAMEPAD_A)) {
			switch (pause_ButtonID) {
			case 0:
				gIsPaussed = false;//ゲーム再開
				noAction = true;
				break;
			case 1:
				MAIN_Release();
				Pose_Release();
				InitializeScene(SCENE_TITLE); //タイトル
				gIsPaussed = false;
				break;
			}
		}
		if (IsKeyPush('W') || IsPadPush(0, XINPUT_GAMEPAD_DPAD_UP)) {
			pause_ButtonID--;
			if (pause_ButtonID < 0)pause_ButtonID = 0;//一番上で停止
			PlaySE(SelectSE[0]);//SE再生
		}
		if (IsKeyPush('S') || IsPadPush(0, XINPUT_GAMEPAD_DPAD_DOWN)) {
			pause_ButtonID++;
			if (pause_ButtonID > 1)pause_ButtonID = 1;//一番下で停止
			PlaySE(SelectSE[0]);//SE再生
	    }
	}
}

void Pose_Render(){
	if (gIsPaussed) {
		RenderSprite(pauseBook);
		for (int i = 0;i < 2;i++) {
			if (pause_ButtonID == i) {
				RenderSpritePos(pause_button_on[i], SCREEN_WIDTH /6- BUTTON_WIDTH/2, SCREEN_HEIGHT /3- BUTTON_HEIGHT /2+150*i-100);
			}
			else {
				RenderSpritePos(pause_button_off[i], SCREEN_WIDTH /6- BUTTON_WIDTH/2, SCREEN_HEIGHT /3- BUTTON_HEIGHT /2+150*i-100);
			}
		}
		RenderSprite(pauseText);
		RenderSpritePos(icespearP[icespearPause.sprite_index], SCREEN_WIDTH/2+BULLET_WIDTH/2+100, SCREEN_HEIGHT/6- BULLET_HEIGHT/2);
		RenderSpritePos(iceBlockP[iceBlockPause.sprite_index], SCREEN_WIDTH/2+600, SCREEN_HEIGHT / 2+90);
	}
}
