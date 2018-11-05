#include "stdafx.h"
#include"cellTestServer.h"

cellTestServer::cellTestServer() {}
cellTestServer::~cellTestServer() {}

int cellTestServer::startCellTestServer(unsigned long&threadID, unsigned long&processID)
{
	// 本地计算机中的pythonw命令路径和需要执行的python服务程序路径
	std::string pyexe = "C:\\ProgramData\\Anaconda3\\pythonw.exe";
	std::string pyscript = "D:\\project\\vs2015\\c_python-call\\c_python-call\\examples\\cellTestServer.py";
	std::string exec = pyexe + " " + "\"" + pyscript + "\"";
	// 这里创建进程所需要的变量
	STARTUPINFOA si;
	// PROCESS_INFORMATION*pi = new PROCESS_INFORMATION;

	// ZeroMemory(&si, sizeof(si));
	// si.cb = sizeof(si);
	// ZeroMemory(pi, sizeof(PROCESS_INFORMATION));
	::memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi;
	::memset(&pi, 0, sizeof(pi));

	// 启动线程 
	if (!CreateProcessA(NULL,   // No module name (use command line)
		(LPSTR)exec.c_str(),        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		(LPSTARTUPINFOA)&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		// cout << "Failure Process ID = " << pi.dwProcessId << endl;
		return 1;
	}
	// cout << "Succeed Process ID = " << pi.dwProcessId << endl;
	std::cout << "type of threadID:" << typeid(pi.dwThreadId).name() << std::endl;
	// 返回开辟的进程号和进程下的线程号
	threadID = (pi.dwThreadId);
	processID = (pi.dwProcessId);
	return 0;
}

int cellTestServer::killCellTestServer(char*processName)
{
	HANDLE handle;
	HANDLE handle1;
	//调用CreatToolhelp32Snapshot来获取快照,用THREADENTRY32来获取线程信息等 就会用到TlHelp32.h 头文件
	handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	// 通过遍历后台程序找出程序名为:pythonw.exe的程序
	PROCESSENTRY32 *info;
	info = new PROCESSENTRY32;
	info->dwSize = sizeof(PROCESSENTRY32);
	Process32First(handle, info);

	// 调用的是系统函数
	while (Process32Next(handle, info) != FALSE)
	{
		//指向进程名字   
		//strcmp字符串比较函数同要结束相同   
		//if(wcscmp(c,info->szExeFile) == 0) 
		info->szExeFile;
		//strcmp() 函数用来比较设定要关闭的进程和快照中的进程名字。
		//MessageBox (NULL, info->szExeFile, TEXT ("HelloMsg"), 0);   
		//PROCESS_TERMINATE表示为结束操作打开,FALSE=可继承,info->th32ProcessID=进程ID   
		if (strcmp(processName, info->szExeFile) == 0)
		{
			//OpenProcess 函数用来打开一个已存在的进程对象，并返回进程的句柄。
			//结束进程   
			handle1 = OpenProcess(PROCESS_TERMINATE, FALSE, info->th32ProcessID);
			//TerminateProcess是C++编程语言中的一种函数，终止指定进程及其所有的线程
			TerminateProcess(handle1, 0);
		}

	}
	// 一定记得关闭句柄，否则容易导致资源泄露
	CloseHandle(handle);

	return 0;
}