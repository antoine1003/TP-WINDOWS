#include <stdio.h>
#include "dllhandler.h"
#include "parking2.h"
#include <thread>
// Library loaded to be able to use the bool type
#include<iostream>

#define SLEEPING_TIME_MS 5000

#pragma region Variables Declaration
// Structure for the param of apagar func
struct PARKING2_aparcar_params {
    HCoche HC;
    void* DATOS;
    TIPO_FUNCION_APARCAR_COMMIT FUNC_APAG_COM;
    TIPO_FUNCION_PERMISO_AVANCE FUNC_PERM_AVA;
    TIPO_FUNCION_PERMISO_AVANCE_COMMIT FUNC_PERM_AVA_COM;
};

// Debug state
bool debug = false;
// Parking useed
int firstFitParking[80] = {0};
int bestFitParking[80] = {0};
int worstFitParking[80] = {0};
int lastFitParking[80] = {0};
using namespace std;
#pragma endregion

#pragma region Declaration DLL functions
// Function PARKING2_inicio
typedef int (*DLLFUNCINIT)(TIPO_FUNCION_LLEGADA f_llegadas[], TIPO_FUNCION_SALIDA f_salidas[],long intervalo, bool d);
 DLLFUNCINIT fPARKING2_inicio;

// Function PARKING2_fin
typedef int (*DLLFUNCFIN)();
DLLFUNCFIN fPARKING2_fin;

// Function PARKING2_getNUmero
typedef int (*DLLFUNCGETNUM)(HCoche hc);
DLLFUNCGETNUM fPARKING2_getNUmero;

//Function PARKING2_aparcar
typedef int (*DLLFUNCAPAC)(HCoche ,void *datos,TIPO_FUNCION_APARCAR_COMMIT, TIPO_FUNCION_PERMISO_AVANCE, TIPO_FUNCION_PERMISO_AVANCE_COMMIT);
DLLFUNCAPAC fPARKING2_aparcar;

//Function fPARKING2_getDatos
typedef void* (*DLLFUNCGETDAT)(HCoche);
DLLFUNCGETDAT fPARKING2_getDatos;
#pragma endregion

#pragma region Library handling
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
    // Converting it to the LPCWSTR format
    // Loading the DLL
    HINSTANCE loadLibraryReturnValue = LoadLibrary("parking2.dll");

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
            #pragma region Load all needed functions
            // PARKING2_inicio
            fPARKING2_inicio = (DLLFUNCINIT)GetProcAddress(loadLibraryReturnValue,"PARKING2_inicio");
            if (fPARKING2_inicio == NULL)
            {
                printf("Error: GetProcAddress inicio\n");
                return -1;
            }
            
            // PARKING2_fin
            fPARKING2_fin = (DLLFUNCFIN)GetProcAddress(loadLibraryReturnValue,"PARKING2_fin");
            if (fPARKING2_fin == NULL)
            {
                printf("Error: GetProcAddress fPARKING2_fin\n");
                return -1;
            }

            // PARKING2_getNUmero
            fPARKING2_getNUmero = (DLLFUNCGETNUM)GetProcAddress(loadLibraryReturnValue,"PARKING2_getNUmero");
            if (fPARKING2_getNUmero == NULL)
            {
                printf("Error: GetProcAddress PARKING2_getNUmero\n");
                return -1;
            }

            // PARKING2_getNUmero
            fPARKING2_aparcar = (DLLFUNCAPAC)GetProcAddress(loadLibraryReturnValue,"PARKING2_aparcar");
            if (fPARKING2_aparcar == NULL)
            {
                printf("Error: GetProcAddress PARKING2_aparcar\n");
                return -1;
            }

            // PARKING2_getNUmero
            fPARKING2_getDatos = (DLLFUNCGETDAT)GetProcAddress(loadLibraryReturnValue,"PARKING2_getDatos");
            if (fPARKING2_getDatos == NULL)
            {
                printf("Error: GetProcAddress PARKING2_getDatos\n");
                return -1;
            }
            #pragma endregion
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

#pragma endregion

#pragma region permisoAvance & permisoAvanceCommit & commit
void goForward(HCoche hc)
{

}

void goForwardCommit(HCoche hc)
{
    
}

void commit(HCoche hc)
{

}
#pragma endregion

#pragma region Wrapper function
DWORD WINAPI PARKING2_aparcar_wrapper(LPVOID lpParameter)
{
    PARKING2_aparcar_params *params = (PARKING2_aparcar_params*) lpParameter;
    int value = fPARKING2_aparcar(params->HC, params->DATOS, params->FUNC_APAG_COM, params->FUNC_PERM_AVA, params->FUNC_PERM_AVA_COM);
    delete params;
    cout << "Moncul";
    return value;
}
#pragma endregion

#pragma region Entering car functions  
int firstFitEnter(HCoche hc)
{   
    // Thread parametters
    DWORD threadId;

    PARKING2_aparcar_params* params = new PARKING2_aparcar_params;
    params->HC = hc;
    params->DATOS = fPARKING2_getDatos(hc);
    params->FUNC_APAG_COM = &commit;
    params->FUNC_PERM_AVA = &goForward;
    params->FUNC_PERM_AVA_COM = &goForwardCommit;

    HANDLE hThread = CreateThread(NULL, 0, &PARKING2_aparcar_wrapper, params, 0, &threadId);
    if (!hThread)
        delete params;
    cout << "***********************Bite";
    return 0;
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
    printf("firstFitOut");
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
        printf("Please provide the required parameter(s)");
        exit(0);
    }

    loadLibrary();

    int returnInicio = fPARKING2_inicio(f_llegadasP, f_salidaP, interval, debug);

    if (returnInicio == -1) {
        PERROR("fPARKING2_inicio");
        exit(0);
    }
    
    Sleep(SLEEPING_TIME_MS);
    int finReturn = fPARKING2_fin();
    
    return 0;
}

/* 
 * TODO: - Add FreeLibrary() when the programme is closed
 *       - 
 */