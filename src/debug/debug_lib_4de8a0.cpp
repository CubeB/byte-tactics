// Decompiled by space-bunny-free. Names are provisional.
// Builds the full path of a file that lives in the Visual C++ source tree.
// If the name has no directory part, the Visual Studio install directory is
// found once from the MSDevDir environment variable, cut down to its drive
// (everything from the first backslash on, and any ';' comment, is dropped),
// and the two usual source roots are tried in turn, MFC first, then the CRT.
// DAT_00528ae4 is the "MSDevDir already read" flag, DAT_00528ae8 the CRT root,
// DAT_00528ed0 the MFC root, DAT_005119b8 the empty default prefix.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char DAT_005119b8[];
extern char DAT_00528ae4;
extern char DAT_00528ae8[0x1e8];
extern char DAT_00528ed0[0x1e8];

bool __cdecl FileExists(char* dir, char* name);

// FUNCTION: 0x4de8a0
void __cdecl GetSourceFilePath(char* out, char* name)
{
    char* dir = DAT_005119b8;
    if (strchr(name, '\\') == 0) {
        if (!DAT_00528ae4) {
            DAT_00528ae4 = 1;
            char* msdev = getenv("MSDevDir");
            if (msdev) {
                strcpy(DAT_00528ae8, msdev);
                char* p = strchr(DAT_00528ae8, ';');
                if (p)
                    *p = 0;
                p = strrchr(DAT_00528ae8, '\\');
                if (p)
                    *p = 0;
                strcpy(DAT_00528ed0, DAT_00528ae8);
                strcat(DAT_00528ae8, "\\vc\\crt\\src\\");
                strcat(DAT_00528ed0, "\\vc\\mfc\\src\\");
            }
        }
        if (FileExists(DAT_00528ed0, name)) {
            dir = DAT_00528ed0;
        } else if (FileExists(DAT_00528ae8, name)) {
            dir = DAT_00528ae8;
        }
    }
    sprintf(out, "%s%s", dir, name);
}
