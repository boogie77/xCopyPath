#include "stdafx.h"

void changePath(TCHAR* path) {
	TCHAR* tmp = path;
	while (*tmp != L'\x00') {
		if (!_tcsncmp(tmp, L"\\", 1)) {
			*tmp = L'/';
		}
		tmp += 1;
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	int argc = 0;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	if (argc < 2)
		return 1;

	TCHAR path[MAX_PATH + 2]{};
	if (_tcslen(argv[1]) > MAX_PATH + 2)
		return 1;
	_tcsnccpy_s(path, argv[1], _tcslen(argv[1]));
	changePath(path);
	size_t pathSize = (_tcslen(path) + 1) * 2;
	if (!OpenClipboard(0))
		return 1;
	EmptyClipboard();
	HGLOBAL hClipboardData = GlobalAlloc(GMEM_MOVEABLE, pathSize);
	if (!hClipboardData) {
		DWORD err = GetLastError();
		// printf("GlobalAlloc: %ul\n", err);
		return err;
	}

	TCHAR* pData = (TCHAR*)GlobalLock(hClipboardData);

	memcpy_s(pData, pathSize, path, pathSize);

	GlobalUnlock(hClipboardData);

	SetClipboardData(CF_UNICODETEXT, hClipboardData);

	CloseClipboard();

    return 0;
}

