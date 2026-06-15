#include "UnlimitedLib/UnlimitedLib.h"
#include "Stage.h"
#include "Game.h"
#include "Enemy.h"
#include "Player.h"
#include "Item.h"
#include "MagicShard.h"
#include "FlameDemon.h"
#include "StageSelect.h"
#include "Result.h"
#include "GameSound.h"
#include <math.h>

float CameraX;//カメラのX座標
float CameraY;
int background[3];//背景ID
int tiles[6];//タイル画像ID
int stage[STAGE_HEIGHT][STAGE_WIDTH];
float Block_Size = BlockSIZE;
float snowScrollY[3];   // 雪のスクロールY位置
float snowSpeed[3] = { 0.1f, 0.2f, 0.5f }; // レイヤーごとの速度倍率
int stageEffect[2];

void STAGE_Load(const char* filename){
	//データ初期化
	for (int y = 0;y < 11;y++) {
		for (int x = 0;x < 180;x++) {
			stage[y][x] = 0;
		}
	}
	//ファイルオープン
	FILE* fp = fopen(filename, "rt"); //ReadText(文字読み込み)モードでファイル開く
	//縦11段分ループ
	for (int y = 0;y < 11;y++)
	{
		char data[256];
		fgets(data, 256, fp);//ファイルから1行Get
		//1行分解析＆格納
		for (int x = 0; x < 180; x++) {
			char d = data[x];
			// ブロックチェック
			if (d >= '0' && d <= '9') {
				int tile = d - '0';
				stage[y][x] = tile;
				continue;
			}
			//ASCIIコード　　'0' =48  '1' =49
			//'0'を0にするには
			int tile = d - '0'; //d - 48
			stage[y][x] = tile;
		}
	}
	//ファイルクローズ
	fclose(fp);
}

void STAGE_LoadAttr(const char* filename)
{
	//ファイルオープン
	FILE* fp = fopen(filename, "rt"); //ReadText(文字読み込み)モードでファイル開く
	//縦11段分ループ
	for (int y = 0;y < 11;y++)
	{
		char data[256];
		fgets(data, 256, fp);//ファイルから1行Get
		//1行分解析＆格納
		for (int x = 0; x < 180; x++) {
			char d = data[x];
			char d2 = data[x];
			char d3 = data[x];
			char d4 = data[x];
			//敵チェック
			if (d == '!') {
				ENEMY_Set(x * BlockSIZE + (BlockSIZE / 2), y * BlockSIZE + BlockSIZE ,2);
				continue;
			}
			if (d2=='?') {
				ENEMY_Set2(x * BlockSIZE + (BlockSIZE / 2), y * BlockSIZE + BlockSIZE, 2);
				continue;
			}
			if (d3 == '@') {
				Item_Set(x * BlockSIZE + (BlockSIZE / 2), y * BlockSIZE + BlockSIZE);
				continue;
			}
			if (d3 == 'M') {
				MS_Set(x * BlockSIZE + (BlockSIZE / 2), y * BlockSIZE + BlockSIZE);
				continue;
			}
			if (d4 == 'B') {
				FlameDemon_Set(x * BlockSIZE + (BlockSIZE / 2), y * BlockSIZE + BlockSIZE);
				continue;
			}
		}
	}
	//ファイルクローズ
	fclose(fp);
}

//座標(x,y)のタイル番号をゲット
//座標をタイル番号に変換するには、座標/96する
int STAGE_GetTileNo(int x, int y) {
	//左からtx番目、上からty番目
	int tx = x / BlockSIZE;
	int ty = y / BlockSIZE;
	//(tx,ty)のブロック

	 // 範囲外チェック
	if (tx < 0 || tx >= STAGE_WIDTH || ty < 0 || ty >= STAGE_HEIGHT) {
		return 0; // 画面外は空ブロック扱い
	}
	int tile = stage[ty][tx];
	return tile;
}

void STAGE_Initialize()
{
	stageEffect[0] = CreateSprite("assets/stage/Effect/snow_00.dds");
	stageEffect[1] = CreateSprite("assets/stage/Effect/sparks_00.dds");

	tiles[1] = CreateSprite("assets/stage/block/block_ground_00.dds");//画像の読み込み
	tiles[2] = CreateSprite("assets/stage/block/block_ground_02.dds");
	tiles[3] = CreateSprite("assets/stage/block/block_ground_01.dds");
	tiles[4] = CreateSprite("assets/stage/block/block_ground_04.dds");
	tiles[5] = CreateSprite("assets/stage/block/block_ground_03.dds");
	
	switch (Select)
	{
	case 1:
		STAGE_Load("assets/stage/stage01.txt");
		STAGE_LoadAttr("assets/stage/Stage01_attr.txt");
		background[0] = CreateSprite("assets/stage/BackGround/background_stage1_03.png");
		background[1] = CreateSprite("assets/stage/BackGround/background_stage1_02.png");
		background[2] = CreateSprite("assets/stage/BackGround/background_stage1_01.png");
		BGM_Play("assets/BGM/bgm_stage_00.ogg");
		break;
	case 2:
		STAGE_Load("assets/stage/stage02.txt");
		STAGE_LoadAttr("assets/stage/Stage02_attr.txt");
		background[2] = CreateSprite("assets/stage/BackGround/background_stage2_01.png");
		background[1] = CreateSprite("assets/stage/BackGround/background_stage2_02.png");
		background[0] = CreateSprite("assets/stage/BackGround/background_stage2_03.png");
		BGM_Play("assets/BGM/bgm_stage_00.ogg");
		break;
	case 3:
		STAGE_Load("assets/stage/Stage03.txt");
		STAGE_LoadAttr("assets/stage/Stage03_attr.txt");
		background[2] = -1;
		background[1] = -1;
		background[0] = CreateSprite("assets/stage/background_stage_03.dds");
		BGM_Play("assets/BGM/bgm_boss_00.ogg");
		break;
	}
	CameraX = 0.0f;
	for (int i = 0; i < 3; i++) {
		snowScrollY[i] = 0.0f;
	}
}

