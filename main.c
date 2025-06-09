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
    default:
        movW = movH = 1;
        break;
    }

    // Allocate with maximum potential tiles (e.g., T or L may have holes)
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

            if (!skip)
            {
                mov[movLen++] = (baseY + y) * width + (randX + x);
            }
        }
    }
}

void saveMov()
{
    for (int i = 0; i < movLen; i++)
    {
        board[mov[i]].c = fg;
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
    // Check if movement is possible
    for (int i = 0; i < movLen; i++)
    {
        int x = mov[i] % width;
        int y = mov[i] / width;
        int newX = x + dx;
        int newY = y + dy;

        // Out of bounds?
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
        // Collision with static block?
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

    // Move piece
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

    // Get top-left anchor (min x and y)
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

    // Convert to local grid coordinates
    int localX[movLen], localY[movLen];
    for (int i = 0; i < movLen; i++)
    {
        int x = mov[i] % width;
        int y = mov[i] / width;
        localX[i] = x - maxX;
        localY[i] = y - maxY;
    }

    // Determine piece bounding box
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

    // Apply rotation
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

        // Move down every second
        if (now - lastTick >= 1000)
        {
            calc(0, 1);
            lastTick = now;
        }

        // Handle input
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
