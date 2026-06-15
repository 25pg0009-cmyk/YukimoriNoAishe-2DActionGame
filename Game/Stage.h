#pragma once

void STAGE_Initialize();//‰Šú‰»
void STAGE_Release();//‰ğ•ú
void STAGE_Update();//XV
void STAGE_Render();//•`‰æ
int STAGE_GetTileNo(int x, int y);
void STAGE_Load(const char* filename);
void  STAGE_LoadAttr(const char* filename);
#define BlockSIZE 96
#define STAGE_WIDTH 180	
#define STAGE_HEIGHT 11

extern float CameraX;
extern float CameraY;

extern float Block_Size;
extern int stage[STAGE_HEIGHT][STAGE_WIDTH];
extern int Select;
