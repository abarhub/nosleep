// nosleep.cpp : Définit le point d'entrée de l'application.
//
#include <windows.h>
#include <shellapi.h>
#include <string> 
#include <codecvt> 

#include "framework.h"
#include "nosleep.h"
#include "easylogging++.h"
#include "config.h"
#include "traitement.h"

#define MAX_LOADSTRING 100

INITIALIZE_EASYLOGGINGPP

const int ID_TIMER1 = 105;

// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale

// Déclarations anticipées des fonctions incluses dans ce module de code :
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

Config* config = nullptr;
Traitement* traitement=nullptr;
boolean timerInitialise = false;

//using namespace std;
// Function to convert LPCWSTR to std::string 
std::string ConvertLPCWSTRToString(const LPCWSTR lpcwszStr)
{
    // Create a converter object to convert between wide 
    // strings and UTF-8 encoded strings 
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> > converter;

    // Create a wstring from the LPCWSTR input 
    std::wstring wstr(lpcwszStr);

    // Convert the wide string to an std::string 
    return converter.to_bytes(wstr);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    //lpCmdLine[0]

    LPWSTR* szArgList;
    int argCount;

    szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
    if (szArgList == NULL)
    {
        MessageBox(NULL, L"Unable to parse command line", L"Error", MB_OK);
        return 10;
    }

    for (int i = 0; i < argCount; i++)
    {
        //MessageBox(NULL, szArgList[i], L"Arglist contents", MB_OK);
        LOG(INFO) << "argument '" << szArgList[i] << "'";
    }
    if (argCount == 2) {
        std::string fichier = ConvertLPCWSTRToString(szArgList[1]);
        LOG(INFO) << "fichier de conf '" << fichier << "'";
        config = parseConfig(fichier);
    }
    else {
        LOG(ERROR) << "Erreur dans les paramètres";
    }

    LocalFree(szArgList);

    // TODO: Placez le code ici.

    // Initialise les chaînes globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_NOSLEEP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Effectue l'initialisation de l'application :
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NOSLEEP));

    MSG msg;

    

    // Boucle de messages principale :
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FONCTION : MyRegisterClass()
//
//  OBJECTIF : Inscrit la classe de fenêtre.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NOSLEEP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_NOSLEEP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FONCTION : InitInstance(HINSTANCE, int)
//
//   OBJECTIF : enregistre le handle d'instance et crée une fenêtre principale
//
//   COMMENTAIRES :
//
//        Dans cette fonction, nous enregistrons le handle de l'instance dans une variable globale, puis
//        nous créons et affichons la fenêtre principale du programme.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Stocke le handle d'instance dans la variable globale

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   SetTimer(hWnd, ID_TIMER1, 1000, (TIMERPROC)NULL);
   if (config != nullptr&&traitement==nullptr) {
       LOG(INFO) << "initialisation";
       traitement = new Traitement(config);
       traitement->initialisation();
       timerInitialise = true;
   }

   return TRUE;
}

//
//  FONCTION : WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  OBJECTIF : Traite les messages pour la fenêtre principale.
//
//  WM_COMMAND  - traite le menu de l'application
//  WM_PAINT    - Dessine la fenêtre principale
//  WM_DESTROY  - génère un message d'arrêt et retourne
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analyse les sélections de menu :
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Ajoutez ici le code de dessin qui utilise hdc...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        if (timerInitialise) {
            timerInitialise = false;
            KillTimer(hWnd, ID_TIMER1);
        }
        PostQuitMessage(0);
        break;
    case WM_TIMER:

        switch (wParam)
        {
        case ID_TIMER1:
            // process the 1-second timer 
            if (traitement != nullptr) {
                if (traitement->traitement()) {
                    KillTimer(hWnd, ID_TIMER1);
                    traitement->fin();
                    LOG(INFO) << "fin de traitement";
                    timerInitialise = false;
                    PostMessage(hWnd, WM_CLOSE, 0, 0);
                }
            }
            return 0;
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Gestionnaire de messages pour la boîte de dialogue À propos de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
