#include "2048Func.h"

int main();
_Bool Game(_Bool mode);


int main()
{
    char cmd;
    WelcomePage();

    cmd=getch();
    switch(cmd)
    {
        case 'q':
            exit(0);
        case 'n':
            Game(0);
            break;
        case 'c':
            Game(1);
            break;
        case 'r':
            break;
        default:
            puts("Retry!");

    }
}

_Bool Game(_Bool mode)
{
    char cmd,c;
    char *data = (char *)malloc(5*sizeof(char));
    FILE *save;
    _Bool alive=1,moved=0,NewRecord=0;
    unsigned int score=0,record;
    unsigned short *num = malloc(16*sizeof(unsigned short));
    for(int8_t i=0;i<16;i++)
        *(num+i)=0;
    if(mode)
    {

        if(ReadSave(num,&score))
        {
            puts("Error! New game starts.");
            fopen("2048save","w");
            score=0;
            for(int8_t i=0;i<16;i++)
                *(num+i)=0;
            RandonCreate(num);
            RandonCreate(num);
        }
    }
    else
    {
        RandonCreate(num);
        RandonCreate(num);
    }
    PrintNum(num);
    while(alive)
    {
        puts("Use WSAD to move the numbers.");
        cmd=getch();
        switch(cmd)
        {
            case 'w':
                //puts("Up");
                moved=up(num,&score);
                break;
            case 's':
                moved=down(num,&score);
                break;
            case  'a':
                moved=left(num,&score);
                break;
            case 'd':
                moved=right(num,&score);
                break;
            case 'q':
                Save(num,&score);
                return 0;
            default:
                puts("Retry!");
        }
        //puts("1");
        //system("clear");
        if(moved)
            alive=RandonCreate(num);
        else
            puts("Retry!");

        PrintNum(num);
        printf("Your score: %d",score);
    }
    fopen("2048save","w");

    int8_t i1=0;
    if((save=fopen("highest","r"))==NULL)
        NewRecord=1;
    else
    {
        if((c=getc(save))==EOF)
            NewRecord=1;
        else
            while(48<=c && c<=57)
            {
                *(data+i1)=c;
                c=getc(save);
                i1++;
            }
        record=atoi(data);
        if(record<score)
            NewRecord=1;
    }
    if(NewRecord)
    {
        puts("New Record!");
        save=fopen("highest","w");
        sprintf(data,"%d",score);
        fputs(data,save);
        fclose(save);
    }
    else
        puts("Oops!");
}