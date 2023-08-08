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

    if (argv[1] == "add")
    { //--- SAVE
        std::cout << "Adding" << std::endl;
        char *password = "brillant";
        DWORD cbCreds = 1 + strlen(password);

        CREDENTIALW cred = {0};
        cred.Type = CRED_TYPE_GENERIC;
        cred.TargetName = L"FOO/account";
        cred.CredentialBlobSize = cbCreds;
        cred.CredentialBlob = (LPBYTE)password;
        cred.Persist = CRED_PERSIST_LOCAL_MACHINE;
        cred.UserName = L"paula";

        BOOL ok = ::CredWriteW(&cred, 0);
        wprintf(L"CredWrite() - errno %d\n", ok ? 0 : ::GetLastError());
        if (!ok)
            exit(1);
    }

    if (strcmp(argv[1], "get") == 0)
    { //--- RETRIEVE
        std::cout << "Getting" << std::endl;
        PCREDENTIALW pcred;
        BOOL ok = ::CredReadW(L"FOO/account", CRED_TYPE_GENERIC, 0, &pcred);
        wprintf(L"CredRead() - errno %d\n", ok ? 0 : ::GetLastError());
        if (!ok)
            exit(1);
        wprintf(L"Read username = '%s', password='%S' (%d bytes)\n",
                pcred->UserName, (char *)pcred->CredentialBlob, pcred->CredentialBlobSize);
        // Memory allocated by CredRead() must be freed!
        ::CredFree(pcred);
    }
}