#include <stdio.h>
#include "dllhandler.h"
// Library loaded to be able to use the bool type
#include <stdbool.h>

/* 
 * TODO: Add FreeLibrary() when the programme is closed
 */

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    // Perform actions based on the reason for calling.
    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            printf("Initialize once for each new process...\n");
            // Initialize once for each new process.
            // Return FALSE to fail DLL load.
            break;
        }

        case DLL_THREAD_ATTACH:
        {
            printf("Do thread-specific initialization...\n");
            // Do thread-specific initialization.
            break;
        }

        case DLL_THREAD_DETACH:
        {
            printf("Do thread-specific cleanup...\n");
            // Do thread-specific cleanup.
            break;
        }

        case DLL_PROCESS_DETACH:
        {
            printf("Perform any necessary cleanup...\n");
            // Perform any necessary cleanup.
            break;
        }
    }

    // Successful DLL_PROCESS_ATTACH.
    return TRUE;
}


/**
 * Load the library
 * @return 0 if the library is successfuly chared, -1 otherwise
 */
int loadLibrary()
{
    // Full path to the library
    char* lpLibFileName = "C:/Users/antoi/Google Drive/ESAIP/ERASMUS2/SOII/TP WINDOWS/parking2.dll";

    // Loading the DLL
    HINSTANCE loadLibraryReturnValue = LoadLibrary(lpLibFileName);

    // Check is the library has been charged correctly
    if(loadLibraryReturnValue != NULL){
        /*
         * Initialize the library
         * The third parameter is NULL if we are using a dynamic load and non-NULL for a static load
         */
        bool returnValueDllMain = DllMain(loadLibraryReturnValue, DLL_PROCESS_ATTACH, NULL);

        // Check if the initialization was successful
        if(returnValueDllMain == TRUE)
        {
            FARPROC getProcAddReturnValue = GetProcAddress(loadLibraryReturnValue,"PARKING2_inicio");
            if (getProcAddReturnValue == NULL)
            {
                printf("Error: GetProcAddress\n");
                return -1;
            }
            printf("El valor de la funcion es %d.\n", getProcAddReturnValue()); 
        }
        else
        {
            printf("Error: DllMain\n");
            return -1;
        }
    }
    else
    {
        printf("Error: LoadLibrary\n");
        return -1;
    }
}

 /*HANDLE GetStdHandle( DWORD nStdHandle);
    // Puede ser el parámetro: STD_INPUT_HANDLE, STD_OUTPUT_HANDLE o STD_ERROR_HANDLE.
BOOL SetStdHandle( DWORD nStdHandle, HANDLE hHandle);*/

int main()
{
    loadLibrary();
    // printf() displays the string inside quotation
    printf("Hello, ta mere!");
    return 0;
}