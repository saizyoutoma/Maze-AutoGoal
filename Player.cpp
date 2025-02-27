#include "Player.h"
#include "./Source/Input.h"
#include "./Stage.h"
#include "globals.h"
#include "ImGui/imgui.h"

namespace {
	//enum DIR
	//{
	//	UP,
	//	DOWN,
	//	LEFT,
	//	RIGHT,
	//	NONE,
	//	MAXDIR
	//};
	DIR inputDir = NONE;
	const int directions[4][2] = { {0,-1},{1,0},{0,1},{-1,0} };
}


Player::Player()
	: pos_({ CHA_WIDTH, CHA_HEIGHT }), playerImage_(-1),currentDir(UP),
      reachedGoal_(false),speed_(0.5f)
{
}

Player::~Player()
{
}

void Player::Update()
{
	int ox = pos_.x, oy = pos_.y;

	Stage* stage = (Stage*)FindGameObject<Stage>();
	Rect playerRect = { pos_.x, pos_.y, CHA_WIDTH, CHA_HEIGHT };

    Rect goalRect = stage->GetGoalRect();

    if (CheckHit(playerRect, goalRect)) {
        reachedGoal_ = true;
        return;
    }

    const int directions[4][2] = {
        {0, -1},  // UP
        {1, 0},   // RIGHT
        {0, 1},   // DOWN
        {-1, 0}   // LEFT
    };

    for (int i = 0; i < 4; i++)
    {
        DIR rightDir = (DIR)((currentDir + 1) % 4);
        int dx = directions[rightDir][0];
        int dy = directions[rightDir][1];

        if (CanMoveTo(pos_.x + dx * CHA_WIDTH*speed_, pos_.y + dy * CHA_HEIGHT*speed_, stage))
        {
            currentDir = rightDir; 
            pos_.x += dx * CHA_WIDTH*speed_;
            pos_.y += dy * CHA_HEIGHT*speed_;
            break;
        }

        dx = directions[currentDir][0];
        dy = directions[currentDir][1];
        if (CanMoveTo(pos_.x + dx * CHA_WIDTH*speed_, pos_.y + dy * CHA_HEIGHT*speed_, stage))
        {
            pos_.x += dx * CHA_WIDTH*speed_;
            pos_.y += dy * CHA_HEIGHT*speed_;
            break;
        }
        currentDir = (DIR)((currentDir + 3) % 4); 
    }

    for (auto& obj : stage->GetStageRects())
    {
        if (CheckHit(playerRect, obj))
        {
            pos_.x = ox;
            pos_.y = oy;
        }
    }
}

void Player::Draw()
{
	ImGui::Begin("config 1");

	ImGui::End();

	DrawBox(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, GetColor(255, 10, 10), TRUE);
}

bool Player::CheckHit(const Rect& me, const Rect& other)
{
	if (me.x < other.x + other.w &&
		me.x + me.w > other.x &&
		me.y < other.y + other.h &&
		me.y + me.h > other.y)
	{
		return true;
	}
	return false;
}

bool Player::CanMoveTo(int x,int y,Stage* stage)
{
	for (auto& obj : stage->GetStageRects())
	{
		if (CheckHit(Rect{ x,y,CHA_WIDTH,CHA_HEIGHT }, obj))
		{
			return false;
		}
	}
	return true;
}
