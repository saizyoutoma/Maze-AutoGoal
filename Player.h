#pragma once
#include "./Library/GameObject.h"
#include "./globals.h"
#include "Stage.h"
class Player :
    public GameObject
{
    int playerImage_;
    Point pos_;
    DIR currentDir;
    float speed_;
    bool reachedGoal_;
public:
	Player();
    ~Player();
    void Update() override;
    void Draw() override;
	bool CheckHit(const Rect& me,const Rect& other);

    bool CanMoveTo(int x, int y,Stage* stage);
};

