
#pragma once
typedef struct {
	float x;
	float y;
	float radius;
	float height;
} Collider;
extern Collider playerCollider;

//ステージとキャラクターの当たり判定
void CheckBlock(float* charX, float* charY, float* VelocityY, float* charWidth, float* charHeight, float* speed,
	float* BlockX, float* BlockY, float* BlockSize, bool* IsGround);
void HitIceBlock(float* MagicX, float* MagicY, float* MagicWidth, float* MagicHeight, bool* IsAlived,
	float* BlockX, float* BlockY, float* BlockSize);
bool HitBullet(float* CharX1, float* CharY1, float* CharWidth1, float* CharHeight1,
	float* CharX2, float* CharY2, float* CharWidth2, float* CharHeight2, float* Life, float* CharAtk, bool* isAlived);
bool HitPlayer(float* CharX1, float* CharY1, float* CharWidth1, float* CharHeight1,
	float* CharX2, float* CharY2, float* CharWidth2, float* CharHeight2, float* Life, float* CharAtk);
bool HitObject(float* CharX, float* CharY, float* CharWidth, float* CharHeight,
	float* ItemX, float* ItemY, float* ItemWidth, float* ItemHeight,bool* itemIsAlived);
