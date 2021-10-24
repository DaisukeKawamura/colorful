#pragma once

#define MAP_WIDTH (200) //���̍ő喇��
#define MAP_HEIGHT (8)  //�c�̍ő喇��

enum ObjectStatus
{
	NONE,        //��
	BLOCK,       //���A��
	Floor,       //�����炷�蔲���\��
	ITEM,        //�A�C�e��
	RING,        //�F�ς������O
	BREAK_ITEM,  //���A�C�e��
	BREAK_RING,  //��RING
	RedBLOCK,    //�ԐF�u���b�N
	BlueBLOCK,   //�F�u���b�N
	GreenBLOCK,  //�ΐF�u���b�N
	YellowBLOCK, //���F�u���b�N
	RedFloor,    //�ԐF�̏�
	BlueFloor,   //�F�̏�
	GreenFloor,  //�ΐF�̏�
	YellowFloor, //���F�̏�
	COLLECTION   //�R���N�V����
};

/// <summary>
/// CSV�t�@�C���̓ǂݍ���
/// </summary>
/// <param name="map"> �ǂݍ���CSV�t�@�C���̕ۑ��� </param>
/// <param name="FilePath"> CSV�t�@�C���̃p�X </param>
/// <param name="LoadStopNumber"> �����Ɏw�肳�ꂽ�l���q�b�g����Ɠǂݍ��݂��I������ </param>
/// <returns> 0�ȊO�Ŏ��s�A0�Ő��� </returns>
int LoadCSV(int map[MAP_HEIGHT][MAP_WIDTH], const char* FilePath, int LoadStopNumber = -1);

/// <summary>
/// CSV�t�@�C���̓ǂݍ���(1�����z��)
/// </summary>
/// <param name="mapArray"> �ǂݍ���CSV�t�@�C���̕ۑ��� </param>
/// <param name="FilePath"> CSV�t�@�C���̃p�X </param>
/// <param name="LoadStopNumber"> �����Ɏw�肳�ꂽ�l���q�b�g����Ɠǂݍ��݂��I������ </param>
/// <returns> 0�ȊO�Ŏ��s�A0�Ő��� </returns>
int LoadCSV1D(int* mapArray, const size_t& mapSize, const char* FilePath, int LoadStopNumber = -1);
