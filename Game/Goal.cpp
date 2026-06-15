#include "UnlimitedLib/UnlimitedLib.h"
#include "Goal.h"
#include "Game.h"
#include "Player.h"
#include "Stage.h"
#include "GameSound.h"
#include "MainGame.h"
#include "GameOver.h"
#include "Animation.h"

int goal[10];
float goal_x;
float goal_y;
bool HitGoal(float* CharX, float* CharY, float* CharWidth, float* CharHeight,
	float* goalX, float* goalY, float* goalWidth, float* goalHeight,bool*IsGoal);
float Goal_width = GOAL_WIDTH ;
float Goal_height = GOAL_HEIGHT;
AnimationUniqueInfo Goal;
AnimationInfo goal_ptn = {
	//-1ループ,-2一度の再生で終了
	{10.0f,{0,1,2,3,4,5,6,7,8,9,-1}},
};

void GOAL_Initialize(){
	   CreateSpriteMatrix("assets/stage/goal/goal_00.dds",
		288,480,
		10,1,10,
		 goal
		);
	goal_x = 16900.0f;
	goal_y =600.0f;
}

void GOAL_Release()
{
	ReleaseSpriteMatrix(goal,10);
}

void GOAL_Update()
{
	Animation(DeltaTime, Goal, goal_ptn);	
}

void GOAL_Render()
{
	RenderSpritePos(goal[Goal.sprite_index], goal_x - GOAL_WIDTH / 2 - CameraX, goal_y - GOAL_HEIGHT / 2);
}

bool HitGoal(float* CharX, float* CharY, float* CharWidth, float* CharHeight,
	float* goalX, float* goalY, float* goalWidth, float* goalHeight,bool* IsGoal) {

	float CharLeft = *CharX - *CharWidth / 2;
	float CharRight = *CharX + *CharWidth / 2;
	float CharTop = *CharY - *CharHeight / 2;
	float CharBottom = *CharY + *CharHeight / 2;

	float GoalLeft = *goalX - *goalWidth / 2;
	float GoalRight = *goalX + *goalWidth / 2;
	float GoalTop = *goalY - *goalHeight / 2;
	float GoalBottom = *goalY + *goalHeight / 2;

	if (CharRight >= GoalLeft && CharLeft <= GoalRight &&
		CharBottom >= GoalTop && CharTop <= GoalBottom) {
		*IsGoal = false;
		return true;
	}
	return false;
}
