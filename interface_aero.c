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




void desenhar_tela(HWND hwnd, HDC hdc, HBITMAP map_tiles, HBITMAP sprite, HBITMAP sprite_mask, int pos_x, int pos_y){




    HBITMAP hMemBitmap = CreateCompatibleBitmap(hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
    HDC tiles= CreateCompatibleDC(hdc);
    SelectObject(tiles,map_tiles);
    HDC hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, hMemBitmap);


    SelectObject(tiles,sprite);

    SelectObject(tiles,sprite_mask);



    BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcMem, 0, 0, SRCCOPY);



    //SelectObject(hdcMem, hbmOld);

    DeleteObject(hMemBitmap);

    DeleteDC(tiles);
    DeleteDC(hdcMem);




};


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


