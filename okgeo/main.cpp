#include <QApplication>

#include "src/middle/manager.h"
#include "src/fend/uimain/uimain.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MANAGER->init();
    UiMain uiMain;
    uiMain.show();
    return app.exec();
}
