#include "randyGlue.h"

#include <QOpenGLTexture>

#include <QDebug>


RandyGlueWidget::RandyGlueWidget(QWidget *parent, QLabel* labelRef)
	: QOpenGLWidget(parent)
	, statusLabelRef( labelRef )
{

	QSizePolicy expand;
	setAutoFillBackground( false );

	expand.setHorizontalPolicy(QSizePolicy::Expanding);
	expand.setVerticalPolicy(QSizePolicy::Expanding);
	setSizePolicy(expand);	


    connect(&renderThread, SIGNAL(frameIsReady()), this, SLOT(thrFinished()));

}

void RandyGlueWidget::thrFinished()
{
	qDebug() << __FUNCTION__ << "was invoked";

	if (statusLabelRef)
	{
		statusLabelRef->setText( "Ready" );
	}

	glBuffer = *renderThread.getBuffer();

	update();
}

RandyGlueWidget::~RandyGlueWidget()
{
}

QSize RandyGlueWidget::sizeHint() const
{
	return QSize( 1024, 768 );
}


void RandyGlueWidget::resizeGL(int width, int height)
{
	qDebug() << __FUNCTION__ << "invoked with " << width << "x" <<height;

	viewSize = QSize( width, height );

	if (statusLabelRef)
	{
		statusLabelRef->setText( "Busy" );
	}

	renderThread.resizeFrame( viewSize );
}

void RandyGlueWidget::paintGL()
{

	glPixelStorei(GL_UNPACK_ROW_LENGTH, glBuffer.bytesPerLine()/4 );
	glDrawPixels( glBuffer.width(), glBuffer.height(), GL_RGBA, GL_UNSIGNED_BYTE, glBuffer.bits() );

	//qDebug() << "glGetError: " << glGetError();
}
