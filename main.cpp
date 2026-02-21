#include <QApplication>
#include "mainwindow.h"
#include "ImageLibrary.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto* library = new ImageLibrary();

    MainWindow w(library);
    w.show();

    return a.exec();
}
