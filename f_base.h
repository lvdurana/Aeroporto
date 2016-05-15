#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "resource.h"

//Tamanho de cada segmento do bitmap de caixa(3x3)
#define BOX_WIDTH 16
#define BOX_HEIGHT 16

//Tamanho da janela
#define WINDOW_PADDING_X 7
#define WINDOW_PADDING_Y 28

#define CANVAS_WIDTH 480
#define CANVAS_HEIGHT 360
#define WINDOW_WIDTH (CANVAS_WIDTH + WINDOW_PADDING_X)
#define WINDOW_HEIGHT (CANVAS_HEIGHT + WINDOW_PADDING_Y)


//Bitmaps
#define BMP_FUNDO 0
#define BMP_CAIXA 1
#define BMP_AVIAO 2
#define BMP_AVIAO_MASK 3
#define NUM_BITMAPS 4

//Sprites
#define SPRITE_AV_WIDTH 32
#define SPRITE_AV_HEIGHT 32

//Caixas da tela
#define TEXT_SIZE_A 14
#define TEXT_SIZE_B 12

#define AP_LIST_WIDTH 8
#define AP_LIST_HEIGHT 6

#define AP_LIST_A_X (CANVAS_WIDTH - BOX_WIDTH*AP_LIST_WIDTH)
#define AP_LIST_A_Y 0
#define AP_LIST_B_X 0
#define AP_LIST_B_Y (CANVAS_HEIGHT - BOX_HEIGHT*AP_LIST_HEIGHT)

#define BOX_X_PADDING_1 4
#define BOX_Y_PADDING_1 4
#define BOX_Y_PADDING_2 8

#define TEXT_SIZE_LIMIT_1 (BOX_WIDTH*AP_LIST_WIDTH - BOX_X_PADDING_1*3 - SPRITE_AV_WIDTH)
#define TEXT_SIZE_LIMIT_2 (BOX_WIDTH*AP_LIST_WIDTH - BOX_X_PADDING_1*2)


//Botões

#define BT_PERMITIR_POUSO 0
#define BT_LISTA_POUSOS 1
#define BT_PERMITIR_DECOLAGEM 2
#define BT_LISTA_DECOLAGENS 3
#define NUM_BUTTONS 4


#define SPEED_BUTTONS_WIDTH 48
#define SPEED_BUTTONS_HEIGHT 32
#define SPEED_BUTTONS_X WINDOW_PADDING_X
#define SPEED_BUTTONS_Y (LAB_HEIGHT + 4)
#define SPEED_BUTTONS_PADDING 10

#define CONTROL_BUTTONS_WIDTH 64
#define CONTROL_BUTTONS_HEIGHT 32
#define CONTROL_BUTTONS_Y (LAB_HEIGHT + 4)
#define CONTROL_BUTTONS_PADDING 10

#define RESET_BUTTON_X (LAB_WIDTH - CONTROL_BUTTONS_WIDTH)
#define OPTIONS_BUTTON_X (RESET_BUTTON_X - CONTROL_BUTTONS_PADDING - CONTROL_BUTTONS_WIDTH)


#define MAX_CHARS_NOME 50
#define MAX_CHARS_COD 5
#define MAX_CHARS_LOCAL 20



//Especificação das direções dos sprites
#define DIRECTION_UP 0
#define DIRECTION_RIGHT 1
#define DIRECTION_LEFT 2
#define DIRECTION_DOWN 3
#define REVERSE_DIRECTION(x) ((~x) & 3)

#define FRAME_DURATION(x) (8/x)

#define MAX_FPS 15
#define FPS_TIMER 0x0001






typedef struct frame{
    int counter;
    clock_t last_frame;
    clock_t clocks_per_frame;

} frame_count;

typedef struct cha{
    POINT pos; //em pixel
    POINT pos_map; //em coordenadas do mapa
    int movement; //pizels de distância do destino;
    int direction; //direção a qual o sprite está virado
    int speed; //pixels percorridos por frame;
    int frame; // frame da animação do sprite
    int frame_duration; //numero de frames restantes até a alteração do frame
    BOOL active;

} character;

typedef struct avi{
    char nome[MAX_CHARS_NOME];
    char codigo[MAX_CHARS_COD];
    char origem[MAX_CHARS_LOCAL];
    char destino[MAX_CHARS_LOCAL];
    int num_passageiros;
    int hora;

}aviao;

typedef struct FILA
{
    aviao *dado;
    struct FILA *prox;
} fila;

