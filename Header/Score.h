#pragma once
class Score
{
public:
	Score();

	~Score();
	int score[5];
	int medal[5];

	void Init();

	void MaxScore(const int stageNo, const int score, const int medal);

};