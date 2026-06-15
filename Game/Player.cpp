#include "UnlimitedLib/UnlimitedLib.h"
#include "Player.h"
#include "Game.h"
#include "IceSpear.h"
#include "IceBlock.h"
#include "hitblock.h"
#include "stage.h"
#include "UI.h"
#include "Enemy.h"
#include "flameimp.h"
#include "flameslime.h"
#include "Item.h"
#include "Fireball.h"
#include "StageSelect.h"
#include "GameOver.h"
#include "GameSound.h"
#include "Goal.h"
#include "FlameDemon.h"
#include "MainGame.h"
#include "Animation.h"
#include <math.h>
#include "Xinput.h"


Player player;
float pl_width = PLAYER_RADIUS;
float pl_height = PLAYER_HEIGHT/1.2;
float screen_remainder = SCREEN_REMAINDER;
float pl_hpmax;
float pl_mpmax;
float if_frame;
float mpcount;
bool hitenemy1;
bool hitenemy2;
bool hititem;
bool hitfireball;
bool hitdemon;
bool hitbigfireball;
int playerhandle[500];
int playeranim_frame = 0;
int playeranim_max_fram = 40;
int time;
int damageSE;
int pl_moveSE;
int JumpVoiceSE[2];
int BlockVoiceSE[2];
int jumpVoiceIndex = 0;
int blockVoiceIndex = 0;

float pl_x ;
float pl_y ;
bool isJump;
bool isHitenemy;
bool FadeWait; //開始待ち
int goalSE;
bool isHitgoal;
bool isgoal;
bool noAction;

AnimationUniqueInfo player_anim;
AnimationInfo  player_animptn = {
	//-1ループ,-2一度の再生で終了   
	{
		/*待機*/{28.0f,{0,1,2,3,4,5,6,7,8,9,10,
						  11,12,13,14,15,16,17,18,19,20,
						  21,22,23,24,25,26,27,28,29,30,
						  31,32,33,34,35,36,37,38,39, -1}},

		/*ジャンプ*/{20.0f,{57,58,59,60,61,62,63,-2}},

		/*落下*/{30.0f,{100,-2}},

		/*着地*/{30.0f,{101,102,103,104,105,106,-2}},

		/*攻撃*/{40.0f,{150,151,152,153,154,155,156,157,158,159,
						  160,161,162,163,-2}},

		/*移動*/{21.0f,{200,201,202,203,204,205,206,
						  207,208,209,210,211,212,213,-1},
					 {0,0,0,0,4,0,0,0,0,0,
                       0,4,0,0,}},

					   /*被弾*/{5.0f,{250,-2},{0,7,0}},

		/*死亡*/{20.0f,{300,301,302,303,304,305,306,307,308,309,
		                  310,311,312,313,314,315,316,317,318,319,
						  320,321,322,323,324,325,326,327,328,329,
						  330,331,332,333,-2},
	                     {0,0,0,0,0,0,0,8,0,0,
						  0,0,0,0,0,0,0,0,0,0,
						  0,0,0,0,0,0,0,0,0,0,
						  0,0,0,0}},

		/*ゴール*/{20.0f,{350,351,352,353,354,355,356,357,358,359,
						    360,361,362,363,364,365,366,367,368,369,
						    370,371,372,373,374,375,376,377,378,379,
						    380,381,382,383,384,385,386,387,388,389,
							390,-2},
                           {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
							}},
	/*アイスブロック*/{20.0f,{400,401,402,403,-2}},
	}

};

//プレイヤーアニメーションステート
enum {

	STATE_IDLE ,
	STATE_JUMP ,
	STATE_FALL ,
	STATE_LANDING, 
	STATE_ATTACK ,
	STATE_RUN ,
	STATE_HIT ,
	STATE_DEAD ,
	STATE_GOAL ,
	STATE_USEBLOCK ,

};

int PlayerState;
int PlayerSE[3];

void PLAYER_DEAD();
void PLAYERVsGoal();

void PLAYER_HEAL(float val) {
	player.hp += val;
	if (player.hp > pl_hpmax)player.hp = pl_hpmax;
}

