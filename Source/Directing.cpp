#include "./Header/Directing.h"

void Directing::Init()
{
	//シェイク初期化
	shakeFlag = true;
	shakeX = 0;
	shakeY = 0;
	//スコアイージング初期化
	scoreFlag = true;
	scoreDirectFlag = true;
	//周回フラグ初期化
	lap1Flag = true;
	lap2Flag = true;
	stopLap1Flag = true;
	stopLap2Flag = true;
	wallFlag = true;
	pFlyFlag = true;
	wallTime = 8;
	jumpTime = 0;
	//走るパーティクル削除
	for (int i = (int)run.size() - 1; i >= 0; i--)
	{
		delete run[i];
		run.erase(run.begin() + i);
	}
	//色変えパーティクル削除
	for (int i = (int)ring.size() - 1; i >= 0; i--)
	{
		delete ring[i];
		ring.erase(ring.begin() + i);
	}
	//壊れるかべ削除
	for (int i = (int)wall.size() - 1; i >= 0; i--)
	{
		delete wall[i];
		wall.erase(wall.begin() + i);
	}
	//爆発削除
	for (int i = (int)explosion.size() - 1; i >= 0; i--)
	{
		delete explosion[i];
		explosion.erase(explosion.begin() + i);
	}
	//ジャンプ削除
	for (int i = (int)jump.size() - 1; i >= 0; i--)
	{
		delete jump[i];
		jump.erase(jump.begin() + i);
	}
}
//画像読み込み
void Directing::ParticleInit(DrawPolygon *draw)
{
	static bool isInit = false;
	if (isInit == false)
	{
		this->draw = draw;
		this->graph1 = this->draw->LoadTextrue(L"./Resources/particle.jpg");
		this->graph2 = this->draw->LoadTextrue(L"./Resources/effect2.png");
		this->graph3 = this->draw->LoadTextrue(L"./Resources/effect3.png");
		this->lap1Graph = this->draw->LoadTextrue(L"./Resources/lap1.png");
		this->lap2Graph = this->draw->LoadTextrue(L"./Resources/lap2.png");
		this->wallBreakGraph = this->draw->LoadTextrue(L"./Resources/box.png");
		this->SelectGraph = this->draw->LoadTextrue(L"./Resources/stageselect.png");
		this->SelectBotanGraph = this->draw->LoadTextrue(L"./Resources/stageselectBotan.png");
		this->starGraph = this->draw->LoadTextrue(L"./Resources/hosi.png");
		this->medalGraph = this->draw->LoadTextrue(L"./Resources/medaru.png");
		this->SelectBackBotan = this->draw->LoadTextrue(L"./Resources/backbutton.png");
		this->percentGraph = this->draw->LoadTextrue(L"./Resources/percent.png");
		this->stageNumberGraph = this->draw->LoadTextrue(L"./Resources/stagenum.png");
		this->scoreNumberGraph = this->draw->LoadTextrue(L"./Resources/scorenum.png");
		this->retryButtonGraph = this->draw->LoadTextrue(L"./Resources/retrybutton.png");
		this->selectbuttonGraph = this->draw->LoadTextrue(L"./Resources/selectbutton.png");
		this->particlePolygon = this->draw->CreateRect(10, 10);
		this->wallBreak3D = this->draw->Create3Dbox(10.0f, 10.0f, 20.0f);
		this->draw->NormalizeUV(wallBreak3D, wallBreakGraph);
	}
}
// 死亡時シェイク
void Directing::ShakeUpdate()
{
	//シェイク初期化
	shakeX = 0, shakeY = 0;
	//シェイクが続く時間
	if (shakeTime > 0)
	{
		shakeX = (rand() % shakeString - shakeString / 2) * shkeStringTime;
		shakeY = (rand() % shakeString - shakeString / 2) * shkeStringTime;
		shakeTime -= 1;
		shkeStringTime -= 0.1f;
	}
}

