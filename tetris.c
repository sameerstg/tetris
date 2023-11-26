#include <stdio.h>
#include <stdlib.h>
#include<windows.h>
#define width 14
#define height 20

char matrix[height][width] ;

void main()
{

    for(int y = 0;y<height;y++)
    {

        for(int x = 0;x<width;x++)
        {
		matrix[height][width]= '#';
        }
    }
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

