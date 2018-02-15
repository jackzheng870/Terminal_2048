//
// Created by Tan on 2018/1/7.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#ifndef DEV_2048_2048_FUNC_H
#define DEV_2048_2048_FUNC_H

_Bool RandonCreate(unsigned short *num);
void WelcomePage();
_Bool CheckDie(unsigned short *num);
_Bool left(unsigned short *num,unsigned int *score);
_Bool right(unsigned short *num,unsigned int *score);
_Bool up(unsigned short *num,unsigned int *score);
_Bool down(unsigned short *num,unsigned int *score);
void PrintNum(unsigned short *num);
void Save(unsigned short *num,unsigned int *score);
_Bool ReadSave(unsigned short *num , unsigned int *score);

void Save(unsigned short *num,unsigned int *score)
{
    char c;
    char *data = malloc(5*sizeof(char));
    _Bool NewRecord=0;
    unsigned int record;
    FILE *save;
    save= fopen("2048save","w");
    for(int8_t i=0;i<16;i++)
    {
        fprintf(save,"#");
        sprintf(data,"%d",*(num+i));
        fputs(data,save);
    }

    //save=fopen("2048save","a");
    fprintf(save,"#");
    sprintf(data,"%d",*score);
    fputs(data,save);
    fprintf(save,"#");

    free(data);
    fclose(save);

}

_Bool ReadSave(unsigned short *num , unsigned int *score)
{
    //puts("2!");
    char *data = (char*)malloc(5*sizeof(char));
    char c='#';
    FILE *save;
    int8_t i2=0;
    unsigned int check;
    //puts("Start check");
    if((save=fopen("2048save","r"))==NULL)
        return 1;

    //puts("3!");
    for(int8_t i1=0;i1<16;i1++)
    {
        //puts("4!");
        //printf("No.%d",i1);
        if((c=getc(save))==EOF)
            return 1;

        i2=0;
        while(c<=47 || c>=58)
            if((c=getc(save))==EOF)
                return 1;


        while(47<c && c<58)
        {

            //puts("Enter");
            *(data+i2)=c;
            i2++;
            if((c=getc(save))==EOF)
                return 1;
        }

        *(num+i1) = atoi(data);
        check=*(num+i1);
        while(check!=1 && check!=0)
        {

            if ((check % 2) != 0)
                return 1;
            check /= 2;
        }
        //puts("1!");
    }
    //puts("4!");

    //puts("5!");
    while(c<=47 || c>=58)
        if((c=getc(save))==EOF)
            return 1;
    i2=0;
    while(47<c && c<58)
    {
        //puts("Enter");
        *(data+i2)=c;
        i2++;
        if((c=getc(save))==EOF)
            return 1;

    }
    if(*data==' ')
        return 1;
    *score=atoi(data);
    check=*score;
    if(check!=0)
        if((check%2) !=0)
            return 1;

    return 0;
}

int getche()
{

    struct termios t;
    int c;

    tcgetattr(0,&t);
    t.c_lflag&=~ICANON;
    tcsetattr(0,TCSANOW,&t);
    fflush(stdout);
    c=getchar();
    t.c_lflag|=ICANON;
    tcsetattr(0,TCSANOW,&t);
    return c;
}

_Bool CheckDie(unsigned short *num)
{
    for(int8_t i=0;i<16;i++)
        if(*(num+i)==0)
            return 0;
    for (int8_t i2 = 0; i2 < 4; i2++)
    {
        for (int8_t i3 = 0; i3 < 3; i3++)
        {
            if (*(num + 4 * i2 + i3) == *(num + 4 * i2 + i3 + 1) )
            {
                return 0;
            }
        }
    }
    for(int8_t i2=0;i2<4;i2++)
        for(int8_t i3=0;i3<3;i3++)
            if(*(num+i2+4*i3)==*(num+i2+4*i3+4))
            {
                return 0;
            }

    return 1;
}

