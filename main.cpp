#include "ImageEditorUI.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageEditorUI w;
    w.show();

    return a.exec();
}
