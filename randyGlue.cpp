#include "randyGlue.h"

#include <QPainter>
#include <QPaintEvent>
#include <QOpenGLTexture>

#include <complex>

#include <QDebug>


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

//todo: refactor this method
int findMandyCount( double r, double i )
{
	std::complex<double> z( 0, 0 );
	std::complex<double> c( r, i );

	int itc = 0;
	bool stop = false;

	do {
		z = pow(z,2) + c;
		++itc;
		if (abs(z) > 4)
		{
			stop = true;
		}
	} while ((itc < 255) && (!stop));
	itc = itc & 0xff;
	//qDebug() << "iterations: " << itc << "for" << r << i;

	return itc;
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

	//todo: move rendering into another thread
	// screen to space translation
	int screenWidth = 721;
	int screenHeight = 721;

	double margin = 2.0;
	double left = -margin;
	//double right = margin;
	double top = margin;
	//double bottom = -margin;

	double stepx = (2.0*margin) / double(screenWidth);
	double stepy = (2.0*margin) / double(screenHeight);


	int col = 0;
	double real = left;
	double imag = top;

	int padding = 10;

	for (int y = 0; y < screenHeight; y++)
	{
		real = left;
		for (int x = 0; x < screenWidth; x++)
		{
			// lets use (real,imag)
			col = findMandyCount( real, imag );
			colorValue.setRgb( 0, col, 0 );
			screenImageRef->setPixelColor( x+padding, y+padding, colorValue );

			real += stepx;
		}
		imag -= stepy;
	}

	// draw axes
	int oy = screenHeight/2;
	int ox = screenWidth/2;
	colorValue.setRgb( 0xff, 0xff, 0xff );
	for (int x = 0; x < screenWidth; x++)
	{
		screenImageRef->setPixelColor( padding + x, padding + oy, colorValue );
		if ((x % 180)==0)
		{
			for (int y = -4; y <= 4; y++)
				screenImageRef->setPixelColor( padding + x, padding + oy + y, colorValue );
		}
	}
	for (int y = 0; y < screenHeight; y++)
	{
		screenImageRef->setPixelColor( padding + ox, padding + y, colorValue );
		if ((y % 180)==0)
		{
			for (int x = -4; x <= 4; x++)
				screenImageRef->setPixelColor( padding + ox + x, padding + y, colorValue );
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
