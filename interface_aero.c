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

/*int inicializar_personagem(character *rato){
    rato->pos.x = INITIAL_POSITION_X*SIZE_CELL_X;
    rato->pos.y = INITIAL_POSITION_Y*SIZE_CELL_Y;
    rato->pos_map.x = 0;
    rato->pos_map.y = 0;
    rato->movement = 0;
    rato->speed = SPEED_LOW;
    rato->frame = 0;
    rato->frame_duration = FRAME_DURATION(SPEED_LOW);
    rato->pilha = criar_dado_pilha();
    rato->pilha->dado = INITIAL_POSITION;
    rato->pilha->prox = NULL;
    rato->active = FALSE;

}*/

int atualizar_movimento_personagem(character *pers){

    int move;
    //Executar movimentação
    if(pers->movement){
        move = (pers->speed > pers->movement) ? pers->movement : pers->speed;
        switch(pers->direction){
        case DIRECTION_UP:
            pers->pos.y -= move;
            break;
        case DIRECTION_DOWN:
            pers->pos.y += move;
            break;
        case DIRECTION_RIGHT:
            pers->pos.x += move;
            break;
        case DIRECTION_LEFT:
            pers->pos.x -= move;
            break;
        }

        pers->movement -= move;

    }

    //Atualizar frame
    pers->frame_duration--;
    if(!(pers->frame_duration)){
        pers->frame ^= 1;
        pers->frame_duration = FRAME_DURATION(pers->speed);
    }

}

int verificar_movimentacao(character *pers){
    if(pers->active && !pers->movement )
        return 1;
    return 0;

}




void desenhar_tela(HWND hwnd, HDC hdc, HBITMAP *bitmaps, fila *fila_decolagem, fila *fila_pouso){




    HBITMAP hMemBitmap = CreateCompatibleBitmap(hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
    HDC fundo= CreateCompatibleDC(hdc);
    SelectObject(fundo,bitmaps[BMP_FUNDO]);
    HDC hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, hMemBitmap);

    //Desenhar fundo
    BitBlt(hdcMem, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, fundo, 0, 0, SRCCOPY);

    //desenhar_caixa(hdcMem,bitmaps[BMP_CAIXA], AP_LIST_A_X, AP_LIST_A_Y, AP_LIST_WIDTH, AP_LIST_HEIGHT);
    //desenhar_caixa(hdcMem,bitmaps[BMP_CAIXA], AP_LIST_B_X, AP_LIST_B_Y, AP_LIST_WIDTH, AP_LIST_HEIGHT);
    char *text = (char*)malloc(sizeof(char)*50);
    sprintf(text,"Próxima decolagem");

    if(fila_decolagem != NULL)
        desenhar_caixa_info(hdcMem,bitmaps,AP_LIST_A_X,AP_LIST_A_Y,AP_LIST_WIDTH,AP_LIST_HEIGHT,fila_decolagem->dado,text);
    else
        desenhar_caixa_info(hdcMem,bitmaps,AP_LIST_A_X,AP_LIST_A_Y,AP_LIST_WIDTH,AP_LIST_HEIGHT,NULL,text);

    sprintf(text,"Próximo Pouso");

    if(fila_pouso != NULL)
        desenhar_caixa_info(hdcMem,bitmaps,AP_LIST_B_X,AP_LIST_B_Y,AP_LIST_WIDTH,AP_LIST_HEIGHT,fila_pouso->dado,text);
    else
        desenhar_caixa_info(hdcMem,bitmaps,AP_LIST_B_X,AP_LIST_B_Y,AP_LIST_WIDTH,AP_LIST_HEIGHT,NULL,text);

    /*SelectObject(fundo,bitmaps[BMP_AVIAO]);
    BitBlt(hdcMem,BOX_A_SPRITE_POSITION_X,BOX_A_SPRITE_POSITION_Y,SPRITE_AV_WIDTH,SPRITE_AV_HEIGHT,fundo,0,0,SRCAND);

    SelectObject(fundo,bitmaps[BMP_AVIAO_MASK]);
    BitBlt(hdcMem,BOX_A_SPRITE_POSITION_X,BOX_A_SPRITE_POSITION_Y,SPRITE_AV_WIDTH,SPRITE_AV_HEIGHT,fundo,0,0,SRCPAINT);*/

    //escrever_info_aviao(hdcMem,AP_LIST_A_X,AP_LIST_A_Y, fila_decolagem->dado);


    BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcMem, 0, 0, SRCCOPY);



    //SelectObject(hdcMem, hbmOld);

    DeleteObject(hMemBitmap);

    DeleteDC(fundo);
    DeleteDC(hdcMem);




};

