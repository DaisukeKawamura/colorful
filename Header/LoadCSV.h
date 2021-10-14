#pragma once

#define MAP_WIDTH (100) //横の最大枚数
#define MAP_HEIGHT (6)  //縦の最大枚数

enum ObjectStatus
{
	NONE,            //空
	BLOCK,           //床、壁
	ITEM,            //アイテム
	RING,            //色変えリング
	COLOR_WALL,      //色が有ってればダメージを受けない壁
	BREAK_RING,      //元RING
	BREAK_ITEM,      //元ITEM
	BREAK_COLOR_WALL //元COLOR_WALL
};

/// <summary>
/// CSVファイルの読み込み
/// </summary>
/// <param name="map"> 読み込んだCSVファイルの保存先 </param>
/// <param name="FilePath"> CSVファイルのパス </param>
/// <param name="LoadStopNumber"> ここに指定された値がヒットすると読み込みを終了する </param>
/// <returns> 0以外で失敗、0で成功 </returns>
int LoadCSV(int map[MAP_HEIGHT][MAP_WIDTH], const char* FilePath, int LoadStopNumber = -1);
