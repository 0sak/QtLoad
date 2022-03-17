#pragma once

#include <QtWidgets/QWidget>
#include "ui_qtload.h"
#include "ListProcesses.h"
#include <iostream>

class QtLoad : public QWidget
{
    Q_OBJECT

public:
    QtLoad(QWidget *parent = Q_NULLPTR);
    void initProcessList();
    void loadProcessListView();
    BOOL CreateRemoteThreadInject(DWORD ID, const char* dll);

private slots:
    void on_selectDllButton_clicked();
    void on_processListWidget_currentItemChanged();
    void on_injectButton_clicked();

private:
    ListProcesses listProcesses;
    Ui::QtLoadClass ui;
};
