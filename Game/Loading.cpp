#include "UnlimitedLib/UnlimitedLib.h"
#include "Loading.h"
#include"Game.h"
#include<time.h>
#include <process.h>
#define TEXT_WIDTH 668
#define TEXT_HEIGHT 120

int loadingBG;
int loadingText[4];
int loading;
int frameCount;
extern int SceneIndex;
extern int LoadingSceneIndex;
HANDLE hThread;		//スレッドハンドル
bool isThreadEnd;	//スレッド終了フラグ

///ローディングスレッド(これが並走する関数です)
unsigned int _stdcall loadingFunc(void *pParam)
{
	LoadingScene(LoadingSceneIndex, false); // シーン切り替えなしの初期化処理のみ呼び出し
	// 上の重い処理が終わったらisThreadEndフラグをtrueにする
	isThreadEnd = true;
	return 0;
}

void Loading_Initialize()
{
	loadingBG = CreateSprite("assets/GameScene/loadingBG.dds");
	loadingText[0] = CreateSprite("assets/GameScene/load_00.dds");
	loadingText[1] = CreateSprite("assets/GameScene/load_01.dds");
	loadingText[2] = CreateSprite("assets/GameScene/load_02.dds");
	loadingText[3] = CreateSprite("assets/GameScene/load_03.dds");
	isThreadEnd = false; // ローディング終了フラグ初期化
	hThread = (HANDLE)_beginthreadex(NULL, 0, loadingFunc, NULL, 0, NULL); // 並走する別のプログラム(loadingFunc)を起動
	frameCount = 0;
}

void Loading_Update()
{
	if (isThreadEnd) { // データ読み込みが終了していた場合、次のシーンに遷移
		Loading_Release();
		SceneIndex = LoadingSceneIndex;
	}
	frameCount+=2*(DeltaTime*50);
}

void Loading_Render()
{
	RenderSprite(loadingBG);
	for (int i = 0;i < 4;i++) {
		if (frameCount / 28 % 4 >= i) {
			RenderSpritePos(
				loadingText[i],
				SCREEN_WIDTH / 2 - TEXT_WIDTH / 2 + 600,
				SCREEN_HEIGHT / 2 - TEXT_HEIGHT / 2 + 450);
		}
	}
}

void Loading_Release()
{
	ReleaseSprite(loadingBG);
	ReleaseSprite(loadingText[0]);
	ReleaseSprite(loadingText[1]);
	ReleaseSprite(loadingText[2]);
	ReleaseSprite(loadingText[3]);
}