//初期化処理
void PLAYER_Initialize() {
	damageSE = CreateSEOgg("assets/SE/se_player_damage_00.ogg");
	pl_moveSE= CreateSEOgg("assets/SE/se_player_move_00.ogg");
	//初期ステート
	PlayerState = STATE_IDLE;

	PlayerSE[0] = CreateSEOgg("assets/SE/se_player_jump_00-1.ogg");
	PlayerSE[1] = CreateSEOgg("assets/SE/se_player_magic_attack_00.ogg");
	PlayerSE[2] = CreateSEOgg("assets/SE/se_player_magic_block_00.ogg");
	goalSE = CreateSEOgg("assets/SE/se_clear_00.ogg");
	JumpVoiceSE[0] = CreateSEOgg("assets/SE/Voice/Player/player_voice_jump_00.ogg");
	JumpVoiceSE[1] = CreateSEOgg("assets/SE/Voice/Player/player_voice_jump_01.ogg");
	BlockVoiceSE[0] = CreateSEOgg("assets/SE/Voice/Player/player_voice_iceblock_00.ogg");
	BlockVoiceSE[1] = CreateSEOgg("assets/SE/Voice/Player/player_voice_iceblock_01.ogg");

	jumpVoiceIndex = 0;
	blockVoiceIndex = 0;

	//待機
	CreateSpriteMatrix("assets/Player/player_stop_00.png",
		PLAYER_WIDTH, PLAYER_HEIGHT,
		8, 5, 40,
		playerhandle);
	//ジャンプ
	CreateSpriteMatrix("assets/Player/player_jump_00.png",
		PLAYER_WIDTH, PLAYER_HEIGHT,
		8, 3, 21,
		&playerhandle[50]);
	CreateSpriteMatrix("assets/Player/player_jump_01.png",
		PLAYER_WIDTH, PLAYER_HEIGHT,
		7, 1, 7,
		&playerhandle[100]);
	//攻撃
	CreateSpriteMatrix("assets/Player/player_magic_attack_00.png",
		PLAYER_WIDTH, PLAYER_HEIGHT,
		7, 2, 14,
		&playerhandle[150]);
	//移動
	CreateSpriteMatrix("assets/Player/player_move_00.png",
		PLAYER_WIDTH, PLAYER_HEIGHT,
		8, 2, 14,
		&playerhandle[200]);
	//被弾
	CreateSpriteMatrix("assets/Player/player_damage_00.png",
		PLAYER_WIDTH, PLAYER_HEIGHT,
		1, 1, 1,
		&playerhandle[250]);
	//死亡
	CreateSpriteMatrix("assets/Player/player_dead_00.png",
		PLAYER_WIDTH, PLAYER_HEIGHT,
		8, 5, 34,
		&playerhandle[300]);
	//ゴール
	CreateSpriteMatrix("assets/Player/player_goal_00.png",
		PLAYER_WIDTH, PLAYER_HEIGHT,
		8, 6, 41,
		&playerhandle[350]);
	//アイスブロック生成
	CreateSpriteMatrix("assets/Player/player_magic_block_00.png",
		PLAYER_WIDTH, PLAYER_HEIGHT,
		4, 1, 4,
		&playerhandle[400]);

	player.positionX = 100;
	player.positionY = SCREEN_HEIGHT-PLAYER_HEIGHT;
	player.VelocityY = 0.0f;
	player.speed = 0.0f;
	player.isGround = false;//空中
	player.isAlived = true;
	player.Dir = true;//最初は右向き
	player.count = 0;
	player.hp = HP_MAX;
	player.mp = MP_MAX;
	player.atk = PLAYER_ATTACK;
	pl_mpmax = MP_MAX;
	pl_hpmax = HP_MAX;
	mpcount = 0;
	if_frame = 0;
	hitenemy1 = false;
	hitenemy2 = false;
	hititem = false;
	hitdemon = false;
	player_anim.isPlaying = true;
	isHitenemy = false;
	
	PlayerState = STATE_IDLE;
	FadeWait = false;
	isHitgoal = false;
	isgoal = false;
}

void PLAYER_Release() {
	ReleaseSpriteMatrix(playerhandle, 500);
	ReleaseSE(damageSE);
	ReleaseSE(pl_moveSE);
	ReleaseSE(goalSE);
	ReleaseSE(JumpVoiceSE[0]);
	ReleaseSE(JumpVoiceSE[1]);
	ReleaseSE(BlockVoiceSE[0]);
	ReleaseSE(BlockVoiceSE[1]);
}

