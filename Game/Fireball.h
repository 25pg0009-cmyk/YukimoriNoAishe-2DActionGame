#pragma once
#define FIRE_BALL_MAX 100
#define FIRE_ACTIVE_FRAME 3
//ファイアボールパラメータ
struct FireParam {
	bool IsActive;
	float Dir;
	bool viewing_dir;
	float x;
	float y;
	float atk;
	float speed;
	float count;
	bool IsHoming;
	int owner;
};
extern struct FireParam fireParam[FIRE_BALL_MAX];

void Fire2(float* x, float* y, float* speed, float* Dir);
void FIRE_Initialize();//初期化
void FIRE_Release();//解放
void FIRE_Update();//更新
void FIRE_Render();//描画
void SetFireHitEffect(int n, float x, float y);

bool CircleVsRect(float cX, float cY, float cR,
	float rX, float rY, float rW, float rH);
float Clamp(float value, float min, float max);
