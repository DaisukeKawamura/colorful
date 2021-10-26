
#include "./Header/Score.h"


Score::Score()
{
}

Score::~Score()
{
}

void Score::Init()
{
	for (int i = 0; i < 5; i++)
	{
		score[i] = 0;
		medal[i] = 0;
	}
}

void Score::MaxScore(const int stageNo, const int score, const int medal)
{

	if (this->score[stageNo] < score)
	{
		this->score[stageNo] = score;
	}

	if (this->medal[stageNo] < medal)
	{
		this->medal[stageNo] = medal;
	}
}