void PLAYER_Update() {
	if (player.isAlived) {
		//描画位置との座標補正
		pl_x = player.positionX - pl_width / 2 - CameraX;
		pl_y = player.positionY - pl_height + screen_remainder;

		PLAYER_AnimUpdate();
		Animation(DeltaTime, player_anim, player_animptn);
		playeranim_frame++;
		time = playeranim_frame;
		if (PlayerState != STATE_DEAD && PlayerState!=STATE_GOAL) {
			if (PlayerState != STATE_HIT) {
				PLAYER_Move();
			}
			PLAYER_Jump();
			PLAYER_CollisionUpdate();
			PLAYER_UseMagic();
		}
		PLAYER_HealMp();
		PLAYER_SetUI();
	}
	PLAYER_DEAD();
}

//判定更新
void PLAYER_CollisionUpdate() {
	PLAYERVsEnemy();
	PLAYERVsStage();
	PLAYERVsIceBlock();
	PLAYERVsGoal();
}

void PLAYER_DEAD() {
	//死亡処理
	//落下死亡orHP0時死亡
	if (player.hp <= 0) {
		player.hp = 0;
		player.speed = 0;
		BGM_Stop();
		
		//死亡時重力をかける
		player.VelocityY += PLAYER_GRAVITY * (DeltaTime * 50);
		player.positionY += player.VelocityY * (DeltaTime * 50);
		PLAYERVsStage();
		//アニメーション終了でゲームオーバーへ
		if (PlayerState == STATE_DEAD && !player_anim.isPlaying) {
			gameOverChange = true;
			GameOver_Update();
		}
	}
	if (player.positionY > SCREEN_HEIGHT + pl_height / 2) {
		BGM_Stop();
		player.speed=0;
		player.isAlived = false;
		gameOverChange = true;
		GameOver_Update();
	}
}

//移動処理
void PLAYER_Move() {
	player.speed = 0;
	float LeftStickX = 0.0f;

	LeftStickX = GetPadLeftStickX(0);

	if (fabsf(LeftStickX) < 0.3f)
	{

		if (IsKeyPress('D') || IsPadPress(0, XINPUT_GAMEPAD_DPAD_RIGHT)) {
			LeftStickX = 0.5f;
			
		}
		if (IsKeyPress('A') || IsPadPress(0, XINPUT_GAMEPAD_DPAD_LEFT)) {
			LeftStickX = -0.5f;
			
		}
	}

	if (LeftStickX > 0.3f) {
		player.speed = PLAYER_SPEED;
		player.Dir = true;//右
	}
	if (LeftStickX < -0.3f) {
		player.speed = -PLAYER_SPEED;
		player.Dir = false;//左
	}
	player.positionX += player.speed * (DeltaTime*50);
}

//ジャンプ処理
void PLAYER_Jump() {
	//決定キーの入力の重なり回避処理（ポーズ画面等）
	if (noAction) {
		noAction = false;
		return;
	}
	
	if (player.VelocityY < 0) {
		player.isGround = false;
	}

	if (PlayerState!=STATE_LANDING&&player.isGround &&(IsKeyPush(VK_SPACE)||IsPadPush(0,XINPUT_GAMEPAD_A))){
		player.VelocityY = -PLAYER_JUMP_FORCE;
		player.isGround = false;
		PlaySE(PlayerSE[0], false, 0.5f);
		PlaySE(JumpVoiceSE[jumpVoiceIndex], false, 0.5f);
		jumpVoiceIndex = (jumpVoiceIndex + 1) % 2;
	}

	//重力処理
	player.VelocityY += PLAYER_GRAVITY*(DeltaTime*50);
	player.positionY += player.VelocityY *(DeltaTime * 50);
}

