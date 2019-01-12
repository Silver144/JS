#include "GThread2.h"

void GThread2::run()
{
	init();
	int begin = clock();
	FILE *fPtr = fopen("outputFix.txt", "w");
	generate();
	gene finalSeq = out();
	Ftime = calculate_time(length, finalSeq.sequence, LFlag);
	output_file(length, finalSeq.sequence, fPtr, LFlag);
	int end = clock();
	double cost = (double)(end - begin) / CLOCKS_PER_SEC;
	fprintf(fPtr, "Time Used: %.3lf\r\n", cost);
	fprintf(fPtr, "Ended Time: %d\r\n", finalSeq.time);
	fclose(fPtr);
	LFlag2 = 1;
	emit finishF2();
}