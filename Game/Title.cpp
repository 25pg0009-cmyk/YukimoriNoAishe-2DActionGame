#include "UnlimitedLib/UnlimitedLib.h"
#include "Title.h"
#include "Game.h"
#include "StageSelect.h"
#include "GameSound.h"


#define TITLE_WIDTH 712
#define TILE_HEIGHT 364
#define MENU_START_WIDTH 336
#define MENU_START_HEIGHT 60
#define MENU_END_WIDTH 228
#define MENU_ENDHEIGHT 56

int TitleBG;//タイトル画像
int TitleMenu_on[2];
int TitleMenu_off[2];
int TitleMenuID;
int Title;
int arrowice;
int arrowiceX;
int arrowiceY;
float Angle;//回転角度
void TITLE_Initialize()
{
	SetFadeOut(0, 0, 0, 1.5f);
	TitleBG = CreateSprite("assets/GameScene/Title/title_back_00.dds");
	Title= CreateSprite("assets/GameScene/Title/title_text_00.dds");
	SetSpriteSize(Title, TITLE_WIDTH*1.45f, TILE_HEIGHT*1.45f);
	TitleMenu_on[0]= CreateSprite("assets/GameScene/Title/title_game_start_00.dds");
	TitleMenu_on[1]= CreateSprite("assets/GameScene/Title/title_game_end_00.dds");

    TitleMenu_off[0]= CreateSprite("assets/GameScene/Title/title_game_start_01.dds");
	TitleMenu_off[1]= CreateSprite("assets/GameScene/Title/title_game_end_01.dds");

	arrowice = CreateSprite("assets/GameScene/Result/select_cursor_pose_00.dds");
	Angle = 0.0f;

	SelectSE[0] = CreateSEOgg("assets/SE/se_select_00.ogg");
	SelectSE[1] = CreateSEOgg("assets/SE/se_decision_00.ogg");
	BGM_Play("assets/BGM/bgm_title_00.ogg");

	TitleMenuID = 0;
	arrowiceX = 750;
	arrowiceY = 860;

	SetBGMVolume(1.0f);
	SetSEVolume(1.0f);
}

void TITLE_Release()
{
	ReleaseSprite(TitleBG);
	ReleaseSprite(Title);
	ReleaseSprite(TitleMenu_on[0]);
	ReleaseSprite(TitleMenu_on[1]);
	ReleaseSprite(TitleMenu_off[0]);
	ReleaseSprite(TitleMenu_off[1]);
	ReleaseSprite(arrowice);
	ReleaseSE(SelectSE[0]);
	ReleaseSE(SelectSE[1]);
}

void TITLE_Update()
{
	
	// メニュー選択
	if (IsKeyPush('W') || IsPadPush(0, XINPUT_GAMEPAD_DPAD_UP)) {
		TitleMenuID--; //上のメニューへ
		arrowiceY = 860;
		if (TitleMenuID < 0) TitleMenuID = 0; //一番上で停止
		PlaySE(SelectSE[0]);//SE再生
	}
	if (IsKeyPush('S') || IsPadPush(0, XINPUT_GAMEPAD_DPAD_DOWN)) {
		TitleMenuID++; //下のメニューへ
		arrowiceY = 940;
		if (TitleMenuID > 1) TitleMenuID = 1; //一番下で停止
		PlaySE(SelectSE[0], true);//SE再生
	}

	// エンターキーでメニュー決定へ
	if (IsKeyPush(VK_RETURN) || IsPadPush(0, XINPUT_GAMEPAD_A)) {
		switch (TitleMenuID) {
		case 0:InitializeScene(SCENE_STAGESELECT);break;
		case 1: PostQuitMessage(0); break;
		}
		TITLE_Release();
		PlaySE(SelectSE[1]);//SE再生
		BGM_Stop();
		return;
	}
}

void TITLE_Render(){
	RenderSprite(TitleBG);
	RenderSpritePos(Title, SCREEN_WIDTH/2- TITLE_WIDTH*1.45f/2, SCREEN_HEIGHT/2-TILE_HEIGHT*1.45f/2-120);
	
	// メニュー表示(512*64の画像が2段)
	if (TitleMenuID == 0) {
		RenderSpritePos(TitleMenu_on[0], SCREEN_WIDTH / 2 - MENU_START_WIDTH / 2, (SCREEN_HEIGHT / 2 + MENU_START_HEIGHT * 4+50));
	}else {
		RenderSpritePos(TitleMenu_off[0], SCREEN_WIDTH / 2 - MENU_START_WIDTH / 2, (SCREEN_HEIGHT / 2 + MENU_START_HEIGHT * 4+50));
	}
	if (TitleMenuID == 1) {
		RenderSpritePos(TitleMenu_on[1], (SCREEN_WIDTH / 2 - MENU_END_WIDTH / 2)-5, (SCREEN_HEIGHT / 2 + MENU_ENDHEIGHT * 4) + 150);
	}else {
		RenderSpritePos(TitleMenu_off[1], (SCREEN_WIDTH / 2 - MENU_END_WIDTH / 2)-5, (SCREEN_HEIGHT / 2 + MENU_ENDHEIGHT * 4) + 150);
	}
	Angle += 0.05*(DeltaTime*50);
	RenderSpriteRot(arrowice,96/2,96/2, arrowiceX, arrowiceY, Angle,0.7f);
}
