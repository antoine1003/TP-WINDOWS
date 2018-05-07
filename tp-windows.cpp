#include <stdio.h>
#include "dllhandler.h"
#include "parking2.h"
// Library loaded to be able to use the bool type
#include<iostream>

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

std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

/**
 * Load the library
 * @return 0 if the library is successfuly chared, -1 otherwise
 */
int loadLibrary(TIPO_FUNCION_LLEGADA f_llegadas[], TIPO_FUNCION_SALIDA f_salidas[],long intervalo, bool d)
{
    // Full path to the library
    // Converting it to the LPCWSTR format
    // Loading the DLL
    HINSTANCE loadLibraryReturnValue = LoadLibrary("C:/Users/antoi/Google Drive/ESAIP/ERASMUS2/SOII/TP WINDOWS/parking2.dll");

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
            FARPROC fPARKING2_inicio = GetProcAddress(loadLibraryReturnValue,"PARKING2_inicio");
            if (fPARKING2_inicio == NULL)
            {
                printf("Error: GetProcAddress\n");
                return -1;
            }
            printf("El valor de la funcion es %d.\n", fPARKING2_inicio()); 
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

#pragma region Entering car functions  
int firstFitEnter(HCoche hc)
{
  return -1;
} 

int nextFitEnter(HCoche hc)
{
   return -1;
}

int bestFitEnter(HCoche hc)
{  
  return -2;
}

int worstFitEnter(HCoche hc)
{
    return -2;
}
#pragma endregion  


#pragma region Getting out car functions
int firstFitOut(HCoche hc)
{
  return -1;
} 

int nextFitOut(HCoche hc)
{
   return -1;
}

int bestFitOut(HCoche hc)
{  
  return -2;
}

int worstFitOut(HCoche hc)
{
    return -2;
}
#pragma endregion

int main(int argc, char *argv[])
{
    TIPO_FUNCION_LLEGADA f_llegadasP[4] = {&firstFitEnter, &nextFitEnter, &bestFitEnter, &worstFitEnter};
    TIPO_FUNCION_SALIDA f_salidaP[4] = {&firstFitOut, &nextFitOut, &bestFitOut, &worstFitOut};
    // Check parameters    
    long interval = 10;
    bool debug = false;
    if(argc == 3)
    {
        interval = (long)argv[1];
        std::string debugStr = argv[2];
        if(debugStr.compare("D") == 0)
            debug = true;
    }
    else if(argc == 2){
        interval = (long)argv[1];
    }
    else 
    {
        printf("Please provide the requiered parameter(s)");
        exit(0);
    }

    loadLibrary(f_llegadasP, f_salidaP, interval, debug);
    // printf() displays the string inside quotation
    printf("Hello, World!");
    return 0;
}