#include "qtload.h"
#include "stdafx.h"

QtLoad::QtLoad(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

void QtLoad::on_selectDllButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select Dll"), "c:\\", tr("Dynamic Linked Library (*.dll)"));
    if(fileName.length() > 0)
        ui.selectedDllLabel->setText(fileName);
    else
        ui.selectedDllLabel->setText("< could not select file >");
}

void QtLoad::loadProcessListView() {
    for (int i = 0; i < listProcesses.processes.size(); i++){
        QString id = QString::number(listProcesses.processes.at(i).processID);
        QString name = QString::fromWCharArray(listProcesses.processes.at(i).processName);

        if (name.contains("<")) {
            continue;
        }

        QListWidgetItem* item = new QListWidgetItem(name, ui.processListWidget);
        item->setData(Qt::UserRole, id);
        ui.processListWidget->setCurrentItem(item);
    }
}

void QtLoad::on_processListWidget_currentItemChanged() {

    QListWidgetItem* curItem = ui.processListWidget->currentItem();

    if (curItem) {
        ui.procNameLabel->setText("Proc Name : " + curItem->text());
        ui.procIDLabel->setText("ProcID : ");
        ui.pidLabel->setText(curItem->data(Qt::UserRole).toString());
    }
    else {
        ui.procNameLabel->setText("<no item selected>");
        ui.procIDLabel->setText("<no item selected>");
        ui.pidLabel->setText("");
    }
}

void QtLoad::on_injectButton_clicked() {
    QListWidgetItem* curItem = ui.processListWidget->currentItem();

    if (curItem) {
        if (ui.selectedDllLabel->text().contains("<")) {
            ui.selectedDllLabel->setText("<No .dll was selected>");
            return;
        }

        char dllPath[MAX_PATH];
        strcpy(dllPath, ui.selectedDllLabel->text().toStdString().c_str());
        DWORD PID = ui.pidLabel->text().toInt();
        

        CreateRemoteThreadInject(PID, dllPath);
    }
}

BOOL QtLoad::CreateRemoteThreadInject(DWORD ID, const char* dll) {
    //Declare the handle of the process.
    HANDLE Process;

    //Declare the memory we will be allocating
    LPVOID Memory;

    //Declare LoadLibrary
    LPVOID LoadLibrary;

    //If there's no process ID we return false.
    if (!ID)
        return false;

    //Open the process with read , write and execute priviledges
    Process = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, ID);

    //Get the address of LoadLibraryA
    LoadLibrary = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");

    // Allocate space in the process for our DLL
    Memory = (LPVOID)VirtualAllocEx(Process, NULL, strlen(dll), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    // Write the string name of our DLL in the memory allocated
    WriteProcessMemory(Process, (LPVOID)Memory, dll, strlen(dll), NULL);

    // Load our DLL
    CreateRemoteThread(Process, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibrary, (LPVOID)Memory, NULL, NULL);

    //Let the program regain control of itself
    CloseHandle(Process);

    VirtualFree((LPVOID)Memory, strlen(dll), NULL);

    return true;
}

void QtLoad::initProcessList() {
    //listProcesses.initConsole();
    listProcesses.loadAllProcesses();
}