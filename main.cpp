#include <iostream>
#include <fstream>
#include <winbgim.h>
#include <stdlib.h>
#include<graphics.h>
#include<conio.h>
#include<windows.h>
#include<mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include<math.h>
#include<cstdlib>
#include<time.h>

using namespace std;

#define MAX 20
#define FUNDAL GREEN

ofstream fout("output.out");

int stanga,sus,width,height,latura, numar, ok=1, castig, mode, cnt;
int foxX=1,foxY=5;
int vulpei=1,vulpej=5;
int bestcoloana=2,bestcaine=4,caine[5][2],ordine=1;

char sens[5]={'s','s','s','s','s'}; //s sau d (stanga sau dreapta) pt caine

bool gata;
bool initPC=true;
bool isPlaying=false;

int culoare[3]= {FUNDAL,LIGHTRED,YELLOW};

int TablaDeJoc[MAX][MAX];


//void mutarePiesa(int jucator, int& mutarea);
void drawButton(int x, int y, int latime, int inalt, const char* text, int culoare, int dimtext);
void deseneazaPiesa(int linia, int coloana, int culoare);
void mutarePiesa(int jucator, int mode);
int castigat(int jucator); //, int mutarea);
void Meniu();
void tabla(int mode);
void mutare();
void desen();
void PvP_window();
void PvC_window();
void bordare();
bool vulpeincoltita(int linia2, int coloana2);
void roundedRectangle(int x, int y, int latime, int inaltime, int raza, int culcontur, int culumplere);
bool miscarevalida(int x, int y);
void moveFox(int& foxX, int& foxY);
void PvC_dificulty_window(int jucator);
void mutarePvC(int jucator, int dificulty);
void mutarePC(int jucator, int dificulty);
void moveDog_rand();
void moveFox_rand(int& foxX, int& foxY);
int cainii_incoltesc(int x, int y);

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

    //setfillstyle(SOLID_FILL,culoare);
    //setcolor(BLACK);
    roundedRectangle(x, y, latime, inalt, 20, BLACK, culoare);
    //floodfill(x+1, y+1, BLACK);
    setbkcolor(BLACK);
    setcolor(WHITE);
    settextstyle(SIMPLEX_FONT, HORIZ_DIR, dimtext);

    lat_text=textwidth(const_cast<char*>(text));
    inalt_text=textheight(const_cast<char*>(text));

    textx=x+(latime-lat_text)/2;
    texty=y+(inalt-inalt_text)/2;
    outtextxy(textx, texty, const_cast<char*>(text));
}

void roundedRectangle(int x, int y, int latime, int inaltime, int raza, int culcontur, int culumplere)
{
    setcolor(culcontur);
    setfillstyle(SOLID_FILL, culumplere);

    // Deseneaza colturile rotunjite
    fillellipse(x+raza, y+raza, raza, raza);
    fillellipse(x+latime-raza, y+raza, raza, raza);
    fillellipse(x+raza, y+inaltime-raza, raza, raza);
    fillellipse(x+latime-raza, y+inaltime-raza, raza, raza);

    // Deseneaza dreptunghiurile centrale (fara colturi)
    bar(x+raza, y, x+latime-raza, y+inaltime);
    bar(x, y+raza, x+latime, y+inaltime-raza);

    // Deseneaza conturul dreptunghiului
    arc(x+raza, y+raza, 90, 180, raza);
    arc(x+latime-raza, y+raza, 0, 90, raza);
    arc(x+raza, y+inaltime-raza, 180, 270, raza);
    arc(x+latime-raza, y+inaltime-raza, 270, 360, raza);
    line(x+raza, y, x+latime-raza, y);
    line(x+raza, y+inaltime, x+latime-raza, y+inaltime);
    line(x, y+raza, x, y+inaltime-raza);
    line(x+latime, y+raza, x+latime, y+inaltime-raza);
}


