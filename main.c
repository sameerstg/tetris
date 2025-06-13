#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#define width 14
#define height 20

char fg = '#';
char bg = '*';

typedef struct
{
    char c;
} tile;

char tileTypes[] = {'T', 'S', 'L', 'I', 'Z'};

char getRandTileType()
{
    return tileTypes[rand() % (sizeof(tileTypes) / sizeof(tileTypes[0]))];
}

tile board[width * height];

int *mov = NULL;
int movW = 0, movH = 0, movLen = 0;

int getRandomWidth(int sub)
{
    return rand() % (width - sub);
}

void render()
{
    system("cls");
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int pos = y * width + x;
            int isMov = 0;
            for (int i = 0; i < movLen; i++)
            {
                if (mov[i] == pos)
                {
                    isMov = 1;
                    break;
                }
            }
            printf("%c", isMov ? fg : board[pos].c);
        }
        printf("\n");
    }
    fflush(stdout);
}

void checkAndClearLines()
{
    for (int y = height - 1; y >= 0; y--)
    {
        int full = 1;
        for (int x = 0; x < width; x++)
        {
            if (board[y * width + x].c != fg)
            {
                full = 0;
                break;
            }
        }

        if (full)
        {
            for (int x = 0; x < width; x++)
            {
                board[y * width + x].c = bg;
            }

            for (int ty = y - 1; ty >= 0; ty--)
            {
                for (int x = 0; x < width; x++)
                {
                    board[(ty + 1) * width + x].c = board[ty * width + x].c;
                }
            }

            for (int x = 0; x < width; x++)
            {
                board[x].c = bg;
            }

            y++;
        }
    }
}

void saveMov()
{
    for (int i = 0; i < movLen; i++)
    {
        board[mov[i]].c = fg;
    }

    checkAndClearLines();
}

void createNewMov()
{
    if (mov)
    {
        free(mov);
        mov = NULL;
    }

    char newType = getRandTileType();

    switch (newType)
    {
    case 'S':
        movW = 2;
        movH = 2;
        break;
    case 'I':
        movW = 1;
        movH = 4;
        break;
    case 'T':
        movW = 3;
        movH = 2;
        break;
    case 'L':
        movW = 2;
        movH = 3;
        break;
    case 'Z':
        movW = 3;
        movH = 2;
        break;
    default:
        movW = movH = 1;
        break;
    }

    mov = malloc(sizeof(int) * movW * movH);
    movLen = 0;

    int randX = getRandomWidth(movW);
    int baseY = 0;

    for (int y = 0; y < movH; y++)
    {
        for (int x = 0; x < movW; x++)
        {
            int skip = 0;

            if (newType == 'T' && y == 0 && (x == 0 || x == 2))
                skip = 1;
            if (newType == 'L' && x == 1 && y != 2)
                skip = 1;
            if (newType == 'Z' && !(
                                      (x == 0 && y == 0) ||
                                      (x == 1 && y == 0) ||
                                      (x == 1 && y == 1) ||
                                      (x == 2 && y == 1)))
                skip = 1;

            if (!skip)
            {
                mov[movLen++] = (baseY + y) * width + (randX + x);
            }
        }
    }
}

void init()
{
    for (int i = 0; i < width * height; i++)
    {
        board[i].c = bg;
    }
    createNewMov();
}

void calc(int dx, int dy)
{
    for (int i = 0; i < movLen; i++)
    {
        int x = mov[i] % width;
        int y = mov[i] / width;
        int newX = x + dx;
        int newY = y + dy;

        if (newX < 0 || newX >= width || newY >= height)
        {
            if (dy > 0)
            {
                saveMov();
                createNewMov();
            }

            return;
        }

        int newPos = newY * width + newX;

        int isOverlapping = 0;
        for (int j = 0; j < movLen; j++)
        {
            if (newPos == mov[j])
            {
                isOverlapping = 1;
                break;
            }
        }
        if (!isOverlapping && board[newPos].c == fg)
        {
            if (dy > 0)
            {
                saveMov();
                createNewMov();
            }
            return;
        }
    }

    for (int i = 0; i < movLen; i++)
    {
        int x = mov[i] % width;
        int y = mov[i] / width;
        mov[i] = (y + dy) * width + (x + dx);
    }

    render();
}

void rotate()
{
    if (mov == NULL)
        return;

    int maxX = width, maxY = height;
    for (int i = 0; i < movLen; i++)
    {
        int x = mov[i] % width;
        int y = mov[i] / width;
        if (x < maxX)
            maxX = x;
        if (y < maxY)
            maxY = y;
    }

    int localX[movLen], localY[movLen];
    for (int i = 0; i < movLen; i++)
    {
        int x = mov[i] % width;
        int y = mov[i] / width;
        localX[i] = x - maxX;
        localY[i] = y - maxY;
    }

    int boxW = 0, boxH = 0;
    for (int i = 0; i < movLen; i++)
    {
        if (localX[i] + 1 > boxW)
            boxW = localX[i] + 1;
        if (localY[i] + 1 > boxH)
            boxH = localY[i] + 1;
    }

    int newPos[movLen];
    for (int i = 0; i < movLen; i++)
    {
        int newX = boxH - 1 - localY[i];
        int newY = localX[i];
        int boardX = maxX + newX;
        int boardY = maxY + newY;

        if (boardX < 0 || boardX >= width || boardY < 0 || boardY >= height)
            return;

        int pos = boardY * width + boardX;

        int isOverlapping = 0;
        for (int j = 0; j < movLen; j++)
        {
            if (pos == mov[j])
            {
                isOverlapping = 1;
                break;
            }
        }
        if (!isOverlapping && board[pos].c == fg)
            return;

        newPos[i] = pos;
    }

    for (int i = 0; i < movLen; i++)
        mov[i] = newPos[i];

    render();
}

void movePiece(int key)
{
    if (key == 'a')
        calc(-1, 0);
    else if (key == 'd')
        calc(1, 0);
    else if (key == 's')
        calc(0, 1);
    else if (key == 'w')
        rotate();
    else if (key == ' ')
    {
        while (1)
        {
            int canMove = 1;
            for (int i = 0; i < movLen; i++)
            {
                int x = mov[i] % width;
                int y = mov[i] / width;
                int newY = y + 1;

                if (newY >= height)
                {
                    canMove = 0;
                    break;
                }

                int newPos = newY * width + x;

                int isOverlapping = 0;
                for (int j = 0; j < movLen; j++)
                {
                    if (newPos == mov[j])
                    {
                        isOverlapping = 1;
                        break;
                    }
                }

                if (!isOverlapping && board[newPos].c == fg)
                {
                    canMove = 0;
                    break;
                }
            }

            if (!canMove)
            {
                saveMov();
                createNewMov();
                break;
            }

            for (int i = 0; i < movLen; i++)
            {
                int x = mov[i] % width;
                int y = mov[i] / width;
                mov[i] = (y + 1) * width + x;
            }
        }

        render();
    }
}

int main()
{
    srand(time(NULL));
    init();
    render();

    DWORD lastTick = GetTickCount();
    while (1)
    {
        DWORD now = GetTickCount();

        if (now - lastTick >= 1000)
        {
            calc(0, 1);
            lastTick = now;
        }

        if (_kbhit())
        {
            int key = getch();
            if (key == 'q')
                break;
            movePiece(key);
        }

        Sleep(10);
    }

    if (mov)
        free(mov);

    return 0;
}
