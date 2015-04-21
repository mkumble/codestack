#include <stdio.h>
#include <iostream.h>
#include<graphics.h>
#include<dos.h>
#include<ctype.h>
#include<conio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>


#define TOPLT 218
#define HORI 196
#define TOPRT 191
#define VERT 179
#define BOTLT 192
#define BOTRT 217


void box(int x1, int y1, int x2, int y2)
{
    int i;


    gotoxy(x1,y1);
    cprintf("%c",TOPLT);
    for(i=x1+1;i<x2;i++)
    cprintf("%c",HORI);

    cprintf("%c",TOPRT);
    for(i=y1+1;i<y2;i++)
    {
	gotoxy(x1,i);	cprintf("%c",VERT);
	gotoxy(x2,i);	cprintf("%c",VERT);
    }
    gotoxy(x1,y2); cprintf("%c",BOTLT);
    for(i=x1+1;i<x2;i++)
    cprintf("%c",HORI);

    cprintf("%c",BOTRT);
    return;
}/* box */


