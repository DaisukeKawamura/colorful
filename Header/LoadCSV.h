#pragma once

#define MAP_WIDTH (200) //横の最大枚数
#define MAP_HEIGHT (8)  //縦の最大枚数

enum ObjectStatus
{
	NONE,          //空
	BLOCK,         //ブロック
	NOPAINT_BLOCK, //塗れないブロック
	FLOOR,         //下からすり抜け可能床
	NOPAINT_FLOOR, //塗れないすり抜け可能床
	ITEM,          //アイテム
	RING,          //色変えリング
	BREAK_ITEM,    //元アイテム
	BREAK_RING,    //元RING
	RED_BLOCK,     //赤色ブロック
	BLUE_BLOCK,    //青色ブロック
	GREEN_BLOCK,   //緑色ブロック
	YELLOW_BLOCK,  //黄色ブロック
	RED_FLOOR,     //赤色の床
	BLUE_FLOOR,    //青色の床
	GREEN_FLOOR,   //緑色の床
	YELLOW_FLOOR,  //黄色の床
	WARP,          //ワープ
	COLLECTION     //コレクション
};

/// <summary>
/// CSVファイルの読み込み
/// </summary>
/// <param name="map"> 読み込んだCSVファイルの保存先 </param>
/// <param name="FilePath"> CSVファイルのパス </param>
/// <param name="LoadStopNumber"> ここに指定された値がヒットすると読み込みを終了する </param>
/// <returns> 0以外で失敗、0で成功 </returns>
int LoadCSV(int map[MAP_HEIGHT][MAP_WIDTH], const char* FilePath, int LoadStopNumber = -1);

/// <summary>
/// CSVファイルの読み込み(1次元配列)
/// </summary>
/// <param name="mapArray"> 読み込んだCSVファイルの保存先 </param>
/// <param name="FilePath"> CSVファイルのパス </param>
/// <param name="LoadStopNumber"> ここに指定された値がヒットすると読み込みを終了する </param>
/// <returns> 0以外で失敗、0で成功 </returns>
int LoadCSV1D(int* mapArray, const size_t& mapSize, const char* FilePath, int LoadStopNumber = -1);