_Bool RandonCreate(unsigned short *num)
{
    srand((unsigned)time(NULL));
    int8_t amt=0,RndNum=0;
    unsigned short *emt[16];
    int8_t i1=0;
    for(int8_t i2=0;i2<16;i2++)
        if(*(num+i2)==0)
        {
            emt[i1]=num+i2;
            i1++;
            amt++;
        }
    //if(amt==0)
    //    return 0;
    RndNum = rand()%4+1;
    if(RndNum==1 || RndNum==3)
        RndNum=2;
    //printf("%d\n",RndNum);
    *emt[rand()%amt]=RndNum;
    if(amt==1)
    {
        if(CheckDie(num))
            return 0;
    }
    return 1;


}

void WelcomePage()
{
    puts("Welcome to 2048! Choose an option:");
    puts("n=New Game\nc=Continue\nq=Quit\nr=Highest Record");
}

_Bool left(unsigned short *num,unsigned int *score)
{
    _Bool moved = 0;
    for (int8_t i1 = 0; i1 < 3; i1++) {
        for (int8_t i2 = 0; i2 < 4; i2++) {
            for (int8_t i3 = 0; i3 < 3; i3++) {
                if ((*(num + 4 * i2 + i3) == 0) && (*(num + 4 * i2 + i3 + 1) != 0)) {
                    *(num + 4 * i2 + i3) = *(num + 4 * i2 + i3 + 1);
                    *(num + 4 * i2 + i3 + 1) = 0;
                    moved = 1;
                }
            }
        }
    }
    
    for (int8_t i2 = 0; i2 < 4; i2++) {
        for (int8_t i3 = 0; i3 < 3; i3++) {
            if ((*(num + 4 * i2 + i3) == *(num + 4 * i2 + i3 + 1)) && (*(num + 4 * i2 + i3 + 1) != 0)) {
                *(num + 4 * i2 + i3) += *(num + 4 * i2 + i3 + 1);
                *score += *(num + 4 * i2 + i3);
                *(num + 4 * i2 + i3 + 1) = 0;

                moved = 1;
            }
        }
    }
    
    for(int8_t i1=0;i1<3;i1++)
    {
        for (int8_t i2 = 0; i2 < 4; i2++)
        {
            for (int8_t i3 = 0; i3 < 3; i3++)
            {
                if ((*(num+4*i2+i3) == 0) && (*(num+4 * i2 + i3 + 1) != 0))
                {
                    *(num+4*i2+i3) = *(num+4 * i2 + i3 + 1);
                    *(num+4 * i2 + i3 + 1) = 0;
                }
            }
        }
    }
    
    return moved;
}

_Bool right(unsigned short *num,unsigned int *score)
{
    _Bool moved=0;
    for(int8_t i1=3;i1>0;i1--)
    {
        for (int8_t i2=3; i2>-1; i2--)
        {
            for (int8_t i3=3; i3>0; i3--)
            {
                if (*(num+4*i2+i3) == 0 && *(num+4 * i2 + i3-1) != 0)
                {
                    *(num+4*i2+i3) = *(num+4 * i2 + i3-1);
                    *(num+4 * i2 + i3-1) = 0;
                    moved=1;
                }
            }
        }
    }
    
    for (int8_t i2 = 3; i2 > -1; i2--)
    {
        for (int8_t i3 = 3; i3 > 0; i3--)
        {
            if ((*(num+4*i2+i3)==*(num+4 * i2 + i3-1)) && *(num+4*i2+i3)!=0)
            {
                *(num+4*i2+i3) += *(num+4 * i2 + i3-1);
                *(num+4 * i2 + i3-1) = 0;
                *score+=*(num+4*i2+i3);
                moved=1;
            }
        }
    }
    
    for(int8_t i1=3;i1>0;i1--)
    {
        for (int8_t i2=3; i2>-1; i2--)
        {
            for (int8_t i3=3; i3>0; i3--)
            {
                if ((*(num+4*i2+i3) == 0) && (*(num+4 * i2 + i3-1) != 0))
                {
                    *(num+4*i2+i3) = *(num+4 * i2 + i3-1);
                    *(num+4 * i2 + i3-1) = 0;
                }
            }
        }
    }
    
    return moved;
}