//敵との接触判定
void PLAYERVsEnemy() {
	//無敵時間
	if (if_frame > 0) {
		if_frame-=DeltaTime;
		if (if_frame <= 0) {
			if_frame = 0;
		}
	}
	
	//複数の敵との接触判定
	if (if_frame <= 0) {
		for (int n = 0; n < ENEMY_MAX; n++) {
			if (enemyParam[n].collision) {
				//インプ
				if (enemyParam[n].IsActive) {//生存している敵のみ判定

					float imp_X = enemyParam[n].x - enemy_width / 2 - CameraX;
					float imp_Y = enemyParam[n].y - enemy_height + screen_remainder;

					hitenemy1 = HitPlayer(&pl_x, &pl_y, &pl_width, &pl_height,
						&imp_X, &imp_Y, &enemy_width, &enemy_height, &player.hp, &enemyParam[n].atk);
					if (hitenemy1) {
						if_frame = IF_FRAME;//無敵時間IF_FRAME
						isHitenemy = true;
						PlaySE(damageSE, false, 0.5f);
					}
				}else if (enemyParam[n].state = enemyParam[n].STATE_DEATH) {
					!enemyParam[n].collision;
				}
			}
			//スライム
			if (enemyParam2[n].collision) {
				if (enemyParam2[n].IsActive) {//生存している敵のみ判定

					float slime_X = enemyParam2[n].x - enemy2_width / 2 - CameraX;
					float slime_Y = enemyParam2[n].y - enemy2_height + screen_remainder;

					//判定処理
					hitenemy2 = HitPlayer(&pl_x, &pl_y, &pl_width, &pl_height,
						&slime_X, &slime_Y, &enemy2_width, &enemy2_height, &player.hp, &enemyParam2[n].atk);
					//当たってたら
					if (hitenemy2) {
						if_frame = IF_FRAME;//無敵時間IF_FRAME
						isHitenemy = true;
						PlaySE(damageSE, false, 0.5f);
					}
				}
			}
			else if (enemyParam2[n].state = enemyParam2[n].STATE_DEATH) {
				!enemyParam2[n].collision;
			}
		}
		//ファイヤーボール(インプ）
		for (int b = 0; b < FIRE_BALL_MAX; b++) {
			if (!fireParam[b].IsActive) { continue; }
				hitfireball = CircleVsRect(fireParam[b].x  - CameraX, fireParam[b].y , fire_size/2,
					pl_x, pl_y, pl_width, pl_height);

				if (hitfireball) {
					player.hp = player.hp - fireParam[b].atk;
					fireParam[b].IsActive = false;
					SetFireHitEffect(b, fireParam[b].x, fireParam[b].y);
					if_frame = IF_FRAME;//無敵時間IF_FRAME
					PlaySE(damageSE,false,0.5f);
					isHitenemy = true;
					continue;
				}
 
		}
		//炎の魔人
		if (!notHitboss) {
			if (demon.isAlived) {
				float demon_X = demon.positionX - demon_width / 2;
				float demon_Y = demon.positionY - demon_height + screen_remainder;

				hitdemon = HitPlayer(&pl_x, &pl_y, &pl_width, &pl_height,
					&demon_X, &demon_Y, &demon_width, &demon_height, &player.hp, &demon.atk);
				if (hitdemon) {
					if_frame = IF_FRAME;
					isHitenemy = true;
					PlaySE(damageSE, false, 0.5f);
				}
			}
			if (fire_holizontal.isAlived) {
				hitbigfireball = CircleVsRect(fire_holizontal.positionX, fire_holizontal.positionY, bigfire_size / 2,
					player.positionX - pl_width / 2, player.positionY - pl_height + screen_remainder, pl_width, pl_height);
				if (hitbigfireball) {
					player.hp = player.hp - demon.atk;
					fire_holizontal.isAlived = false;
					HitBigFire_Set(fire_holizontal.positionX, fire_holizontal.positionY);
					if_frame = IF_FRAME;
					isHitenemy = true;
					PlaySE(damageSE, false, 0.5f);
				}
			}
		}
	}
}

