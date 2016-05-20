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

#define WINDOW_LIST_WIDTH 384
#define WINDOW_LIST_HEIGHT 320


//Bitmaps
#define BMP_WINDOW 0 //Bitmap utilizado para o processamento dos desenhos
#define BMP_FUNDO 1
#define BMP_CAIXA 2
#define BMP_AVIAO 3
#define BMP_AVIAO_MASK 4
#define NUM_BITMAPS 5

//Sprites
#define SPRITE_AV_FRONT_X 128
#define SPRITE_AV_LEFT_X 0
#define SPRITE_AV_RIGHT_X 64
#define SPRITE_AV_WIDTH 64
#define SPRITE_AV_HEIGHT 32
#define SPRITE_AV_FRONT_WIDTH 32

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
#define BOX_Y_PADDING_2 4

#define TEXT_SIZE_LIMIT_1 (BOX_WIDTH*AP_LIST_WIDTH - BOX_X_PADDING_1*3 - SPRITE_AV_WIDTH)
#define TEXT_SIZE_LIMIT_2 (BOX_WIDTH*AP_LIST_WIDTH - BOX_X_PADDING_1*2)

//Botões
#define BT_PERMITIR_POUSO 0
#define BT_LISTA_POUSOS 1
#define BT_PERMITIR_DECOLAGEM 2
#define BT_LISTA_DECOLAGENS 3
#define NUM_BUTTONS 4

#define BUTTONS_WIDTH 140
#define BUTTONS_HEIGHT 32
#define BUTTONS_X WINDOW_PADDING_X
#define BUTTONS_Y (LAB_HEIGHT + 4)
#define BUTTONS_PADDING 10

#define MAX_CHARS_NOME 50
#define MAX_CHARS_COD 5
#define MAX_CHARS_LOCAL 20

//Especificação das direções dos sprites
#define DIRECTION_RIGHT 1
#define DIRECTION_LEFT 0

#define LIST_MAX_SIZE 12
#define LIST_MAX_COLLUMNS 3


#define FRAME_DURATION(x) (8/x)

#define MAX_FPS 30
#define FPS_TIMER 0x0001

//Constantes do aeroporto
#define FILA_DECOLAGEM 0
#define FILA_POUSO 1

#define LOCAL_AEROPORTO "Curitiba-BR"




typedef struct frame{
    int counter;
    clock_t last_frame;
    clock_t clocks_per_frame;

} frame_count;

typedef struct cha{
    POINT pos; //em pixel
    POINT speed; //pixels percorridos por frame;
    int direction; //direção à qual o sprite está virado
    int animation_frame; //estado da animação
    BOOL active;
    BOOL visible;

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


void inicia_fila(fila **pista);
fila *cria_nodo();
void push(fila **pista, aviao *dado);
aviao *pop(fila **pista);
aviao *cria_nodo_aviao();
int contar_elem_fila(fila *fil);

int inicializar_contador_de_frames(HWND hwnd, frame_count *frame);
int atualizar_frame(HWND hwnd, frame_count *frame);
void desenhar_tela(HDC hdc, HBITMAP *bitmaps, fila *fila_decolagem, fila *fila_pouso, character *aviao);
void desenhar_lista(HWND hwnd, HDC hdc, HBITMAP *bitmaps, fila *fila_desenhar);
void escrever_info_aviao(HDC hdc, int ox, int oy, aviao *av, char *title);
void desenhar_caixa(HDC hdc, HBITMAP caixa, int ox, int oy, int width, int height);
void desenhar_caixa_info(HDC hdc, HBITMAP *bitmaps, int ox, int oy, int width, int height, aviao *av, char *text);
void desenhar_texto(HDC hdc, char *text, int size, int pos_x, int pos_y, int limit_x, int modifier);
HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent);
void criar_botoes(HWND hwnd, HWND *buttons);

BOOL CALLBACK window_edit_proc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK window_list_proc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);



