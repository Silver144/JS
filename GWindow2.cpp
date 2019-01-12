#include <QMainWindow>
#include <GWindow2.h>
#include <QPainter>


//block blocks[MAX_NUM][MAX_NUM];
//int jobNum, macNum, all_num, parents, length, flag, x_all, Ftime;

QColor csecol2(int i)
{
	switch (i)
	{
	case -1: return QColor(100, 100, 100);
	case 1: return QColor(30, 144, 255);
	case 2: return QColor(0, 205, 102);
	case 3: return QColor(118, 238, 198);
	case 4: return QColor(238, 238, 0);
	case 5: return QColor(205, 92, 92);
	case 6: return QColor(153, 50, 204);
	case 7: return QColor(135, 206, 250);
	case 8: return QColor(239, 173, 14);
	case 9: return QColor(255, 127, 80);
	case 0: return QColor(238, 99, 99);
	}
	return QColor(rand() * rand() % 256, rand() * rand() % 256, rand() * rand() % 256);
}

void GWindow2::paintEvent(QPaintEvent*)
{
	QPainter painter;
	painter.begin(this);
	painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
	painter.setPen(QPen(Qt::GlobalColor::transparent, 0, Qt::SolidLine));
	painter.drawRect(0, 0, 1280, 960);
	painter.setPen(QPen(Qt::GlobalColor::black, 0, Qt::SolidLine));
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
	QFont font2;
	font2.setFamily("Helvetica");
	font2.setPointSize(20);
	painter.setFont(font2);
	painter.setPen(QPen(QColor(100, 100, 100), 0, Qt::SolidLine));
	painter.drawText(100, 170, QStringLiteral("Jobshop Scheduling Solutions"));
	painter.setPen(QPen(QColor(175, 175, 175), 0, Qt::SolidLine));
	font2.setPointSize(14);
	painter.setFont(font2);
	painter.drawText(100, 210, QStringLiteral("after Accepting Repairing Instructions"));
	painter.setPen(QPen(Qt::GlobalColor::transparent, 0, Qt::SolidLine));
	for (int i = 1; i <= macNum; i++)
	{
		for (int j = 0; j < onum[i]; j++)
		{
			painter.setBrush(QBrush(csecol2(blocks[i][j].jobRec), Qt::SolidPattern));
			double ra = double(blocks[i][j].start) / double(Ftime), ra1 = double((blocks[i][j].end - blocks[i][j].start)) / double(Ftime);
			int rec1 = ra * 1000 + 150, rec2 = ra1 * 1000;
			painter.drawRect(rec1, ML[i - 1] - 20, rec2, 35);
		}
	}
	painter.end();
}


GWindow2::GWindow2()
{
	setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
	int width = GetSystemMetrics(SM_CXFULLSCREEN);
	int height = GetSystemMetrics(SM_CYFULLSCREEN);
	this->setWindowTitle("AnswerFix");
	this->setWindowFlags(Qt::WindowCloseButtonHint);
	this->setFixedSize(1280, 960);
	this->move((width - 1280) / 2, (height - 960) / 2);
}