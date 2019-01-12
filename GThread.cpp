#include "GThread.h"
#pragma execution_character_set("utf-8")

gene pop[3000], finalSeq;
job jobs[MAX_NUM];
block blocks[MAX_NUM][MAX_NUM];
int jobNum, macNum, all_num, parents, length, flag, x_all, Ftime;

int gapNum[MAX_NUM], lastTime[MAX_NUM], start[MAX_NUM][MAX_NUM], end[MAX_NUM][MAX_NUM], last[MAX_NUM];

int onum[MAX_NUM];
int ostart[MAX_NUM][MAX_NUM];
int oend[MAX_NUM][MAX_NUM];
int jobDone[MAX_NUM];
int LFlag, LFlag2, TFlag, CFlag;

void insert_repair(double start, int total, int machine_num) {
	machine_num++;
	int operation_num = 0;
	int start_time;
	int push_time;
	int cur_num;
	for (cur_num = 0; cur_num < onum[machine_num]; cur_num += 1) {
		if (blocks[machine_num][cur_num].start >= start) {
			operation_num = cur_num;
			break;
		}
	}
	if (operation_num == 0) start_time = start;
	else if (start < blocks[machine_num][operation_num - 1].end) {
		start_time = blocks[machine_num][operation_num - 1].end;
	}
	else {
		start_time = start;
	}

	if (start_time + total > blocks[machine_num][operation_num].start)
		push_time = start_time + total - blocks[machine_num][operation_num].start;

	onum[machine_num]++;

	for (cur_num = onum[machine_num] - 1; cur_num > operation_num; cur_num--)
		blocks[machine_num][cur_num] = blocks[machine_num][cur_num - 1];
	blocks[machine_num][operation_num].start = start_time;
	blocks[machine_num][operation_num].end = start_time + total;
	blocks[machine_num][operation_num].jobRec = -1;
	if (push_time == 0) return;
	for (cur_num = operation_num + 1; cur_num < onum[machine_num]; cur_num++) {
		blocks[machine_num][cur_num].start += push_time;
		blocks[machine_num][cur_num].end += push_time;
	}
	int cur_mac;
	for (cur_mac = 1; cur_mac <= macNum; cur_mac++) {
		if (cur_mac == machine_num) continue;
		for (cur_num = 0; cur_num < onum[cur_mac]; cur_num++) {
			if (blocks[cur_mac][cur_num].start > start_time) {
				blocks[cur_mac][cur_num].start += push_time;
				blocks[cur_mac][cur_num].end += push_time;
			}
		}
	}
	int temp = Ftime;
	Ftime += push_time;
	EVER_TIME = (double)EVER_TIME * (double)temp / (double)Ftime;
}

void insert_fix_out(int start_time, int end_time, int mac_num)
{
	if (start_time != 0) {
		gapNum[mac_num]++;
		start[mac_num][0] = 0;
		end[mac_num][0] = start_time;
	}
	ostart[mac_num + 1][0] = start_time;
	oend[mac_num][0] = end_time;
	onum[mac_num]++;
	lastTime[mac_num] = end_time;
	blocks[mac_num][0].start = start_time;
	blocks[mac_num][0].end = end_time;
	blocks[mac_num][0].jobRec = -1;
}

void insert_fix(int start_time, int end_time, int mac_num)
{
	if (start_time != 0) {
		gapNum[mac_num]++;
		start[mac_num][0] = 0;
		end[mac_num][0] = start_time;
	}
	lastTime[mac_num] = end_time;
}

int insert_one(int i, int j)
{
	int k;
	int startTime, endTime;
	int mac = jobs[i].mac[j];
	int flag = 0;
	int tm = jobs[i].time[j];
	for (k = 0; k < gapNum[mac]; k++)
	{
		if (flag)
			break;
		if (end[mac][k] - last[i] < tm)
			continue;
		if (end[mac][k] - start[mac][k] < tm)
			continue;
		flag = 1;

		if (last[i] >= start[mac][k])
			startTime = last[i];
		else
			startTime = start[mac][k];
		int temp = end[mac][k];
		end[mac][k] = startTime;
		endTime = tm + startTime;

		int l;
		for (l = gapNum[mac]; l > k + 1; l--)
		{
			start[mac][l] = start[mac][l - 1];
			end[mac][l] = end[mac][l - 1];
		}
		start[mac][k + 1] = endTime;
		end[mac][k + 1] = temp;
		gapNum[mac]++;
		last[i] = start[mac][k + 1];
	}
	if (!flag)
	{
		if (lastTime[mac] < last[i])
		{

			end[mac][gapNum[mac]] = last[i];
			start[mac][gapNum[mac]] = lastTime[mac];
			lastTime[mac] = last[i] + tm;
			last[i] += tm;
			gapNum[mac]++;
			return lastTime[mac];
		}
		lastTime[mac] += tm;
		last[i] = lastTime[mac];
	}
	return lastTime[mac];
}

