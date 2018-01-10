#include "randyGlue.h"

#include <QOpenGLTexture>

#include <QDebug>


RandyGlueWidget::RandyGlueWidget(QWidget *parent, QLabel* labelRef)
	: QOpenGLWidget( parent )
	, statusLabelRef( labelRef )
{
	QSizePolicy expand;
	setAutoFillBackground( false );

	expand.setHorizontalPolicy(QSizePolicy::Expanding);
	expand.setVerticalPolicy(QSizePolicy::Expanding);
	setSizePolicy(expand);	

    connect( &renderThread, SIGNAL(frameIsReady()),
    		 this, 			SLOT(thrFinished()),
    		 Qt::QueuedConnection );
}

void RandyGlueWidget::thrFinished()
{
	qDebug() << __FUNCTION__ << "was invoked";

	renderThread.copyFrameTo( glBufferCopy );

	if (statusLabelRef)
	{
		statusLabelRef->setText( "Ready" );
		statusLabelRef->update();
	}

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

	int squareLen = (width < height) ? width : height;
	viewSize = QSize( squareLen, squareLen );

	bool willRedraw = renderThread.resizeFrame( viewSize );

	if (statusLabelRef && willRedraw)
	{
		statusLabelRef->setText( "Busy" );
		statusLabelRef->update();
	}

}

void RandyGlueWidget::paintGL()
{

	if (!glBufferCopy.isNull())
	{
		// todo:
		// * add a small padding
		// * center image to widget
		glPixelStorei(GL_UNPACK_ROW_LENGTH, glBufferCopy.bytesPerLine()/4 );
		glDrawPixels( glBufferCopy.width(), glBufferCopy.height(), GL_RGBA, GL_UNSIGNED_BYTE, glBufferCopy.bits() );
	}
	else
	{
		qDebug() << "Paint requested, but no image available";
	}

	//qDebug() << "glGetError: " << glGetError();
}
