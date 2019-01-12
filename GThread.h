#pragma once
#include <QThread>
#include "jobshop.h"
#pragma execution_character_set("utf-8")

class jobshop;

const int GAP = 300, ENCRYPT_NUM = 400, MAX_POP = 800;
const double GER_MUL = 1.75, DIE_RATIO = 1.1, MUT_MUL = 1;

extern int jobNum, macNum, all_num, parents, length, flag, x_all, Ftime, LFlag, LFlag2, TFlag, CFlag;
extern double EVER_TIME;

extern int gapNum[MAX_NUM], lastTime[MAX_NUM], start[MAX_NUM][MAX_NUM], end[MAX_NUM][MAX_NUM], last[MAX_NUM];

extern int onum[MAX_NUM];
extern int ostart[MAX_NUM][MAX_NUM];
extern int oend[MAX_NUM][MAX_NUM];
extern int jobDone[MAX_NUM];

extern int fix_num;


struct fix_ope {
	int start;
	int end;
	int ope_num;
};

extern fix_ope fix[MAX_NUM];

struct block {
	int start;
	int end;
	int jobRec;
	int opeRec;
};

extern block blocks[MAX_NUM][MAX_NUM];

struct job
{
	int n;
	int time[MAX_NUM];
	int mac[MAX_NUM];
};

extern job jobs[MAX_NUM];

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

extern gene finalSeq;

extern gene pop[3000];

extern void insert_fix_out(int start_time, int end_time, int mac_num);
extern void insert_fix(int start_time, int end_time, int mac_num);
extern int insert_one(int i, int j);
extern int calculate_time(int n, int *_gene, int flag);
extern int s_best(int flag);
extern void cal_len();
extern void select_func();
extern int is_zero(int *cha, int len);
extern void encrypt_one(int num);
extern void encrypt_main();
extern void cal_p();
extern void init();
extern int cmp(const void *a, const void *b);
extern void insert(int i, int j);
extern void output(int n, int *seq, int flag);
extern void die();
extern void replace();
extern void output_file(int n, int *seq, FILE *fPtr, int flag);
extern gene cross_once(int which);
extern void cross();
extern void mutate(int num);
extern void generate();
extern gene out();
extern void insert_repair(double start, int total, int machine_num);

class GThread : public QThread {
	Q_OBJECT

signals:
	void finishF();

public:
	//GThread();
	void run();
};