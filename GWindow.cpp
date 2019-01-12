#include <QMainWindow>
#include <GWindow.h>
#include <QPainter>

//block blocks[MAX_NUM][MAX_NUM];
//int jobNum, macNum, all_num, parents, length, flag, x_all, Ftime;

//double EVER_TIME;

QColor csecol(int i, int j, int k)
{
	double ra = double(blocks[j][k].start) / double(Ftime), ra1 = double((blocks[j][k].end - blocks[j][k].start)) / double(Ftime);
	double rc = double(blocks[j][k].end - blocks[j][k].start) / double(Ftime);
	double rec1 = ra * 1000 + 150, rec2 = ra1 * 1000, rec = rc * 1000;
	switch (i)
	{
	case -1: return QColor(200, 200, 200);
	case 1: return QColor(30, 144, 255, rec1 > EVER_TIME + 150 ? 30 : 255);
	case 2: return QColor(0, 205, 102, rec1 > EVER_TIME + 150 ? 30 : 255);
	case 3: return QColor(118, 238, 198, rec1 > EVER_TIME + 150 ? 30 : 255);
	case 4: return QColor(238, 238, 0, rec1 > EVER_TIME + 150 ? 30 : 255);
	case 5: return QColor(205, 92, 92, rec1 > EVER_TIME + 150 ? 30 : 255);
	case 6: return QColor(153, 50, 204, rec1 > EVER_TIME + 150 ? 30 : 255);
	case 7: return QColor(135, 206, 250, rec1 > EVER_TIME + 150 ? 30 : 255);
	case 8: return QColor(239, 173, 14, rec1 > EVER_TIME + 150 ? 30 : 255);
	case 9: return QColor(255, 127, 80, rec1 > EVER_TIME + 150 ? 30 : 255);
	case 0: return QColor(238, 99, 99, rec1 > EVER_TIME + 150 ? 30 : 255);
	}
	return QColor(rand() * rand() % 256, rand() * rand() % 256, rand() * rand() % 256);
}

void GWindow::paintEvent(QPaintEvent*)
{
	QPainter painter;
	painter.begin(this);
	painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
	painter.setPen(QPen(Qt::GlobalColor::transparent, 3, Qt::SolidLine));
	painter.drawRect(0, 0, 1280, 960);
	painter.setPen(QPen(Qt::GlobalColor::black, 3, Qt::SolidLine));
	QFont font;
	font.setFamily("Microsoft YaHei");
	font.setPointSize(10);
	font.setItalic(true);
	painter.setFont(font);
	int ML[10]{ 300, 350, 400, 450, 500, 550, 600, 650, 700, 750 };
	int wid = 300;
	for (int i = 0; i < macNum; i++)
	{
		QString s1 = "M", s2 = QString::number(i);
		s1 = s1 + s2;
		painter.drawText(100, wid + 50 * i, s1);
	}
	QFont font2;
	if (!LFlag2)
	{
		font2.setFamily("Helvetica");
		painter.setPen(QPen(QColor(100, 100, 100), 0, Qt::SolidLine));
		font2.setPointSize(20);
		painter.setFont(font2);
		painter.drawText(100, 210, QStringLiteral("Jobshop Scheduling Solutions"));
	}
	else
	{
		font2.setFamily("Helvetica");
		font2.setPointSize(20);
		painter.setFont(font2);
		painter.setPen(QPen(QColor(100, 100, 100), 0, Qt::SolidLine));
		painter.drawText(100, 170, QStringLiteral("Jobshop Scheduling Solutions"));
		painter.setPen(QPen(QColor(175, 175, 175), 0, Qt::SolidLine));
		font2.setPointSize(14);
		painter.setFont(font2);
		painter.drawText(100, 210, QStringLiteral("after Accepting Repairing Instructions"));
	}
	painter.setPen(QPen(Qt::GlobalColor::transparent, 0, Qt::SolidLine));
	for (int i = 1; i <= macNum; i++)
	{
		for (int j = 0; j < jobNum; j++)
		{
			//if (blocks[i][j].start > EVER_TIME) break;
			painter.setBrush(QBrush(csecol(blocks[i][j].jobRec, i, j), Qt::SolidPattern));
			//double ra = double(blocks[i][j].start) / double(Ftime), ra1 = double((blocks[i][j].end - blocks[i][j].start)) / double(Ftime);
			//double rc = double(EVER_TIME - blocks[i][j].start) / double(Ftime);
			double ra = double(blocks[i][j].start) / double(Ftime), ra1 = double((blocks[i][j].end - blocks[i][j].start)) / double(Ftime);
			double rc = double(blocks[i][j].end - blocks[i][j].start) / double(Ftime);
			int rec1 = ra * 1000 + 150, rec2 = ra1 * 1000, rec = rc * 1000;
			painter.drawRect(rec1, ML[i - 1] - 20, rec2, 35);
			if (150 + EVER_TIME > rec1)
			{
				font2.setPointSize(10);
				painter.setFont(font2);
				painter.setPen(QPen(Qt::GlobalColor::white, 0, Qt::SolidLine));
				if(blocks[i][j].jobRec >= 0) painter.drawText(rec1 + rec / 2 - 5, ML[i - 1] + 5, QString::number(blocks[i][j].jobRec));
				else
				{
					if (rec1 + rec2 > 150 + EVER_TIME)
						painter.drawText(rec1 + rec / 2 - 20, ML[i - 1] + 5, QStringLiteral("fixing"));
					else painter.drawText(rec1 + rec / 2 - 20, ML[i - 1] + 5, QStringLiteral("fixed"));
				}
				painter.setPen(QPen(Qt::GlobalColor::transparent, 0, Qt::SolidLine));
			}
			//else painter.drawRect(rec1, ML[i - 1] - 20, rec, 35);
		}
	}
	if (EVER_TIME >= 1000 && CFlag == 0)
	{
		FILE *fPtr = fopen("output2.txt", "w");
		output_file(length, finalSeq.sequence, fPtr, 1);
		CFlag = 1;
		fclose(fPtr);
	}
	painter.setPen(QPen(Qt::GlobalColor::black, 3, Qt::SolidLine));
	if (EVER_TIME < 1000) painter.drawLine(150 + EVER_TIME, 250, 150 + EVER_TIME, 780);
	else
	{
		painter.setPen(QPen(Qt::GlobalColor::red, 3, Qt::SolidLine));
		painter.drawLine(1150, 250, 1150, 780);
		painter.setPen(QPen(Qt::GlobalColor::black, 3, Qt::SolidLine));
	}
	painter.drawLine(150, 800, 1150, 800);
	painter.setFont(font);
	for (int i = 0; i <= 5; i++)
	{
		painter.drawText(150 + double(i) / 5 * 1000 - 5, 840, QString::number(int(Ftime * double(i) / 5)));
	}
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