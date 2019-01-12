#include <GThread.h>

class GWindow2 : public QMainWindow {

	Q_OBJECT

public:
	GWindow2();

private slots:
	void paintEvent(QPaintEvent*);
};