void Directing::ShakeStart(float shakeTime, int shakeString)
{
	if (shakeFlag == true)
	{
		//シェイクする時間
		this->shakeTime = shakeTime;
		//シェイクの強さ
		this->shakeString = shakeString;
		//減衰
		shkeStringTime = shakeTime / 10;
		shakeFlag = false;
	}
}
//スコアイージング
void Directing::scoreEasingStart(XMFLOAT3 start, XMFLOAT3 end, float time)
{
	if (scoreFlag == true)
	{
		scoreFlag = false;
		this->scoreStart = Vector3(start.x, start.y, start.z);
		this->scoreEnd = Vector3(end.x, end.y, end.z);
		this->scoreMaxTime = time;
		scoreEasingTime = 0;
	}
}

XMFLOAT3 Directing::scoreEasing()
{
	scoreEasingTime++;

	scoreTimeRate = min(scoreEasingTime / scoreMaxTime, 1.0f);
	Vector3 position;

	position = easeOut(scoreStart, scoreEnd, scoreTimeRate);
	/*position = easeIn(start, end, timeRate);*/
	//position = easeInOut(start, end, timeRate);
	if (scoreTimeRate >= 1.0f)
	{
		scoreDirectFlag = false;
	}
	return XMFLOAT3(scoreStart.x, position.y, scoreStart.z);
}
void Directing::scoreDraw(const int score, const int medal, const int  selectRetryFlag)
{
	if (scoreDirectFlag == true)
	{
		scoreTime = 0;
		for (int i = 0; i < 3; i++)
		{
			starScale[i] = 0;
			medalScale[i] = 0;
		}
	}
	if (scoreDirectFlag == false)
	{
		const int starR = 160;
		const int medalR = 65;
		const int scale = 15;
		//星
		for (int i = 0; i < 3; i++)
		{
			if (scoreTime > i * 10 && score >= clearScore[i])
			{
				if (scoreTime < 5 + i * 10)
				{
					starScale[i] += scale;
				}
				else if (scoreTime < 9 + i * 10)
				{
					starScale[i] -= scale;
				}
				draw->DrawTextrue(375 + (181 * i) - starScale[i] / 2, 330 - starScale[i] / 2, starR + starScale[i], starR + starScale[i], 0, starGraph, XMFLOAT2(0.0f, 0.0f));

			}
		}
		for (int i = 0; i < 3; i++)
		{
			//メダル
			if (scoreTime > i * 10 + 30 && medal >= i + 1)
			{
				if (scoreTime < 35 + i * 10)
				{
					medalScale[i] += scale;
				}
				else if (scoreTime < 39 + i * 10)
				{
					medalScale[i] -= scale;
				}
				draw->DrawTextrue(530 + (77 * i) - medalScale[i] / 2, 510 - medalScale[i] / 2, medalR + medalScale[i], medalR + medalScale[i], 0, medalGraph, XMFLOAT2(0.0f, 0.0f));
			}

		}
		//スコア数字
		NumberDraw(score, 800, 280, 60, 60);
		//パーセント
		draw->DrawTextrue(830, 248, 70, 70, 0, percentGraph, XMFLOAT2(0.0f, 0.0f));
		ClearOverSelect();
		if (selectRetryFlag)
		{
			//リトライボタン
			draw->DrawTextrue(843, 637, 135 + clearOverSelectR, 135 + clearOverSelectR, 0, retryButtonGraph);
			//セレクトボタン
			draw->DrawTextrue(1014, 637, 135, 135, 0, selectbuttonGraph);
		}
		else
		{
			//セレクトボタン
			draw->DrawTextrue(1014, 637, 135 + clearOverSelectR, 135 + clearOverSelectR, 0, selectbuttonGraph);
			//リトライボタン
			draw->DrawTextrue(843, 637, 135, 135, 0, retryButtonGraph);
		}
		scoreTime++;
	}
}

