#include "mandyWindow.h"
#include "randyGlue.h"

#include <QGridLayout>
#include <QLabel>

MandyApplicationWindow::MandyApplicationWindow()
{
	setWindowTitle( tr("gluing it all together") );

	// status panel
	QLabel* label = new QLabel( tr("Status:") );
	statusLabel = new QLabel( tr("Ready") );

	QGridLayout* statusPanelLayout = new QGridLayout();
	statusPanelLayout->addWidget( label, 0, 0, Qt::AlignRight );
	statusPanelLayout->addWidget( statusLabel, 0, 1, Qt::AlignLeft );


	RandyGlueWidget *glw = new RandyGlueWidget( this, statusLabel );

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget( glw );
	mainLayout->addLayout( statusPanelLayout );

	// apply setup
	setLayout( mainLayout );
}

MandyApplicationWindow::~MandyApplicationWindow()
{
	if (statusLabel)
	{
		delete statusLabel;
		statusLabel = nullptr;
	}
}
