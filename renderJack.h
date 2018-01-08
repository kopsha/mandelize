#ifndef RENDERJACK_H
#define RENDERJACK_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QImage>

class RenderJack : public QThread
{
	Q_OBJECT

public:
	explicit RenderJack( QObject* parent = Q_NULLPTR );
	virtual ~RenderJack();

	void resizeFrame( const QSize& desiredSize );
	void requestNewFrame( const QRect& visibleArea );

	void run() override;

	const QImage* getBuffer() const;

signals:
	void frameIsReady();

private:
	int findMandyCount( double r, double i );

private:
	QMutex	muex;
	QWaitCondition condition;

	QImage* bufferRef;

	QSize	size;
	QRectF	area;

	bool 	shouldRestart;
	bool 	shouldAbort;
};

#endif