void Meniu()
{
    int xbs,ybp,ybc,latbut,inaltbut,ybe;


    int screenx=GetSystemMetrics(SM_CXSCREEN);
    int screeny=GetSystemMetrics(SM_CYSCREEN);

    initwindow(screenx,screeny);
    setbkcolor(WHITE);
    cleardevice();

    if(castig==0)
    {
        setcolor(RED);
        settextstyle(SIMPLEX_FONT,HORIZ_DIR,6);
        outtextxy(400,60,"CAINII SI VULPEA");
    }

    if(castig==1)
    {
        setcolor(GREEN);
        settextstyle(SIMPLEX_FONT,HORIZ_DIR,6);
        outtextxy(400,60,"CAINII AU CASTIGAT!");
    }

    if(castig==2)
    {
        setcolor(GREEN);
        settextstyle(SIMPLEX_FONT,HORIZ_DIR,6);
        outtextxy(400,60,"VULPEA A CASTIGAT!");
    }

    xbs=screenx/3+25;
    latbut=400;
    inaltbut=150;
    ybp=screeny/4-inaltbut/2+25;
    ybc=screeny/2-inaltbut/2+25;
    ybe=3*screeny/4-inaltbut/2+25;

    readimagefile("Fox_1.jpg",0,screeny-500,360,screeny-140);
    readimagefile("Dog_1.jpg",screenx-405,screeny-440,screenx,screeny-160);

    drawButton(xbs, ybp, latbut, inaltbut, "PvP", 9, 4);
    drawButton(xbs, ybc, latbut, inaltbut, "PvC", 5, 4);
    drawButton(xbs, ybe, latbut, inaltbut, "Exit", 12, 4);
    drawButton(20,20,200,100,"Music",13,2);

    while (1)
    {
        int col=2;
        for(int i=1;i<=4;i++)
        {
            caine[i][0]=8;
            caine[i][1]=col;
            col+=2;
        }
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

              if(x>=20 && x<=220 && y>=20 && y<=120)
              {
                  if (!isPlaying)
                    {
                       PlaySound("music_2.wav", NULL, SND_ASYNC | SND_LOOP);
                       isPlaying = true;
                    }
                  else
                    {
                       PlaySound(NULL, 0, 0); // Oprește melodia
                       isPlaying = false;
                    }
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
    bar(xmijloc-23,ymijloc-23,xmijloc+23,ymijloc+23);
    // afisez cerc rosu aprins
    //setcolor(culoare);
    //setfillstyle(SOLID_FILL,culoare);
    //fillellipse(xmijloc,ymijloc,18,18);
    if(culoare==12)
        readimagefile("dogg_2.jpg",x1+5,y1+5,x1+45,y1+45);
     if(culoare==14)
        readimagefile("foxx_2.jpg",x1+5,y1+5,x1+45,y1+45);
}

void mutarePiesa(int jucator, int mode)
{
    int linia1,coloana1,linia2,coloana2,x,y,dogNumber=0;
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

            if(TablaDeJoc[linia1][coloana1]!=jucator)
                click=false;
        }
    }
    while (!click);
    for(int i=1;i<=4;i++)
    {
        if(caine[i][0]==linia1 && caine[i][1]==coloana1)
        {
            dogNumber=i;
            i=5;
        }
    }
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

        if(jucator==2)
        {
            setfillstyle(SOLID_FILL,FUNDAL);
            bar(300,60,900,100);
            if(mode==1)
            {
              setcolor(YELLOW);
              settextstyle(SIMPLEX_FONT,HORIZ_DIR,5);
              outtextxy(300,60,"ESTE RANDUL CAINELUI:");
            }
            vulpei=linia2;
            vulpej=coloana2;
        }

        if(jucator==1)
        {
            caine[dogNumber][0]=linia2;
            caine[dogNumber][1]=coloana2;

            setfillstyle(SOLID_FILL,FUNDAL);
            bar(300,60,1100,100);
            if(mode==1)
            {
              setcolor(YELLOW);
              settextstyle(SIMPLEX_FONT,HORIZ_DIR,5);
              outtextxy(300,60,"ESTE RANDUL VULPII:");
            }
        }


        TablaDeJoc[linia1][coloana1]=0;
        TablaDeJoc[linia2][coloana2]=jucator;
        deseneazaPiesa(linia1,coloana1,FUNDAL);
        deseneazaPiesa(linia2,coloana2,culoare[jucator]);

        fout<<"Jocul "<<cnt<<":"<<endl;
        for(int p=1;p<=8;p++)
            {
               for(int q=1;q<=8;q++)
                   fout<<TablaDeJoc[p][q]<<" ";
               fout<<endl;
            }
              fout<<endl<<endl;


        if(jucator==2)
        {
            vulpei=linia2;
            vulpej=coloana2;
        }

        if (jucator == 1 && vulpeincoltita(vulpei, vulpej) && mode==1)
        {
            castigat(1); // Dogs win
        }
        if( (jucator == 2 && linia2 == 8) || (caine[1][0]<vulpei && caine[2][0]<vulpei && caine[3][0]<vulpei && caine[4][0]<vulpei) )
            castigat(2);// Fox wins by reaching row 8

}