void escrever_info_aviao(HDC hdc, int ox, int oy, aviao *av, char *title){

    int desl_y = oy;
    int desl_x1 = ox + BOX_X_PADDING_1;
    int desl_x2 = ox + BOX_X_PADDING_1 + SPRITE_AV_WIDTH + BOX_X_PADDING_1;
    char *text = (char*)malloc(sizeof(char)*50);

    desenhar_texto(hdc,title,TEXT_SIZE_A,desl_x1,desl_y,TEXT_SIZE_LIMIT_2,DT_WORDBREAK|DT_CENTER);

    if(av != NULL){
        desl_y += TEXT_SIZE_A + BOX_Y_PADDING_2;
        desenhar_texto(hdc,av->nome,TEXT_SIZE_A,desl_x2,desl_y,TEXT_SIZE_LIMIT_1,DT_WORDBREAK);

        desl_y += TEXT_SIZE_A;
        sprintf(text,"Cod: %s",av->codigo);
        desenhar_texto(hdc,text,TEXT_SIZE_B,desl_x2,desl_y,TEXT_SIZE_LIMIT_1,DT_WORDBREAK);

        desl_y += TEXT_SIZE_B;
        sprintf(text,"Saída: %d",av->hora);
        desenhar_texto(hdc,text,TEXT_SIZE_B,desl_x2,desl_y,TEXT_SIZE_LIMIT_1,DT_WORDBREAK);

        desl_y = oy+ BOX_Y_PADDING_1 + BOX_Y_PADDING_2 + TEXT_SIZE_A + SPRITE_AV_HEIGHT + BOX_Y_PADDING_1;
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

    //BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcMem, 0, 0, SRCCOPY);

    //DeleteObject(hMemBitmap);

    DeleteDC(dc_caixa);
    //DeleteDC(hdcMem);


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


    //sprintf(text,"%.*f",precision,dado);
    /*if(dado != 0){

        HWND mat_cor = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(GM_BMP_MATRIZ2));
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, mat_cor);
        BitBlt(hdc, pos_x+1, pos_y, M_FIELD_WIDTH-2, M_FIELD_HEIGHT, hdcMem, 0, 0, SRCCOPY);
    };*/

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
    int x_pos = 0;
    int y_pos = 0;


    buttons[PAUSE_BUTTON] = CreateWindow("BUTTON", "||",WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x_pos, y_pos, SPEED_BUTTONS_WIDTH, SPEED_BUTTONS_HEIGHT,
        hwnd, NULL, GetModuleHandle(NULL), NULL);
    x_pos += SPEED_BUTTONS_WIDTH + SPEED_BUTTONS_PADDING;

    buttons[SPEED_LOW_BUTTON] = CreateWindow("BUTTON", "1",WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x_pos, y_pos, SPEED_BUTTONS_WIDTH, SPEED_BUTTONS_HEIGHT,
        hwnd, NULL, GetModuleHandle(NULL), NULL);
    x_pos += SPEED_BUTTONS_WIDTH + SPEED_BUTTONS_PADDING;

    buttons[SPEED_MID_BUTTON] = CreateWindow("BUTTON", "2",WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x_pos, y_pos, SPEED_BUTTONS_WIDTH, SPEED_BUTTONS_HEIGHT,
        hwnd, NULL, GetModuleHandle(NULL), NULL);
    x_pos += SPEED_BUTTONS_WIDTH + SPEED_BUTTONS_PADDING;

    buttons[SPEED_HIGH_BUTTON] = CreateWindow("BUTTON", "3",WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x_pos, y_pos, SPEED_BUTTONS_WIDTH, SPEED_BUTTONS_HEIGHT,
        hwnd, NULL, GetModuleHandle(NULL), NULL);



};

/*int verificar_botao_pressionado(HWND hwnd, HWND pressed, labirinto *lab, character *rato, HWND *buttons){

    if(pressed == buttons[PAUSE_BUTTON]){
        rato->active = FALSE;
    }
    if(pressed == buttons[SPEED_LOW_BUTTON]){
        rato->active = TRUE;
        rato->speed = SPEED_LOW;
    }
    if(pressed == buttons[SPEED_MID_BUTTON]){
        rato->active = TRUE;
        rato->speed = SPEED_MID;
    }
    if(pressed == buttons[SPEED_HIGH_BUTTON]){
        rato->active = TRUE;
        rato->speed = SPEED_HIGH;
    }
    if(pressed == buttons[RESET_BUTTON]){
        inicializar_labirinto(lab,rato,FALSE);
    }



}*/


