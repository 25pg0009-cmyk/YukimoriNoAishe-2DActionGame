#include "UnlimitedLib/UnlimitedLib.h"
#include "IceSpear.h"
#include "Player.h"
#include "Stage.h"
#include "Game.h"
#include "hitblock.h"
#include "Animation.h"
#include "FlameDemon.h"
#include "flameimp.h"
#include "flameslime.h"
#include "MainGame.h"

Player icespear[BULLET_MAX];
float IceSpear_height = BULLET_HEIGHT;
float IceSpear_radius = BULLET_WIDTH;
int icespearhandle[6];
bool hitblockspear[BULLET_MAX];
int iceSpearEffectHandles[10];
int bullet_effectsize;
//アイススピアアニメーション
AnimationInfo  icespear_animptn{
	{
		/*00弾*/{12.0f,{0,1,2,3,4,5,-1}},
	}
};

//ヒットエフェクトアニメーション
#define H_BULLET_EFFECT_SIZE 200 //hitsize
#define B_BULLET_EFFECT_SIZE 192 //breaksize

struct IceSpearEffect {
	bool isAlived;
	float x, y;
	bool Dir;
	AnimationUniqueInfo iceSpearHitAnim;
};
IceSpearEffect icespeareffect[BULLET_MAX];

AnimationInfo iceSpearHitAnimptn{
	{
		/*00ヒットエフェクト*/{25.0f,{0,1,2,3,-2}},
		/*01壊れるエフェクト*/{15.0f,{5,6,7,8,-2}},
	}
};
enum {
	STATE_HITSPEAR,
	STATE_BREAKSPEAR,
};
int iceSpearEffectState[BULLET_MAX];

void SetIceEffect(int n, float x, float y,bool Dir);

void IceSpear_Initialize() {
	
	//弾モーション
	CreateSpriteMatrix("assets/player/icespear_00.png",
		BULLET_WIDTH,BULLET_HEIGHT,
		6,1,6,
		icespearhandle);
	//エフェクトモーション
	CreateSpriteMatrix("assets/player/icespear_hit_00.png",
		H_BULLET_EFFECT_SIZE, H_BULLET_EFFECT_SIZE,
		4, 1, 4,
		iceSpearEffectHandles);
	CreateSpriteMatrix("assets/player/icespear_break_00.png",
		B_BULLET_EFFECT_SIZE, B_BULLET_EFFECT_SIZE,
		4, 1, 4,
		&iceSpearEffectHandles[5]);
	
	//弾 BULLET_WIDTH;の初期化
	for (int i = 0;i < BULLET_MAX;i++) {
		icespear[i].isAlived = false;//現在弾死んでる
		icespeareffect[i].isAlived = false;
		hitblockspear[i] = false;
		
	}
}

void IceSpear_Release(){
	ReleaseSpriteMatrix(icespearhandle,6);
	ReleaseSpriteMatrix(iceSpearEffectHandles, 10);
}

//全ての弾を更新
void IceSpear_Update() {
	for (int i = 0;i < BULLET_MAX;i++) {
		//生存している弾のみ更新する
		if (icespear[i].isAlived) {
			Animation(DeltaTime, icespear[i].pl_anim_info, icespear_animptn);

			if (icespear[i].Dir) {
				icespear[i].positionX += icespear[i].speed * (DeltaTime * 50);
			}
			else {
				icespear[i].positionX -= icespear[i].speed * (DeltaTime * 50);
			}
			//I_Frame経過で弾削除
			icespear[i].count+=DeltaTime;
			if (icespear[i].count > I_Frame) {
				icespear[i].isAlived = false;
				iceSpearEffectState[i] = STATE_BREAKSPEAR;
				bullet_effectsize = B_BULLET_EFFECT_SIZE;
				SetIceEffect(i, icespear[i].positionX, icespear[i].positionY, icespear[i].Dir);
				continue;
			}
			hitblockspear[i] = hitBlockSpear(i);
		}
		//アイススピアが当たったら
		if (hitblockspear[i]|| hiticespear_imp|| hiticespear_slime|| hiticespear_demon) {
			icespear[i].isAlived = false;
			iceSpearEffectState[i] = STATE_HITSPEAR;
			bullet_effectsize = H_BULLET_EFFECT_SIZE;
			SetIceEffect(i, icespear[i].positionX, icespear[i].positionY,icespear[i].Dir);
			hitblockspear[i] = false;
			hiticespear_imp = false;
			hiticespear_slime = false;
			hiticespear_demon = false;
		}
		// エフェクト更新
		if (icespeareffect[i].isAlived) {
			ChangeAnimation(iceSpearEffectState[i], icespeareffect[i].iceSpearHitAnim, iceSpearHitAnimptn);
			Animation(DeltaTime, icespeareffect[i].iceSpearHitAnim, iceSpearHitAnimptn);
			if (!icespeareffect[i].iceSpearHitAnim.isPlaying) {
				icespeareffect[i].isAlived = false;
			}
		}
	}
}