int castigat(int jucator)
{
    for(int i=1;i<=8;i++)
            for(int j=1;j<=8;j++)
                TablaDeJoc[i][j]=0;
    foxX=1;
    foxY=5;
    if(jucator==2)
    {
        castig=2;
        Meniu();

    }
    else if(jucator==1)
    {
        castig=1;
        Meniu();
    }


    closegraph();
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
    setcolor(YELLOW);
    for (i=1; i<=numar; i++)
        for (j=1; j<=numar; j++)
            rectangle(stanga+latura*(i-1),sus+latura*(j-1),stanga+latura*i,sus+latura*j);
}

void tabla(int mode)
{
    initPC=true;
    for(int i=1;i<=4;i++)
        sens[i]='s';

    for(int i=0;i<=9;i++)
    {
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

    if(mode==1)
    {
      setcolor(YELLOW);
      settextstyle(SIMPLEX_FONT,HORIZ_DIR,5);
      outtextxy(300,60,"ESTE RANDUL CAINELUI:");
    }

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
    cnt++;
    //fout<<"Jocul "<<cnt<<":"<<endl;

    do
    {
        mutarePiesa(1,1);
        mutarePiesa(2,1);
    }
    while (1);
}

bool strategyStart()
{
//check if the fox is next to a dog
return ( (vulpei==(caine[1][0]-1) && (vulpej==(caine[1][1]+1) || vulpej==(caine[1][1]-1))) || (vulpei==(caine[2][0]-1) && (vulpej==(caine[2][1]+1) || vulpej==(caine[2][1]-1))) || (vulpei==(caine[3][0]-1) && (vulpej==(caine[3][1]+1) || vulpej==(caine[3][1]-1))) || (vulpei==(caine[4][0]-1) && (vulpej==(caine[4][1]+1) || vulpej==(caine[4][1]-1))));

}

void mutarePC(int jucator, int dificulty)
{
    int currentColumn, currentLine, maxLine, selectedDog;
    bool moveValid = false;


    // Logic for the AI controlling the dogs
    if (jucator == 1) // Player = Dogs
    {
        if(dificulty==1)
        {
            if(initPC)
            {

              int col = 2;
              for (int i=1; i<=4; i++)
                {
                    caine[i][0] = 8; // All dogs start in row 8
                    caine[i][1] = col; // Assign column positions
                    col += 2; // Move to the next column
                }

                initPC=false;
            }

            moveDog_rand();

            fout<<"Jocul "<<cnt<<":"<<endl;
            for(int p=1;p<=8;p++)
            {
               for(int q=1;q<=8;q++)
                   fout<<TablaDeJoc[p][q]<<" ";
               fout<<endl;
            }
              fout<<endl<<endl;

        }
        else
        {
        // Initialization step - Place the dogs at their starting positions
           if (initPC)
          {
            int col = 2; // Starting column for the first dog
            for (int i = 1; i <= 4; i++)
            {
                caine[i][0] = 8; // All dogs start in row 8
                caine[i][1] = col; // Assign column positions
                col += 2; // Move to the next column
            }
            initPC = false; // Mark initialization complete
          }

        // **Strategy Start**: Attempt to block the fox
        if (strategyStart())
        {
            bestcoloana = vulpej; // Target the fox's current column

            for (int i = 1; i <= 4; i++)
            {
                // Look for the dog closest to the fox
                if (abs(vulpej - bestcoloana) <= abs(vulpej - caine[i][1]) && caine[i][0] > 1)
                {
                    // If the dog can move in the correct direction (left or right diagonal)
                    if ((sens[i] == 's' && TablaDeJoc[caine[i][0] - 1][caine[i][1] - 1] == 0) ||
                        (sens[i] == 'd' && TablaDeJoc[caine[i][0] - 1][caine[i][1] + 1] == 0))
                    {
                        bestcoloana = caine[i][1];
                        bestcaine = i;
                    }
                }
            }

            // Execute the chosen move
            currentColumn = caine[bestcaine][1];
            currentLine = caine[bestcaine][0];


            if (sens[bestcaine] == 's') // Move diagonally left
            {
                TablaDeJoc[currentLine][currentColumn] = 0; // Clear old position on board
                TablaDeJoc[currentLine - 1][currentColumn - 1] = jucator; // Update board
                deseneazaPiesa(currentLine, currentColumn, FUNDAL); // Clear old position
                deseneazaPiesa(currentLine - 1, currentColumn - 1, culoare[jucator]); // Draw new position
                sens[bestcaine] = 'd'; // Switch direction
                caine[bestcaine][0] = currentLine - 1;
                caine[bestcaine][1] = currentColumn - 1;
            }
            else if (sens[bestcaine] == 'd') // Move diagonally right
            {
                TablaDeJoc[currentLine][currentColumn] = 0; // Clear old position on board
                TablaDeJoc[currentLine - 1][currentColumn + 1] = jucator; // Update board
                deseneazaPiesa(currentLine, currentColumn, FUNDAL); // Clear old position
                deseneazaPiesa(currentLine - 1, currentColumn + 1, culoare[jucator]); // Draw new position
                sens[bestcaine] = 's'; // Switch direction
                caine[bestcaine][0] = currentLine - 1;
                caine[bestcaine][1] = currentColumn + 1;
            }

            if(cainii_incoltesc(caine[bestcaine][0],caine[bestcaine][1]))
                castigat(1);
        }
        // **Fallback Strategy**: Find the highest dog and advance it forward
        else
        {
            maxLine = 0;
            selectedDog = 1;

            // Iterate over all dogs to find the one in the furthest row that can move
            for (int i = 1; i <= 4; i++)
            {
                currentColumn = caine[i][1];
                currentLine = caine[i][0];
                if (currentLine > maxLine && TablaDeJoc[currentLine - 1][currentColumn] == 0)
                {
                    maxLine = currentLine;
                    selectedDog = i;
                }
            }
            if(caine[1][0]<vulpei && caine[2][0]<vulpei && caine[3][0]<vulpei && caine[4][0]<vulpei)
                castigat(2);
            // Move the selected dog
            currentColumn = caine[selectedDog][1];
            currentLine = caine[selectedDog][0];

            
            if (sens[selectedDog] == 's') // Move diagonally left
            {
                TablaDeJoc[currentLine][currentColumn] = 0; // Clear old position on board
                TablaDeJoc[currentLine - 1][currentColumn - 1] = jucator; // Update new position on board
                deseneazaPiesa(currentLine, currentColumn, FUNDAL); // Clear old position
                deseneazaPiesa(currentLine - 1, currentColumn - 1, culoare[jucator]); // Draw new position
                sens[selectedDog] = 'd'; // Switch direction
                caine[selectedDog][0] = currentLine - 1;
                caine[selectedDog][1] = currentColumn - 1;
            }
            else if (sens[selectedDog] == 'd') // Move diagonally right
            {
                TablaDeJoc[currentLine][currentColumn] = 0; // Clear old position on board
                TablaDeJoc[currentLine - 1][currentColumn + 1] = jucator; // Update new position on board
                deseneazaPiesa(currentLine, currentColumn, FUNDAL); // Clear old position
                deseneazaPiesa(currentLine - 1, currentColumn + 1, culoare[jucator]); // Draw new position
                sens[selectedDog] = 's'; // Switch direction
                caine[selectedDog][0] = currentLine - 1;
                caine[selectedDog][1] = currentColumn + 1;
            }

             if(cainii_incoltesc(caine[selectedDog][0],caine[selectedDog][1]))
                castigat(1);
        }

        fout<<"Jocul "<<cnt<<":"<<endl;
        for(int p=1;p<=8;p++)
        {
            for(int q=1;q<=8;q++)
                fout<<TablaDeJoc[p][q]<<" ";
            fout<<endl;
        }
        fout<<endl<<endl;
     }
    }
    else if (jucator == 2)
    {
        if(dificulty==1)
        {
            moveFox_rand(foxX,foxY);

            fout<<"Jocul "<<cnt<<":"<<endl;
            for(int p=1;p<=8;p++)
            {
               for(int q=1;q<=8;q++)
                   fout<<TablaDeJoc[p][q]<<" ";
               fout<<endl;
            }
              fout<<endl<<endl;
        }
        else
        {
            moveFox(foxX,foxY);

            fout<<"Jocul "<<cnt<<":"<<endl;
            for(int p=1;p<=8;p++)
            {
                for(int q=1;q<=8;q++)
                    fout<<TablaDeJoc[p][q]<<" ";
                fout<<endl;
            }
            fout<<endl<<endl;
        }
    }
}

void mutarePvC(int jucator, int dificulty)
{
    initPC=true;
    for(int i=1;i<=4;i++)
        sens[i]='s';

    cnt++;
    //fout<<"Jocul "<<cnt<<endl;

    do
    {

        if(jucator==1)
        {
            mutarePC(1, dificulty);
            mutarePiesa(2,2);
        }
        else if(jucator==2)
        {
            mutarePiesa(1,2);
            mutarePC(2, dificulty);
        }
    }
    while (1);
}

void PvP_window()
{
    tabla(1);
    mutarePvP();

}

void PvC_window()
{
    int screenx=GetSystemMetrics(SM_CXSCREEN);
    int screeny=GetSystemMetrics(SM_CYSCREEN);

    setbkcolor(WHITE);
    cleardevice();

    setcolor(BLUE);
    settextstyle(SIMPLEX_FONT,HORIZ_DIR,7);
    outtextxy(400,60,"Alege ce vrei sa fii:");

    int xb=570;
    int ybc=200;
    int ybv=500;
    int latbut=350;
    int inaltbut=200;

    //setcolor(WHITE);
    readimagefile("Fox_1.jpg",0,screeny-500,360,screeny-140);
    readimagefile("Dog_1.jpg",screenx-405,screeny-440,screenx,screeny-160);

    drawButton(xb,ybc,latbut,inaltbut,"CAINI",10,5);
    drawButton(xb,ybv,latbut,inaltbut,"VULPE",12,5);

    while (1)
    {
        int x=mousex();
        int y=mousey();
        if (ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);

                if(x>=xb && x<=xb+latbut && y>=ybc && y<=ybc+inaltbut)
                {
                    PvC_dificulty_window(1);
                    //tabla(2);
                    //mutarePvC(2);
                }
                if(x>=xb && x<=xb+latbut && y>=ybv && y<=ybv+inaltbut)
                {
                    PvC_dificulty_window(2);
                    //tabla(2);
                    //mutarePvC(1);
                }

            }
    }

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

bool miscarevalida(int x, int y)
{
    return x>=1 && x<=8 && y>=1 && y<=8 && TablaDeJoc[x][y]==0;
}

void moveFox(int& foxX, int& foxY)
{
    int bestMoveX=foxX;
    int bestMoveY=foxY;
    bool foundMove=false;

    int Directieinainte[2][2] = {{1, -1}, {1, 1}};
    int Directieinapoi[2][2] = {{-1, -1}, {-1, 1}};


    for (int i=0; i<2; i++)
    {
        int newX = foxX + Directieinainte[i][0];
        int newY = foxY + Directieinainte[i][1];
        if (miscarevalida(newX, newY))
        {
            bestMoveX=newX;
            bestMoveY=newY;
            foundMove=true;
            break;
        }
    }


    if (!foundMove)
    {
        for (int i=0; i<2; i++)
        {
            int newX = foxX + Directieinapoi[i][0];
            int newY = foxY + Directieinapoi[i][1];
            if (miscarevalida(newX, newY))
            {
                bestMoveX=newX;
                bestMoveY=newY;
                foundMove=true;
                break;
            }
        }
    }

    // Daca nu exista mutari valide
    if (!foundMove)
         castigat(1);


    TablaDeJoc[foxX][foxY]=0;
    deseneazaPiesa(foxX, foxY, FUNDAL);

    foxX=bestMoveX;
    foxY=bestMoveY;

    TablaDeJoc[foxX][foxY]=2;
    deseneazaPiesa(foxX, foxY, culoare[2]);

    if(foxX==8)
        castigat(2);

}

void PvC_dificulty_window(int jucator)
{
    int screenx=GetSystemMetrics(SM_CXSCREEN);
    int screeny=GetSystemMetrics(SM_CYSCREEN);

    setbkcolor(WHITE);
    cleardevice();

    setcolor(BLUE);
    settextstyle(SIMPLEX_FONT,HORIZ_DIR,7);
    outtextxy(400,60,"Alege Dificultatea:");

    int xb=570;
    int ybc=200;
    int ybv=500;
    int latbut=350;
    int inaltbut=200;

    //setcolor(WHITE);
    readimagefile("Fox_1.jpg",0,screeny-500,360,screeny-140);
    readimagefile("Dog_1.jpg",screenx-405,screeny-440,screenx,screeny-160);

    drawButton(xb,ybc,latbut,inaltbut,"EASY",10,5);
    drawButton(xb,ybv,latbut,inaltbut,"MEDIUM",11,5);

    if(jucator==1)
    {

       while (1)
      {
        int x=mousex();
        int y=mousey();
        if (ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);

                if(x>=xb && x<=xb+latbut && y>=ybc && y<=ybc+inaltbut)
                {
                    //PvC_dificulty_window(2);
                    tabla(2);
                    srand(time(NULL));
                    mutarePvC(2,1);
                }
                if(x>=xb && x<=xb+latbut && y>=ybv && y<=ybv+inaltbut)
                {
                    //PvC_dificulty_window(1);
                    tabla(2);
                    //srand(time(NULL))
                    mutarePvC(2,2);
                }

            }
      }

    }
    else
    {
        while (1)
       {
        int x=mousex();
        int y=mousey();
        if (ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);

                if(x>=xb && x<=xb+latbut && y>=ybc && y<=ybc+inaltbut)
                {
                    //PvC_dificulty_window(2);
                    tabla(2);
                    srand(time(NULL));
                    mutarePvC(1,1);
                }
                if(x>=xb && x<=xb+latbut && y>=ybv && y<=ybv+inaltbut)
                {
                    //PvC_dificulty_window(1);
                    tabla(2);
                    mutarePvC(1,2);
                }

            }
       }

    }

}

