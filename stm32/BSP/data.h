#ifndef __DATA_H__
#define __DATA_H__

#include "includes.h"

/* �ṹ�� */
struct carData
{
//	char name[8];
//	char ID[10];
//	char carID[8];

	char name[8];
	char ID[10];
	char carID[8];
	char stWeight;
	char weight;
};

void save_in_buffer(struct carData CAR, char ADweight);

#endif
