#include "mandyWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    /*** this part enables multisampling -- later dude
    QSurefaceFormat fmt; // doesn't make any sense
    fmt.setSamples( 4 );
    QSurefaceFormat::setDefaultFormat( fmt );
    ***/

    MandyApplicationWindow mw;
    mw.resize(mw.sizeHint());
    mw.show();

    return app.exec();
}
