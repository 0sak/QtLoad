#pragma once

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <Psapi.h>
#include <vector>

struct processInfo {
	TCHAR processName[MAX_PATH];
	DWORD processID;
};

class ListProcesses {
public:
	bool loadAllProcesses();
	void initConsole();
	std::vector<processInfo> processes;

private:
	void printProcessNameAndID(DWORD processID);

};