void STAGE_Release()
{
	ReleaseSprite(stageEffect[0]);
	ReleaseSprite(stageEffect[1]);

	ReleaseSprite(background[0]);
	ReleaseSprite(background[1]);
	ReleaseSprite(background[2]);

	ReleaseSprite(tiles[0]);
	ReleaseSprite(tiles[1]);
	ReleaseSprite(tiles[2]);
	ReleaseSprite(tiles[3]);
	ReleaseSprite(tiles[4]);
	ReleaseSprite(tiles[5]);

	for (int y = 8;y < 11;y++) {
		for (int x = 0;x < 180;x++) {
			ReleaseSprite(stage[y][x]);
		}
	}
}

void STAGE_Update(){
	if (Select != 3) {
		float LimitR = CameraX + SCREEN_WIDTH / 3;
		if (player.positionX > LimitR) {
			CameraX = player.positionX - SCREEN_WIDTH / 3;
		}
		float LimitL = CameraX + SCREEN_WIDTH / 3;
		if (player.positionX < LimitL) {
			CameraX = player.positionX - SCREEN_WIDTH / 3;
		}
		//スクロール制限
		if (CameraX < 0)CameraX = 0; //0より左には行かない
		float MaxCameraX = (STAGE_WIDTH * BlockSIZE) - SCREEN_WIDTH;
		if (CameraX > MaxCameraX) CameraX = MaxCameraX;
	}else {
		CameraX = 0;
	}

	// 雪スクロール更新（レイヤーごとに速度を変える）
	for (int i = 0; i < 3; i++) {
		snowScrollY[i] += snowSpeed[i] * DeltaTime * 50;
		if (snowScrollY[i] >= SCREEN_HEIGHT) {
			snowScrollY[i] = 0;
		}
	}
}

void STAGE_Render(){
	//背景
	float scroll_speed[3] = { 0.1f,0.2f,0.5f };
	for (int bg = 0; bg < 3; bg++) {
		float scrollX = -fmod(CameraX * scroll_speed[bg], SCREEN_WIDTH);
		for (int i = 0; i < 2; i++) {
			RenderSpritePos(background[bg], scrollX + SCREEN_WIDTH * i, 0.0f);
		}
	}
	//エフェクト
	if (Select == 1) {
		for (int layer = 0; layer < 3; layer++) {
			// 背景
			if (background[layer] != -1) {
				float scrollY = -fmod(CameraX * scroll_speed[layer], SCREEN_WIDTH);
				for (int i = 0; i < 2; i++) {
					RenderSpritePos(background[layer], scrollY + SCREEN_WIDTH * i, 0.0f);
				}
			}
			// 雪をレイヤーごとに描画（雪も奥・中・手前でスクロール速度が違う）
			for (int n = 0; n < 9; n++) {
				RenderSpritePos(stageEffect[0], SCREEN_WIDTH * n - CameraX, snowScrollY[layer]);
				RenderSpritePos(stageEffect[0], SCREEN_WIDTH * n - CameraX, snowScrollY[layer] - SCREEN_HEIGHT);
			}
		}
	}

	if (Select == 2) {
		for (int layer = 0; layer < 3; layer++) {
			// 背景
			if (background[layer] != -1) {
				float scrollY = -fmod(CameraX * scroll_speed[layer], SCREEN_WIDTH);
				for (int i = 0; i < 2; i++) {
					RenderSpritePos(background[layer], scrollY + SCREEN_WIDTH * i, 0.0f);
				}
			}
			for (int n = 0; n < 9; n++) {
				RenderSpritePos(stageEffect[1], SCREEN_WIDTH * n - CameraX, -snowScrollY[layer]);
				RenderSpritePos(stageEffect[1], SCREEN_WIDTH * n - CameraX, -snowScrollY[layer] - SCREEN_HEIGHT);
			}
		}
	}
	if (Select == 3) {
		for (int layer = 0; layer < 3; layer++) {
			RenderSpritePos(stageEffect[1], 0.0f, -snowScrollY[layer]);
			RenderSpritePos(stageEffect[1], 0.0f, -snowScrollY[layer] - SCREEN_HEIGHT);
		}
	}
	//ブロック
	for (int y = 0;y < 11;y++) {//縦に11段
		for (int x = 0;x < 180;x++) {//横に180段
			int block = stage[y][x];
			if (block != 0) {
				RenderSpritePos(tiles[block], x * 96.0f - CameraX, y * 96.0f + 24.0f);//描画	
			}
		}
	}
}
