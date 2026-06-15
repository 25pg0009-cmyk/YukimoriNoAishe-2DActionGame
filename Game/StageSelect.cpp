#include "UnlimitedLib/UnlimitedLib.h"
#include "StageSelect.h"
#include "Game.h"
#include "Stage.h"
#include"MainGame.h"
#include"GameSound.h"
#include"Loading.h"
#include "GameOver.h"
#include "MagicShard.h"
#include "Player.h"
#include <math.h>
#include "Animation.h"
#include "Goal.h"
#define SELECT_WIDTH 248
#define SELECT_HEIGHT 104

//セレクト画面
int selectstage;
int stage_select_on[3];
int stage_select_off[3];
int stage_NotOpen[3];
int SelectMenuID;
int Select ;
int aisheSelect;
int selectBook;
int pen;
float pen_x;
float pen_y;
int penMove_y[3] = { 380,534,684 };
int selectEffect;
int aisheBook[40];
int selectCollection[2];
int selectCollectionNotOpen;
int currentCollection;
int currentCollection2;
float frame = 0.0f;

AnimationInfo book_ptn = {
	// -1ループ, -2一度の再生で終了
	{20.0f,{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,
	24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,-1}},
};

AnimationUniqueInfo bookanim;

void  Select_Initialize() {
	selectEffect = CreateSprite("assets/GameScene/Select/stage_select_light_00.dds");
	pen = CreateSprite("assets/GameScene/Select/select_cursor_stage_select_00.png");
	selectBook= CreateSprite("assets/GameScene/Select/stage_select_book_00.dds");
	selectstage = CreateSprite("assets/GameScene/Select/stage_select_back_00.dds");
	stage_select_on[0] = CreateSprite("assets/GameScene/Select/stage_select_button_one_00.dds");
	stage_select_on[1] = CreateSprite("assets/GameScene/Select/stage_select_button_two_00.dds");
	stage_select_on[2] = CreateSprite("assets/GameScene/Select/stage_select_button_three_00.dds");
	aisheSelect= CreateSprite("assets/GameScene/Select/stage_select_aishe_00.dds");

	selectCollection[0] = CreateSprite("assets/GameScene/Result/collection_00.png");
    selectCollection[1] = CreateSprite("assets/GameScene/Result/collection_01.png");
	for (int i = 0;i < 2;i++) {
		SetSpriteSize(selectCollection[i],83.0f/1.5f,144.0f/1.5f);
	}
	stage_select_off[0] = CreateSprite("assets/GameScene/Select/stage_select_button_one_01.dds");
	stage_select_off[1] = CreateSprite("assets/GameScene/Select/stage_select_button_two_01.dds");
	stage_select_off[2] = CreateSprite("assets/GameScene/Select/stage_select_button_three_01.dds");

	stage_NotOpen[0] = CreateSprite("assets/GameScene/Select/stage_select_button_one_02.dds");
	stage_NotOpen[1] = CreateSprite("assets/GameScene/Select/stage_select_button_two_02.dds");
	stage_NotOpen[2] = CreateSprite("assets/GameScene/Select/stage_select_button_three_02.dds");
	selectCollectionNotOpen = CreateSprite("assets/GameScene/Select/collection_02.dds");
	SetSpriteSize(selectCollectionNotOpen, 83.0f / 1.5f, 144.0f / 1.5f);
	SelectSE[0] = CreateSEOgg("assets/SE/se_select_00.ogg");
	SelectSE[1] = CreateSEOgg("assets/SE/se_decision_00.ogg");

	CreateSpriteMatrix("assets/GameScene/Select/stage_select_aishe_book_00.dds",
		416, 184,
		8, 5, 40,
		aisheBook
	);
	Select = STAGE3;
	pen_x=1100;
	pen_y = penMove_y[0];
	SelectMenuID = 0;
}

void  Select_Release() {
	ReleaseSprite(selectCollection[0]);
	ReleaseSprite(selectCollection[1]);
	ReleaseSpriteMatrix(aisheBook, 40);
	ReleaseSprite(selectEffect);
	ReleaseSprite(pen);
	ReleaseSprite(selectBook);
	ReleaseSprite(selectstage);
	ReleaseSprite(aisheSelect);
	ReleaseSprite(stage_select_on[0]);
	ReleaseSprite(stage_select_on[1]);
	ReleaseSprite(stage_select_on[2]);
	ReleaseSprite(stage_select_off[0]);
	ReleaseSprite(stage_select_off[1]);
	ReleaseSprite(stage_select_off[2]);
	ReleaseSprite(stage_NotOpen[0]);
	ReleaseSprite(stage_NotOpen[1]);
	ReleaseSprite(stage_NotOpen[2]);
	ReleaseSprite(selectCollectionNotOpen);
	ReleaseSE(SelectSE[0]);
	ReleaseSE(SelectSE[1]);
}

void DebugStageRelease() {
	if (IsKeyPush('A')&& IsKeyPress('L')) {
		currentCollection = 3;
		currentCollection2 = 3;
	}
}

