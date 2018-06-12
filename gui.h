#pragma once

#include <QtWidgets/QMainWindow>
#include <windows.h>
#include <QPushButton>
#include <omp.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <QPainter.h>
#include <qlabel.h>
#include <qthread.h>

class GThread : public QThread
{
	Q_OBJECT

signals:
	void finishF();

public:
	//GThread();
	void run();
};

class jobshop : public QMainWindow
{
	Q_OBJECT

public:
	jobshop(QWidget *parent = Q_NULLPTR);
	QPushButton *startButton;
	QPushButton *paintButton;

private slots:
	//void start();
	//void paintEvent(QPaintEvent*);
	//void Gstart();
	void paint();

private:
	GThread * gathread;
};

class GWindow : public QMainWindow {

	Q_OBJECT

public:
	GWindow();

private slots:
	void paintEvent(QPaintEvent*);
};
