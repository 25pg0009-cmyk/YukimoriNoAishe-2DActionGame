#pragma once
void SOUND_Initialize();//èâä˙âª
void SOUND_Release();
void BGM_Stop();
void BGM_Play(const char* bgmname);
extern int SelectSE[2];
void SE_Play(int n);
void SetBGMVolume(float vol);
void SetSEVolume(float vol);
