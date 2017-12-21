#include "mandyWindow.h"
#include "randyGlue.h"

#include <QGridLayout>
#include <QLabel>

MandyApplicationWindow::MandyApplicationWindow()
{
	setWindowTitle( tr("gluing it all together") );

	QLabel* label = new QLabel( tr("Status:") );
	statusLabelRef = new QLabel( tr("Ready") );

	QHBoxLayout* horizLayout = new QHBoxLayout;
	horizLayout->addWidget( label, 0, Qt::AlignRight );
	horizLayout->addWidget( statusLabelRef, 0, Qt::AlignLeft );

	RandyGlueWidget *glw = new RandyGlueWidget( this );

	QVBoxLayout *verticalLayout = new QVBoxLayout;
	verticalLayout->addLayout( horizLayout );
	verticalLayout->addWidget( glw );
	setLayout( verticalLayout );
}
