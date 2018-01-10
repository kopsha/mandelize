#ifndef MANDYWINDOW_H
#define MANDYWINDOW_H

#include <QWidget>
#include <QLabel>

class MandyApplicationWindow : public QWidget
{
	Q_OBJECT

public:
	MandyApplicationWindow();
	~MandyApplicationWindow();

private:
	QLabel* statusLabel;
};

#endif