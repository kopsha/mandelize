#include "randyGlue.h"

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QOpenGLTexture>

RandyGlueWidget::RandyGlueWidget(QWidget *parent)
: QOpenGLWidget(parent), screenImageRef( nullptr )
{

	QImage image(3, 3, QImage::Format_Indexed8);
	QSizePolicy expand;
	setAutoFillBackground( false );

	expand.setHorizontalPolicy(QSizePolicy::Expanding);
	expand.setVerticalPolicy(QSizePolicy::Expanding);
	setSizePolicy(expand);	
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

	// rebuild screenImageRef
	if (screenImageRef)
	{
		delete screenImageRef;
		screenImageRef = nullptr;
	}

	QColor colorValue;
	screenImageRef = new QImage( width, height, QImage::Format_RGB32 );

	int i = 0;
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			i = (x + y) & 0xff;
			colorValue.setRgb( i, (i*4)%256, 255 - i );
			screenImageRef->setPixelColor( x, y, colorValue );
			//i = (i+1) & 0xff;
		}
	}

	glBuffer = *screenImageRef;
}

void RandyGlueWidget::paintGL()
{

	glPixelStorei(GL_UNPACK_ROW_LENGTH, glBuffer.bytesPerLine()/4 );
	glDrawPixels( glBuffer.width(), glBuffer.height(), GL_RGBA, GL_UNSIGNED_BYTE, glBuffer.bits() );

	//qDebug() << "glGetError: " << glGetError();
}
