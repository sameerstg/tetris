#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#define width 14
#define height 20

char fg = '1';
char bg = '0';

typedef struct
{
    char c;
} tile;

char tileTypes[4] = {'T', 'S', 'L', 'I'};

char getRandTileType()
{
    return tileTypes[rand() % (sizeof(tileTypes) / sizeof(tileTypes[0]))];
}

tile board[width * height];

int *mov = NULL; // Moving piece indices on board
int movW = 0;    // Moving piece width
int movH = 0;    // Moving piece height

int getRandomWidth(int sub)
{
    return rand() % (width - sub);
}

void render()
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int pos = y * width + x;
            int isMov = 0;
            for (int i = 0; i < movW * movH; i++)
            {
                if (mov[i] == pos)
                {
                    isMov = 1;
                    break;
                }
            }
            if (isMov)
            {
                printf("%c", fg);
            }
            else
            {
                printf("%c", board[pos].c);
            }
        }
        printf("\n");
    }
}

void createNewMov()
{
    if (mov != NULL)
    {
        free(mov);
        mov = NULL;
        movW = 0;
        movH = 0;
    }

    char newType = getRandTileType();
    newType = 'T';
    if (newType == 'S')
    {
        movW = 2;
        movH = 2;
    }
    else if (newType == 'I')
    {
        movW = 1;
        movH = 3;
    }
    else if (newType == 'T')
    {
        movW = 3;
        movH = 2;
    }
    else if (newType == 'L')
    {
        movW = 2;
        movH = 3;
    }
    else
    {
        movW = 1;
        movH = 1;
    }

    mov = malloc(sizeof(int) * (movW * movH + (newType == 'T' || newType == 'L' ? 2 : 0)));
    int randX = getRandomWidth(movW - 1);
    int i = 0;
    for (int y = 0; y < movH; y++)
    {
        for (int x = 0; x < movW; x++)
        {
            if (newType == 'L' && x == 1 && y != 2)
                continue;
            if (newType == 'T' && y == 0 && (x == 0 || x == 2))
                continue;
            mov[i++] = randX + x + (y * width);
        }
    }
}

// Fix: save moving piece to board permanently
void saveMov()
{
    for (int i = 0; i < movW * movH; i++)
    {
        board[mov[i]].c = fg;
    }
}

void init()
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            board[y * width + x].c = bg;
        }
    }
    createNewMov();
}

// dx = change in x (horizontal), dy = change in y (vertical)
void calc(int dx, int dy)
{
    for (int i = 0; i < sizeof(mov) / sizeof(mov[0]); i++)
    {
        int x = mov[i] % width;
        int y = mov[i] / width;

        int newX = x + dx;
        int newY = y + dy;

        // Check boundaries
        printf("%d \n", (sizeof(mov) / sizeof(mov[0])));
        if (newX < 0 || newX >= width || newY >= height)
        {
            if (dy != 0)
            {
                saveMov();
                createNewMov();
            }
            return;
        }

        // int newPos = newY * width + newX;

        // // Check collision with placed blocks
        // if (board[newPos].c == fg)
        // {
        //     if (dy != 0)
        //     {
        //         printf("2");
        //         saveMov();
        //         createNewMov();
        //     }
        //     return;
        // }
    }

    // Move piece if no collision
    for (int i = 0; i < movW * movH; i++)
    {
        int x = mov[i] % width;
        int y = mov[i] / width;
        mov[i] = (y + dy) * width + (x + dx);
    }

    system("cls");
    render();
}

void move(int key)
{
    if (key == 'a')
    { // Move left
        calc(-1, 0);
    }
    else if (key == 'd')
    { // Move right
        calc(1, 0);
    }
    else if (key == 's')
    { // Move down faster
        calc(0, 1);
    }
}

int main()
{
    system("cls");

    srand(time(NULL));
    init();
    render();

    DWORD lastTick = GetTickCount();

    while (1)
    {
        DWORD now = GetTickCount();

        // Move piece down every 1 second
        if (now - lastTick >= 1000)
        {
            calc(0, 1);
            lastTick = now;
        }

        // Check keyboard input (non-blocking)
        if (_kbhit())
        {
            int key = getch();
            if (key == 'q')
            {
                printf("Quitting\n");
                break;
            }
            move(key);
        }

        Sleep(10); // reduce CPU usage
    }

    if (mov)
        free(mov);

    return 0;
}
