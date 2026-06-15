#include "UnlimitedLib/UnlimitedLib.h"
#include "Game.h"
#include "hitblock.h"
#include "Stage.h"
#include "IceBlock.h"
//ブロック判定
void CheckBlock(float* charX, float* charY, float* VelocityY, float* charWidth, float* charHeight,float* speed,
	float* BlockX, float* BlockY, float* BlockSize, bool* IsGround) {

	float  CharTop = *charY - *charHeight;
	float  CharBottom = *charY;
	float  CharRight = *charX + *charWidth / 2;
	float  CharLeft = *charX - *charWidth / 2;

	float BlockTop = *BlockY * (*BlockSize);
	float BlockBottom = BlockTop + *BlockSize;
	float BlockLeft = *BlockX * (*BlockSize);
	float BlockRight = BlockLeft + *BlockSize;

	float wallheightSize = BlockSIZE / 2;
	float wallwidthSize = BlockSIZE / 3;
//左右の当たり判定
	if (CharBottom >= BlockTop + wallheightSize && CharTop <= BlockBottom - wallheightSize) {

		//ブロックの右壁の当たり判定(座標補正かえる
		if (CharRight >= BlockLeft-1 && CharRight <= BlockLeft + wallwidthSize) {
			*speed = 0.0f;
			*charX = BlockLeft - *charWidth / 2 - 1;
			CharRight = *charX - *charWidth / 2;
			CharLeft = *charX + *charWidth / 2;

		}
		//ブロックの左壁の当たり判定
		if (CharLeft >= BlockRight - wallwidthSize && CharLeft <= BlockRight+1) {
			*speed = 0.0f;
			*charX = BlockRight + *charWidth / 2+1;
			CharRight = *charX - *charWidth / 2;
			CharLeft = *charX + *charWidth / 2;
		}
	}

//天井と床の当たり判定
	if (CharRight >= BlockLeft+1 && CharLeft <= BlockRight-1) {
		//床
		if (CharBottom >= BlockTop-1 && CharBottom <= BlockTop + wallheightSize) {
			*charY = BlockTop-1;
			*VelocityY = 0;
			CharTop = *charY - *charHeight;
			CharBottom = *charY;
			*IsGround = true;
		}
		//天井
		if (CharTop >= BlockBottom - wallheightSize && CharTop <= BlockBottom) {
 			*charY = BlockBottom + *charHeight+1;
			*VelocityY = 0;
			CharTop = *charY - *charHeight;
			CharBottom = *charY;
		}
	}

}
//アイスブロックの当たり判定（ステージのブロックまたはアイスブロック同士）
void HitIceBlock(float* MagicX, float* MagicY, float* MagicWidth, float* MagicHeight,bool* IsAlived,
	float* BlockX, float* BlockY, float* BlockSize) {

	//中心座標
	float MagicLeft = *MagicX - *MagicWidth / 2;
	float MagicRight = *MagicX + *MagicWidth / 2;
	float MagicTop = *MagicY - *MagicHeight/2;
	float MagicBottom = *MagicY+ *MagicHeight / 2;

	//中心座標
	float BlockLeft = *BlockX - *BlockSize/2;
	float BlockRight = *BlockX + *BlockSize/2;
	float BlockTop = *BlockY - *BlockSize/2;
	float BlockBottom = *BlockY + *BlockSize/2;


	// AABB判定
	if (MagicRight >= BlockLeft && MagicLeft <= BlockRight &&
		MagicBottom >= BlockTop && MagicTop <= BlockBottom) {
		*IsAlived = false;
	}
	if (MagicRight >= BlockLeft-1 && MagicLeft <= BlockRight &&
		MagicBottom >= BlockTop && MagicTop <= BlockBottom) {
		*IsAlived = false;
	}
} 

//プレイヤーの弾（アイススピア）と敵の当たり判定または敵の弾（ファイアボール）とプレイヤーの当たり判定
bool HitBullet(float* BulletX, float* BulletY,float* BulletWidth,float* BulletHeight,
			  float* CharX, float* CharY, float* CharWidth, float* CharHeight,float* Life,float*CharAtk,bool* isAlived) {
	//中心座標
	float BulletLeft = *BulletX ;
	float BulletRight = *BulletX + *BulletWidth ;
	float BulletTop = *BulletY ;
	float BulletBottom= *BulletY + *BulletHeight ;

	//中心座標
	float CharLeft = *CharX ;
	float CharRight = *CharX + *CharWidth ;
	float CharTop = *CharY;
	float CharBottom= *CharY + *CharHeight;


	// AABB判定
	if (BulletRight >= CharLeft+1 && BulletLeft <= CharRight-1 &&
		BulletBottom >= CharTop+1 && BulletTop <= CharBottom-1) {

		*Life = *Life - *CharAtk;
		*isAlived = false;

		return true;
	}
	return false;
}

bool HitPlayer(float* CharX1, float* CharY1, float* CharWidth1, float* CharHeight1,
	float* CharX2, float* CharY2, float* CharWidth2, float* CharHeight2, float* Life, float* CharAtk) {

	float CharLeft1 = *CharX1;
	float CharRight1 = *CharX1 + *CharWidth1;
	float CharTop1 = *CharY1;
	float CharBottom1 = *CharY1 + *CharHeight1;

	float CharLeft2 = *CharX2;
	float CharRight2 = *CharX2 + *CharWidth2;
	float CharTop2 = *CharY2;
	float CharBottom2 = *CharY2 + *CharHeight2;


	// AABB判定
	if (CharRight1 >= CharLeft2 && CharLeft1 <= CharRight2 &&
		CharBottom1 >= CharTop2 && CharTop1 <= CharBottom2) {

		*Life = *Life - *CharAtk;

		return true;
	}
	return false;
}

bool HitObject(float* CharX, float* CharY, float* CharWidth, float* CharHeight,
	float* ObjectX, float* ObjectY, float* ObjectWidth, float* ObjectHeight,bool* ObjectIsAlived) {

	float CharLeft = *CharX ;
	float CharRight = *CharX + *CharWidth ;
	float CharTop = *CharY;
	float CharBottom = *CharY + *CharHeight;

	float ObjectLeft = *ObjectX ;
	float ObjectRight = *ObjectX + *ObjectWidth ;
	float ObjectTop = *ObjectY ;
	float ObjectBottom = *ObjectY + *ObjectHeight;

	if (CharRight > ObjectLeft && CharLeft < ObjectRight &&
		CharBottom > ObjectTop && CharTop < ObjectBottom) {
		*ObjectIsAlived = false;
		return true;
	}
	return false;
}