//ステージとの当たり判定
void PLAYERVsStage() {
	//当たり判定処理(左右の壁を5点、上下は３点で判定を取る）
	//頭上チェック
	int topY = player.positionY - pl_height;
	//頭上のブロック
	int tileU = STAGE_GetTileNo(player.positionX, topY);
	int tileUR = STAGE_GetTileNo(player.positionX + pl_width / 2, topY);
	int tileUL = STAGE_GetTileNo(player.positionX - pl_width / 2, topY);
	//頭上がブロックだった場合
	if (tileU > 0 || tileUR > 0 || tileUL > 0) {
		int ty = topY / BlockSIZE;
		int bottomY = (ty + 1) * BlockSIZE;
		player.positionY = bottomY + pl_height;
		player.VelocityY = 0;
	}

	if (player.positionY >= 0) {//落下中
		int tile = STAGE_GetTileNo(player.positionX, player.positionY);//足元のタイルをゲット
		int tileR = STAGE_GetTileNo(player.positionX + pl_width / 1.5f, player.positionY);
		int tileL = STAGE_GetTileNo(player.positionX - pl_width / 1.5f, player.positionY);
		if (tile > 0 || tileR > 0 || tileL > 0) {
			int ty = player.positionY / BlockSIZE;
			player.positionY = ty * BlockSIZE;
			player.VelocityY = 0;
			player.isGround = true;
		}
		else {
			player.isGround = false;
		}
	}

	//右当たり判定
	//右ブロックチェック
	int RightX = player.positionX + pl_width;
	int tileR = STAGE_GetTileNo(RightX, player.positionY - 1);
	//頭上の右もチェック
	int tileHeadR = STAGE_GetTileNo(RightX, player.positionY - pl_height);
	//中央のブロックもチェック
	int tileBodyR = STAGE_GetTileNo(RightX, player.positionY - pl_height / 2);

	int tileBodyR2 = STAGE_GetTileNo(RightX, player.positionY - pl_height / 1.5f);

	int tileBodeyR3 = STAGE_GetTileNo(RightX, player.positionY - pl_height / 3);
	if (tileR > 0 || tileHeadR > 0 || tileBodyR > 0 || tileBodyR2 > 0 || tileBodeyR3 > 0) {
		int BlockLeftX = RightX / BlockSIZE;
		int bottomX = BlockLeftX * BlockSIZE;
		player.positionX = bottomX - pl_width;
		player.speed = 0;
	}

	//左当たり判定
	//左ブロックチェック
	int LeftX = player.positionX - pl_width;
	int tileL = STAGE_GetTileNo(LeftX, player.positionY - 1);
	//頭上の左もチェック
	int tileHeadL = STAGE_GetTileNo(LeftX, player.positionY - pl_height);
	//中央の左もチェック
	int tileBodyL = STAGE_GetTileNo(LeftX, player.positionY - pl_height / 2);

	int tileBodyL2 = STAGE_GetTileNo(LeftX, player.positionY - pl_height / 1.5f);

	int tileBodyL3 = STAGE_GetTileNo(LeftX, player.positionY - pl_height / 3);

	if (tileL > 0 || tileHeadL > 0 || tileBodyL > 0 || tileBodyL2 > 0 || tileBodyL3 > 0) {
		int BlockRightX = LeftX / BlockSIZE;
		int bottomX = (BlockRightX + 1) * BlockSIZE;
		player.positionX = bottomX + pl_width;
		player.speed = 0;
	}

	//画面端判定(ステージ１,2のみ)
	if (player.positionX < 0 + PLAYER_WIDTH / 2) {
		player.positionX = 0 + PLAYER_WIDTH / 2;
	}
	if (player.positionX > SCREEN_WIDTH * 9 - PLAYER_WIDTH / 2) {
		player.positionX = SCREEN_WIDTH * 9 - PLAYER_WIDTH / 2;
	}
	//画面端（ステージ３の場合）
	if (Select == 3) {
		if (player.positionX > SCREEN_WIDTH - PLAYER_WIDTH / 2) {
			player.positionX = SCREEN_WIDTH - PLAYER_WIDTH / 2;
		}
	}
}

//アイスブロック当たり判定
void PLAYERVsIceBlock() {
	for (int i = 0; i < ICEBLOCK_MAX; i++) {
		if (IceBlock[i].isAlived == false) { continue; }//死んでいるアイスブロックはスキップ
		float iceX = IceBlock[i].positionX / Ice_Size;
		float iceY = IceBlock[i].positionY / Ice_Size;

		CheckBlock(&player.positionX, &player.positionY, &player.VelocityY, &pl_width, &pl_height, &player.speed,
			&iceX, &iceY, &Ice_Size, &player.isGround);
	}
}

