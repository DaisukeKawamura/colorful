#pragma once

#define MAP_WIDTH (100) //横の最大枚数
#define MAP_HEIGHT (6)  //縦の最大枚数

enum ObjectStatus
{
	NONE,            //空
	BLOCK,           //床、壁
	Floor,			 //下からすり抜け可能床
	ITEM,            //アイテム
	RING,            //色変えリング
	BREAK_ITEM,		 //元アイテム
	BREAK_RING,      //元RING
	RedBLOCK,		 //赤色ブロック
	BlueBLOCK,		 //青色ブロック
	GreenBLOCK,		 //緑色ブロック
	YellowBLOCK,	 //黄色ブロック
	RedFloor,		 //赤色の床
	BlueFloor,		 //青色の床
	GreenFloor,		 //緑色の床
	YellowFloor,	 //黄色の床
};

/// <summary>
/// CSVファイルの読み込み
/// </summary>
/// <param name="map"> 読み込んだCSVファイルの保存先 </param>
/// <param name="FilePath"> CSVファイルのパス </param>
/// <param name="LoadStopNumber"> ここに指定された値がヒットすると読み込みを終了する </param>
/// <returns> 0以外で失敗、0で成功 </returns>
int LoadCSV(int map[MAP_HEIGHT][MAP_WIDTH], const char* FilePath, int LoadStopNumber = -1);
