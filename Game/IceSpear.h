#pragma once
#include"Player.h"
#define BULLET_MAX  50//’e‚Ìo‚¹‚é‡Œv”
#define BULLET_SPEED 12//’e‚Ì‘¬‚³
#define BULLET_INTERVAL 0.5//’e‚Ìo‚·ŠÔŠu
#define BULLET_WIDTH  192//’e•
#define BULLET_HEIGHT 96 //’e‚‚³

#define I_Frame 0.8//’e‚ªÁ‚¦‚é‚Ü‚Å‚ÌƒtƒŒ[ƒ€
void IceSpear_Initialize();
void IceSpear_Release();
void IceSpear_Update();
void IceSpear_Render();
bool hitBlockSpear(int n);


void Fire(float x, float y, float speed,bool Dir);
extern Player icespear[BULLET_MAX];
extern float IceSpear_height;
extern float IceSpear_radius;





