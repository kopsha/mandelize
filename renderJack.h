#ifndef RENDERJACK_H
#define RENDERJACK_H

#include <QThread>

class RenderJack : public QThread
{
	Q_OBJECT
public:
	void run() override;

signals:
	void resultReady();

};

#endif