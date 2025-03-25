#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define LARGURA 40
#define ALTURA 40
#define VELOCIDADE 150
#define TAMANHO_MAXIMO 100

typedef struct
{
    int x[TAMANHO_MAXIMO];
    int y[TAMANHO_MAXIMO];
    int comprimento;
    int direcao_x;
    int direcao_y;
} Cobra;

typedef struct {
    int x;
    int y;
} Comida;

Cobra cobra;
Comida comida;

bool game_over = false;

char tela_buffer[ALTURA][LARGURA];

void gerar_comida() {
    comida.x = rand() % (LARGURA - 2) + 1;
    comida.y = rand() % (ALTURA - 2) + 1;
}

void inicializar()
{
    initscr();
    cbreak();
    nodelay(stdscr, TRUE);
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();

    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);

    cobra.comprimento = 1;
    cobra.x[0] = LARGURA / 2;
    cobra.y[0] = ALTURA / 2;
    cobra.direcao_x = 1;
    cobra.direcao_y = 0;

    gerar_comida();

    for (int i = 0; i < ALTURA; i++)
    {
        for (int j = 0; j < LARGURA; j++)
        {
            if (i == 0 || i == ALTURA - 1 || j == 0 || j == LARGURA - 1)
            {
                tela_buffer[i][j] = '#';
            }
            else
            {
                tela_buffer[i][j] = ' ';
            }
        }
    }
}

void atualizar()
{
    int novo_x = cobra.x[0] + cobra.direcao_x;
    int novo_y = cobra.y[0] + cobra.direcao_y;

    if (novo_x <= 0 || novo_x >= LARGURA - 1 || novo_y <= 0 || novo_y >= ALTURA - 1)
    {
        game_over = true;
        return;
    }

    if (novo_x == comida.x && novo_y == comida.y)
    {
        cobra.comprimento++;
        gerar_comida();
    }
    for (int i = cobra.comprimento - 1; i > 0; i--)
    {
        cobra.x[i] = cobra.x[i - 1];
        cobra.y[i] = cobra.y[i - 1];
    }

    cobra.x[0] = novo_x;
    cobra.y[0] = novo_y;
}

void desenhar()
{
    for (int i = 1; i < ALTURA - 1; i++)
    {
        for (int j = 1; j < LARGURA - 1; j++)
        {
            tela_buffer[i][j] = ' ';
        }
    }

    tela_buffer[comida.y][comida.x] = '@';
    
    for (int i = 0; i < cobra.comprimento; i++)
    {
        int y = cobra.y[i];
        int x = cobra.x[i];
        tela_buffer[y][x] = '*';
    }

    for (int i = 0; i < ALTURA; i++)
    {
        for (int j = 0; j < LARGURA; j++)
        {
            mvaddch(i, j, tela_buffer[i][j]);
        }
    }

    refresh();
}

void processar_entrada()
{
    int tecla = getch();

    switch (tecla)
    {
    case KEY_UP:
        if (cobra.direcao_y == 0)
        {
            cobra.direcao_x = 0;
            cobra.direcao_y = -1;
        }
        break;
    case KEY_DOWN:
        if (cobra.direcao_y == 0)
        {
            cobra.direcao_x = 0;
            cobra.direcao_y = 1;
        }
        break;
    case KEY_LEFT:
        if (cobra.direcao_x == 0)
        {
            cobra.direcao_x = -1;
            cobra.direcao_y = 0;
        }
        break;
    case KEY_RIGHT:
        if (cobra.direcao_x == 0)
        {
            cobra.direcao_x = 1;
            cobra.direcao_y = 0;
        }
        break;
    case 'x':
    case 'q':
        game_over = true;
        break;
    }
}

int main()
{
    inicializar();

    while (!game_over)
    {
        desenhar();
        processar_entrada();
        atualizar();
        napms(VELOCIDADE);
    }

    endwin();

    return 0;
}
