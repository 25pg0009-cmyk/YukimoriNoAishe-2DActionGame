#pragma once

void UI_Initialize();
void UI_Release();
void UI_Update();
void UI_Render();

void UI_InitializeLife();
void UI_ReleaseLife();
void UI_UpdateLife();
void UI_RenderLife();
void UI_SetLife(int now, int max);
void UI_InitializeMP();
void UI_ReleaseMP();
void UI_UpdateMP();
void UI_RenderMP();
void UI_SetMP(int now, int max);
void UI_MsInitialize();
void UI_MsRelease();
void UI_MsUpdate();
void UI_MsRender();


void UI_BossInitialize();
void UI_ReleaseBoss();
void UI_UpdateBoss();
void UI_RenderBoss();
void UI_SetLifeBoss(int now, int max);

#define DecreaseRate  1  //ÉQÅ[ÉWÇÃå∏è≠ó¶
#define IncreaseRate  0.2  //ëùâ¡ó¶