void Fire(float x, float y, float speed,bool Dir) {
	int n = -1;
	//使われていない弾を探し出す
	for (int i = 0;i < BULLET_MAX;i++) {
		if (icespear[i].isAlived == false) {//i番目が生存しているか確認する
			//生存していない場合はｎに空いてる番号を代入する
			n = i;
			break;
		}
	}
	//空きがなければ終了
	if (n == -1) {
		return;
	}
	//弾の初期化
	SetSpriteSize(icespear[n].handle, BULLET_WIDTH, BULLET_HEIGHT);
	icespear[n].Dir = Dir;
	if (icespear[n].Dir) {
		icespear[n].positionX = x + 90;
	}
	else {
		icespear[n].positionX = x - 90 - PLAYER_WIDTH;
	}
	icespear[n].positionY = y - PLAYER_HEIGHT / 2;
	icespear[n].speed = speed;
	icespear[n].isAlived = true;
	icespear[n].count = 0;
}

void SetIceEffect(int n, float x, float y, bool Dir) {

	icespeareffect[n].x = x;
	icespeareffect[n].y = y;
	icespeareffect[n].Dir = Dir;
	//アニメーション初期化
	icespeareffect[n].iceSpearHitAnim.isPlaying = true;
	icespeareffect[n].isAlived = true;
	icespeareffect[n].iceSpearHitAnim.frame = 0;
	icespeareffect[n].iceSpearHitAnim.ID = 0;
	icespeareffect[n].iceSpearHitAnim.sprite_index = iceSpearHitAnimptn.param[0].ptn[(int)icespeareffect[n].iceSpearHitAnim.frame];
}

//弾の当たり判定
bool hitBlockSpear(int n) {
	if (icespear[n].Dir) {
		//右当たり判定
		//右ブロックチェック
		int RightX = icespear[n].positionX + IceSpear_radius;
		int tileR = STAGE_GetTileNo(RightX, icespear[n].positionY-1 );
		//頭上の右もチェック
		int tileHeadR = STAGE_GetTileNo(RightX, icespear[n].positionY - IceSpear_height / 2);
		//中央のブロックもチェック
		int tileBodyR = STAGE_GetTileNo(RightX, icespear[n].positionY - IceSpear_height/2);
		if (tileR > 0 || tileHeadR > 0 || tileBodyR > 0) {
			return true;
		}
		return false;
	}else {
		//左当たり判定
		//左ブロックチェック
		int LeftX = icespear[n].positionX - IceSpear_radius;
		int tileL = STAGE_GetTileNo(LeftX, icespear[n].positionY-1 );
		//頭上の左もチェック
		int tileHeadL = STAGE_GetTileNo(LeftX, icespear[n].positionY - IceSpear_height / 2);
		//中央の左もチェック
		int tileBodyL = STAGE_GetTileNo(LeftX, icespear[n].positionY - IceSpear_height/2);
		if (tileL > 0 || tileHeadL > 0 || tileBodyL > 0) {
			return true;
		}
		return false;
	}
}

void IceSpear_Render() {
	for (int i = 0; i < BULLET_MAX; i++) {
		if (icespear[i].isAlived) {
			//弾の移動
			if (icespear[i].Dir) {//右
				//弾の表示
				RenderSpritePos(icespearhandle[icespear[i].pl_anim_info.sprite_index], icespear[i].positionX - CameraX, icespear[i].positionY );
			}
			else {//左
				//弾の表示
				RenderSpritePosLR(icespearhandle[icespear[i].pl_anim_info.sprite_index], (icespear[i].positionX ) - CameraX, icespear[i].positionY );
			}
		}
		if (icespeareffect[i].isAlived) {
			if (icespeareffect[i].Dir) {
				RenderSpritePos(iceSpearEffectHandles[icespeareffect[i].iceSpearHitAnim.sprite_index],
					icespeareffect[i].x - CameraX, icespeareffect[i].y-50);
			}
			else {
				RenderSpritePosLR(iceSpearEffectHandles[icespeareffect[i].iceSpearHitAnim.sprite_index],
					icespeareffect[i].x  - CameraX, icespeareffect[i].y-50);
			}
		}
	}

}
