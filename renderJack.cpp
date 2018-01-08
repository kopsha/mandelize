#include "renderJack.h"

#include <complex>

#include <QDebug>

RenderJack::RenderJack( QObject* parent )
: QThread( parent )
, bufferRef( nullptr )
, size( 1024, 768 )
, area( QPointF(-2.0,2.0), QPointF(2.0,-2.0) )
, shouldRestart( false )
, shouldAbort( false )
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
	if (bufferRef)
	{
		delete bufferRef;
		bufferRef = nullptr;
	}
}

const QImage* RenderJack::getBuffer() const
{
	return bufferRef;
}

void RenderJack::resizeFrame( const QSize& desiredSize )
{
	QMutexLocker locker(&muex);

	if (this->size != desiredSize)
	{
		this->size = desiredSize;

		// redo some computing !?! or just trigger them
		// rebuild bufferRef
		if (bufferRef)
		{
			delete bufferRef;
			bufferRef = nullptr;
		}

		bufferRef = new QImage( desiredSize, QImage::Format_RGB32 );

	    if (!isRunning()) {
	        start(LowPriority);
	    } else {
	        shouldRestart = true;
	        condition.wakeOne();
	    }
	}
}

void RenderJack::requestNewFrame( const QRect& visibleArea )
{
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
	qDebug() << __FUNCTION__ << "has begun";

	QColor colorValue;

	forever
	{
        muex.lock();

		double lenF = 2.0;

		double stepx = (2.0*lenF) / double(size.width());
		double stepy = (2.0*lenF) / double(size.height());


		int col = 0;
		double left = area.left();
		double imag = area.top();
		double real = left;

		int padding = 10;	// pixels
		int height = size.height();
		int width = size.width();

        muex.unlock();


		for (int y = 0; y < height; y++)
		{
			if (shouldAbort)
			{
				return;
			}
			if (shouldRestart)
			{
				break;
			}
			real = left;
			for (int x = 0; x < width; x++)
			{
				// lets use (real,imag)
				col = findMandyCount( real, imag );
				if (col >= 0)
				{
					colorValue.setRgb( 0, col, 0 );
				}
				else
				{
					colorValue.setRgb( 255+col, 0-col, 0 );
				}

				//bufferRef->setPixelColor( x+padding, y+padding, colorValue );

				real += stepx;
			}
			imag -= stepy;
		}

		// draw axes
		int oy = height/2;
		int ox = width/2;
		colorValue.setRgb( 0xff, 0xff, 0xff );
		for (int x = 0; x < width; x++)
		{
			//bufferRef->setPixelColor( padding + x, padding + oy, colorValue );
			if ((x % 180)==0)
			{
				//for (int y = -4; y <= 4; y++)
					//bufferRef->setPixelColor( padding + x, padding + oy + y, colorValue );
			}
		}
		for (int y = 0; y < height; y++)
		{
			//bufferRef->setPixelColor( padding + ox, padding + y, colorValue );
			if ((y % 180)==0)
			{
				//for (int x = -4; x <= 4; x++)
					//bufferRef->setPixelColor( padding + ox + x, padding + y, colorValue );
			}
		}
		/********/
		qDebug() << __FUNCTION__ << "shouldRestart == " << shouldRestart;

        muex.lock();
        if (!shouldRestart)
        	emit frameIsReady();
            condition.wait(&muex);
        shouldRestart = false;
        muex.unlock();
	}

	qDebug() << __FUNCTION__ << "did ended";

}
