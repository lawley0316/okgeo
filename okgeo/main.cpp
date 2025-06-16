#include <QApplication>

#include "src/middle/manager.h"
#include "src/fend/uimain/uimain.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MANAGER->Initialize();
    UiMain uiMain;
    uiMain.show();
    return app.exec();
}
