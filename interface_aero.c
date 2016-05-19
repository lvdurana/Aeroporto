#include "f_base.h"

int inicializar_contador_de_frames(HWND hwnd, frame_count *frame){
    frame->counter = 0;
    frame->last_frame = clock();
    frame->clocks_per_frame = CLOCKS_PER_SEC / MAX_FPS;
    SetTimer(hwnd,FPS_TIMER ,frame->clocks_per_frame,(TIMERPROC)NULL);

}

int atualizar_frame(HWND hwnd, frame_count *frame){
    clock_t cur_time, time_diff;
    cur_time = clock();
    time_diff = frame->clocks_per_frame - (cur_time - frame->last_frame);
    time_diff = time_diff < 0 ? 0 : time_diff;
    frame->last_frame = cur_time;

    //printf("%d\n", time_diff);
    SetTimer(hwnd,FPS_TIMER ,time_diff ,(TIMERPROC)NULL);


}

void desenhar_tela(HDC hdc, HBITMAP *bitmaps, fila *fila_decolagem, fila *fila_pouso){

    HBITMAP hMemBitmap = CreateCompatibleBitmap(hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
    HDC fundo= CreateCompatibleDC(hdc);
    SelectObject(fundo,bitmaps[BMP_FUNDO]);
    HDC hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, hMemBitmap);

    //Desenhar fundo
    BitBlt(hdcMem, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, fundo, 0, 0, SRCCOPY);

    char *text = (char*)malloc(sizeof(char)*50);
    sprintf(text,"Próximo pouso");

    if(fila_pouso != NULL)
        desenhar_caixa_info(hdcMem,bitmaps,AP_LIST_A_X,AP_LIST_A_Y,AP_LIST_WIDTH,AP_LIST_HEIGHT,fila_pouso->dado,text);
    else
        desenhar_caixa_info(hdcMem,bitmaps,AP_LIST_A_X,AP_LIST_A_Y,AP_LIST_WIDTH,AP_LIST_HEIGHT,NULL,text);

    sprintf(text,"Próxima decolagem");

    if(fila_decolagem != NULL)
        desenhar_caixa_info(hdcMem,bitmaps,AP_LIST_B_X,AP_LIST_B_Y,AP_LIST_WIDTH,AP_LIST_HEIGHT,fila_decolagem->dado,text);
    else
        desenhar_caixa_info(hdcMem,bitmaps,AP_LIST_B_X,AP_LIST_B_Y,AP_LIST_WIDTH,AP_LIST_HEIGHT,NULL,text);

    BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcMem, 0, 0, SRCCOPY);

    DeleteObject(hMemBitmap);

    DeleteDC(fundo);
    DeleteDC(hdcMem);
};

void desenhar_lista(HDC hdc, HBITMAP *bitmaps, fila *fila_desenhar){

    HBITMAP hMemBitmap = CreateCompatibleBitmap(hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
    HDC hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, hMemBitmap);
    fila *aux = fila_desenhar;
    char *text = (char*)malloc(sizeof(char)*50);
    int count = 0;


    //Desenhar elementos da fila na tela
    while(aux != NULL){
        int x = (count%3)*AP_LIST_WIDTH*BOX_WIDTH;
        int y = (count/3)*AP_LIST_HEIGHT*BOX_HEIGHT;

        sprintf(text,"Avião #%d",count+1);
        desenhar_caixa_info(hdcMem,bitmaps, x, y,AP_LIST_WIDTH,AP_LIST_HEIGHT,aux->dado,text);
        aux = aux->prox;
        count++;

    };


    BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcMem, 0, 0, SRCCOPY);
    DeleteObject(hMemBitmap);
    DeleteDC(hdcMem);

};



