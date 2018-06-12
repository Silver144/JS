#include "jobshop.h"

jobshop *jobshopC;
GWindow *GWindows;

#define MAX_NUM 25
#define MAX_LEN 600
const int GAP = 300, ENCRYPT_NUM = 400, MAX_POP = 800;
const double GER_MUL = 1.75, DIE_RATIO = 1.1, MUT_MUL = 1;

HHOOK keyHook = NULL;

struct job
{
	int n;
	int time[MAX_NUM];
	int mac[MAX_NUM];
} jobs[MAX_NUM];

typedef struct gene
{
	int sequence[MAX_LEN];
	double fitness;
	double live_p;
	int caled;
	int alive;
	int time;
	int haveBeenCrossed;
}gene;

struct block
{
	int start;
	int end;
	int jobRec;
	int opeRec;
} blocks[MAX_NUM][MAX_NUM];

gene pop[3000];

static int flag = 0;

int jobNum, macNum, all_num, parents, length, x_all, Ftime;

int gapNum[MAX_NUM], lastTime[MAX_NUM], start[MAX_NUM][MAX_NUM], end[MAX_NUM][MAX_NUM], last[MAX_NUM];

int onum[MAX_NUM];
int ostart[MAX_NUM][MAX_NUM];
int oend[MAX_NUM][MAX_NUM];
int jobDone[MAX_NUM];

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

void GWindow::paintEvent(QPaintEvent*)
{
	if (flag == 0) return;
	QPainter painter;
	painter.begin(this);
	QFont font;
	font.setFamily("Microsoft YaHei");
	font.setPointSize(10);
	font.setItalic(true);
	painter.setFont(font);
	int ML[10]{ 300, 350, 400, 450, 500, 550, 600, 650, 700, 750 };
	painter.drawText(100, 300, QStringLiteral("M0"));
	painter.drawText(100, 350, QStringLiteral("M1"));
	painter.drawText(100, 400, QStringLiteral("M2"));
	painter.drawText(100, 450, QStringLiteral("M3"));
	painter.drawText(100, 500, QStringLiteral("M4"));
	painter.drawText(100, 550, QStringLiteral("M5"));
	painter.drawText(100, 600, QStringLiteral("M6"));
	painter.drawText(100, 650, QStringLiteral("M7"));
	painter.drawText(100, 700, QStringLiteral("M8"));
	painter.drawText(100, 750, QStringLiteral("M9"));
	painter.setPen(QPen(Qt::GlobalColor::black, 0, Qt::SolidLine));
	for (int i = 1; i <= macNum; i++)
	{
		for (int j = 0; j < jobNum; j++)
		{
			/*switch (blocks[i][j].jobRec)
			{
			case 1: painter.setPen(QPen(Qt::GlobalColor::darkGray, 0, Qt::SolidLine)); break;
			case 2: painter.setPen(QPen(Qt::GlobalColor::blue, 0, Qt::SolidLine)); break;
			case 3: painter.setPen(QPen(Qt::GlobalColor::color0, 0, Qt::SolidLine)); break;
			case 4: painter.setPen(QPen(Qt::GlobalColor::color1, 0, Qt::SolidLine)); break;
			case 5: painter.setPen(QPen(Qt::GlobalColor::cyan, 0, Qt::SolidLine)); break;
			case 6: painter.setPen(QPen(Qt::GlobalColor::darkBlue, 0, Qt::SolidLine)); break;
			case 7: painter.setPen(QPen(Qt::GlobalColor::darkGreen, 0, Qt::SolidLine)); break;
			case 8: painter.setPen(QPen(Qt::GlobalColor::darkMagenta, 0, Qt::SolidLine)); break;
			case 9: painter.setPen(QPen(Qt::GlobalColor::darkRed, 0, Qt::SolidLine)); break;
			case 0: painter.setPen(QPen(Qt::GlobalColor::darkYellow, 0, Qt::SolidLine)); break;
			}*/
			switch (blocks[i][j].jobRec)
			{
			case 1: painter.setBrush(QBrush(Qt::GlobalColor::darkGray, Qt::SolidPattern)); break;
			case 2: painter.setBrush(QBrush(Qt::GlobalColor::blue, Qt::SolidPattern)); break;
			case 3: painter.setBrush(QBrush(Qt::GlobalColor::color0, Qt::SolidPattern)); break;
			case 4: painter.setBrush(QBrush(Qt::GlobalColor::color1, Qt::SolidPattern)); break;
			case 5: painter.setBrush(QBrush(Qt::GlobalColor::cyan, Qt::SolidPattern)); break;
			case 6: painter.setBrush(QBrush(Qt::GlobalColor::darkBlue, Qt::SolidPattern)); break;
			case 7: painter.setBrush(QBrush(Qt::GlobalColor::darkGreen, Qt::SolidPattern)); break;
			case 8: painter.setBrush(QBrush(Qt::GlobalColor::darkMagenta, Qt::SolidPattern)); break;
			case 9: painter.setBrush(QBrush(Qt::GlobalColor::darkRed, Qt::SolidPattern)); break;
			case 0: painter.setBrush(QBrush(Qt::GlobalColor::darkYellow, Qt::SolidPattern)); break;
			}
			double ra = double(blocks[i][j].start) / double(Ftime), ra1 = double((blocks[i][j].end - blocks[i][j].start)) / double(Ftime);
			int rec1 = ra * 1000 + 150, rec2 = ra1 * 1000;
			painter.drawRect(rec1, ML[i - 1] - 10, rec2, 35);
		}
	}
	//painter.drawRoundRect(100, 100, 40, 100, 10, 10);
	//painter.setBrush(QBrush(Qt::GlobalColor::magenta, Qt::SolidPattern));//设置画刷形式 
	//painter.drawRoundRect(460, 200, 160, 160, 10, 10);
	painter.end();
}


