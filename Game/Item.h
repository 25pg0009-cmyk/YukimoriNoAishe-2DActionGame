#pragma once
void Item_Initialize();//‰Šú‰»
void Item_Release();//‰ğ•ú
void Item_Update();//XV
void Item_Render();//•`‰æ

void Item_Set(float x, float y);

#define ITEM_MAX 2
#define ITEM_SIZE 112
#define HEAL_HP 30


struct Item_Param
{
	int handle;
	float x;
	float y;
	bool IsActive;
};
extern Item_Param itemParam[ITEM_MAX];
extern float item_size;