void moveDog_rand()
{
    int dx[]={-1,-1};
    int dy[]={-1,1};

    for(int i=0; i<10; i++)
    {
        int dog=rand()%4+1;
        int dir=rand()%2; // Alege o direcție aleatorie noua
        int newX = caine[dog][0]+dx[dir];
        int newY = caine[dog][1]+dy[dir];

        if (miscarevalida(newX, newY))
       {
            TablaDeJoc[caine[dog][0]][caine[dog][1]]=0;
            deseneazaPiesa(caine[dog][0], caine[dog][1], FUNDAL);
            caine[dog][0]=newX;
            caine[dog][1]=newY;
            TablaDeJoc[caine[dog][0]][caine[dog][1]]=1;
            deseneazaPiesa(caine[dog][0], caine[dog][1], culoare[1]);

            if(cainii_incoltesc(newX,newY))
                castigat(1);

            break;

       }
    }



}

void moveFox_rand(int& foxX, int& foxY)
{
    int dx[]={-1, 1, -1, 1};
    int dy[]={-1, -1, 1, 1};


    if(vulpeincoltita(foxX,foxY))
    {
        //fout<<"AICI ESTE BUBA";
        castigat(1);
        return;
    }


    bool moved=false;
    for(int i=0; !moved; i++)
    {
        int dir=rand()%4; // Alege o direcție aleatorie noua
        int newX = foxX+dx[dir];
        int newY = foxY+dy[dir];


        if (miscarevalida(newX, newY))
       {
            if(newX==8 || (caine[1][0]<newX && caine[2][0]<newX && caine[3][0]<newX && caine[4][0]<newX))
            {
             castigat(2);
             return;
            }

            TablaDeJoc[foxX][foxY]=0;
            deseneazaPiesa(foxX, foxY, FUNDAL);
            foxX=newX;
            foxY=newY;
            TablaDeJoc[newX][newY]=2;
            deseneazaPiesa(foxX, foxY, culoare[2]);

            //if(vulpeincoltita(foxX,foxY))
               //castigat(1);

             //break;
             moved=true;

       }

    }

}