//プレイヤーRunパーティクル
void Directing::RunUpdate(XMFLOAT3 pPos, XMFLOAT4 pColor)
{
	//生み出す
	for (int i = 0; i < 5; i++)
	{
		XMFLOAT3 Pos = pPos;
		Pos.x += rand() % 10 - 5;
		Pos.y += rand() % 4 - 2;
		XMFLOAT3 speed = { -2.0f,0.0f,0.0f };

		XMFLOAT4 color = pColor;

		run.push_back(new Particle(Pos, speed, XMFLOAT3(0.5, 0.5, 0.5), color, 50));
	}

	//更新
	for (size_t i = 0; i < run.size(); i++)
	{
		run[i]->pos = run[i]->pos + run[i]->speed;

		if (run[i]->time >= 20)
		{//徐々に大きくする
			run[i]->scale = run[i]->scale + Vector3(0.05f, 0.05f, 0.05f);
		}
		//色を薄くしていく
		run[i]->color = run[i]->color - XMFLOAT4(0.04f, 0.04f, 0.04f, 0.04f);

		run[i]->time--;

		if (run[i]->time == 0)
		{
			run[i]->DelFlag = false;
		}
	}
	//削除
	for (int i = (int)run.size() - 1; i >= 0; i--)
	{
		if (run[i]->DelFlag == false)
		{
			delete run[i];
			run.erase(run.begin() + i);
		}
	}
}

void Directing::RunDraw()
{
	//描画
	for (size_t i = 0; i < run.size(); i++)
	{
		draw->Draw(particlePolygon, run[i]->pos, run[i]->rotaMat, run[i]->scale, run[i]->color, graph1);
	}
}

//色変わる瞬間のパーティクル
void Directing::RingStart()
{
	ringTime = 10;
}

void Directing::RingUpdate(XMFLOAT3 pPos, XMFLOAT4 pColor)
{
	if (ringTime > 0)
	{
		//生み出す
		for (int i = 0; i < 20; i++)
		{
			XMFLOAT3 Pos = pPos;
			Pos.x += rand() % 18 - 9.0f;
			Pos.y -= 2.0f;
			XMFLOAT3 speed = { 2.8f,3.0f,0.0f };

			XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };

			ring.push_back(new Particle(Pos, speed, XMFLOAT3(0.5, 0.5, 0.5), color, 20));
		}
		ringTime--;
	}

	//更新
	for (size_t i = 0; i < ring.size(); i++)
	{
		ring[i]->pos = ring[i]->pos + ring[i]->speed;

		//色を薄くしていく
		ring[i]->color = ring[i]->color - XMFLOAT4{ 0.08f,0.08f,0.08f,0.08f };

		ring[i]->time--;
		if (ring[i]->time == 0)
		{
			ring[i]->DelFlag = false;
		}
	}
	//削除
	for (int i = (int)ring.size() - 1; i >= 0; i--)
	{
		if (ring[i]->DelFlag == false)
		{
			delete ring[i];
			ring.erase(ring.begin() + i);
		}
	}
}

void Directing::RingDraw()
{
	//描画
	for (size_t i = 0; i < ring.size(); i++)
	{
		draw->Draw(particlePolygon, ring[i]->pos, ring[i]->rotaMat, ring[i]->scale, ring[i]->color, graph2);
	}
}
//1週目演出
void Directing::Lap1Update(XMFLOAT3 start, XMFLOAT3 end, float time)
{
	if (lap1Flag == true)
	{
		lap1Flag = false;
		this->lap1Start = Vector3(start.x, start.y, start.z);
		this->lap1End = Vector3(end.x, end.y, end.z);
		this->lap1MaxTime = time;
		lap1EasingTime = 0;
	}

	lap1EasingTime++;

	lap1TimeRate = min(lap1EasingTime / lap1MaxTime, 1.0f);

	if (lap1TimeRate >= 1.0f)
	{
		stopLap1Flag = false;
	}

	//lap1Pos = easeOut(lap1Start, lap1End, lap1TimeRate);
	lap1Pos = easeIn(lap1Start, lap1End, lap1TimeRate);
	//lap1Pos = easeInOut(lap1Start, lap1End, lap1TimeRate);
}

