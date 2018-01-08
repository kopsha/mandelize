#ifndef RANDYGLUE_H
#define RANDYGLUE_H

#include "renderJack.h"

#include <QOpenGLWidget>
#include <QImage>

class RandyGlueWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	RandyGlueWidget(QWidget *parent);
	virtual ~RandyGlueWidget();
	QSize sizeHint() const;

protected:
	void resizeGL(int width, int height) override;
	void paintGL() override;

private slots:
	void thrFinished();
	void thrStarted();

private:
	QImage *screenImageRef;
	QImage glBuffer;

	QSize viewSize;

	RenderJack renderThread;
};

#endif