int cainii_incoltesc(int x, int y)
{

    ///Cainii incoltesc vulpea la mijloc
     if(TablaDeJoc[x][y]==1 && TablaDeJoc[x-2][y]==1 && TablaDeJoc[x-1][y-1]==2 && TablaDeJoc[x][y-2]==1 && TablaDeJoc[x-2][y-2]==1)
        return 1;

     if(TablaDeJoc[x][y]==1 && TablaDeJoc[x-2][y]==1 && TablaDeJoc[x-1][y+1]==2 && TablaDeJoc[x][y+2]==1 && TablaDeJoc[x-2][y+2]==1)
        return 1;

     if(TablaDeJoc[x][y]==1 && TablaDeJoc[x+2][y]==1 && TablaDeJoc[x+1][y-1]==2 && TablaDeJoc[x][y-2]==1 && TablaDeJoc[x+2][y-2]==1)
        return 1;

     if(TablaDeJoc[x][y]==1 && TablaDeJoc[x+2][y]==1 && TablaDeJoc[x+1][y+1]==2 && TablaDeJoc[x][y+2]==1 && TablaDeJoc[x+2][y+2]==1)
        return 1;

    ///Cainii incoltesc vulpea la margine

    if(TablaDeJoc[x][y]==1 && TablaDeJoc[x-2][y]==1 && TablaDeJoc[x-1][y-1]==2 && y==2)
         return 1;

    if(TablaDeJoc[x][y]==1 && TablaDeJoc[x+2][y]==1 && TablaDeJoc[x+1][y-1]==2 && y==2)
         return 1;

    if(TablaDeJoc[x][y]==1 && TablaDeJoc[x-2][y]==1 && TablaDeJoc[x-1][y-1]==2 && y==7)
         return 1;

    if(TablaDeJoc[x][y]==1 && TablaDeJoc[x+2][y]==1 && TablaDeJoc[x+1][y-1]==2 && y==7)
         return 1;

    if(TablaDeJoc[x][y]==1 && TablaDeJoc[x][y-2]==1 && TablaDeJoc[x-1][y-1]==2 && x==2)
        return 1;

    if(TablaDeJoc[x][y]==1 && TablaDeJoc[x][y+2]==1 && TablaDeJoc[x-1][y+1]==2 && x==2)
         return 1;

    ///Un Caine incolteste vulpea la margine

    if(x==2 && y==2 && TablaDeJoc[1][1]==2)
        return 1;

     if(x==2 && y==7 && TablaDeJoc[8][8]==2)
        return 1;

    return 0;

}
