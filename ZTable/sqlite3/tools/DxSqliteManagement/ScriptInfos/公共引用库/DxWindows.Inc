{这个是 $I包含指令包含库的文件}
function Beep(dwFreq, dwDuration: DWORD): Boolean; stdcall; external 'kernel32.dll' name 'Beep';
function LoadLibrary(lpLibFileName: PChar): HMODULE;stdcall; external 'kernel32.dll' name 'LoadLibraryA';
function LoadLibraryEx(lpLibFileName: PChar; hFile: THandle; dwFlags: DWORD): HMODULE; stdcall; external 'kernel32.dll' name 'LoadLibraryExA';
function GetProcAddress(AhModule: HMODULE; lpProcName: PChar): pointer; stdcall; external 'kernel32.dll' name 'GetProcAddress';
procedure Sleep(dwMilliseconds: DWORD); stdcall;external 'kernel32.dll' name 'Sleep';
function GetSystemDirectory(lpBuffer: PChar; uSize: UINT): UINT; stdcall;external 'kernel32.dll' name 'GetSystemDirectoryA';
function GetWindowsDirectory(lpBuffer: PChar; uSize: UINT): UINT; stdcall;external 'kernel32.dll' name 'GetWindowsDirectoryA';
function GetTempPath(nBufferLength: DWORD; lpBuffer: PChar): DWORD; stdcall;external 'kernel32.dll' name 'GetTempPathA';
function GetTickCount: DWORD; stdcall;external 'kernel32.dll' name 'GetTickCount';
//这里如果你需要某些API函数请您自行添加