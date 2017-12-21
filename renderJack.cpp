#include "renderJack.h"

#include <QDebug>

void RenderJack::run()
{
	qDebug() << __FUNCTION__ << "was invoked";

	emit resultReady();
}