//Calculate the time of every sequence of gene
int calculate_time(int n, int *_gene, int flag)
{
	int i, j;
	int *jobDone = (int *)calloc(MAX_NUM, sizeof(int));

	int ret = 0;

	memset(gapNum, 0, sizeof(gapNum));
	memset(lastTime, 0, sizeof(lastTime));
	memset(start, 0, sizeof(start));
	memset(end, 0, sizeof(end));
	memset(last, 0, sizeof(last));

	if (flag) {
		for (i = 0; i < fix_num; i++) {
			insert_fix(fix[i].start, fix[i].end, fix[i].ope_num + 1);
		}
	}

	for (i = 0; i < n; i++) {
		int num = jobDone[_gene[i]];
		jobDone[_gene[i]]++;
		int tim = insert_one(_gene[i], num);
		if (tim > ret)
			ret = tim;
	}
	free(jobDone);
	return ret;
}

//If the flag equals 0, then choose the best parent, else choose the best child
int s_best(int flag)
{
	int temp;
	switch (flag)
	{
	case 0:
	{
		temp = 0;
		for (int i = 1; i < parents; i++)
			if (pop[i].time < pop[temp].time)
				temp = i;
		return temp;
	}
	case 1:
	{
		temp = parents;
		for (int i = parents; i < all_num; i++)
			if (pop[i].time < pop[temp].time)
				temp = i;
		return temp;
	}
	}
	return 1;
}

//Calculate the length of sequence of gene
void cal_len()
{
	int len = 0;
	for (int i = 0; i < jobNum; i++)
		len += jobs[i].n;
	length = len;
}

/*To select the best parent and best child, if best parent is better than child, then replace
one of the children with the best parent*/
void select_func()
{
	int bestPar = s_best(0), bestChi = s_best(1);
	if (pop[bestPar].time < pop[bestChi].time)
	{
		if (parents < all_num)
		{
			int choice, flag = 1;
			for (;;)
			{
				unsigned long long r = rand() * rand();
				choice = r % all_num;
				if (choice >= parents)
				{
					pop[choice] = pop[bestPar];
					break;
				}
			}
		}
	}
}

//Judge whether the number of arrry cha whose length is len is all zero 
int is_zero(int *cha, int len)
{
	int flag = 1;
	for (int i = 0; i < len; i++)
		if (cha[i] != 0)
			return 0;
	return 1;
}


void encrypt_one(int num)
{
	pop[num].caled = 0;
	pop[num].alive = 1;
	pop[num].haveBeenCrossed = 0;
	int jobN[30], gene_len = 0;
	memset(jobN, 0, sizeof(jobN));
#pragma omp parallel for
	for (int i = 0; i < jobNum; i++)
		jobN[i] = jobs[i].n;
	while (!is_zero(jobN, jobNum))
	{
		unsigned long long r = rand() * rand();
		int temp = r % jobNum;
		if (jobN[temp] != 0)
		{
			pop[num].sequence[gene_len++] = temp;
			jobN[temp]--;
		}
	}
	return;
}

// Generate the whole gene pool with the population of ENCRYPT_NUM
void encrypt_main()
{
	for (int i = 0; i < ENCRYPT_NUM; i++)
		encrypt_one(i);
	x_all = all_num = parents = ENCRYPT_NUM;
}



//Calculate the probability of living of every gene
void cal_p()
{
	double base = 0;
	for (int i = 0; i < all_num; i++)
		base += pop[i].fitness;
	base = base / all_num * DIE_RATIO;
	for (int i = 0; i < all_num; i++)
		pop[i].live_p = pop[i].fitness / base;
}

