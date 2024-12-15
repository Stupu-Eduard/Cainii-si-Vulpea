#include <iostream>
#include <winbgim.h>
#include <stdlib.h>

using namespace std;

#define MAX 20
#define FUNDAL CYAN

int stanga,sus,width,height,latura, numar;
bool gata;

int culoare[3]= {FUNDAL,LIGHTRED,YELLOW};

int TablaDeJoc[MAX][MAX];

void deseneazaPiesa(int linia, int coloana, int culoare)
{
    int x1, y1, x2, y2;
    int xmijloc, ymijloc;
    // stergere
    setcolor(FUNDAL);
    setfillstyle(SOLID_FILL,FUNDAL);
    x1=stanga+latura*(coloana-1);
    y1=sus+latura*(linia-1);
    x2=x1+latura;
    y2=y1+latura;
    xmijloc=(x1+x2)/2;
    ymijloc=(y1+y2)/2;
    bar(xmijloc-20,ymijloc-20,xmijloc+20,ymijloc+20);
    // afisez cerc rosu aprins
    setcolor(culoare);
    setfillstyle(SOLID_FILL,culoare);
    fillellipse(xmijloc,ymijloc,18,18);
}

void mutarePiesa(int jucator)
{
    int linia1,coloana1,linia2,coloana2,x,y;
    int click=false;
    do
        if(ismouseclick(WM_LBUTTONDOWN) && !click)
        {
            click=true;
            clearmouseclick(WM_LBUTTONDOWN);
            x=mousex();
            y=mousey();
            linia1=(y-sus)/latura+1;
            coloana1=(x-stanga)/latura+1;
        }
while (!click);
    click=false;
    do
        if(ismouseclick(WM_LBUTTONDOWN) && !click)
        {
            click=true;
            clearmouseclick(WM_LBUTTONDOWN);
            x=mousex();
            y=mousey();
            linia2=(y-sus)/latura+1;
            coloana2=(x-stanga)/latura+1;
        }
    while (!click);

    if(TablaDeJoc[linia1][coloana1]==jucator && TablaDeJoc[linia2][coloana2]==0)
    {
        if(TablaDeJoc[linia1][coloana1]==2 && linia2==linia1+1 && (coloana2==coloana1+1 || coloana2==coloana1-1) )
        {
            TablaDeJoc[linia1][coloana1]=0;
            TablaDeJoc[linia2][coloana2]=jucator;
            deseneazaPiesa(linia1,coloana1,FUNDAL);
            deseneazaPiesa(linia2,coloana2,culoare[jucator]);
        }
        else if(TablaDeJoc[linia1][coloana1]==1 && (linia2==linia1+1 || linia2==linia1-1) && (coloana2==coloana1+1 || coloana2==coloana1-1))
        {
            TablaDeJoc[linia1][coloana1]=0;
            TablaDeJoc[linia2][coloana2]=jucator;
            deseneazaPiesa(linia1,coloana1,FUNDAL);
            deseneazaPiesa(linia2,coloana2,culoare[jucator]);
        }
    }
}

bool castigat(int jucator)
{
    return false;
}

void desen()
{
    int i,j;
    numar=8;
    width=400;
    height=400;
    latura=width/numar;
    sus=(getmaxy()-width)/2;
    stanga=(getmaxx()-height)/2;
    setbkcolor(FUNDAL);
    clearviewport();
    setcolor(BLUE);
    for (i=1; i<=numar; i++)
        for (j=1; j<=numar; j++)
            rectangle(stanga+latura*(i-1),sus+latura*(j-1),stanga+latura*i,sus+latura*j);
}

void tabla()
{
    int screenx=GetSystemMetrics(SM_CXSCREEN);
    int screeny=GetSystemMetrics(SM_CYSCREEN);
    initwindow(screenx,screeny,"Cainii si vulpea");
    desen();

    gata=false;
    for (int i=1; i<=numar; i++)
        if (i % 2 == 0)
        {
            TablaDeJoc[numar][i]=1;
            deseneazaPiesa(numar,i,culoare[1]);
        }
    TablaDeJoc[1][numar/2+1]=2;
    deseneazaPiesa(1,numar/2+1,culoare[2]);
}

void mutare()
{
    do
    {
        mutarePiesa(1);
        mutarePiesa(2);
    }
    while (!castigat(1) && !castigat(2));

    getch();
    closegraph();
}

int main()
{
    tabla();
    mutare();

    return 0;
}