void escrever_info_aviao(HDC hdc, int ox, int oy, aviao *av, char *title){

    int desl_y = oy;
    int desl_x1 = ox + BOX_X_PADDING_1;
    int desl_x2 = ox + BOX_X_PADDING_1 + SPRITE_AV_WIDTH + BOX_X_PADDING_1;
    char *text = (char*)malloc(sizeof(char)*50);

    desenhar_texto(hdc,title,TEXT_SIZE_A,desl_x1,desl_y,TEXT_SIZE_LIMIT_2,DT_WORDBREAK|DT_CENTER);

    if(av != NULL){
        int hora = av->hora/3600;
        int min = (av->hora%3600)/60;
        int sec = av->hora%60;

        desl_y += TEXT_SIZE_A + BOX_Y_PADDING_2;
        desenhar_texto(hdc,av->nome,TEXT_SIZE_A,desl_x2,desl_y,TEXT_SIZE_LIMIT_1,DT_WORDBREAK);

        desl_y += TEXT_SIZE_A;
        sprintf(text,"Cod: %s",av->codigo);
        desenhar_texto(hdc,text,TEXT_SIZE_B,desl_x2,desl_y,TEXT_SIZE_LIMIT_1,DT_WORDBREAK);

        desl_y += TEXT_SIZE_B;
        sprintf(text,"Saída: %02d:%02d:%02d",hora,min,sec);
        desenhar_texto(hdc,text,TEXT_SIZE_B,desl_x2,desl_y,TEXT_SIZE_LIMIT_1,DT_WORDBREAK);

        desl_y = oy+ BOX_Y_PADDING_1 + BOX_Y_PADDING_2 + TEXT_SIZE_A + SPRITE_AV_HEIGHT + BOX_Y_PADDING_1;
        sprintf(text,"Origem: %s",av->origem);
        desenhar_texto(hdc,text,TEXT_SIZE_B,desl_x1,desl_y,TEXT_SIZE_LIMIT_2,DT_WORDBREAK);

        desl_y += TEXT_SIZE_B;
        sprintf(text,"Destino: %s",av->destino);
        desenhar_texto(hdc,text,TEXT_SIZE_B,desl_x1,desl_y,TEXT_SIZE_LIMIT_2,DT_WORDBREAK);

        desl_y += TEXT_SIZE_B;
        sprintf(text,"Num. passageiros: %d",av->num_passageiros);
        desenhar_texto(hdc,text,TEXT_SIZE_B,desl_x1,desl_y,TEXT_SIZE_LIMIT_2,DT_WORDBREAK);
    }


}

void desenhar_caixa(HDC hdc, HBITMAP caixa, int ox, int oy, int width, int height){

    HDC dc_caixa= CreateCompatibleDC(hdc);
    SelectObject(dc_caixa,caixa);

    int i,j;
    int right_seg = BOX_WIDTH*2;
    int bottom_seg = BOX_HEIGHT*2;

    //Desenhar grade
    BitBlt(hdc, ox, oy, BOX_WIDTH, BOX_HEIGHT, dc_caixa, 0, 0, SRCCOPY);
    for(i=1;i<width-1;i++)
    BitBlt(hdc, ox+BOX_WIDTH*i, oy, BOX_WIDTH, BOX_HEIGHT, dc_caixa, BOX_WIDTH, 0, SRCCOPY);
    //desenhar final do bitmap
    if(width > 1)
        BitBlt(hdc, ox+BOX_WIDTH*i, oy, BOX_WIDTH, BOX_HEIGHT, dc_caixa, right_seg, 0, SRCCOPY);

    for(j=1;j<height-1;j++){
        BitBlt(hdc, ox, oy+BOX_HEIGHT*j, BOX_WIDTH, BOX_HEIGHT, dc_caixa, 0, BOX_HEIGHT, SRCCOPY);
        for(i=1;i<width-1;i++)
            BitBlt(hdc, ox+BOX_WIDTH*i, oy+BOX_HEIGHT*j, BOX_WIDTH, BOX_HEIGHT, dc_caixa, BOX_WIDTH, BOX_HEIGHT, SRCCOPY);
        if(width > 1)
            BitBlt(hdc, ox+BOX_WIDTH*i, oy+BOX_HEIGHT*j, BOX_WIDTH, BOX_HEIGHT, dc_caixa, right_seg, BOX_HEIGHT, SRCCOPY);
    }
    if(height > 1){
        BitBlt(hdc, ox, oy+BOX_HEIGHT*j, BOX_WIDTH, BOX_HEIGHT, dc_caixa, 0, bottom_seg, SRCCOPY);
        for(i=1;i<width-1;i++)
            BitBlt(hdc, ox+BOX_WIDTH*i, oy+BOX_HEIGHT*j, BOX_WIDTH, BOX_HEIGHT, dc_caixa, BOX_WIDTH, bottom_seg, SRCCOPY);
        if(width > 1)
            BitBlt(hdc, ox+BOX_WIDTH*i, oy+BOX_HEIGHT*j, BOX_WIDTH, BOX_HEIGHT, dc_caixa, right_seg, bottom_seg, SRCCOPY);
    };


    DeleteDC(dc_caixa);


};