void  Select_Update() {
	DebugStageRelease();
	Animation(DeltaTime, bookanim, book_ptn);
	if (currentCollection > 3) {
		currentCollection = 3;
	}
	if (currentCollection2 > 3) {
		currentCollection2 = 3;
	}
	
	if (IsKeyPush('W') || IsPadPush(0, XINPUT_GAMEPAD_DPAD_UP)) {
		SelectMenuID--;
		if (SelectMenuID < 0)SelectMenuID = 0;//一番上で停止
		PlaySE(SelectSE[0]);//SE再生
		pen_y = penMove_y[SelectMenuID];
	}
	if (IsKeyPush('S') || IsPadPush(0, XINPUT_GAMEPAD_DPAD_DOWN)) {
		SelectMenuID++;
		if (SelectMenuID > 2)SelectMenuID =2;//一番下で停止
		PlaySE(SelectSE[0]);//SE再生
		pen_y = penMove_y[SelectMenuID];
	}
	if (IsKeyPush(VK_ESCAPE) || IsPadPush(0, XINPUT_GAMEPAD_B)) {
		Select_Release();
		InitializeScene(SCENE_TITLE);
	}

	if (IsKeyPush(VK_RETURN) || IsPadPush(0, XINPUT_GAMEPAD_A)) {
		noAction = true;
		bool canEnter = false; // 遷移できるかどうか
		switch (SelectMenuID) {
		case 0:canEnter = true;Select = STAGE1;break;
		case 1:if (currentCollection == 3) {
				canEnter = true;
				Select = STAGE2;
		} break;
		case 2: if (currentCollection2 == 3) {
				canEnter = true;
				Select = STAGE3;
		   }break;
		}
		if (canEnter) {
			PlaySE(SelectSE[1]); // 決定音
			Select_Release();
			LoadingScene(SCENE_MAINGAME, true);
			return;
		}
	}
}
void  Select_Render() {
   RenderSpritePos(selectstage, SCREEN_WIDTH/2- SCREEN_WIDTH/2, SCREEN_HEIGHT/2- SCREEN_HEIGHT/2);
   RenderSpritePos(selectBook, 0, 0);
   RenderSpritePos(aisheSelect, SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2);
   RenderSpritePos(selectEffect, 160, SCREEN_HEIGHT / 2-270);
   for (int i = 0; i < 3; i++) {
	   bool unlocked = false;
	   // ステージ1は常に解放
	   if (i == 0) {
		   unlocked = true;
	   }
	   // ステージ2は currentCollection が 3 で解放
	   else if (i == 1 && currentCollection >= 3) {
		   unlocked = true;
	   }
	   // ステージ3は currentCollection2 が 3 で解放
	   else if (i == 2 && currentCollection2 >= 3) {
		   unlocked = true;
	   }
	   if (unlocked) {
		   if (SelectMenuID == i) {
			   RenderSpriteRot(stage_select_on[i], SELECT_WIDTH / 2, SELECT_HEIGHT / 2, SCREEN_WIDTH / 2 + SELECT_WIDTH + 100, SCREEN_HEIGHT / 2 - SELECT_HEIGHT / 2 - 70 + 150 * i, 0.07f, 1.0f);
		   }else {
			   RenderSpriteRot(stage_select_off[i], SELECT_WIDTH / 2, SELECT_HEIGHT / 2,SCREEN_WIDTH / 2 + SELECT_WIDTH + 100,SCREEN_HEIGHT / 2 - SELECT_HEIGHT / 2 - 70 + 150 * i,0.07f, 1.0f);
		   }
	   }else {
		   // 未解放ステージ
		   RenderSpriteRot(stage_NotOpen[i], SELECT_WIDTH / 2, SELECT_HEIGHT / 2, SCREEN_WIDTH / 2 + SELECT_WIDTH + 100,SCREEN_HEIGHT / 2 - SELECT_HEIGHT / 2 - 70 + 150 * i,0.07f, 1.0f);
	   }
   }
 // 毎フレーム加算
 frame += DeltaTime;
 float ys = pen_y + sinf(frame * 2.0f) * 20.0f; // 10.0f が振幅（上下の幅）
 RenderSpritePos(pen, pen_x -5, ys - 80);
  RenderSpritePos(aisheBook[bookanim.sprite_index], 460, 570);
  //S1
  for (int i = 0; i < 3; i++) {
	  RenderSpriteRot(selectCollection[1],COLLECTION_WIDTH / 2, SELECT_HEIGHT / 2,(SCREEN_WIDTH - COLLECTION_WIDTH * 2 + 280) + 65 * i,SCREEN_HEIGHT / 2 - SELECT_HEIGHT,0.1f, 1.0f);
  }
  for (int i = 0; i < ((currentCollection >= 3) ? 3 : currentCollection); i++) {
	  RenderSpriteRot(selectCollection[0],COLLECTION_WIDTH / 2, SELECT_HEIGHT / 2,(SCREEN_WIDTH - COLLECTION_WIDTH * 2 + 280) + 65 * i,SCREEN_HEIGHT / 2 - SELECT_HEIGHT,0.1f, 1.0f);
  }
  //S2
  if (currentCollection >= 3) {
	  for (int i = 0; i < 3; i++) {
		  RenderSpriteRot(selectCollection[1],COLLECTION_WIDTH / 2, SELECT_HEIGHT,(SCREEN_WIDTH - COLLECTION_WIDTH * 2 + 270) + 65 * i,(SCREEN_HEIGHT / 2 - 40) + 144,0.1f, 1.0f);
	  }
	  for (int i = 0; i < ((currentCollection2 >= 3) ? 3 : currentCollection2); i++) {
		  RenderSpriteRot(selectCollection[0],COLLECTION_WIDTH / 2, SELECT_HEIGHT,(SCREEN_WIDTH - COLLECTION_WIDTH * 2 + 270) + 65 * i,(SCREEN_HEIGHT / 2 - 40) + 144,0.1f, 1.0f);
	  }
  }else { //未解放
	  for (int i = 0; i < 3; i++) {
		  RenderSpriteRot(selectCollectionNotOpen, COLLECTION_WIDTH / 2, SELECT_HEIGHT,(SCREEN_WIDTH - COLLECTION_WIDTH * 2 + 270) + 65 * i,(SCREEN_HEIGHT / 2 - 40) + 144,0.1f, 1.0f);
	  }
  }
}
