#include "renderJack.h"

#include <complex>

#include <QDebug>

RenderJack::RenderJack( QObject* parent )
	: QThread( parent )
	, frameBuffer( nullptr )
	, size( 1, 1 )
	, area( QPointF(-2.0,2.0), QPointF(2.0,-2.0) )
	, shouldRestart( false )
	, shouldAbort( false )
	, bufferNotProcessed( false )
{
}

RenderJack::~RenderJack()
{
	muex.lock();
	shouldAbort = true;
	condition.wakeOne();
	muex.unlock();

	wait();

    // safe cleanup...
	if (frameBuffer)
	{
		delete frameBuffer;
		frameBuffer = nullptr;
	}
}

void RenderJack::copyFrameTo( QImage& dest )
{
	QMutexLocker locker(&muex);
	dest = frameBuffer->copy();
	bufferNotProcessed = false;
}

bool RenderJack::resizeFrame( const QSize& desiredSize )
{
	static bool willRedraw;

	QMutexLocker locker(&muex);

	if (size != desiredSize)
	{
		size = desiredSize;

		if (isRunning())
		{
			shouldRestart = true;
			condition.wakeOne();
		}
		else
		{
			start( LowPriority );
		}
		willRedraw = true;
	}
	else
	{
		willRedraw = false;
	}

	return willRedraw;
}

void RenderJack::requestNewFrame( const QRect& visibleArea )
{
	// *** !!!not used yet!!! ***
	QMutexLocker locker(&muex);

	if (this->area != visibleArea)
	{
		// nooo, use QRect and convert it to floats
		// this->area = visibleArea;
		// recompute some shit !!?

		if (isRunning())
		{
			shouldRestart = true;
			condition.wakeOne();
		}
		else
		{
			start( LowPriority );
		}
	}
}

//todo: refactor this method
int RenderJack::findMandyCount( double r, double i )
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
		else if (abs(z) < 0.01)
		{
			stop = true;
		}
	} while ((itc < 255) && (!stop));
	itc = itc & 0xff;
	//qDebug() << "iterations: " << itc << "for" << r << i;

	if (std::imag(z) < 0)
	{
		itc = -itc;
	}

	return itc;
}

void RenderJack::run()
{
	static QColor colorValue;
	static const double lenF = 2.0;

	int frameWidth;
	int frameHeight;
	int linearColor;
	int ox, oy, axUnit;
	double stepX;
	double stepY;
	const double left = this->area.left();
	const double top = this->area.top();

	double real;
	double imag;

	forever
	{
		muex.lock();

		frameWidth = this->size.width();
		frameHeight = this->size.height();
		bool xx = this->bufferNotProcessed;

		muex.unlock();

		if (xx)
			break;

		// redo some computing !?! or just trigger them
		// rebuild frameBuffer
		if (frameBuffer)
		{
			delete frameBuffer;
			frameBuffer = nullptr;
		}

		// q: is there a smarter way to rebuild frame without realloc ?
		frameBuffer = new QImage( frameWidth, frameHeight, QImage::Format_RGB32 );
		qDebug() << __FUNCTION__ << "new frame size: " << frameWidth;

		stepX = (2.0*lenF) / double(frameWidth);
		stepY = (2.0*lenF) / double(frameHeight);

		linearColor = 0;
		imag = top;

		for (int y = 0; y < frameHeight; y++)
		{
			// check the flags
			if (this->shouldAbort)
			{
				return;
			}
			if (this->shouldRestart)
			{
				break;
			}

			real = left;
			for (int x = 0; x < frameWidth; x++)
			{
				linearColor = findMandyCount( real, imag );
				if (linearColor >= 0)
				{
					colorValue.setRgb( 0, linearColor, 0 );
				}
				else
				{
					colorValue.setRgb( 255+linearColor, 0-linearColor, 0 );
				}
				frameBuffer->setPixelColor( x, y, colorValue );
				real += stepX;
			}
			imag -= stepY;
		}

		// draw axes
		oy = frameHeight/2;
		ox = frameWidth/2;
		axUnit = frameWidth / 4;
		colorValue.setRgb( 0xff, 0xff, 0xff );
		for (int x = 0; x < frameWidth; x++)
		{
			frameBuffer->setPixelColor( x, oy, colorValue );
			if ((x % axUnit)==0)
			{
				for (int y = -4; y <= 4; y++)
					frameBuffer->setPixelColor( x, oy + y, colorValue );
			}
		}
		for (int y = 0; y < frameHeight; y++)
		{
			frameBuffer->setPixelColor( ox, y, colorValue );
			if ((y % axUnit)==0)
			{
				for (int x = -4; x <= 4; x++)
					frameBuffer->setPixelColor( ox + x, y, colorValue );
			}
		}
		/********/

		muex.lock();
		if (shouldRestart)
		{
			shouldRestart = false;
		}
		else
		{
			emit frameIsReady();
			bufferNotProcessed = true;
			condition.wait(&muex);
		}
		muex.unlock();
	}
}
