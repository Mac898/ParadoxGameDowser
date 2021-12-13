//Static Defines
#define INITGUID

//Headers
#include <winerror.h>
#include <iostream>
#include <list>
#include <algorithm>
#include <cstring>
#include <string>
#include <filesystem>
#include <locale>
#include <codecvt>
#include <unistd.h>
#include <Windows.H>
#include <shlobj.h>
#include <knownfolders.h>
#include <combaseapi.h>
#include <winapifamily.h>
#include <minwindef.h>
#include <processthreadsapi.h>
using namespace std;

//Declerations
bool isparadox, isnotparadox, ispa;
const wchar_t* paradoxpath = L"\\Programs\\Paradox Interactive\\bootstrapper-v2.exe";
const wchar_t* notparadoxpath = L"D:\\games\\NotParadoxLauncher\\bootstrapper-v2.exe";
const wchar_t* papath = L"\\Prison Architect64.exe";
STARTUPINFOW si;
PROCESS_INFORMATION pi;

//main
int main(int argc, char *args[]) {
    //Get launcher directories
    PWSTR localDir = NULL;
    SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &localDir);
    //add together the path
    wstring paradoxs(localDir);
    paradoxs += wstring(paradoxpath);
    const wchar_t* paradox = paradoxs.c_str();
    //print debug
    //printf("%S",(wchar_t *)paradox);
    //cout << "\n";
    //printf("%S",(wchar_t *)notparadoxpath);
    //Find out if it is paradox or not paradox
    for(int i = 0; i < argc; i++) {
        if(strcmp(args[i], "--paradox") == 0) {
            isparadox = true;
            isnotparadox = false;
            ispa = false;
        }
        if(strcmp(args[i], "--notparadox") == 0) {
            isnotparadox = true;
            isparadox = false;
            ispa = false;
        }
        if(strcmp(args[i], "--pa") == 0) {
            isnotparadox = false;
            isparadox = false;
            ispa = true;
        }
    }
    //Duplicate args into the finalargs for pass to the bootstrapper.
    //Debug 
    //Get current working directory for arguments to bootstrapper-v2.exe
    wchar_t wd[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, wd);
    //wcout << endl << wd;
    //Prepare args into the final cmdline
    const wchar_t* arg1 = L"--pdxlGameDir ";
    const wchar_t* arg2 =  L" --gameDir ";
    wstring cmdline = wstring(arg1);
    cmdline += wstring(wd);
    cmdline += wstring(L" ");
    cmdline += wstring(arg2);
    cmdline += wstring(wd);
    wchar_t* cmdlinef = const_cast<wchar_t*>(cmdline.c_str());
    //finally, launch the appropriate launcher
    //cout << isparadox << "\n" << isnotparadox;
    if(isparadox) {
        //Launch paradox launcher passing arguments
        memset(&si, 0, sizeof(si));
        si.cb = sizeof(si);
        CreateProcessW(paradox, cmdlinef, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    } else if(isnotparadox) {
        //Launch not paradox launcher passing arguments
        memset(&si, 0, sizeof(si));
        si.cb = sizeof(si);
        CreateProcessW(notparadoxpath, cmdlinef, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    } else if(ispa) {
        wstring paps(wd);
        paps += wstring(papath);
        const wchar_t* pap = paps.c_str();
        wcout << pap;
        CreateProcessW(pap, L"", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    }
    else {
        return 1;
    }
    return 0;
}