#include "f_base.h"

TCHAR szClassName[] = _T("Aeroporto");
HWND main_window;

//Elementos do labirinto

frame_count FPS_count;

//Elementos da interface
HBITMAP bitmaps[NUM_BITMAPS];
HWND buttons[NUM_BUTTONS];

fila *fila_decolagem, *fila_pouso;



LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = NULL;//(HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;//MAKEINTRESOURCE(IDR_MYMENU);
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    if (!RegisterClassEx (&wincl))
        return 0;

    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Labirinto do Rato"),       /* Title Text */
           WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           WINDOW_WIDTH,                 /* The programs width */
           WINDOW_HEIGHT,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    ShowWindow (hwnd, nCmdShow);
    main_window = hwnd;

    //Loop principal
    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

};

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{



    switch (message)
    {
        case WM_CREATE:
            {
                bitmaps[BMP_FUNDO] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(AP_BMP_FUNDO));
                bitmaps[BMP_CAIXA] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(AP_BMP_CAIXA));
                bitmaps[BMP_AVIAO] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(AP_BMP_AVIAO));
                bitmaps[BMP_AVIAO_MASK] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(AP_BMP_AVIAO));
                //sprite = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(LR_BMP_RATO));
                //sprite_mask = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(LR_BMP_RATO));
                CreateBitmapMask(bitmaps[BMP_AVIAO_MASK],RGB(255,255,255));


                inicia_fila(&fila_decolagem);
                inicia_fila(&fila_pouso);

                aviao *avi = cria_nodo_aviao();
                sprintf(avi->nome,"Boeing");
                sprintf(avi->codigo,"566");
                sprintf(avi->origem,"Nevada-US");
                sprintf(avi->destino,"Hinamizawa-JP");
                avi->hora = 200;
                avi->num_passageiros = 78;

                push(&fila_decolagem,avi);


                criar_botoes(hwnd, buttons);

                inicializar_contador_de_frames(hwnd, &FPS_count);
                UpdateWindow(hwnd);
            };
            break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            desenhar_tela(hwnd,hdc,bitmaps,fila_decolagem,fila_pouso);

            EndPaint(hwnd, &ps);

        }
        break;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case BN_CLICKED:
                    if(1){
                        //verificar_botao_pressionado(hwnd,lParam,&lab,&rato,buttons);
                        InvalidateRect(hwnd,NULL,1);
                        UpdateWindow(hwnd);
                    };

                break;
            }
            break;
        case WM_LBUTTONDOWN:
            {

                //rato.active ^= 1;
            }
        break;

        case WM_TIMER:
            switch(wParam){

                case FPS_TIMER:
                    InvalidateRect(hwnd,NULL,1);
                    UpdateWindow(hwnd);
                    atualizar_frame(hwnd, &FPS_count);
                break;
            break;
            }
        break;
        case WM_ERASEBKGND:
            return (LRESULT)1;

        case WM_DESTROY:
            PostQuitMessage (0);
            break;
        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
};
