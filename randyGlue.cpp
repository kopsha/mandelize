#include "randyGlue.h"

#include <QOpenGLTexture>

#include <QDebug>


RandyGlueWidget::RandyGlueWidget(QWidget *parent)
: QOpenGLWidget(parent), screenImageRef( nullptr )
{

	QSizePolicy expand;
	setAutoFillBackground( false );

	expand.setHorizontalPolicy(QSizePolicy::Expanding);
	expand.setVerticalPolicy(QSizePolicy::Expanding);
	setSizePolicy(expand);	


    connect(&renderThread, SIGNAL(frameReady()), this, SLOT(thrFinished()));

}

void RandyGlueWidget::thrFinished()
{
	qDebug() << __FUNCTION__ << "was invoked";

}

void RandyGlueWidget::thrStarted()
{
	qDebug() << __FUNCTION__ << "was invoked";

}

RandyGlueWidget::~RandyGlueWidget()
{
	delete screenImageRef;
}

QSize RandyGlueWidget::sizeHint() const
{
	return QSize( 1024, 768 );
}


void RandyGlueWidget::resizeGL(int width, int height)
{
	qDebug() << __FUNCTION__ << "invoked with " << width << "x" <<height;

	//todo: move rendering into another thread
	viewSize = QSize( width, height );
	renderThread.resizeFrame( viewSize );

	//glBuffer = *screenImageRef;
}

void RandyGlueWidget::paintGL()
{

	glPixelStorei(GL_UNPACK_ROW_LENGTH, glBuffer.bytesPerLine()/4 );
	glDrawPixels( glBuffer.width(), glBuffer.height(), GL_RGBA, GL_UNSIGNED_BYTE, glBuffer.bits() );

	//qDebug() << "glGetError: " << glGetError();
}
