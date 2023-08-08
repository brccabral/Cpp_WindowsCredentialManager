#include <windows.h>
#include <wincred.h>
#include <wchar.h>
#pragma hdrstop

#pragma comment(lib, "advapi32.lib") // Or pass it to the cl command line.

#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return EXIT_FAILURE;
    }
    std::cout << argv[1] << std::endl;

    if (strcmp(argv[1], "help") == 0)
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "CredManager add <target> <username> <password>" << std::endl;
        std::cout << "CredManager get <target>" << std::endl;

        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "add") == 0)
    { //--- SAVE
        std::cout << "Adding" << std::endl;
        char *password = argv[4];

        wchar_t *wTargetName = new wchar_t[4096];
        MultiByteToWideChar(0, 0, argv[2], (int)strlen(argv[2]), wTargetName, (int)strlen(argv[2]));
        wchar_t *wUsername = new wchar_t[4096];
        MultiByteToWideChar(0, 0, argv[3], (int)strlen(argv[3]), wUsername, (int)strlen(argv[3]));

        DWORD cbCreds = (DWORD)(1 + strlen(password));

        CREDENTIALW cred = {0};
        cred.Type = CRED_TYPE_GENERIC;
        cred.TargetName = wTargetName;
        cred.CredentialBlobSize = cbCreds;
        cred.CredentialBlob = (LPBYTE)password;
        cred.Persist = CRED_PERSIST_LOCAL_MACHINE;
        cred.UserName = wUsername;

        BOOL ok = ::CredWriteW(&cred, 0);
        wprintf(L"CredWrite() - errno %d\n", ok ? 0 : ::GetLastError());

        delete[] wTargetName;
        delete[] wUsername;
        if (!ok)
            exit(1);
    }

    if (strcmp(argv[1], "get") == 0)
    { //--- RETRIEVE
        std::cout << "Getting" << std::endl;
        PCREDENTIALW pcred;
        wchar_t *wString = new wchar_t[4096];
        MultiByteToWideChar(CP_ACP, 0, argv[2], -1, wString, 4096);
        BOOL ok = ::CredReadW(wString, CRED_TYPE_GENERIC, 0, &pcred);
        wprintf(L"CredRead() - errno %d\n", ok ? 0 : ::GetLastError());

        delete[] wString;
        if (!ok)
            exit(1);
        wprintf(L"Read username = '%s', password='%S' (%d bytes)\n",
                pcred->UserName, (char *)pcred->CredentialBlob, pcred->CredentialBlobSize);
        // Memory allocated by CredRead() must be freed!
        ::CredFree(pcred);
    }
}