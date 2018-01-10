#ifndef RANDYGLUE_H
#define RANDYGLUE_H

#include "renderJack.h"

#include <QOpenGLWidget>
#include <QImage>
#include <QLabel>

class RandyGlueWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	RandyGlueWidget(QWidget *parent, QLabel* labelRef);
	virtual ~RandyGlueWidget();
	QSize sizeHint() const;

protected:
	void resizeGL(int width, int height) override;
	void paintGL() override;

private slots:
	void thrFinished();

private:
	QLabel* statusLabelRef;
	QImage glBufferCopy;

	QSize viewSize;

	RenderJack renderThread;
};

#endif