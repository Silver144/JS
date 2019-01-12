#include <GThread.h>

class GWindow : public QMainWindow {

	Q_OBJECT

public:
	GWindow();

private slots:
	void paintEvent(QPaintEvent*);
};