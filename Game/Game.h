#pragma once

#define SCREEN_WIDTH	1920
#define SCREEN_HEIGHT 1080
#define SCREEN_REMAINDER 24   //Y画面の余りブロック数

extern float  DeltaTime;

void Initialize();
void Finalize();
void GameMain();

// シーンID
#define SCENE_TITLE 1
#define SCENE_STAGESELECT 2
#define SCENE_MAINGAME 3
#define SCENE_RESULT 4
#define SCENE_GAMEOVER 5
#define SCENE_LOADING 6
void InitializeScene(int scene);
void LoadingScene(int scene, bool loading);

void SetFade(bool fadein, float duration);
void SetFadeColor(float r, float g, float b);

bool IsFadeFinished();
void SetFadeIn(float r, float g, float b, float duration);
void SetFadeOut(float r, float g, float b, float duration);
