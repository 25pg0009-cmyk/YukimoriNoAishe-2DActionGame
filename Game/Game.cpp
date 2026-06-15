#include "MainGame.h"
#include "StageSelect.h"
#include "GameOver.h"
#include "Game.h"
#include "Title.h"
#include "Result.h"
#include "GameSound.h"
#include "Loading.h"

//現在のシーン 
int SceneIndex;
//シーン初期化
int LoadingSceneIndex;
// フレームの経過時間
float DeltaTime;
//経過時間計測用
LONGLONG count_freq;//カウンタ周波数
LONGLONG count_before;//前回のカウンタ

//フェード関連
float FadeRate;
float FadeDuration; //フェード秒数
bool FadeIn; //true:FadeIn  false:FadeOut
float FadeR, FadeG, FadeB;

void SetFade(bool fadein, float duration)
{
	FadeIn = fadein;
	FadeDuration = duration;
	FadeRate = 0.0f;
}

void SetFadeColor(float r, float g, float b)
{
	FadeR = r;
	FadeG = g;
	FadeB = b;
}

bool IsFadeFinished() { return (FadeRate >= 1.0); }

void SetFadeIn(float r, float g, float b, float duration)
{
	SetFadeColor(r, g, b);
	SetFade(true, duration);
}

void SetFadeOut(float r, float g,float b, float duration) {
	SetFadeColor(r, g, b);
	SetFade(false, duration);
}

//シーン初期化(ローディングなしでシーンを切り替え)
void InitializeScene(int scene)
{
	SceneIndex = scene;
	switch (SceneIndex) {
	case SCENE_TITLE:TITLE_Initialize(); break;
	case SCENE_STAGESELECT:Select_Initialize(); break;
	case SCENE_MAINGAME: MAIN_Initialize(); break;
	case SCENE_RESULT:Result_Initialize(); break;
	case SCENE_LOADING:Loading_Initialize();break;
	}
}

//シーン初期化(loading=trueでローディング画面を経由する loading=falseは内部で使用しているので指定禁止)
void LoadingScene(int scene, bool loading)
{
	if (loading) { // 重いシーン初期化はローディングをはさむ
		LoadingSceneIndex = scene;
		scene = SceneIndex = SCENE_LOADING;
	}
	switch (scene) {
	case SCENE_TITLE:TITLE_Initialize(); break;
	case SCENE_STAGESELECT:Select_Initialize(); break;
	case SCENE_MAINGAME: MAIN_Initialize(); break;
	case SCENE_RESULT:Result_Initialize(); break;
	case SCENE_LOADING:Loading_Initialize(); break;
	}
}

// 初期化
void Initialize()
{
	SOUND_Initialize();
	// 初期シーン
	InitializeScene(SCENE_TITLE);
	// 時間計測初期化
	DeltaTime = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&count_freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&count_before);
}

// 終了処理
void Finalize()
{
	SOUND_Release();
	//解放
	switch (SceneIndex) {
	case SCENE_TITLE:TITLE_Release(); break;
	case SCENE_STAGESELECT:Select_Release(); break;
	case SCENE_MAINGAME: MAIN_Release(); break;
	case SCENE_RESULT:Result_Release(); break;
	case SCENE_LOADING:Loading_Release();break;
	}
}

// メインループ
void GameMain()
{
	//時間計測
	LONGLONG now;
	QueryPerformanceCounter((LARGE_INTEGER*)&now);
	//経過カウント
	LONGLONG deltaCaunt = now - count_before;
	count_before = now;
	//経過時間
	DeltaTime = (float)((double)deltaCaunt / (double)count_freq);
	//フェード処理
	FadeRate += DeltaTime / FadeDuration; //FadeDuration秒で1.0に
	if (FadeRate > 1.0f) {
		FadeRate = 1.0f;//1.0fで止める
	}

	// 更新
	switch (SceneIndex) {
	case SCENE_TITLE:TITLE_Update(); break;
	case SCENE_STAGESELECT:Select_Update(); break;
	case SCENE_MAINGAME: MAIN_Update(); break;
	case SCENE_RESULT:Result_Update(); break;
	case SCENE_LOADING:Loading_Update();break;
	}
	
	// 描画
	switch (SceneIndex) {
	case SCENE_TITLE:TITLE_Render(); break;
	case SCENE_STAGESELECT:Select_Render(); break;
	case SCENE_MAINGAME: MAIN_Render(); break;
	case SCENE_RESULT:Result_Render(); break;
	case SCENE_LOADING:Loading_Render();break;
	}
	// フェード
	int alpha = (int)((1.0f - FadeRate) * 255);
	if (FadeIn == true) alpha = 255 - alpha;
	int r = (int)(FadeR * 255);
	int g = (int)(FadeG * 255);
	int b = (int)(FadeB * 255);
	RenderPoly(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_RGBA(r, g, b, alpha));
}
