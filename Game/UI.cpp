#include "UnlimitedLib/UnlimitedLib.h"
#include "UI.h"
#include "Player.h"
#include "MagicShard.h"
#include "Stage.h"
#include "Game.h"
#include "FlameDemon.h"
#include "math.h"
#define GAUGE_Hight 48
#define UI_Ms_W 73
#define UI_Ms_H 137


float  lifegauge;
float  lifebar;
float  lifetxt;
float  UI_LifeMax;
float  UI_Life;
float  UI_LifeCurrent;//現在のゲージ値

void UI_Initialize() {
	UI_InitializeLife();
	UI_InitializeMP();
	if (Select == 1 || Select == 2) {
		UI_MsInitialize();
	}
	else if (Select == 3)
	{
		UI_BossInitialize();
	}
}

void UI_Release() {
	UI_ReleaseLife();
	UI_ReleaseMP();
	if (Select == 1 || Select == 2) {
		UI_MsRelease();
	}
	else if (Select == 3)
	{
		UI_ReleaseBoss();
	}
}

void UI_Update() {
	UI_UpdateLife();
	UI_UpdateMP();
	if (Select == 3)
	{
		UI_UpdateBoss();
	}
}

void UI_Render() {
	UI_RenderLife();
	UI_RenderMP();
	if (Select == 1 || Select == 2) {
		UI_MsRender();
	}
	else if (Select == 3)
	{
		UI_RenderBoss();
	}
}

void UI_InitializeLife() {
	UI_LifeMax = HP_MAX;
	UI_Life = HP_MAX;
	UI_LifeCurrent = HP_MAX;
	lifegauge = CreateSprite("assets/UI/hp_gauge_base_00.png");
	lifebar = CreateSprite("assets/UI/hp_gauge_gauge_08.dds");
	lifetxt = CreateSprite("assets/UI/hp_gauge_text_00.png");
}
void UI_ReleaseLife() {
	ReleaseSprite(lifebar);
	ReleaseSprite(lifegauge);
	ReleaseSprite(lifetxt);
}
void UI_UpdateLife() {
if (UI_LifeCurrent < UI_Life) {
	UI_LifeCurrent += IncreaseRate * (DeltaTime * 50);
	if (UI_LifeCurrent > UI_Life) {
		UI_LifeCurrent = UI_Life;
	}
}
if (UI_LifeCurrent > UI_Life) {
	UI_LifeCurrent -= IncreaseRate * (DeltaTime * 50);
	if (UI_LifeCurrent < UI_Life) {
		UI_LifeCurrent = UI_Life;
	}
}
}
void UI_RenderLife() {
	float liferate = UI_LifeCurrent / UI_LifeMax;
	//ゲージ幅調整
	SetSpriteSize(lifebar, GetSpriteTextureWidth(lifebar) * liferate, GetSpriteTextureHeight(lifebar));
	//画像のどの部分を使うか
	SetSpriteUV(lifebar, 0, 0, GetSpriteTextureWidth(lifebar) * liferate, GetSpriteTextureHeight(lifebar));

	RenderSpritePos(lifegauge, 40, 40);
	RenderSpritePos(lifebar, 40, 40);
	RenderSpritePos(lifetxt, 40,  40);
}
void UI_SetLife(int now, int max) {
	UI_LifeMax = max;
	UI_Life = now;
}
float  mpgauge;
float  mpbar;
float  mptxt;
float  UI_MpMax;
float  UI_Mp;
float  UI_MpCurrent;//現在のゲージ値
void UI_InitializeMP() {
	UI_MpMax = MP_MAX;
	UI_Mp = MP_MAX;
	UI_MpCurrent = MP_MAX;
	mpgauge = CreateSprite("assets/UI/mp_gauge_base_00.png");
	mpbar=CreateSprite("assets/UI/mp_gauge_gauge_08.png");
	mptxt= CreateSprite("assets/UI/mp_gauge_text_00.png");

}
void UI_ReleaseMP() {
	ReleaseSprite(mpgauge);
	ReleaseSprite(mpbar);
	ReleaseSprite(mptxt);
}
void UI_UpdateMP() {
	if (UI_MpCurrent < UI_Mp) {
		UI_MpCurrent += IncreaseRate*(DeltaTime*50);
		if (UI_MpCurrent > UI_Mp) {
			UI_MpCurrent = UI_Mp;
		}
	}
	if (UI_MpCurrent > UI_Mp) {
		UI_MpCurrent -= DecreaseRate * (DeltaTime * 50);
		if (UI_MpCurrent < UI_Mp) {
			UI_MpCurrent = UI_Mp;
		}
	}
}
void UI_RenderMP() {
	float mprate = UI_MpCurrent / UI_MpMax;//x/8
	//ゲージ幅調整
	SetSpriteSize(mpbar, GetSpriteTextureWidth(mpbar) * mprate, GetSpriteTextureHeight(mpbar));
	//画像のどの部分を使うか
	SetSpriteUV(mpbar, 0, 0, GetSpriteTextureWidth(mpbar) * mprate, GetSpriteTextureHeight(mpbar));

	RenderSpritePos(mpgauge, 40, 40 +GAUGE_Hight+10);
	RenderSpritePos(mpbar, 40 , 40 +GAUGE_Hight+10);
	RenderSpritePos(mptxt, 40, 40 +GAUGE_Hight+10);

}
void UI_SetMP(int now, int max) {
	UI_MpMax = max;
	UI_Mp = now;
}

