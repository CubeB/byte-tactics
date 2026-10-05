// Decompiled by Space Bunny Free. Names are provisional.
// Opens a URL in the browser. It tries ShellExecuteA on the URL first; when
// that fails (result <= 31) it looks the file association for the extension up
// under HKEY_CLASSES_ROOT, reads the key's default value into buf1, builds
// "<default>\shell\open\command" in the shared `sub` buffer, reads that
// key's default value into buf2, trims the program path out of the command
// line and finally runs that program with the URL as its parameter. If any of
// that fails, the message box at the end reports the last ShellExecute result.
// Notes on the shape the compiler needs:
// - the sub-key path and the error text are the same `sub` buffer, and the
//   registry key handle and the size/result dword are reused across both
//   RegQueryValueA calls (499 then 999);
// - the `if (p) *p = 0;` is written out in both arms of the quote test: that
//   duplication is what makes MSVC 5 emit a separate strchr call per arm
//   (448 bytes against the original's 460 with the store shared instead).
// Callers pass a http:// URL and the extension to look up (".htm").
#include <windows.h>
#include <stdio.h>

// FUNCTION: 0x4da5b0
void __cdecl OpenUrl(HWND hwnd, char* url, char* ext)
{
    int res = 0;
    char buf1[500];
    char buf2[1000];
    char sub[1000];
    HKEY key;
    long len;
    char* p;

    if (url) {
        res = (int)ShellExecuteA(hwnd, "open", url, 0, ".", 1);
    }
    if (res > 31) {
        return;
    }
    if (!ext) {
        if (url) {
            ext = strrchr(url, '.');
        }
        if (!ext) {
            goto failed;
        }
    }
    len = 499;
    if (RegOpenKeyA(HKEY_CLASSES_ROOT, ext, &key) == 0) {
        if (RegQueryValueA(key, 0, buf1, &len) == 0) {
            RegCloseKey(key);
            buf1[len] = 0;
            sprintf(sub, "%s\\shell\\open\\command", buf1);
            len = 999;
            if (RegOpenKeyA(HKEY_CLASSES_ROOT, sub, &key) == 0) {
                if (RegQueryValueA(key, 0, buf2, &len) == 0) {
                    RegCloseKey(key);
                    buf2[len] = 0;
                    if (buf2[0] == '"') {
                        p = strchr(buf2 + 1, '"');
                        if (p) {
                            *p = 0;
                        }
                    } else {
                        p = strchr(buf2, ' ');
                        if (p) {
                            *p = 0;
                        }
                    }
                    res = (int)ShellExecuteA(hwnd, "open", buf2, url, ".", 1);
                }
            }
        }
    }
failed:
    if (res > 31) {
        return;
    }
    sprintf(sub, "Cannot start %s. ShellExecute result is %d", url, res);
    MessageBoxA(hwnd, sub, "Cavedog", 0);
}
