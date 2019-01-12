#include "GThread.h"

class GThread2 : public QThread {
	Q_OBJECT

signals:
	void finishF2();

public:
	//GThread();
	void run();
};