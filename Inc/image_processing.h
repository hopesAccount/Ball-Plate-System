#ifndef _IMAGE_PROESSING_H_
#define _IMAGE_PROESSING_H_ 

#include <math.h>
#include <stdlib.h>
#include "stm32f7xx_hal.h"
#include "main.h"
#include "dcmi.h"
#include "ov2640.h"
#include "sdram.h"
#include "wifi.h"

#define MAX_HIS_LENGTH 256
#define IMAGE_HEIGHT OV2640_IMG_HEIGHT
#define IMAGE_WIDTH OV2640_IMG_WIDTH
#define IMAGE_SIZE ((IMAGE_HEIGHT) * (IMAGE_WIDTH))

#define BLACK 0
#define WHITE 255

#define GRAY_TH_FLOOR 130
#define GRAY_TH_CEILING 140

#define AREA_TH 4

#define MAX_BALL_DISTANCE 20000
#define MAX_REGION_DISTANCE 4500
#define MIN_REGION_DISTANCE 1000

#define MAX_LEN 1000

typedef enum
{
	PRE_SCAN,
	BALL_SCAN,
	WAIT,
}Scan_Status;

typedef struct
{
	uint16_t row;		//行号
	uint16_t col;		//列号
}Position;

typedef struct 
{
	uint16_t nLabel;	//标号
	uint16_t nRow;		//游程所在行号
	uint16_t nStart;	//起始点列号
	uint16_t nEnd;		//终止点列号
}RunUnit;

typedef struct 
{
	RunUnit data[MAX_LEN];
	int16_t last;
}RunLength;

typedef struct 
{
	/*nMarkValue1 > nMarkValue2*/
	uint16_t nMarkValue1;	//标记值1
	uint16_t nMarkValue2;	//标记值2
}EqualMarkUnit;	

typedef struct 
{
	EqualMarkUnit data[MAX_LEN];
	int16_t last;
}EqualMark;

typedef struct 
{
	uint16_t label[MAX_LEN];
	int16_t last;
}Equals;

extern float his[MAX_HIS_LENGTH];

extern RunLength *runList;
extern EqualMark *markList;
extern Equals *equal;
extern __IO Scan_Status thisStatus;
extern Position region[9];
extern Position ballPos[3];

/*------------Exported functions-----------*/
void Img_Process(void);
void Ball_Scan(void);
void Pre_Scan(void);

/*------------Private functions-----------*/
static float Get_Histogram(uint8_t **image, float *his);
static uint8_t Osu_Threshold(float *his, float avgValue);
static void Gray_To_BW(uint8_t **image);
static void Run_Label(uint8_t **image);
static void Equal_Process(uint16_t *equal, uint16_t nValue1, uint16_t nValue2);
static void Mid_Filter(uint8_t **image);
static void Label_Center(uint8_t **image, Position *pos, Scan_Status thisStatus);

#endif
