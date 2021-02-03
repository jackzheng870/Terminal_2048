// CodeNemo (GitHub)
// C
// Terminal2048 - MacTerminal2048
// CLion
// V1.1
// macOS 10.13

#include "2048Func.h"
#include <termios.h>
#include <unistd.h>
#include <term.h>
//#include <curses.h>

int main();
_Bool Game(_Bool mode);
int getche();
int getch();



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

int main()
{

    char cmd;


    unsigned int highest=0;
    while(1)
    {
        WelcomePage();
        cmd=getch();
        switch(cmd)
        {
            case 't':

            case 'q':
                exit(0);
            case 'n':
                Game(0);
                break;
            case 'c':
                Game(1);
                break;
            case 'r':
                highest=Highest();
                if(highest)
                    printf("%d\n",highest);
                else
                    puts("No records!");
                break;
            default:
                puts("Retry!");
        }
    }

}

_Bool Game(_Bool mode)
{
    char cmd,c;
    char *data = (char *)malloc(7*sizeof(char));
    FILE *save;
    _Bool alive=1,moved=0;
    unsigned int score=0,highest;
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
            usleep(2000);
            RandonCreate(num);
        }
    }
    else
    {
        RandonCreate(num);
        usleep(2000);
        RandonCreate(num);
    }
    PrintNum(num);
    printf("Your score: %d\n",score);
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
        printf("Your score: %d\n",score);
        putchar('\n');
    }
    fopen("2048save","w");
    highest = Highest();
    if(highest < score)
    {
        puts("New record!");
        save=fopen("highest","w");
        sprintf(data,"%d",score);
        fputs(data,save);
        fclose(save);
    }
    else
    {
        puts("Oops!");
    }
    return 0;
}