int UI_Ms[3];
void UI_MsInitialize() {
	UI_Ms[0] = CreateSprite("assets/GameScene/Result/collection_00.dds");
	SetSpriteSize(UI_Ms[0], UI_Ms_W, UI_Ms_H);
	UI_Ms[1] = CreateSprite("assets/GameScene/Result/collection_01.dds");
	SetSpriteSize(UI_Ms[1], UI_Ms_W, UI_Ms_H);
	get_ms_count = 0;
	get_ms_count2 = 0;
}
void UI_MsRelease() {
	ReleaseSprite(UI_Ms[0]);
	ReleaseSprite(UI_Ms[1]);
}
void UI_MsUpdate() {

}
void UI_MsRender() {
	for (int i = 0;i < 3;i++) {
		RenderSpritePos(UI_Ms[1], 40+UI_Ms_W*i, 50 + GAUGE_Hight+UI_Ms_H/2);
	}
	switch (Select) {
    	case 1:	if (get_ms_count > 0) {
    		for (int i = 0;i < get_ms_count;i++) {
    			RenderSpritePos(UI_Ms[0], 40 + UI_Ms_W * i, 50 + GAUGE_Hight + UI_Ms_H / 2);
    		}
    	};break;
    	case 2:if (get_ms_count2 > 0) {
    		for (int i = 0;i < get_ms_count2;i++) {
    			RenderSpritePos(UI_Ms[0], 40 + UI_Ms_W * i, 50 + GAUGE_Hight + UI_Ms_H / 2);
    		}
    	};break;
	}
}


float  bossgauge;
float  bossbar;
float  bosstxt;
float  UI_bossMax;
float  UI_bossLife;
float  UI_bossCurrent;//現在のゲージ値

void UI_BossInitialize() {
	UI_bossMax= DEMON_HP;
	UI_bossLife= DEMON_HP;
	UI_bossCurrent = DEMON_HP;
	bossgauge = CreateSprite("assets/UI/mp_gauge_base_00.png");
	bossbar = CreateSprite("assets/UI/bossUI.png");
	SetSpriteSize(bossbar, 700, 48);
	bosstxt = CreateSprite("assets/UI/mp_gauge_text_00.png");
}

void UI_ReleaseBoss() {
	ReleaseSprite(bossgauge);
	ReleaseSprite(bossbar);
	ReleaseSprite(bosstxt);
}

void UI_SetLifeBoss(int now, int max) {
	UI_bossMax = max;
	UI_bossLife = now;
}

void UI_UpdateBoss() {
	if (UI_bossCurrent < UI_bossLife) {
		UI_bossCurrent += IncreaseRate * (DeltaTime * 50);
		if (UI_bossCurrent > UI_bossLife) {
			UI_bossCurrent = UI_bossLife;
		}
	}
	if (UI_bossCurrent > UI_bossLife) {
		UI_bossCurrent -= DecreaseRate * (DeltaTime * 50);
		if (UI_bossCurrent < UI_bossLife) {
			UI_bossCurrent = UI_bossLife;
		}
	}
}
void UI_RenderBoss() {
	float bossrate = UI_bossCurrent / UI_bossMax;//x/bosshp
	if (fabsf(UI_bossCurrent - UI_bossLife) < 1.0f) {
		UI_bossCurrent = UI_bossLife;
	}
	//ゲージ幅調整
	SetSpriteSize(bossbar, GetSpriteTextureWidth(bossbar) * bossrate, GetSpriteTextureHeight(bossbar));
	//画像のどの部分を使うか
	SetSpriteUV(bossbar, 0, 0, GetSpriteTextureWidth(bossbar) * bossrate, GetSpriteTextureHeight(bossbar));

	RenderSpritePos(bossbar, SCREEN_WIDTH/2-UI_Ms_W*4, SCREEN_HEIGHT- UI_Ms_H*1.2);
}

