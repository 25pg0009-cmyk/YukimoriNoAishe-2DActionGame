#include "UnlimitedLib/UnlimitedLib.h"
#include "Animation.h"
#include "GameSound.h"
#include "MainGame.h"
#include "Player.h"
#include "Game.h"
#include "Fireball.h"

#include "Enemy.h"
#include "flameimp.h"


//アニメーションイベント
void AnimationEvent(int event)
{
	switch (event) {
	case 0: break;
	case 1: SE_Play(0);	break; 
	case 2: SE_Play(1);	break; 
	case 3: SE_Play(2);	break;
	case 4: SE_Play(3);	break;
	//プレイヤーボイス
	case 5: SE_Play(4);	break; 
	case 6: SE_Play(5);	break; 
	case 7: SE_Play(6);	break; 
	case 8: SE_Play(7);	break; 
	case 9: SE_Play(8);	break; 
	case 10: SE_Play(9);break; 
	case 11: SE_Play(10);	break; 
	case 12: SE_Play(11);	break;
    //ボスボイス
	case 13: SE_Play(12);	break;
	case 14: SE_Play(13);break;
	case 15: SE_Play(14);	break;
	case 16: SE_Play(15);	break;

	case 17:	for (int i = 0;i < ENEMY_MAX;i++) {
		if (!enemyParam[i].IsActive) continue;
		if (enemyParam[i].hp <= 0) continue;
		if (!enemyParam[i].fireAttack) continue;
	
	    }break;
	}
}

//アニメーション切り替え
void ChangeAnimation(int id, AnimationUniqueInfo& info, AnimationInfo& anim)
{
	//違うアニメーションに切り替える場合のみ初期化
	if (id != info.ID) {
		info.ID = id;
		info.frame = 0;//最初から
		info.sprite_index = anim.param[id].ptn[(int)info.frame];
		info.isPlaying = true;//再生中
	}
}

void Animation(float deltatime, AnimationUniqueInfo& info, AnimationInfo& anim)
{
	//更新前のフレーム
	int frame_old = (int)info.frame;
	float speed = anim.param[info.ID].speed;//現在のアニメ速度
	info.frame += speed * deltatime;

	for (int i = frame_old; i < (int)info.frame; i++) {
		//パターン決定
		info.sprite_index = anim.param[info.ID].ptn[i];

		//ループ
		if (info.sprite_index == -1) {
			info.frame = 1.0f;
			info.sprite_index = anim.param[info.ID].ptn[0];//最初のコマ
			break;
		}
		//停止
		if (info.sprite_index == -2) {
			info.frame -= 1.0f;//一コマ戻す->最後のコマ
			//info.sprite_index = anim.param[info.ID].ptn[(int)info.frame];//最後のコマ
			info.sprite_index = anim.param[info.ID].ptn[i-1];//最後のコマ
			info.isPlaying = false;//アニメ終了
			break;
		}
	}
	//更新後のフレーム
	int frame_current = (int)info.frame;
	//frameが変わった瞬間のみアニメーションイベント処理
	if (frame_current != frame_old) {
		AnimationEvent(anim.param[info.ID].event[(int)info.frame]);
	}
}
