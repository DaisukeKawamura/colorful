#pragma once
class Score
{
public:
	Score();

	~Score();
	int score[15];
	int medal[15];

	void Init();

	void MaxScore(const int stageNo, const int score, const int medal);

};