GWindow::GWindow()
{
	setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
	int width = GetSystemMetrics(SM_CXFULLSCREEN);
	int height = GetSystemMetrics(SM_CYFULLSCREEN);
	this->setWindowTitle("Answer");
	this->setWindowFlags(Qt::WindowCloseButtonHint);
	this->setFixedSize(1280, 960);
	this->move((width - 1280) / 2, (height - 960) / 2);
}

//Calculate the time of every sequence of gene
int calculate_time(int n, int *_gene)
{
	int i, j;
	int *jobDone = (int *)calloc(MAX_NUM, sizeof(int));

	int ret = 0;

	memset(gapNum, 0, sizeof(gapNum));
	memset(lastTime, 0, sizeof(lastTime));
	memset(start, 0, sizeof(start));
	memset(end, 0, sizeof(end));
	memset(last, 0, sizeof(last));

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

void input_file()
{
	FILE *fp = fopen("C:\\Users\\Aris\\Desktop\\jobshop\\x64\\Debug\\1.txt", "r");
	fscanf(fp, "%d%d", &jobNum, &macNum);

	for (int i = 0; i < jobNum; i++)
	{
		jobs[i].n = macNum;
		for (int j = 0; j < macNum; j++)
		{
			fscanf(fp, "%d%d", &jobs[i].mac[j], &jobs[i].time[j]);
			jobs[i].mac[j]++;
		}
	}
}

void init()
{
	srand(time(NULL));
	encrypt_main();
	cal_len();
	for (int i = 0; i < all_num; i++)
	{
		pop[i].time = calculate_time(length, pop[i].sequence);
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

void output(int n, int *seq)
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

	for (i = 0; i < n; i++)
	{
		int num = jobDone[seq[i]];
		int mac_1 = jobs[seq[i]].mac[num];
		jobDone[seq[i]]++;
		insert(seq[i], num);
	}

	for (i = 1; i <= macNum; i++)
	{
		printf("M%d", i - 1);
		for (j = 0; j < onum[i]; j++)
		{
			if (blocks[i][j].start == blocks[i][j].end)
				continue;
			printf(" (%d,%d-%d,%d)", blocks[i][j].start, blocks[i][j].jobRec, blocks[i][j].opeRec, blocks[i][j].end);
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
void output_file(int n, int *seq, FILE *fPtr)
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
			fprintf(fPtr, " (%d,%d-%d,%d)", blocks[i][j].start, blocks[i][j].jobRec, blocks[i][j].opeRec, blocks[i][j].end);
		}
		fprintf(fPtr, "\r\n");
	}
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
		pop[i].time = calculate_time(length, pop[i].sequence);
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
		x[i].time = calculate_time(length, x[i].sequence);
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

LRESULT CALLBACK keyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode != HC_ACTION) return CallNextHookEx(keyHook, nCode, wParam, lParam);
	PKBDLLHOOKSTRUCT kbhl = (PKBDLLHOOKSTRUCT)lParam;
	if (kbhl->vkCode == 'C') jobshopC->setWindowTitle("Hooked");
	return CallNextHookEx(keyHook, nCode, wParam, lParam);
}



void GThread::run()
{
	input_file();
	init();
	int begin = clock();
	FILE *fPtr = fopen("C:\\Users\\Aris\\Desktop\\jobshop\\x64\\Debug\\output.txt", "w");
	generate();
	gene finalSeq = out();
	Ftime = calculate_time(length, finalSeq.sequence);
	output_file(length, finalSeq.sequence, fPtr);
	int end = clock();
	double cost = (double)(end - begin) / CLOCKS_PER_SEC;
	fprintf(fPtr, "Time Used: %.3lf\r\n", cost);
	fprintf(fPtr, "Ended Time: %d\r\n", finalSeq.time);
	fclose(fPtr);
	flag = 1;
	emit finishF();
}

void jobshop::paint()
{
	GWindows = new GWindow;
	GWindows->show();
	GWindows->update();
}

void setHook()
{
	keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyProc, GetModuleHandle(NULL), 0);
}

jobshop::jobshop(QWidget *parent)
	: QMainWindow(parent)
{
	jobshopC = this;
	setHook();
	setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
	int width = GetSystemMetrics(SM_CXFULLSCREEN);
	int height = GetSystemMetrics(SM_CYFULLSCREEN);
	this->setWindowTitle("jobshop");
	this->setWindowFlags(Qt::WindowCloseButtonHint);
	this->setFixedSize(1280, 960);
	this->move((width - 1280) / 2, (height - 960) / 2);
	startButton = new QPushButton("start", this);
	startButton->setGeometry(QRect(620, 40, 80, 40));
	gathread = new GThread();
	connect(gathread, SIGNAL(finishF()), this, SLOT(paint()));
	//connect(startButton, SIGNAL(clicked()), this, SLOT(Gstart()));
	connect(startButton, SIGNAL(clicked()), gathread, SLOT(start()));
}
