#pragma once
#include"Enemy.h"

#define ENEMY_FI_WIDTH	192
#define ENEMY_FI_HEIGHT 240

void ENEMY_Initialize();//‰Šú‰»
void ENEMY_Release();//‰ğ•ú
void ENEMY_Update();//XV
void ENEMY_Render();//•`‰æ
extern struct EnemyParam enemyParam[ENEMY_MAX];
