#include "./Header/LoadCSV.h"
#include <stdio.h>

int LoadCSV(int map[MAP_HEIGHT][MAP_WIDTH], const char* FilePath, int LoadStopNumber)
{
	if (map == nullptr)
	{
		return -1;
	}

	for (size_t y = 0; y < MAP_HEIGHT; y++)
	{
		for (size_t x = 0; x < MAP_WIDTH; x++)
		{
			// ������
			map[y][x] = 0;
		}
	}

	FILE* fileHandle;
	errno_t err;
	char string[256] = { 0 };

	err = fopen_s(&fileHandle, FilePath, "r");
	if (err != 0)
	{
		return err;
	}

	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		bool end = false;
		fgets(string, 256, fileHandle);
		for (int x = 0, j = 0; x < MAP_WIDTH; j++)
		{
			if (string[j] == '\0' || string[j] == '\n')
			{
				// ���s
				break;
			}
			if (string[j] == ',')
			{
				// ���̐�����
				x++;
			}
			else
			{
				map[y][x] *= 10;
				map[y][x] += string[j] - '0';

				if (map[y][x] == LoadStopNumber)
				{
					// �q�b�g������A�ǂݍ��݂������I�ɏI������B
					end = true;
					break;
				}
			}
		}
		if (end)
		{
			break;
		}
	}
	fclose(fileHandle);

	return 0;
}
