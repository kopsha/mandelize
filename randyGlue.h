#ifndef RANDYGLUE_H
#define RANDYGLUE_H

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

private:
	QImage *screenImageRef;
	QImage glBuffer;
};

#endif