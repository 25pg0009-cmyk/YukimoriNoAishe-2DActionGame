#pragma once
void MagicShard_Initialize();//‰Šú‰»
void MagicShard_Release();//‰ğ•ú
void MagicShard_Update();//XV
void MagicShard_Render();//•`‰æ
void MS_Set(float x, float y);

#define MS_MAX 3

struct MS_Param
{
	float x;
	float y;
	bool IsActive;
};

extern MS_Param msParam[MS_MAX];

extern int get_ms_count;
extern int get_ms_count2;

#define MS_W 96
#define MS_H 192
extern float ms_width;
extern float ms_height;
