/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "StepDetector.h"
#include "StepCount.h"
/*
* 根据StepDetector传入的步点"数"步子
* */

int count = 0;
int mCount = 0;
static long timeOfLastPeak = 0;
static long timeOfThisPeak = 0;
bool flag = 0;

/*
* 连续走十步才会开始计步
* 连续走了9步以下,停留超过3秒,则计数清空
* */

void countStep(void) 
{
	timeOfLastPeak = timeOfThisPeak;
	timeOfThisPeak = StepDetector_tick;
	if (timeOfThisPeak - timeOfLastPeak <= 300){
		if(count<9){
			count++;
		}else if(count == 9){
			count++;
			mCount += count;
		}else{
			mCount++;
		}
		flag=1;
	}else{//超时
		count = 1;//为1,不是0
	}
}



void setSteps(int initValue) 
{
	mCount = initValue;
	count = 0;
	timeOfLastPeak = 0;
	timeOfThisPeak = 0;
}


