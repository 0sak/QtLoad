#include "ListProcesses.h"

void ListProcesses::printProcessNameAndID(DWORD processID) {
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

	//Get handle to process
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);

	//Get process name
	if (NULL != hProcess) {
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
			GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
		}
	}

	//_tprintf(TEXT("%s    (PID: %u) \n"), szProcessName, processID);

	processInfo newProcess;
	_tcscpy(newProcess.processName, szProcessName);
	newProcess.processID = processID;

	processes.push_back(newProcess);


	//Release handle
	CloseHandle(hProcess);

}

bool ListProcesses::loadAllProcesses() {
	//get list of processes
	processes.clear();
	DWORD processList[1024], cbNeeded, cProcesses;
	unsigned int i;

	if (!EnumProcesses(processList, sizeof(processList), &cbNeeded))
		return 1;

	//Calculate how many process identifiers were returned.
	cProcesses = cbNeeded / sizeof(DWORD);

	//Print the name and ID for each process
	for (int i = 0; i < cProcesses; i++) {
		if (processList[i] != 0) {
			printProcessNameAndID(processList[i]);
		}
	}

	return 0;
}

void ListProcesses::initConsole() {
#ifdef _WIN32
	if (AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole()) {
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
		freopen("CONIN$", "r", stdin);
	}
#endif


}