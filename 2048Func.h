//
// Created by Tan on 2018/7/7.
//

#ifndef MT2048_2048FUNC_H
#define MT2048_2048FUNC_H

// CodeNemo (GitHub)
// C
// Terminal2048
// CLion
// V1.1

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


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
unsigned int Highest();
char *RandomStr(int8_t leng);


char *RandomStr(int8_t leng)
{
    char rnd;
    srand((unsigned)time(NULL));
    char *str = (char *)malloc((leng+1) * sizeof(char));
    for(int i=0;i<leng;i++)
    {
        *(str+i) = rand()%66+58;
    }
    return str;
}

void Tutor()
{
    unsigned short *num = malloc(16*sizeof(unsigned short));
    for(int8_t i=0;i<16;i++)
        *(num+i)=0;
    *num = *(num+4) = 2;
    puts("In this game, there's a 4x4 square filled with 16 numbers.");
    //puts("")
}


unsigned int Highest()
{
    char c , *data=malloc(7*sizeof(char));
    u_int8_t i1;
    unsigned int score;
    FILE *save;

    if((save=fopen("highest","r"))==NULL)
        return 0;
    if((c=getc(save))==EOF)
        return 0;
    else
        while(48<=c && c<=57)
        {
            *(data+i1)=c;
            c=getc(save);
            i1++;
        }
    score = atoi(data);
    if(score%2 != 0)
    {
        fopen("highest","w");
        return 0;
    }
    return score;
}

void Save(unsigned short *num,unsigned int *score)
{
    srand(time(NULL));
    int8_t RndNum=0;
    char *data = malloc(5*sizeof(char));
    RndNum = rand()%8+1;
    FILE *save;
    save = fopen("2048save","w");
    sprintf(data , "%d" , RndNum+1);
    fputs(data,save);
    fputs(RandomStr(RndNum),save);

    for(int8_t i=0;i<16;i++)
    {
        fputs(RandomStr(rand()%RndNum+1),save);
        sprintf(data,"%d",*(num+i)+RndNum);
        fputs(data,save);
    }
    fputs(RandomStr(rand()%RndNum+1),save);
    sprintf(data,"%d",*score+RndNum);
    fputs(data,save);
    fputs(RandomStr(rand()%RndNum+1),save);

    free(data);
    fclose(save);

}

_Bool ReadSave(unsigned short *num , unsigned int *score)
{
    for(int8_t i=0;i<16;i++)
        *(num+i)=0;
    char *data = (char*)malloc(5*sizeof(char));
    char c='#';
    FILE *save;
    int8_t i2 , EncNum=0;
    unsigned int check;

    if((save=fopen("2048save","r"))==NULL)
        return 1;

    c = getc(save)-1;
    if('1'<=c && c<='8')
        EncNum = atoi(&c);
    else
        return 1;
    fseek(save , EncNum , SEEK_CUR);


    // Collects 16 nums
    for(int8_t i1=0 ; i1<16 ; i1++)
    {
        i2 = 0;
        for(short i=0; i<5; i++)
            *(data+i)=' ';
        if((c=getc(save))==EOF)
            return 1;

        while(c<'0' || '9'<c)
            if((c=getc(save))==EOF)
                return 1;
        while('0'<=c && c<='9')
        {
            putchar(c);
            *(data+i2) = c;
            i2++;
            if((c=getc(save))==EOF)
                return 1;

        }
        *(num+i1) = atoi(data) - EncNum;
        printf("%d\n",*(num+i1));

        check = *(num+i1);
        while(check!=1 && check)
        {

            if (check % 2)
            {
                //printf("%d %d ", i1 ,*(num+i1));
                puts("Nums error");
                return 1;
            }
            check /= 2;
        }
    }



    while(c<'0' || c>'9')
        if((c=getc(save))==EOF)
            return 1;
    i2=0;
    for(short i=0; i<5; i++)
        *(data+i)=' ';

    while('0'<=c && c<='9')
    {
        *(data+i2)=c;
        i2++;
        if((c=getc(save))==EOF)
            return 1;
    }
    puts(data);
    if(*data==' ')
        return 1;
    //printf("%d\n",atoi(data));
    *score = atoi(data);
    *score -= EncNum;
    //printf("%d", *score);
    check = *score;
    if(check!=0)
        if((check%2) !=0)
        {
            puts("Scores error!");
            return 1;
        }

    return 0;
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




#endif //MT2048_2048FUNC_H
