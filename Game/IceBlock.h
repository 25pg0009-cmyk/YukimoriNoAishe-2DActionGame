#pragma once
#include"Player.h"
#define ICEBLOCKSIZE 144
#define ICEBLOCK_MAX 3
#define ICEBLOCK_FRAME 3
void IceBlock_Initialize();
void IceBlock_Release();
void IceBlock_Render();
void IceBlockUpdate();
void Create(float x, float y, bool Dir);
void SetIceBlockEffect(int n, float x, float y, bool Dir);
extern float Ice_Size;
extern Player IceBlock[ICEBLOCK_MAX];



