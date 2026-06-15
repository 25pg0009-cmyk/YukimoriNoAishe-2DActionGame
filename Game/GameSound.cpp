#include "UnlimitedLib/UnlimitedLib.h"
#include "GameSound.h"
#include"Game.h"
#include"StageSelect.h"
int BGMid;
int SEid[256]; //効果音ID
int SelectSE[2];
//SE管理構造体
struct SE {
	int num; //最大発音数
	int current; // 現在の再生位置
	int seid[10];
};
SE seParam[256];

//ボリューム設定
float BGMVolume = 1.0f;
void SetBGMVolume(float vol) {
	BGMVolume = vol;
}

float SEVolume = 1.0f;
void SetSEVolume(float vol) {
	SEVolume = vol;
}

void SE_Play(int id)
{
	//id番目の再生
   PlaySE(SEid[id],false, SEVolume);
}

void SOUND_Initialize()
{
	BGMid = -1;
	for (int i = 0; i < 256; i++) {
		seParam[i].num = 1;
		seParam[i].current = 0;
		for (int j = 0; j < 10; j++) {
			seParam[i].seid[j] = -1;
		}
	}

	SEid[0] = CreateSEOgg("assets/SE/se_boss_fireball_00.ogg");
	SEid[1] = CreateSEOgg("assets/SE/se_boss_tackle_00.ogg");
	SEid[2] = CreateSEOgg("assets/SE/se_enemy2_move_00.ogg");
	SEid[3] = CreateSEOgg("assets/SE/se_player_move_00.ogg");
	//プレイヤーボイス
	SEid[4] = CreateSEOgg("assets/SE/Voice/Player/player_voice_atack_00.ogg");
	SEid[5] = CreateSEOgg("assets/SE/Voice/Player/player_voice_clear_00.ogg");
	SEid[6] = CreateSEOgg("assets/SE/Voice/Player/player_voice_damage_00.ogg");
	SEid[7] = CreateSEOgg("assets/SE/Voice/Player/player_voice_dead_00.ogg");
	SEid[8] = CreateSEOgg("assets/SE/Voice/Player/player_voice_iceblock_00.ogg");
	SEid[9] = CreateSEOgg("assets/SE/Voice/Player/player_voice_iceblock_01.ogg");
	SEid[10] = CreateSEOgg("assets/SE/Voice/Player/player_voice_jump_00.ogg");
	SEid[11] = CreateSEOgg("assets/SE/Voice/Player/player_voice_jump_01.ogg");
	//ボスボイス
	SEid[12] = CreateSEOgg("assets/SE/Voice/Boss/boss_voice_charge_00.ogg");
	SEid[13] = CreateSEOgg("assets/SE/Voice/Boss/boss_voice_dead_00.ogg");
	SEid[14] = CreateSEOgg("assets/SE/Voice/Boss/boss_voice_fireball_00.ogg");
	SEid[15] = CreateSEOgg("assets/SE/Voice/Boss/boss_voice_tackle_00.ogg");
}

void SOUND_Release() {
	for (int i = 0; i <= 15; i++) {
		if (SEid[i] >= 0) {
			ReleaseSE(SEid[i]);
			SEid[i] = -1;
		}
	}	
}

void BGM_Stop() {
	if (BGMid >= 0) {
		StopSE(BGMid);
		ReleaseSE(BGMid);
		BGMid = -1;
	}
}

void BGM_Play(const char* bgmname) {
	BGM_Stop();
	// 再生
	BGMid = CreateSEOgg(bgmname);
	PlaySE(BGMid, true, BGMVolume); //ループ再生
}