void init()
{
	encrypt_main();
	cal_len();
	for (int i = 0; i < all_num; i++)
	{
		pop[i].time = calculate_time(length, pop[i].sequence, LFlag);
		pop[i].caled = 1;
		pop[i].fitness = 1 / (double)(pop[i].time + 1);
	}
}



int cmp(const void *a, const void *b)
{
	return *((int *)a) - *((int *)b);
}

void insert(int i, int j) {
	int mac = jobs[i].mac[j];
	int flag = 0;

	int startTime, endTime;
	int temp;
	int l, tm;
	tm = jobs[i].time[j];
	int k;
	for (k = 0; k < gapNum[mac]; k++) {
		if (flag) break;
		if (end[mac][k] - last[i] < tm) continue;
		if (end[mac][k] - start[mac][k] < tm) continue;
		flag = 1;

		for (l = 0; l < onum[mac]; l++) {
			if (blocks[mac][l].start >= end[mac][k]) break;
		}

		temp = l;

		for (l = onum[mac]; l > temp; l--) {
			blocks[mac][l].start = blocks[mac][l - 1].start;
			blocks[mac][l].end = blocks[mac][l - 1].end;
			blocks[mac][l].jobRec = blocks[mac][l - 1].jobRec;
			blocks[mac][l].opeRec = blocks[mac][l - 1].opeRec;
		}

		if (last[i] >= start[mac][k]) {
			startTime = last[i];
			blocks[mac][temp].start = last[i];
			blocks[mac][temp].end = last[i] + tm;
			blocks[mac][temp].jobRec = i;
			blocks[mac][temp].opeRec = j;
		}
		else {
			startTime = start[mac][k];
			blocks[mac][temp].start = startTime;
			blocks[mac][temp].end = startTime + tm;
			blocks[mac][temp].jobRec = i;
			blocks[mac][temp].opeRec = j;
		}
		temp = end[mac][k];
		end[mac][k] = startTime;
		endTime = tm + startTime;

		for (l = gapNum[mac]; l > k + 1; l--) {
			start[mac][l] = start[mac][l - 1];
			end[mac][l] = end[mac][l - 1];
		}
		start[mac][k + 1] = endTime;
		end[mac][k + 1] = temp;
		gapNum[mac]++;
		onum[mac]++;
		last[i] = start[mac][k + 1];
	}
	if (!flag) {
		if (lastTime[mac] < last[i]) {
			blocks[mac][onum[mac]].start = last[i];
			blocks[mac][onum[mac]].end = last[i] + tm;
			blocks[mac][onum[mac]].jobRec = i;
			blocks[mac][onum[mac]].opeRec = j;
			onum[mac]++;
			end[mac][gapNum[mac]] = last[i];
			start[mac][gapNum[mac]] = lastTime[mac];
			lastTime[mac] = last[i] + tm;
			last[i] += tm;
			gapNum[mac]++;
			return;
		}
		blocks[mac][onum[mac]].start = lastTime[mac];
		blocks[mac][onum[mac]].end = lastTime[mac] + tm;
		blocks[mac][onum[mac]].jobRec = i;
		blocks[mac][onum[mac]].opeRec = j;
		onum[mac]++;
		lastTime[mac] += tm;
		last[i] = lastTime[mac];
	}
}

void output(int n, int *seq, int flag)
{

	int i, j, k;
	for (i = 0; i < MAX_NUM; i++)
	{
		gapNum[i] = 0;
		lastTime[i] = 0;
		onum[i] = 0;
		jobDone[i] = 0;
		last[i] = 0;
		for (j = 0; j < MAX_NUM; j++)
		{
			start[i][j] = end[i][j] = 0;
			blocks[i][j].start = blocks[i][j].end = blocks[i][j].jobRec = blocks[i][j].opeRec = 0;
			oend[i][j] = ostart[i][j] = 0;
		}
	}

	if (flag)
	{
		for (i = 0; i < fix_num; i++)
		{
			insert_fix_out(fix[i].start, fix[i].end, fix[i].ope_num + 1);
		}
	}

	for (i = 0; i < n; i++)
	{
		int num = jobDone[seq[i]];
		int mac_1 = jobs[seq[i]].mac[num];
		jobDone[seq[i]]++;
		insert(seq[i], num);
	}

	for (i = 1; i <= macNum; i++)
	{
		printf("M%d", i);
		for (j = 0; j < onum[i]; j++)
		{
			if (blocks[i][j].start == blocks[i][j].end)
				continue;
			if (blocks[i][j].jobRec == -1) printf(" (%d,¼ìÐÞ,%d)", blocks[i][j].start, blocks[i][j].end);
			else printf(" (%d,%d-%d,%d)", blocks[i][j].start, blocks[i][j].jobRec, blocks[i][j].opeRec, blocks[i][j].end);
		}
		printf("\n");
	}
}


