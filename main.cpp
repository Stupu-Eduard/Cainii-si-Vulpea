#include <iostream>
#include <winbgim.h>
#include <stdlib.h>
#include<graphics.h>
#include<conio.h>
#include<math.h>

using namespace std;

#define MAX 20
#define FUNDAL CYAN

int stanga,sus,width,height,latura, numar, ok=1;
int vulpei=1,vulpej=5;
int bestcoloana=2,bestcaine=4,caine[5][2],ordine=1;
char sens[5]={'s','s','s','s','s'}; //s sau d (stanga sau dreapta) pt caine

bool gata;
bool initPC=true;

int culoare[3]= {FUNDAL,LIGHTRED,YELLOW};

int TablaDeJoc[MAX][MAX];


void mutarePiesa(int jucator, int& mutarea);
void drawButton(int x, int y, int latime, int inalt, const char* text, int culoare, int dimtext);
void deseneazaPiesa(int linia, int coloana, int culoare);
void mutarePiesa(int jucator);
int castigat(int jucator); //, int mutarea);
void Meniu();
void tabla();
void mutare();
void desen();
void PvP_window();
void PvC_window();
void bordare();
bool vulpeincoltita(int linia2, int coloana2);

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");
    Meniu();

    return 0;
}

void drawButton(int x, int y, int latime, int inalt, const char* text, int culoare, int dimtext)
{
    int lat_text,inalt_text,textx,texty;

    setfillstyle(SOLID_FILL,culoare);
    rectangle(x, y, x+latime, y+inalt);
    floodfill(x+1, y+1, WHITE);
    settextstyle(SIMPLEX_FONT, HORIZ_DIR, dimtext);

    lat_text=textwidth(const_cast<char*>(text));
    inalt_text=textheight(const_cast<char*>(text));

    textx=x+(latime-lat_text)/2;
    texty=y+(inalt-inalt_text)/2;
    outtextxy(textx, texty, const_cast<char*>(text));
}


void Meniu()
{
    int xbs,ybp,ybc,latbut,inaltbut,ybe;

    int screenx=850;
    int screeny=850;

    initwindow(screenx,screeny);

    xbs=screenx/3;
    latbut=200;
    inaltbut=100;
    ybp=screeny/5;
    ybc=screeny/4+inaltbut;
    ybe=screeny/4+3*inaltbut-inaltbut/2;

    drawButton(xbs, ybp, latbut, inaltbut, "PvP", 9, 4);
    drawButton(xbs, ybc, latbut, inaltbut, "PvC", 5, 4);
    drawButton(xbs, ybe, latbut, inaltbut, "Exit", 12, 4);

    while (1)
    {
        int x=mousex();
        int y=mousey();
        if (ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);

               if (x>=xbs && x<=xbs+latbut && y>=ybp && y<=ybp+inaltbut)
                {
                    cleardevice();
                    delay(0);
                    PvP_window();
                    break;
                }

              if (x>=xbs && x<=xbs+latbut && y>=ybc && y<=ybc+inaltbut)
                {
                    cleardevice();
                    delay(0);
                    PvC_window();
                    break;
                }
              if (x>=xbs && x<=xbs+latbut && y>=ybe && y<=ybe+inaltbut)
                {
                  closegraph();
                  exit(0);
                }
            }
    }
}


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
    {
        if(ismouseclick(WM_LBUTTONDOWN) && !click)
        {
            click=true;
            clearmouseclick(WM_LBUTTONDOWN);
            x=mousex();
            y=mousey();
            linia1=(y-sus)/latura+1;
            coloana1=(x-stanga)/latura+1;
        }

        if(TablaDeJoc[linia1][coloana1]!=jucator)
            click=false;
    }
    while (!click);

    click=false;

    do
    {
        if(ismouseclick(WM_LBUTTONDOWN) && !click)
        {
            click=true;
            clearmouseclick(WM_LBUTTONDOWN);
            x=mousex();
            y=mousey();
            linia2=(y-sus)/latura+1;
            coloana2=(x-stanga)/latura+1;
        }
        if(TablaDeJoc[linia2][coloana2]!=0)
            click=false;
        else if(jucator==1 && (linia2!=linia1-1 || (coloana2!=coloana1+1 && coloana2!=coloana1-1)) )
            click=false;
        else if(jucator==2 && ((linia2!=linia1+1 && linia2!=linia1-1) || (coloana2!=coloana1+1 && coloana2!=coloana1-1)) )
            click=false;
    }
    while (!click);



        TablaDeJoc[linia1][coloana1]=0;
        TablaDeJoc[linia2][coloana2]=jucator;
        deseneazaPiesa(linia1,coloana1,FUNDAL);
        deseneazaPiesa(linia2,coloana2,culoare[jucator]);

        if(jucator==2){
            vulpei=linia2;
            vulpej=coloana2;
        }
        if (jucator == 1 && vulpeincoltita(vulpei, vulpej)) {
            castigat(1); // Dogs win
        }
        else if (jucator == 2 && TablaDeJoc[linia2 + 1][coloana2 + 1] == -1 && TablaDeJoc[linia2 + 1][coloana2 - 1] == -1) {
            castigat(2); // Fox wins
        }
}



