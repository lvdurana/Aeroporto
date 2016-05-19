#include "f_base.h"

TCHAR szClassName[] = _T("Aeroporto");


//Elementos do labirinto

frame_count FPS_count;

//Elementos da interface

HWND main_window;
HWND ap_active_window = NULL;
HWND ap_edit_window = NULL;
HBITMAP bitmaps[NUM_BITMAPS];
HWND buttons[NUM_BUTTONS];

int fila_ativa = FILA_DECOLAGEM;

fila *fila_decolagem, *fila_pouso;


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
           _T("Aeroporto"),       /* Title Text */
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

BOOL CALLBACK window_edit_proc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case ADD_BUTTON:
                    {
                        aviao *av = cria_nodo_aviao();
                        char *text = (char*)malloc(sizeof(char)*10);
                        int num;

                        GetWindowText(GetDlgItem(hwnd,NAME_BOX),av->nome,MAX_CHARS_NOME);

                        GetWindowText(GetDlgItem(hwnd,COD_BOX),av->codigo,MAX_CHARS_COD);

                        if(fila_ativa == FILA_DECOLAGEM){
                            GetWindowText(GetDlgItem(hwnd,PLACE_BOX),av->destino,MAX_CHARS_LOCAL);
                            sprintf(av->origem,"%s",LOCAL_AEROPORTO);
                        }
                        else {
                            GetWindowText(GetDlgItem(hwnd,PLACE_BOX),av->origem,MAX_CHARS_LOCAL);
                            sprintf(av->destino,"%s",LOCAL_AEROPORTO);
                        }

                        GetWindowText(GetDlgItem(hwnd,TIME_BOX),text,10);
                        av->hora = atoi(text);

                        GetWindowText(GetDlgItem(hwnd,PASS_BOX),text,10);
                        av->num_passageiros = atoi(text);

                        if(fila_ativa == FILA_DECOLAGEM)
                            push(&fila_decolagem,av);
                        else
                            push(&fila_pouso,av);

                        SendMessage(hwnd,WM_CLOSE,0,0);
                        InvalidateRect(ap_active_window,NULL,1);
                        UpdateWindow(ap_active_window);
                    };
                break;
                case IDCANCEL:
                    SendMessage(hwnd,WM_CLOSE,0,0);
                break;
            }
        break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            ap_edit_window = NULL;
            break;
        default:
            return FALSE;
    }
    return TRUE;
}


BOOL CALLBACK window_list_proc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case ADD_BUTTON:
                    {
                        if(ap_edit_window == NULL)
                            ap_edit_window = CreateDialog(GetModuleHandle(NULL),MAKEINTRESOURCE(1001),hwnd,window_edit_proc);

                    };
                break;
            }
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            char *num = (char*)malloc(sizeof(char)*10);

            if(fila_ativa == FILA_DECOLAGEM){
                desenhar_lista(hdc,bitmaps,fila_decolagem);
                sprintf(num,"%d",contar_elem_fila(fila_decolagem));
            }
            else{
                desenhar_lista(hdc,bitmaps,fila_pouso);
                sprintf(num,"%d",contar_elem_fila(fila_pouso));
            };
            EndPaint(hwnd, &ps);


            SendDlgItemMessage(ap_active_window,TEXT_NUM_DE_AVIOES,WM_SETTEXT,0,num);

        }
        break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            ap_active_window = NULL;
            break;
        default:
            return FALSE;
    }
    return TRUE;
}


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

            desenhar_tela(hdc,bitmaps,fila_decolagem,fila_pouso);

            EndPaint(hwnd, &ps);

        }
        break;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case BN_CLICKED:
                    if(1){
                        if(ap_active_window == NULL){
                            if(lParam == buttons[BT_LISTA_DECOLAGENS]){
                                char *num = (char*)malloc(sizeof(char)*10);
                                sprintf(num,"%d",contar_elem_fila(fila_decolagem));

                                fila_ativa = FILA_DECOLAGEM;
                                ap_active_window = CreateDialog(GetModuleHandle(NULL),MAKEINTRESOURCE(1002),hwnd,window_list_proc);
                                SendDlgItemMessage(ap_active_window,TEXT_NUM_DE_AVIOES,WM_SETTEXT,0,num);

                            }
                            else
                            if(lParam == buttons[BT_LISTA_POUSOS]){
                                char *num = (char*)malloc(sizeof(char)*10);
                                sprintf(num,"%d",contar_elem_fila(fila_pouso));

                                fila_ativa = FILA_POUSO;
                                ap_active_window = CreateDialog(GetModuleHandle(NULL),MAKEINTRESOURCE(1002),hwnd,window_list_proc);
                                SendDlgItemMessage(ap_active_window,TEXT_NUM_DE_AVIOES,WM_SETTEXT,0,num);
                            }
                            else
                            if(lParam == buttons[BT_PERMITIR_DECOLAGEM]){
                                pop(&fila_decolagem);
                            }
                            else
                            if(lParam == buttons[BT_PERMITIR_POUSO]){
                                pop(&fila_pouso);
                            }
                        };
                        InvalidateRect(hwnd,NULL,1);
                        UpdateWindow(hwnd);
                    };

                break;
            }
            break;
        case WM_LBUTTONDOWN:
            {
                //if(ap_active_window == NULL)
                 //   ap_active_window = CreateDialog(GetModuleHandle(NULL),MAKEINTRESOURCE(1002),hwnd,window_list_proc);
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