void die()
{
	if (all_num <= MAX_POP)
		return;
	while (all_num > MAX_POP)
	{
		unsigned long long r = rand() * rand();
		int x = r % all_num;
		int px = 10000 * pop[x].live_p;
		r = rand() * rand();
		int y = r % 10000;
		if (y > px)
		{
			pop[x].alive = 0;
			all_num--;
		}
	}
}

// Replace all the genes that is no longer alive
void replace()
{
	int fron = 0, rear = x_all - 1;
	while (fron < rear)
	{
		while (fron < rear && pop[fron].alive == 1)
			fron++;
		while (fron < rear && pop[rear].alive == 0)
			rear--;
		pop[fron] = pop[rear];
		pop[rear].alive = 0;
	}
}


//Output the answer to file
void output_file(int n, int *seq, FILE *fPtr, int flag)
{
	int i, j, k;
	if (!flag)
		for (i = 0; i < MAX_NUM; i++)
		{
			gapNum[i] = 0;
			lastTime[i] = 0;
			onum[i] = 0;
			jobDone[i] = 0;
			last[i] = 0;
			for (j = 0; j < MAX_NUM; j++)
			{
				start[i][j] = end[i][j] = 0;
				blocks[i][j].start = blocks[i][j].end = blocks[i][j].jobRec = blocks[i][j].opeRec = 0;
				oend[i][j] = ostart[i][j] = 0;
			}
		}
	if (!flag)
		for (i = 0; i < n; i++)
		{
			int num = jobDone[seq[i]];
			int mac_1 = jobs[seq[i]].mac[num];
			jobDone[seq[i]]++;
			insert(seq[i], num);
		}

	for (i = 1; i <= macNum; i++)
	{
		fprintf(fPtr, "M%d", i - 1);
		for (j = 0; j < onum[i]; j++)
		{
			if (blocks[i][j].start == blocks[i][j].end)
				continue;
			if (blocks[i][j].jobRec == -1) fprintf(fPtr, " (%d,¼ìÐÞ,%d)", blocks[i][j].start, blocks[i][j].end);
			else fprintf(fPtr, " (%d,%d-%d,%d)", blocks[i][j].start, blocks[i][j].jobRec, blocks[i][j].opeRec, blocks[i][j].end);
		}
		fprintf(fPtr, "\r\n");
	}
	fprintf(fPtr, "End Time: %d\r\n", Ftime);
}


gene cross_once(int which)
{
	int a1 = 0, a2 = 0, b1 = 0, b2 = 0;
	int y[4];
	while (!(a1 != a2 && a1 != b1 && a1 != b2 && a2 != b1 && a2 != b2 && b1 != b2))
	{
		unsigned long long r = rand() * rand();
		a1 = r % length;
		r = rand() * rand();
		a2 = r % length;
		r = rand() * rand();
		b1 = r % length;
		r = rand() * rand();
		b2 = r % length;
	}
	y[0] = a1, y[1] = a2, y[2] = b1, y[3] = b2;
	qsort(y, 4, sizeof(int), cmp);
	a1 = y[0], a2 = y[1], b1 = y[2], b2 = y[3];
	int newBorn[625];
	int i;
	for (i = 0; i < a1; i++)
		newBorn[i] = pop[which].sequence[i];
	for (; i < a1 + b2 - b1 + 1; i++)
		newBorn[i] = pop[which].sequence[i - a1 + b1];
	for (; i < a1 - a2 + b2; i++)
		newBorn[i] = pop[which].sequence[i - (a1 + b2 - b1 + 1) + a2 + 1];
	for (; i < b2 + 1; i++)
		newBorn[i] = pop[which].sequence[i - (a1 - a2 + b2) + a1];
	for (; i < length; i++)
		newBorn[i] = pop[which].sequence[i];
	gene x = pop[which];
	for (int i = 0; i < length; i++)
		x.sequence[i] = newBorn[i];
	x.caled = 0;
	return x;
}

