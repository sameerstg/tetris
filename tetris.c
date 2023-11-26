#include <stdio.h>
#include <stdlib.h>
#include<windows.h>
#define width 14
#define height 20
/* char makeNewPiece(){ */

/*     /1* char newPiece[3][3] = {{'#'},{'#'},{'#'}},{{'#'},{'#'},{'#'}},{{'#'},{'#'},{'#'}}; *1/ */
/*     /1* return newPiece; *1/ */
/* } */
/* struct piece{ */
/*     int xPos,yPos; */
/*     int xSize,ySize; */
/* } */

char matrix[height][width] = {


    {'#','g','g','g','g','g','g','g','g','g','g','g','g','#'},
    {'#','g','g','g','g','g','g','g','g','g','g','g','g','#'},
    {'#','g','g','g','g','g','g','g','g','g','g','g','g','#'},
    {'#','g','g','g','g','g','g','g','g','g','g','g','g','#'},
    {'#','g','g','g','g','g','g','g','g','g','g','g','g','#'},
    {'#','g','g','g','g','g','g','g','g','g','g','g','g','#'},
    {'#','g','g','g','g','g','g','g','g','g','g','g','g','#'},
    {'#','g','g','g','g','g','g','g','g','g','g','g','g','#'},
    {'#','g','g','g','g','g','g','g','g','g','g','g','g','#'},
    {'#','g','g','g','g','g','g','g','g','g','g','g','g','#'},
    {'#','g','g','g','g','g','g','g','g','g','g','g','g','#'},
    {'#','g','g','g','g','g','g','g','g','g','g','g','g','#'},
    {'#','g','g','g','g','g','g','g','g','g','g','g','g','#'},
    {'#','g','g','g','g','g','g','g','g','g','g','g','g','#'},
    {'#','g','g','g','g','g','g','g','g','g','g','g','g','#'},
    {'#','g','g','g','g','g','g','g','g','g','g','g','g','#'},
    {'#','g','g','g','g','g','g','g','g','g','g','g','g','#'},
    {'#','g','g','g','g','g','g','g','g','g','g','g','g','#'},
    {'#','g','g','g','g','g','g','g','g','g','g','g','g','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
};

void main()
{

    /* piece newPiece; */
    /* newPiece.xpos=4; */
    /* newPiece.yPos = height-1; */

    /* newPiece.width  = newPiece.height = 1; */
    for(int y = 0;y<height;y++)
    {

        for(int x = 0;x<width;x++)
        {
            printf("%s",&matrix[x][y]);
        }
        printf(" \n");
    }
    /* while(1){ */
    /*     Sleep(1000); */
    /*     system("cls"); */

    /* } */

}