void Directing::Lap1Draw()
{
	if (lap1Flag == false && stopLap1Flag == true)
	{
		draw->DrawTextrue(lap1Pos.x, lap1Pos.y, 200, 100, 0, lap1Graph, XMFLOAT2(0.0f, 0.0f));
	}
}

//2週目演出
void Directing::Lap2Start(XMFLOAT3 start, XMFLOAT3 end, float time)
{
	if (lap2Flag == true)
	{
		lap2Flag = false;
		stopLap2Flag = true;
		this->lap2Start = Vector3(start.x, start.y, start.z);
		this->lap2End = Vector3(end.x, end.y, end.z);
		this->lap2MaxTime = time;
		lap2EasingTime = 0;
		lap2TimeRate = 0;
		lap2EasingTime = 0;
	}
}

void Directing::Lap2Update()
{
	if (lap2Flag == false && stopLap2Flag == true)
	{
		lap2EasingTime++;

		lap2TimeRate = min(lap2EasingTime / lap2MaxTime, 1.0f);
		if (lap2TimeRate >= 1.0f)
		{
			stopLap2Flag = false;
		}
		//lap2Pos = easeOut(lap2Start, lap2End, lap2TimeRate);
		lap2Pos = easeIn(lap2Start, lap2End, lap2TimeRate);
		//lap2Pos = easeInOut(lap2Start, lap2End, lap2TimeRate);
	}

}

void Directing::Lap2Draw()
{
	if (lap2Flag == false && stopLap2Flag == true)
	{
		draw->DrawTextrue(lap2Pos.x, lap2Pos.y, 200, 100, 0, lap2Graph, XMFLOAT2(0.0f, 0.0f));
	}
}
//2週目演出
void Directing::FlyStart(XMFLOAT3 start, XMFLOAT3 controlPoint1, XMFLOAT3 controlPoint2, XMFLOAT3 end, float time)
{
	if (pFlyFlag == true)
	{
		pFlyFlag = false;
		this->playerStart = Vector3(start.x, start.y, start.z);
		this->controlPoint1 = Vector3(controlPoint1.x, controlPoint1.y, controlPoint1.z);
		this->controlPoint2 = Vector3(controlPoint2.x, controlPoint2.y, controlPoint2.z);
		this->playerEnd = Vector3(end.x, end.y, end.z);
		this->pFlyMaxTime = time;
		playerAngle = 0.0f;
		pFlyEasingTime = 0;
	}
}
//プレイヤー飛ばし
XMFLOAT3 Directing::PFly()
{
	pFlyEasingTime++;
	playerAngle += 10.0f;
	pFlyTimeRate = min(pFlyEasingTime / pFlyMaxTime, 1.0f);

	Vector3 a = lerp(playerStart, controlPoint1, pFlyTimeRate);

	Vector3 b = lerp(controlPoint1, controlPoint2, pFlyTimeRate);

	Vector3 c = lerp(controlPoint2, playerEnd, pFlyTimeRate);

	Vector3 n = lerp(a, b, pFlyTimeRate);

	Vector3 m = lerp(b, c, pFlyTimeRate);

	Vector3 position = lerp(n, m, pFlyTimeRate);

	return XMFLOAT3(position.x, position.y, position.z);
}
void Directing::BreakWallStart()
{
	wallFlag = false;
}
//壊れる壁と爆発
void Directing::BreakWall(XMFLOAT3 wallPos, XMFLOAT3 player)
{
	if (wallTime > 0)
	{
		explosionPos = wallPos;
		for (int i = 0; i < 10; i++)
		{//壁
			wallPos.x += rand() % 18 - 9.0f;
			wallPos.y += rand() % 100;
			XMFLOAT3 speed = { 3.0f,-3.0f,0.0f };
			speed.x += rand() % 6 - 3.0f;
			speed.y += rand() % 6 - 3.0f;
			XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };

			wall.push_back(new Particle(wallPos, speed, XMFLOAT3(1.0, 1.0, 1.0), color, 20));
		}
		for (int i = 0; i < 20; i++)
		{//爆発
			XMFLOAT3 speed = { 3.0f,-3.0f,0.0f };
			speed.x = rand() % 6 - 3.0f;
			speed.y = (float)(rand() % 6);
			XMFLOAT4 color = { 0.8f,0.8f,1.0f,1.0f };
			explosion.push_back(new Particle(explosionPos, speed, XMFLOAT3(1.0, 1.0, 1.0), color, 20));
		}
		wallTime--;
	}
	//壁
	for (size_t i = 0; i < wall.size(); i++)
	{//更新
		wall[i]->pos = wall[i]->pos + wall[i]->speed;

		wall[i]->time--;
		if (wall[i]->time == 0)
		{
			wall[i]->DelFlag = false;
		}
	}
	for (int i = (int)wall.size() - 1; i >= 0; i--)
	{//削除
		if (wall[i]->DelFlag == false)
		{
			delete wall[i];
			wall.erase(wall.begin() + i);
		}
	}

	//爆発
	for (size_t i = 0; i < explosion.size(); i++)
	{//更新
		explosion[i]->pos = explosion[i]->pos + explosion[i]->speed;

		explosion[i]->time--;
		if (explosion[i]->time == 0)
		{
			explosion[i]->DelFlag = false;
		}
	}
	for (int i = (int)explosion.size() - 1; i >= 0; i--)
	{	//削除
		if (explosion[i]->DelFlag == false)
		{
			delete explosion[i];
			explosion.erase(explosion.begin() + i);
		}
	}
}

