#include "fpworth.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FPWorth w;
    w.show();
    //a.setWindowIcon(QIcon(":/logo.png"));
    return a.exec();
}
