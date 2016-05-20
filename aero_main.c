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

character avi_character;


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
    wincl.hIcon = LoadIcon (MAKEINTRESOURCE(AP_ICON), IDI_APPLICATION);
    wincl.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(AP_ICON), IMAGE_ICON, 16, 16, 0);
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

    liberar_lista(&fila_decolagem);
    liberar_lista(&fila_pouso);


};

BOOL CALLBACK window_edit_proc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_INITDIALOG:
            if(fila_ativa == FILA_DECOLAGEM){
                SetWindowText (hwnd, "Nova decolagem");
                SendDlgItemMessage(hwnd,TEXT_PLACE,WM_SETTEXT,0,"Destino");
            }
            else{
                SetWindowText (hwnd, "Novo pouso");
                SendDlgItemMessage(hwnd,TEXT_PLACE,WM_SETTEXT,0,"Origem");
            };
        break;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case ADD_BUTTON:

                    switch(fila_ativa){
                        case FILA_DECOLAGEM:
                            if(contar_elem_fila(fila_decolagem) < LIST_MAX_SIZE)
                                adicionar_aviao_lista(hwnd,&fila_decolagem,FILA_DECOLAGEM);

                        break;
                        case FILA_POUSO:
                            if(contar_elem_fila(fila_decolagem) < LIST_MAX_SIZE)
                                adicionar_aviao_lista(hwnd,&fila_pouso,FILA_POUSO);

                        break;
                    }

                    SendMessage(hwnd,WM_CLOSE,0,0);
                    InvalidateRect(ap_active_window,NULL,1);
                    UpdateWindow(ap_active_window);


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
        case WM_INITDIALOG:
            if(fila_ativa == FILA_DECOLAGEM)
                SetWindowText (hwnd, "Fila de Decolagens");
            else
                SetWindowText (hwnd, "Fila de Pousos");
        break;
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
                desenhar_lista(hwnd, hdc,bitmaps,fila_decolagem);
                sprintf(num,"%d",contar_elem_fila(fila_decolagem));
            }
            else{
                desenhar_lista(hwnd, hdc,bitmaps,fila_pouso);
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
                inicializar_personagem(&avi_character);

                /*avi_character.visible = TRUE;
                avi_character.pos.y = 100;

                aviao *avi = cria_nodo_aviao();
                sprintf(avi->nome,"Boeing");
                sprintf(avi->codigo,"566");
                sprintf(avi->origem,"Nevada-US");
                sprintf(avi->destino,"Hinamizawa-JP");
                avi->hora = 200;
                avi->num_passageiros = 78;

                push(&fila_pouso,avi);*/


                criar_botoes(hwnd, buttons);

                inicializar_contador_de_frames(hwnd, &FPS_count);
                UpdateWindow(hwnd);
            };
            break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            desenhar_tela(hdc,bitmaps,fila_decolagem,fila_pouso,&avi_character);

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
                                if(fila_decolagem != NULL){
                                    avi_character.direction = DIRECTION_LEFT;
                                    iniciar_animacao(&avi_character);
                                    pop(&fila_decolagem);
                                    verificar_botoes_permitidos(buttons,&avi_character);
                                };
                            }
                            else
                            if(lParam == buttons[BT_PERMITIR_POUSO]){
                                if(fila_pouso != NULL){
                                    avi_character.direction = DIRECTION_RIGHT;
                                    iniciar_animacao(&avi_character);
                                    pop(&fila_pouso);
                                    verificar_botoes_permitidos(buttons,&avi_character);
                                };
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
                    if(avi_character.active)
                        if(atualizar_animacao(&avi_character))
                            verificar_botoes_permitidos(buttons,&avi_character);
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