void Directing::wallDraw()
{
	//描画
	for (size_t i = 0; i < wall.size(); i++)
	{//壁
		draw->Draw(wallBreak3D, wall[i]->pos, wall[i]->rotaMat, wall[i]->scale, wall[i]->color, wallBreakGraph);
	}
}

void Directing::explosionDraw()
{
	for (size_t i = 0; i < explosion.size(); i++)
	{//爆発
		draw->Draw(particlePolygon, explosion[i]->pos, explosion[i]->rotaMat, explosion[i]->scale, explosion[i]->color, graph1);
	}
}

bool Directing::ChangeScene()
{
	return changeColor.w >= 1.0f;
}

void Directing::StartSceneChange()
{
	if (sceneChangeFlag == false)
	{
		sceneChangeFlag = true;
		firstHalfFlag = true;
		changeColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

void Directing::SceneChangeUpdate()
{

	if (sceneChangeFlag == true)
	{
		const float changeSpeed = 0.045f;
		if (firstHalfFlag == true)
		{
			changeColor.w += changeSpeed;
			if (changeColor.w >= 1.0f)
			{
				firstHalfFlag = false;
			}
		}
		else
		{
			if (changeColor.w <= 0.0f)
			{
				sceneChangeFlag = false;
			}
			changeColor.w -= changeSpeed;
		}
	}
}

void Directing::SceneChangeDraw()
{
	if (sceneChangeFlag == true)
	{
		draw->DrawTextrue(0, 0, 1280, 780, 0, 0, XMFLOAT2(0.0f, 0.0f), changeColor);
	}

}
//ジャンプエフェクト
bool Directing::JumEfectJudge(int jumpCount, XMFLOAT4 pColor)
{
	if (jumpCount == 0 && pColor.x == yellowJump.x &&
		pColor.y == yellowJump.y &&
		pColor.z == yellowJump.z &&
		pColor.w == yellowJump.w)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void Directing::JumEfectStart()
{
	jumpTime = 5;
}
void Directing::DoubleJumpUpdate(XMFLOAT3 pPos)
{
	if (jumpTime > 0)
	{
		XMFLOAT3 speed = { 2.8f,-1.0f,0.0f };
		for (int i = 0; i < 15; i++)
		{
			pPos.x += rand() % 2 - 1.0f;
			jump.push_back(new Particle(pPos, speed, XMFLOAT3(0.6, 0.6, 0.6), XMFLOAT4(0.3f, 0.3f, 0.3f, 0.3f), 20));
		}
		jumpTime--;
	}
	for (size_t i = 0; i < jump.size(); i++)
	{//更新
		jump[i]->pos = jump[i]->pos + jump[i]->speed;
		jump[i]->color = jump[i]->color - XMFLOAT4(0.02f, 0.02f, 0.02f, 0.02f);
		jump[i]->time--;
		if (jump[i]->time == 2)
		{
			jump[i]->DelFlag = false;
		}
	}
	for (int i = (int)jump.size() - 1; i >= 0; i--)
	{//削除
		if (jump[i]->DelFlag == false)
		{
			delete jump[i];
			jump.erase(jump.begin() + i);
		}
	}
}

void Directing::DoubleJumpDraw()
{
	for (size_t i = 0; i < jump.size(); i++)
	{//爆発
		draw->Draw(particlePolygon, jump[i]->pos, jump[i]->rotaMat, jump[i]->scale, jump[i]->color, graph1);
	}

}
//ステージセレクト
void Directing::stageSelectInit()
{
	stageSelectFlag = true;
}

void Directing::stageSelectEasingStart(XMFLOAT3 start, XMFLOAT3 end, float Time)
{
	if (stageSelectFlag == true)
	{
		stageSelectFlag = false;
		this->stageSelectStart = Vector3(start.x, start.y, start.z);
		this->stageSelectEnd = Vector3(end.x, end.y, end.z);
		this->stageSelectMaxTime = Time;
		stageSelectEasingTime = 0;
		stageFlag == true;
	}
}

XMFLOAT3 Directing::stageSelectEasing()
{
	stageSelectEasingTime++;

	stageSelectTimeRate = min(stageSelectEasingTime / stageSelectMaxTime, 1.0f);

	Vector3 position = easeOut(stageSelectStart, stageSelectEnd, stageSelectTimeRate);
	/*position = easeIn(start, end, timeRate); */
		//position = easeInOut(start, end, timeRate);
	if (stageSelectTimeRate >= 1.0f)
	{
		stageSelectFlag = true;
	}


	return XMFLOAT3(position.x, stageSelectStart.y, stageSelectStart.z);
}

void Directing::StageSelectUpdate()
{
	if (stageSelectFlag == false)
	{//ステージセレクトイージング
		stageSlectPos = stageSelectEasing();
	}
}

void Directing::StageSelectDraw(const int score[], const int medal[], int window_width, int window_height)
{
	int starOfsetX = 433, starOfsetY = 383;
	int starR = 120, starDistance = 145;
	int medalOfsetX = 554, medalOfsetY = 531;
	int medalR = 51, medalDistance = 60;

	int stageNumOfsetX = 780, stageNumOfsetY = 215;
	int stageNumR = 70;

	int stageNumXY = 70;

	int numberOfsetX = 820, numberOfsetY = 340;

	for (int i = -1; i < 6; i++)
	{//ステージセレクト
		draw->DrawTextrue(stageSlectPos.x + window_width * i, 0, window_width, window_height, 0, SelectGraph, XMFLOAT2(0.0f, 0.0f));

		if (i == -1)
		{//ステージ５番目
			for (int j = 0; j < 3/*star[5]*/; j++)
			{
				if (score[4] >= clearScore[j])
				{
					draw->DrawTextrue(stageSlectPos.x + window_width * i + starOfsetX + starDistance * j,
						starOfsetY, starR, starR, 0, starGraph, XMFLOAT2(0.0f, 0.0f));
				}
			}
			for (int j = 0; j < medal[4]; j++)
			{
				draw->DrawTextrue(stageSlectPos.x + window_width * i + medalOfsetX + medalDistance * j,
					medalOfsetY, medalR, medalR, 0, medalGraph, XMFLOAT2(0.0f, 0.0f));
			}
			//スコア数字
			NumberDraw(score[4], stageSlectPos.x + window_width * i + numberOfsetX, numberOfsetY, 60, 60);
			//パーセント
			draw->DrawTextrue(stageSlectPos.x + window_width * i + 850,
				310, stageNumXY, stageNumXY, 0, percentGraph, XMFLOAT2(0.0f, 0.0f));
			//ステージ番号
			draw->DrawCutTextrue(stageSlectPos.x + stageNumOfsetX + window_width * i, stageNumOfsetY, stageNumR, stageNumR,
				XMFLOAT2(stageNumXY * 5, 0), XMFLOAT2(stageNumXY, stageNumXY), 0, stageNumberGraph);
		}
		else if (i == 5)
		{//ステージ1番目
			for (int j = 0; j < 3/*star[0]*/; j++)
			{
				if (score[0] >= clearScore[j])
				{
					draw->DrawTextrue(stageSlectPos.x + window_width * i + starOfsetX + starDistance * j,
						starOfsetY, starR, starR, 0, starGraph, XMFLOAT2(0.0f, 0.0f));
				}
			}
			for (int j = 0; j < medal[0]; j++)
			{
				draw->DrawTextrue(stageSlectPos.x + window_width * i + medalOfsetX + medalDistance * j,
					medalOfsetY, medalR, medalR, 0, medalGraph, XMFLOAT2(0.0f, 0.0f));
			}
			//スコア数字
			NumberDraw(score[0], stageSlectPos.x + window_width * i + numberOfsetX, numberOfsetY, 60, 60);
			//パーセント
			draw->DrawTextrue(stageSlectPos.x + window_width * i + 850,
				310, stageNumXY, stageNumXY, 0, percentGraph, XMFLOAT2(0.0f, 0.0f));
			//ステージ番号
			draw->DrawCutTextrue(stageSlectPos.x + window_width * i + stageNumOfsetX, stageNumOfsetY, stageNumR, stageNumR,
				XMFLOAT2(stageNumXY, 0), XMFLOAT2(stageNumXY, stageNumXY), 0, stageNumberGraph);
		}
		else
		{//ステージ1～5
			for (int j = 0; j < 3; j++)
			{//星
				if (score[i] >= clearScore[j])
				{
					draw->DrawTextrue(stageSlectPos.x + window_width * i + starOfsetX + starDistance * j,
						starOfsetY, starR, starR, 0, starGraph, XMFLOAT2(0.0f, 0.0f));
				}
			}
			for (int j = 0; j < medal[i]; j++)
			{//メダル
				draw->DrawTextrue(stageSlectPos.x + window_width * i + medalOfsetX + medalDistance * j,
					medalOfsetY, medalR, medalR, 0, medalGraph, XMFLOAT2(0.0f, 0.0f));
			}
			//スコア数字
			NumberDraw(score[i], stageSlectPos.x + window_width * i + numberOfsetX, numberOfsetY, 60, 60);
			//パーセント
			draw->DrawTextrue(stageSlectPos.x + window_width * i + 850,
				310, stageNumXY, stageNumXY, 0, percentGraph, XMFLOAT2(0.0f, 0.0f));

			//ステージ番号
			draw->DrawCutTextrue(stageSlectPos.x + window_width * i + stageNumOfsetX, stageNumOfsetY, stageNumR, stageNumR,
				XMFLOAT2(stageNumXY * i + stageNumXY, 0), XMFLOAT2(stageNumXY, stageNumXY), 0, stageNumberGraph);
		}
	}
	draw->DrawTextrue(50, 50, 96 + selectTitleR, 96 + selectTitleR, 0, SelectBackBotan);
	//左右の→
	draw->DrawTextrue(0.0f, 0.0f, (float)window_width, (float)window_height, 0.0f, SelectBotanGraph, XMFLOAT2(0.0f, 0.0f));
}


void Directing::SelectTitle()
{
	if (selectTitleFlag == true)
	{
		selectTitleR++;
		if (selectTitleR >= 10)
		{
			selectTitleFlag = false;
		}
	}
	else
	{
		selectTitleR--;
		if (selectTitleR <= -10)
		{
			selectTitleFlag = true;
		}
	}
}

void Directing::NumberDraw(const int score, const  int posX, const int posY, const int width, const int height)
{
	int stageNumXY = 70;
	int numDistance = 70;
	//スコアによって変わる
	for (int i = 0; i < 10; i++)
	{
		if (score % 10 == i)
		{
			draw->DrawCutTextrue((float)posX, (float)posY, (float)width, (float)height,
				XMFLOAT2((float)(stageNumXY * i), 0.0f), XMFLOAT2((float)stageNumXY, (float)stageNumXY), 0, scoreNumberGraph);
		}

	}
	for (int i = 0; i < 10; i++)
	{
		if (score / 10 % 10 == i)
		{
			draw->DrawCutTextrue((float)(posX - numDistance), (float)posY, (float)width, (float)height,
				XMFLOAT2((float)(stageNumXY * i), 0.0f), XMFLOAT2((float)stageNumXY, (float)stageNumXY), 0, scoreNumberGraph);
		}

	}
	for (int i = 0; i < 10; i++)
	{
		if (score / 100 % 10 == i)
		{
			draw->DrawCutTextrue((float)(posX - numDistance * 2), (float)posY, (float)width, (float)height,
				XMFLOAT2((float)(stageNumXY * i), 0.0f), XMFLOAT2((float)stageNumXY, (float)stageNumXY), 0, scoreNumberGraph);
		}
	}
}
void Directing::ClearOverSelect()
{
	if (clearOverSelectFlag == true)
	{
		clearOverSelectR++;
		if (clearOverSelectR >= 15)
		{
			clearOverSelectFlag = false;
		}
	}
	else
	{
		clearOverSelectR--;
		if (clearOverSelectR < 0)
		{
			clearOverSelectFlag = true;
		}
	}
}
void Directing::GameOverButtonDraw(XMFLOAT3 scorePos, int selectRetryFlag)
{

	ClearOverSelect();
	if (selectRetryFlag)
	{
		//リトライボタン
		draw->DrawTextrue(512, 445 + scorePos.y, 110+clearOverSelectR, 110 + clearOverSelectR, 0, retryButtonGraph);
		//セレクトボタン
		draw->DrawTextrue(766, 445 + scorePos.y, 110 , 110, 0, selectbuttonGraph);
	}
	else
	{
		//セレクトボタン
		draw->DrawTextrue(766, 445 + scorePos.y, 110 + clearOverSelectR, 110 + clearOverSelectR, 0, selectbuttonGraph);
		//リトライボタン
		draw->DrawTextrue(512, 445 + scorePos.y, 110 , 110 , 0, retryButtonGraph);
	}
}
const  DirectX::XMFLOAT3 operator+(const  DirectX::XMFLOAT3 &lhs, const  DirectX::XMFLOAT3 rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}

const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3 &lhs, const DirectX::XMFLOAT3 rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	return result;
}

const DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4 &lhs, const DirectX::XMFLOAT4 rhs)
{
	XMFLOAT4 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	result.w = lhs.w + rhs.w;
	return result;
}

const DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4 &lhs, const DirectX::XMFLOAT4 rhs)
{
	XMFLOAT4 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	result.w = lhs.w - rhs.w;
	return result;
}
