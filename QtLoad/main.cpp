#include "qtload.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "ListProcesses.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtLoad w;
    w.initProcessList();
    w.loadProcessListView();
    w.show();
    return a.exec();
}