int castigat(int jucator)
{
    for(int i=1;i<=8;i++)
            for(int j=1;j<=8;j++)
                TablaDeJoc[i][j]=0;
    if(jucator==2)
    {
        settextstyle(SIMPLEX_FONT,HORIZ_DIR,4);
        outtextxy(1000,400,"VULPEA A CASTIGAT!");
    }
    else if(jucator==1)
    {
        settextstyle(SIMPLEX_FONT,HORIZ_DIR,4);
        outtextxy(1000,400,"CAINII AU CASTIGAT!");
    }

    delay(300);
    closegraph();
    Meniu();
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
    for(int i=0;i<=9;i++){
            TablaDeJoc[0][i]=-1;
            TablaDeJoc[i][0]=-1;
            TablaDeJoc[9][i]=-1;
            TablaDeJoc[i][9]=-1;
    }
    int screenx=GetSystemMetrics(SM_CXSCREEN);
    int screeny=GetSystemMetrics(SM_CYSCREEN);
    initwindow(screenx,screeny,"Cainii si vulpea");
    desen();
    bordare();

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


void mutarePvP()
{
    do
    {
        mutarePiesa(1);
        mutarePiesa(2);
    }
    while (1);
}

bool strategyStart()
{
//check if the fox is next to a dog
return ( (vulpei==caine[1][0]-1 && (vulpej==caine[1][1]+1 || vulpej==caine[1][1]-1)) || (vulpei==caine[2][0]-1 && (vulpej==caine[2][1]+1 || vulpej==caine[2][1]-1)) || (vulpei==caine[3][0]-1 && (vulpej==caine[3][1]+1 || vulpej==caine[3][1]-1)) || (vulpei==caine[4][0]-1 && (vulpej==caine[4][1]+1 || vulpej==caine[4][1]-1)));

}

void mutarePC(int jucator)
{
    int coloana, linie,liniemax,mutat;
    bool mutareValida = false;

    if (jucator == 1) // Calculatorul reprezintă câinii
    {
        if(initPC)
        {
            int col=2;
            for(int i=1;i<=4;i++)
            {
                caine[i][0]=8;
                caine[i][1]=col;
                col+=2;
            }
            initPC=false;
        }

        if(strategyStart())
        {
            for(int i=1;i<=4;i++)
            {
                coloana=caine[i][1];
                if(abs(vulpej-bestcoloana)<=abs(vulpej-coloana) && caine[i][0]>1)
                {
                    bestcoloana=coloana;
                    bestcaine=i;
                }
            }

            coloana=caine[bestcaine][1];
            linie=caine[bestcaine][0];

            TablaDeJoc[linie][coloana]=0;
            if(sens[bestcaine]=='s')
            {
                TablaDeJoc[linie-1][coloana-1]=jucator;
                deseneazaPiesa(linie,coloana,FUNDAL);
                deseneazaPiesa(linie-1,coloana-1,culoare[jucator]);
                sens[bestcaine]='d';
                caine[bestcaine][0]=linie-1; caine[bestcaine][1]=coloana-1;
            }
            else if(sens[bestcaine]=='d')
            {
                TablaDeJoc[linie-1][coloana+1]=jucator;
                deseneazaPiesa(linie,coloana,FUNDAL);
                deseneazaPiesa(linie-1,coloana+1,culoare[jucator]);
                sens[bestcaine]='s';
                caine[bestcaine][0]=linie-1; caine[bestcaine][1]=coloana+1;
            }
        }
        else
        {
            liniemax=0;mutat=1;
            for(int i=1;i<=4;i++)
            {
                if(caine[i][0]>liniemax && caine[i][0]>1)
                {
                    liniemax=caine[i][0];
                    mutat=i;
                }
            }
            coloana=caine[mutat][1];
            linie=caine[mutat][0];

            if(sens[mutat]=='s')
            {
                TablaDeJoc[linie-1][coloana-1]=jucator;
                deseneazaPiesa(linie,coloana,FUNDAL);
                deseneazaPiesa(linie-1,coloana-1,culoare[jucator]);
                sens[mutat]='d';
                caine[mutat][0]=linie-1; caine[mutat][1]=coloana-1;
            }
            else if(sens[mutat]=='d')
            {
                TablaDeJoc[linie-1][coloana+1]=jucator;
                deseneazaPiesa(linie,coloana,FUNDAL);
                deseneazaPiesa(linie-1,coloana+1,culoare[jucator]);
                sens[mutat]='s';
                caine[mutat][0]=linie-1; caine[mutat][1]=coloana+1;
            }
        }
    }
    else if (jucator == 2)
    {
    }
}

void mutarePvC(int jucator){
    do
    {

        if(jucator==1)
        {
            mutarePC(1);
            mutarePiesa(2);
        }
        else if(jucator==2)
        {
            mutarePiesa(1);
            mutarePC(2);
        }
    }
    while (1);
}

void PvP_window()
{
    tabla();
    mutarePvP();

}

void PvC_window()
{
    tabla();
    mutarePvC(1);
}

bool vulpeincoltita(int linia2, int coloana2)
{
    if(TablaDeJoc[linia2+1][coloana2+1]!=0 && TablaDeJoc[linia2+1][coloana2-1]!=0 && TablaDeJoc[linia2-1][coloana2+1]!=0 && TablaDeJoc[linia2-1][coloana2-1]!=0)
        return 1;

    return 0;
}

void bordare()
{
    for(int i=0; i<=numar+1; i++)
        TablaDeJoc[i][0]=TablaDeJoc[i][numar+1]=-1;
    for(int i=0; i<=numar+1; i++)
        TablaDeJoc[0][i]=TablaDeJoc[numar+1][i]=-1;

}