void desenhar_caixa_info(HDC hdc, HBITMAP *bitmaps, int ox, int oy, int width, int height, aviao *av, char *text){

    HDC dc_aviao = CreateCompatibleDC(hdc);
    int x_aviao = ox+BOX_X_PADDING_1;
    int y_aviao = oy+BOX_Y_PADDING_1 + TEXT_SIZE_A + BOX_Y_PADDING_2;


    desenhar_caixa(hdc,bitmaps[BMP_CAIXA], ox, oy, width, height);


    if(av != NULL){
        SelectObject(dc_aviao , bitmaps[BMP_AVIAO]);
        BitBlt(hdc,x_aviao,y_aviao,SPRITE_AV_WIDTH,SPRITE_AV_HEIGHT,dc_aviao,0,0,SRCAND);

        SelectObject(dc_aviao,bitmaps[BMP_AVIAO_MASK]);
        BitBlt(hdc,x_aviao,y_aviao,SPRITE_AV_WIDTH,SPRITE_AV_HEIGHT,dc_aviao,0,0,SRCPAINT);
    };

    escrever_info_aviao(hdc,ox,oy,av,text);

    DeleteDC(dc_aviao);
}

void desenhar_texto(HDC hdc, char *text, int size, int pos_x, int pos_y, int limit_x, int modifier){
    RECT rect;

    rect.top = pos_y;
    rect.left = pos_x;
    rect.right = pos_x+limit_x;
    rect.bottom = pos_y+size;

    HFONT font_old;
    HFONT font = CreateFont(
     size, //    nHeight,
     0, //     nWidth,
     0, //     nEscapement,
     0, //_In_ int     nOrientation,
     400, //_In_ int     fnWeight,
     0, //  fdwItalic,
     0, //   fdwUnderline,
     0, //   fdwStrikeOut,
     ANSI_CHARSET, //   fdwCharSet,
     OUT_DEFAULT_PRECIS, //   fdwOutputPrecision,
     CLIP_DEFAULT_PRECIS, //   fdwClipPrecision,
     DEFAULT_QUALITY, //   fdwQuality,
     DEFAULT_PITCH | FF_DONTCARE, //   fdwPitchAndFamily,
     NULL // lpszFace
    );


    font_old = SelectObject(hdc,font);

    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, text, -1, &rect, modifier);
    SelectObject(hdc,font_old);
    DeleteObject(font);
}


HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
{
    HDC hdcMem, hdcMem2;
    HBITMAP hbmMask;
    BITMAP bm;

    // Create monochrome (1 bit) mask bitmap.

    GetObject(hbmColour, sizeof(BITMAP), &bm);
    hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

    // Get some HDCs that are compatible with the display driver

    hdcMem = CreateCompatibleDC(0);
    hdcMem2 = CreateCompatibleDC(0);

    SelectObject(hdcMem, hbmColour);
    SelectObject(hdcMem2, hbmMask);

    // Set the background colour of the colour image to the colour
    // you want to be transparent.
    SetBkColor(hdcMem, crTransparent);

    // Copy the bits from the colour image to the B+W mask... everything
    // with the background colour ends up white while everythig else ends up
    // black...Just what we wanted.

    BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

    // Take our new mask and use it to turn the transparent colour in our
    // original colour image to black so the transparency effect will
    // work right.
    BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);

    // Clean up.

    DeleteDC(hdcMem);
    DeleteDC(hdcMem2);

    return hbmMask;
}

void criar_botoes(HWND hwnd, HWND *buttons){
    //Estabelecer posições origem
    int x_pos = AP_LIST_A_X - BUTTONS_WIDTH - BUTTONS_PADDING;
    int y_pos = 0;


    buttons[BT_PERMITIR_POUSO] = CreateWindow("BUTTON", "Permitir pouso",WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x_pos, y_pos, BUTTONS_WIDTH, BUTTONS_HEIGHT,
        hwnd, NULL, GetModuleHandle(NULL), NULL);
    x_pos -= BUTTONS_WIDTH + BUTTONS_PADDING;

    buttons[BT_LISTA_POUSOS] = CreateWindow("BUTTON", "Lista de pousos",WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x_pos, y_pos, BUTTONS_WIDTH, BUTTONS_HEIGHT,
        hwnd, NULL, GetModuleHandle(NULL), NULL);
    x_pos = AP_LIST_WIDTH*BOX_WIDTH + BUTTONS_PADDING;
    y_pos = CANVAS_HEIGHT - BUTTONS_HEIGHT;

    buttons[BT_PERMITIR_DECOLAGEM] = CreateWindow("BUTTON", "Permitir decolagem",WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x_pos, y_pos, BUTTONS_WIDTH, BUTTONS_HEIGHT,
        hwnd, NULL, GetModuleHandle(NULL), NULL);
    x_pos += BUTTONS_WIDTH + BUTTONS_PADDING;

    buttons[BT_LISTA_DECOLAGENS] = CreateWindow("BUTTON", "Lista de decolagens",WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x_pos, y_pos, BUTTONS_WIDTH, BUTTONS_HEIGHT,
        hwnd, NULL, GetModuleHandle(NULL), NULL);



};


