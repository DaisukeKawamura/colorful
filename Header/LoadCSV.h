#pragma once

#define MAP_WIDTH (100) //���̍ő喇��
#define MAP_HEIGHT (6)  //�c�̍ő喇��

enum ObjectStatus
{
	NONE,            //��
	BLOCK,           //���A��
	ITEM,            //�A�C�e��
	RING,            //�F�ς������O
	COLOR_WALL,      //�F���L���Ă�΃_���[�W���󂯂Ȃ���
	BREAK_RING,      //��RING
	BREAK_ITEM,      //��ITEM
	BREAK_COLOR_WALL //��COLOR_WALL
};

/// <summary>
/// CSV�t�@�C���̓ǂݍ���
/// </summary>
/// <param name="map"> �ǂݍ���CSV�t�@�C���̕ۑ��� </param>
/// <param name="FilePath"> CSV�t�@�C���̃p�X </param>
/// <param name="LoadStopNumber"> �����Ɏw�肳�ꂽ�l���q�b�g����Ɠǂݍ��݂��I������ </param>
/// <returns> 0�ȊO�Ŏ��s�A0�Ő��� </returns>
int LoadCSV(int map[MAP_HEIGHT][MAP_WIDTH], const char* FilePath, int LoadStopNumber = -1);
