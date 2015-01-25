/*****************************************************************************\
Copyright (C) 2013-2014 <martinjk at outloook dot com>

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source distribution.
\*****************************************************************************/

#include <include/CH1Z1.h>
#include <include/H1Z1Def.h>
#include <iostream>
#include <string>

HANDLE proc;
char AnsiBuffer[255]; 
const char* destPtr = (const char *)AnsiBuffer;
HWND window = NULL;
TCHAR windowText[100];

bool EnableDebugPrivilege() 
{ 
    HANDLE hThis = GetCurrentProcess(); 
    HANDLE hToken; 
    OpenProcessToken(hThis, TOKEN_ADJUST_PRIVILEGES, &hToken); 
    LUID luid; 
    LookupPrivilegeValue(0, TEXT("setDebugPrivilege"), &luid); 
    TOKEN_PRIVILEGES priv; 
    priv.PrivilegeCount = 1; 
    priv.Privileges[0].Luid = luid; 
    priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
    AdjustTokenPrivileges(hToken, false, &priv, 0, 0, 0); 
    CloseHandle(hToken); 
    CloseHandle(hThis); 
    return true; 
} 

void TCharToChar(const wchar_t* Src, char* Dest, int Size)
{
#pragma warning(push)
#pragma warning(disable: 4267)
	WideCharToMultiByte(CP_ACP, 0, Src, wcslen(Src)+1, Dest , Size, NULL, NULL);
#pragma warning(pop)
} 

int main()
{
	CONSOLE_SCREEN_BUFFER_INFO csbiScreen;
	WORD wOldColAttr;
	GetConsoleScreenBufferInfo((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), &csbiScreen);
	wOldColAttr = csbiScreen.wAttributes;

    EnableDebugPrivilege();

    DWORD dwID;
    HWND sa = FindWindowA(NULL,H1Z1_WINDOW);

    GetWindowThreadProcessId(sa, &dwID);
    proc = OpenProcess(PROCESS_ALL_ACCESS, false, dwID);

	CH1Z1 h1Z1(proc);
	if (proc) 
	{	
		printf("Found -> process..");

		for(;;)
		{
			window = GetForegroundWindow();
			GetWindowText(window, windowText, 100);
			TCharToChar(windowText, AnsiBuffer, sizeof(AnsiBuffer));
			destPtr = (const char *)AnsiBuffer;

			// Only process when window is focused
			if (strcpy_s(AnsiBuffer, H1Z1_WINDOW))
				h1Z1.Process();
		}
	}
	else 
		printf("Failed to find process with H1Z1 version %s, check it!", H1Z1_VERSION);

	printf("Press a key in order to exit..");
	getchar();
}