_Bool up(unsigned short *num,unsigned int *score)
{
    _Bool moved=0;
    for(int8_t i1=0;i1<3;i1++)
    {
        for(int8_t i2=0;i2<4;i2++)
        {
            for(int8_t i3=0;i3<3;i3++)
            {
                if((*(num+i2+4*i3)==0) && (*(num+i2+4*i3+4)!=0))
                {
                    *(num+i2+4*i3)=*(num+i2+4*i3+4);
                    *(num+i2+4*i3+4)=0;
                    moved=1;
                }
            }
        }
    }
    
    for(int8_t i2=0;i2<4;i2++)
        for(int8_t i3=0;i3<3;i3++)
            if((*(num+i2+4*i3)==*(num+i2+4*i3+4)) && (*(num+i2+4*i3)!=0))
            {
                *(num+i2+4*i3) += *(num+i2+4*i3+4);
                *(num+i2+4*i3+4)=0;
                *score += *(num+i2+4*i3);
                moved=1;
            }
    
    for(int8_t i1=0;i1<3;i1++)
    {
        for(int8_t i2=0;i2<4;i2++)
        {
            for(int8_t i3=0;i3<3;i3++)
            {
                if((*(num+i2+4*i3)==0) && (*(num+i2+4*i3+4)!=0))
                {
                    *(num+i2+4*i3)=*(num+i2+4*i3+4);
                    *(num+i2+4*i3+4)=0;
                }
            }
        }
    }
    
    return moved;
}

_Bool down(unsigned short *num,unsigned int *score)
{
    _Bool moved=0;
    for(int8_t i1=0;i1<3;i1++)
        for(int8_t i2=3;i2>-1;i2--)
            for(int8_t i3=3;i3>0;i3--)
                if(*(num+4*i3+i2)==0&&*(num+4*i3+i2-4)!=0)
                {
                    *(num+4*i3+i2)=*(num+4*i3+i2-4);
                    *(num+4*i3+i2-4)=0;
                    moved=1;
                }
    for(int8_t i2=3;i2>-1;i2--)
        for(int8_t i3=3;i3>0;i3--)
            if((*(num+4*i3+i2)==*(num+4*i3+i2-4)) && *(num+4*i3+i2)!=0)
            {
                *(num+4*i3+i2)+=*(num+4*i3+i2-4);
                *(num+4*i3+i2-4)=0;
                *score+=*(num+4*i3+i2);
                moved=1;
            }
    
    for(int8_t i1=0;i1<3;i1++)
        for(int8_t i2=3;i2>-1;i2--)
            for(int8_t i3=3;i3>0;i3--)
                if(*(num+4*i3+i2)==0&&*(num+4*i3+i2-4)!=0)
                {
                    *(num+4*i3+i2)=*(num+4*i3+i2-4);
                    *(num+4*i3+i2-4)=0;
                }
    
    return moved;
}

int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( 0u | ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

void PrintNum(unsigned short *num)
{
    for(int8_t i1=0;i1<4;i1++)
    {
        for(int8_t i2=0;i2<4;i2++)
        {
            printf("%d",*(num+4*i1+i2));
            if(*(num+4*i1+i2)<10)
                printf("     ");
            else
                {
                    if (*(num+4*i1+i2)< 100)
                    printf("    ");
                    else
                    {
                        if (*(num+4*i1+i2)< 1000)
                            printf("   ");
                        else
                        {
                            if (*(num+4*i1+i2)< 10000)
                                printf("  ");
                            else
                                if (*(num+4*i1+i2)< 100000)
                                    printf(" ");
                        }
                    }
                }
        }
        printf("\n");
    }


}

#endif //DEV_2048_2048_FUNC_H
