#pragma once
//アニメーション個別情報
struct AnimationUniqueInfo {
	//アニメーション情報
	int ID;
	float frame;
	int sprite_index;
	bool isPlaying;
};

//アニメーション情報構造体
struct AnimParam {
	float speed;//秒間のコマ数
	int ptn[128];//アニメパターン
	int event[128];//パターン属性（音、エフェクトなど）
};

//アニメーション情報
struct AnimationInfo {
	AnimParam param[32];//アニメーションパターン
};

void ChangeAnimation(int id, AnimationUniqueInfo& info, AnimationInfo& anim);
void Animation(float deltatime, AnimationUniqueInfo& info, AnimationInfo& anim);
void AnimationEvent(int event);