// Select genes randomly and cross them with the total amount of all_num / GER_MUL
void cross()
{
	int randNum;
	int crossed[2000];
	memset(crossed, 0, sizeof(crossed));
	for (int i = 0; i < all_num / GER_MUL; i++)
	{
		unsigned long long r = rand() * rand();
		randNum = r % all_num;
		if (crossed[randNum] == false)
		{
			pop[all_num] = cross_once(randNum);
			crossed[randNum] = true;
			all_num++;
		}
	}
	for (int i = 0; i < all_num; i++)
	{
		if (pop[i].caled)
			continue;
		pop[i].time = calculate_time(length, pop[i].sequence, LFlag);
		pop[i].caled = 1;
		pop[i].fitness = 1 / (double)(pop[i].time + 1);
	}
}

// Proceed the mutation operation on the gene with the number "num"
void mutate(int num)
{
	int a1 = 0, a2 = 0, a3 = 0;
	while (!((a1 != a2) && (a2 != a3) && (a3 != a1)))
	{
		unsigned long long r = rand() * rand();
		a1 = r % length;
		r = rand() * rand();
		a2 = r % length;
		r = rand() * rand();
		a3 = r % length;
	}

	// Generating 
	gene x[6];
	x[0] = x[1] = x[2] = x[3] = x[4] = x[5] = pop[num];
	x[1].sequence[a2] = pop[num].sequence[a3], x[1].sequence[a3] = pop[num].sequence[a2];
	x[2].sequence[a1] = pop[num].sequence[a2], x[2].sequence[a2] = pop[num].sequence[a1];
	x[3].sequence[a1] = pop[num].sequence[a2], x[3].sequence[a2] = pop[num].sequence[a3], x[3].sequence[a3] = pop[num].sequence[a1];
	x[4].sequence[a1] = pop[num].sequence[a3], x[4].sequence[a2] = pop[num].sequence[a1], x[4].sequence[a3] = pop[num].sequence[a2];
	x[5].sequence[a1] = pop[num].sequence[a3], x[5].sequence[a2] = pop[num].sequence[a2], x[5].sequence[a3] = pop[num].sequence[a1];
	int k = 0, min_time = pop[num].time;
#pragma omp parallel for
	for (int i = 1; i <= 5; i++)
	{
		x[i].time = calculate_time(length, x[i].sequence, LFlag);
		if (min_time > x[i].time)
		{
			k = i;
			min_time = x[i].time;
		}
	}
	x[k].caled = 1;
	x[k].fitness = 1 / (double)(x[k].time + 1);
	pop[num] = x[k];
}

//To get the new gene and replace some with better ones 
void generate()
{
	for (int i = 0; i < GAP; i++)
	{
		cross();
		select_func();
		int n = all_num / MUT_MUL;
		for (int j = 0; j < n; j++)
		{
			unsigned long long r = rand() * rand();
			int num = rand() % all_num;
			mutate(num);
		}
		cal_p();
		x_all = all_num;
		die();
		replace();
		parents = all_num;
	}
}

gene out()
{
	int min = 0x3F3F3F3F, k = 0;
	for (int i = 0; i < all_num; i++)
	{
		if (pop[i].time < min && pop[i].time != 0)
		{
			min = pop[i].time;
			k = i;
		}
		//for(int j = 0; j < length; j++) printf("%d%c", pop[i].sequence[j], j == length - 1 ? '\n' : ' ');
		//printf("%d\n", pop[i].time);
	}
	return pop[k];
}

void GThread::run()
{
	LFlag = 0, LFlag2 = 0, CFlag = 0;
	init();
	int begin = clock();
	FILE *fPtr = fopen("output1.txt", "w");
	generate();
	finalSeq = out();
	Ftime = calculate_time(length, finalSeq.sequence, LFlag);
	output_file(length, finalSeq.sequence, fPtr, LFlag);
	int end = clock();
	double cost = (double)(end - begin) / CLOCKS_PER_SEC;
	fprintf(fPtr, "Time Used: %.3lf\r\n", cost);
	//fprintf(fPtr, "Ended Time: %d\r\n", finalSeq.time);
	fclose(fPtr);
	LFlag = 1;
	TFlag = 0;
	emit finishF();
}