void PLAYERVsGoal() {
	float goal_X = goal_x - GOAL_WIDTH / 2 - CameraX;
	float goal_Y = goal_y - GOAL_HEIGHT / 2;
    isgoal= HitObject(&pl_x, &pl_y, &pl_width, &pl_height,
		&goal_X, &goal_Y, &Goal_width, &Goal_height, &isHitgoal);
}

void PLAYER_IsGoal() {
	static bool fadeStart = false; // フェード開始したかどうか
	// ゴールアニメーションが終わったらフェード開始
	if (PlayerState == STATE_GOAL && !player_anim.isPlaying && !fadeStart) {
		SetFadeIn(1, 1, 1, 1.5f);  // 白フェードイン
		FadeWait = true;           // フェード待ち状態にする
		fadeStart = true;          // フェード開始済みにする
		gameStop = false;
	}
	// フェードが終わったらリザルト遷移
	if (FadeWait && IsFadeFinished()) {
		MAIN_Release();
		PlaySE(goalSE, false, 0.3f);
		BGM_Stop();
		InitializeScene(SCENE_RESULT);
		SetFadeOut(1, 1, 1, 3.0f); // 白フェードアウト
		FadeWait = false;          // ここでリセット
		fadeStart = false;
	}
}
	
//魔法使用処理
void PLAYER_UseMagic() {
	player.count-=DeltaTime;
	if (player.count < 0) {
		player.count = 0;
	}
	//魔法処理
	if (player.mp > 0) {
		//弾発射
		if (IsKeyPush('J')||IsPadPush(0, XINPUT_GAMEPAD_X)) {
			if (player.count <= 0 && player.mp >= ICESPEAR_MP&& PlayerState==STATE_ATTACK) {
				player.mp -= ICESPEAR_MP;
				Fire(player.positionX, player.positionY, BULLET_SPEED, player.Dir);
				player.count = BULLET_INTERVAL;
				PlaySE(PlayerSE[1]);
			}
		}
		if (player.mp >= ICEBLOCK_MP ) {
			//アイスブロック接地高さ上限
			if (player.positionY >= 0 + PLAYER_HEIGHT) {
				//アイスブロック生成
				if (IsKeyPush('K') || IsPadPush(0, XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
					player.mp -= ICEBLOCK_MP;
					Create(player.positionX, player.positionY, player.Dir);

					PlaySE(PlayerSE[2]);
				}
			}
		}
	}
	if (player.mp <= 0) {
		player.mp = 0;
	}
}

//MP回復処理
void PLAYER_HealMp() {
	mpcount+=DeltaTime;
	//mp回復処理
	if (mpcount >= FRAME) {
		player.mp += HEAL_MP;
		if (player.mp > pl_mpmax)player.mp = pl_mpmax;
		mpcount = 0;
	}
}

//UIのセット
void PLAYER_SetUI() {
	//UIに現在のHP、MPをセットする
	UI_SetMP(player.mp, pl_mpmax);
	UI_SetLife(player.hp, pl_hpmax);
}


int player_priority=0;//優先順位
//アニメーション優先順位
static int PLAYER_AnimPriority(int state) {
	
	//数値が大きい程優先度高い
	switch (state)
	{
	case STATE_IDLE:
		player_priority = 1;
		break;
	case STATE_RUN:
		player_priority = 1;
		break;
	case STATE_JUMP:
		player_priority = 2;
		break;
	case STATE_FALL:
		player_priority = 3;
		break;
	case STATE_LANDING:
		player_priority = 4;
		break;
	case STATE_ATTACK:
		player_priority = 5;
		break;
	case STATE_USEBLOCK:
		player_priority = 6;
		break;
	case STATE_HIT:
		player_priority = 7;
		break;
	case STATE_GOAL:
		player_priority = 8;
		break;
	case STATE_DEAD:
		player_priority = 9;
		break;
	default:
		player_priority = 0;
		break;
	}
	return player_priority;
}

void PLAYER_ChangeAnimState(int State) {
	int currentState = PLAYER_AnimPriority(PlayerState);
	int newState = PLAYER_AnimPriority(State);

	//ステート比較
	if (newState > currentState) {
		PlayerState = State;
	}
	//待機、走りだった場合の切り替え
	else if (currentState == newState) {
		if (PlayerState == STATE_IDLE || PlayerState == STATE_RUN) {
			if (player.isGround) {
				if (player.speed == 0) {
					PlayerState = STATE_IDLE;
				}
				else {
					PlayerState = STATE_RUN;
				}
			}
		}
	}
	//アニメーション再生終了で待機、走りに切り替え
	else if (!player_anim.isPlaying&&!isJump&&player.hp > 0) {
		if (player.isGround) {
			if (player.speed == 0) {
				PlayerState = STATE_IDLE;
			}
			else {
				PlayerState = STATE_RUN;
			}
		}
	}

	ChangeAnimation(PlayerState, player_anim, player_animptn);
}

void PLAYER_AnimUpdate() {
	//ゴール
	if (isgoal) {
		
		player.VelocityY += PLAYER_GRAVITY * (DeltaTime * 50);
		player.positionY += player.VelocityY * (DeltaTime * 50);
		PLAYERVsStage();
		PLAYER_ChangeAnimState(STATE_GOAL);
		return;
	}


	//死亡
	if (player.hp <= 0) {
		PLAYER_ChangeAnimState(STATE_DEAD);
		return;
	}

	//被弾
	if (isHitenemy) {
 		PLAYER_ChangeAnimState(STATE_HIT);
		isHitenemy = false;
	}

	//アイスブロック生成
	//アイスブロック接地高さ上限
	if (player.positionY >= 0 + PLAYER_HEIGHT) {
		if (player.mp >= ICEBLOCK_MP && IsKeyPush('K') || IsPadPush(0, XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			PlaySE(BlockVoiceSE[blockVoiceIndex], false, 0.5f);
			blockVoiceIndex = (blockVoiceIndex + 1) % 2;
			PLAYER_ChangeAnimState(STATE_USEBLOCK);
		}
	}

	//攻撃
	if (player.mp >= ICESPEAR_MP && player.count == 0&& (IsKeyPush('J') || IsPadPush(0, XINPUT_GAMEPAD_X))) {
		SE_Play(4);
		PLAYER_ChangeAnimState(STATE_ATTACK);
	}

	//ジャンプ＋落下
	if (!player.isGround) {
		isJump = true;
		//ジャンプ
		if (player.VelocityY < 0) {
			PLAYER_ChangeAnimState(STATE_JUMP);
		}
		//落下
		else {
			PLAYER_ChangeAnimState(STATE_FALL);
		}
		
	}

	//着地
	else if (player.isGround && isJump) {
		PLAYER_ChangeAnimState(STATE_LANDING);
		isJump = false;
	}

	//待機＋走り
	else if(player.isGround && !isJump&& player.hp > 0){
		
		//待機
		if (player.speed == 0) {
			PLAYER_ChangeAnimState(STATE_IDLE);
		}
		//走り
		else {
			PLAYER_ChangeAnimState(STATE_RUN);
		}
	}

}

void PLAYER_Render() {
	if (player.isAlived) {
		int blinking = if_frame * 60/2;//フレーム単位に直す(/2は点滅時、処理落ちの影響を受けないように）
		if (blinking % 2 == 0) {//ダメージをくらった際点滅
			//ステージ３の場合(カメラを動かさない)
			if (Select == 3) {
				if (player.Dir) {
					RenderSpritePos(playerhandle[player_anim.sprite_index], player.positionX - PLAYER_WIDTH / 2, player.positionY - PLAYER_HEIGHT + SCREEN_REMAINDER);
				}
				else {
					RenderSpritePosLR(playerhandle[player_anim.sprite_index], player.positionX - PLAYER_WIDTH / 2, player.positionY - PLAYER_HEIGHT + SCREEN_REMAINDER);
				}
			}
			//ステージ１、２の場合
			else {
				if (player.Dir) {
					RenderSpritePos(playerhandle[player_anim.sprite_index], player.positionX - PLAYER_WIDTH / 2 - CameraX, player.positionY - PLAYER_HEIGHT + SCREEN_REMAINDER);
				}
				else {
					RenderSpritePosLR(playerhandle[player_anim.sprite_index], player.positionX - PLAYER_WIDTH / 2 - CameraX, player.positionY - PLAYER_HEIGHT + SCREEN_REMAINDER);
				}
			}
		}
	}
}
