#pragma once

#define MAP_WIDTH (200) //���̍ő喇��
#define MAP_HEIGHT (8)  //�c�̍ő喇��

enum ObjectStatus
{
	NONE,          //��
	BLOCK,         //�u���b�N
	NOPAINT_BLOCK, //�h��Ȃ��u���b�N
	FLOOR,         //�����炷�蔲���\��
	NOPAINT_FLOOR, //�h��Ȃ����蔲���\��
	ITEM,          //�A�C�e��
	RING,          //�F�ς������O
	BREAK_ITEM,    //���A�C�e��
	BREAK_RING,    //��RING
	RED_BLOCK,     //�ԐF�u���b�N
	BLUE_BLOCK,    //�F�u���b�N
	GREEN_BLOCK,   //�ΐF�u���b�N
	YELLOW_BLOCK,  //���F�u���b�N
	RED_FLOOR,     //�ԐF�̏�
	BLUE_FLOOR,    //�F�̏�
	GREEN_FLOOR,   //�ΐF�̏�
	YELLOW_FLOOR,  //���F�̏�
	WARP,          //���[�v
	COLLECTION     //�R���N�V����
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
