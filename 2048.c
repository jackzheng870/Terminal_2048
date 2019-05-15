#include "2048.h"

int main();
_Bool Game(_Bool mode);
_Bool RandomNum(uint16_t (*num)[4]);
_Bool ReadSave(uint16_t (*num)[4], uint32_t *scores);
_Bool Up(uint16_t (*num)[4], uint32_t *scores);
_Bool Down(uint16_t (*num)[4], uint32_t *scores);
_Bool Left(uint16_t (*num)[4], uint32_t *scores);
_Bool Right(uint16_t (*num)[4], uint32_t *scores);
void Print(uint16_t (*num)[4], uint32_t *scores);
uint32_t Highest(_Bool look);
_Bool Save(uint16_t (*num)[4], uint32_t *scores);

int main()
{
    char c=' ';
    while(1)
    {
        puts("Welcome to 2048!");
        puts("n = New game\nc = Continue game\nq/Esc = Quit\nh = View highest scores");
        c = getch();
        flush;
        if('A'<=c && c<='Z')
            c += 32;
        if(c==27)
            c='q';
        switch(c)
        {
            case 'q':
                exit(0);
            case 'n':
                puts("New game");
                pause(0.2);
                clear;
                Game(0);
                break;
            case 'h':
                Highest(1);
                break;
            case 'c':
                pause(0.2);
                clear;
                Game(1);
                break;
            default:
                puts("Retry!");
                pause(0.5);
                clear;
        }
    }
}

_Bool Game(_Bool mode) // 0 = New, 1 = Continue
{
    char c=' ';
    _Bool alive=1, done=0;
    uint16_t (*num)[4] = malloc(16 * sizeof(uint16_t));
    for(uint8_t i0=0; i0<4; i0++)
        for(uint8_t i1=0; i1<4; i1++)
            *(*(num+i0)+i1) = 0;
    uint32_t scores=0;
    if(mode && !ReadSave(num, &scores))
        puts("Found!");
    else
    {
        puts("No unfinished game!");
        for(uint8_t i0=0; i0<4; i0++)
            for(uint8_t i1=0; i1<4; i1++)
                *(*(num+i0)+i1) = 0;
        scores = 0;
        RandomNum(num);
        pause(0.03);
        RandomNum(num);
    }
    puts("Game starts!");
    while(alive)
    {
        Print(num, &scores);
        puts("Press a key to do");
        puts("w = up, s = down, a = left, d = right, q = quit");
        c = getch();
        flush;
        switch(c)
        {
            case 'q':
                Save(num, &scores);
                free(num);
                puts("Returning to main menu");
                pause(1);
                clear;
                return 0;
            case 'w':
                done = Up(num, &scores);
                break;
            case 's':
                done = Down(num, &scores);
                break;
            case 'a':
                done = Left(num, &scores);
                break;
            case 'd':
                done = Right(num, &scores);
                break;
            default:
                puts("Retry!");
                break;
        }
        clear;
        if(done)
           alive = RandomNum(num);
        else
            puts("Retry");
    }
    puts("Game over!");
    remove("data0");
    remove("data1");
    if(scores>Highest(0))
    {
        FILE *saved;
        saved = fopen("data2", "w");
        puts("New record!");
        char* str = malloc(8 * sizeof(char));
        sprintf(str, "%7d", scores);
        fputs(str, saved);
        free(str);
        fclose(saved);
        pause(2);
        clear;
    }
    return 0;
}

// Succeed 0, Fail 1
// if files invalid, remove
_Bool ReadSave(uint16_t (*num)[4], uint32_t *scores)
{
    FILE *saved;
    if(fopen("data0", "r")==NULL || fopen("data1", "r")==NULL)
    {
        puts("Error!");
        return 1;
    }
    uint16_t check = 0;
    char c, *in = malloc(4 * sizeof(char)), *ScoresStr = malloc(8 * sizeof(char));
    for(uint8_t i0=0; i0<4; i0++)
        *(in+i0) = ' ';
    for(uint8_t i0=0; i0<7; i0++)
        *(ScoresStr+i0) = ' ';
    saved = fopen("data0", "r");
    for(uint8_t i0=0; i0<7; i0++)
    {
        fgets(in, 4, saved);
        //puts(in);
        *(ScoresStr+i0) = atoi(in);
    }
    *scores = (unsigned)atoi(ScoresStr);
    if(*scores%2)
    {
        puts("Error!");
        fclose(saved);
        remove("data0");
        remove("data1");
        return 1;
    }
    saved = fopen("data1", "r");
    char *NumStr = malloc(6 * sizeof(char));
    for(uint8_t i0=0; i0<5; i0++)
        *(NumStr+i0) = ' ';
    for(uint8_t i0=0; i0<4; i0++)
        for(uint8_t i1=0; i1<4; i1++)
        {
            for(uint8_t i2=0; i2<5; i2++) // get NumStr[6]
            {
                fgets(in, 4, saved);
                *(NumStr+i2) = atoi(in);
            }
            *(*(num+i0)+i1) = (uint16_t) atoi(NumStr);
            check = *(*(num+i0)+i1);
            while(check && check!=1)
            {
                if(check%2)
                {
                    puts("Error(s)!");
                    fclose(saved);
                    remove("data0");
                    remove("data1");
                    return 1;
                }
                else
                    check /= 2;
            }
        }

    fclose(saved);
    return 0;
}

// alive return 1, dead return 0
// randomly create 2/4, check if dead
_Bool RandomNum(uint16_t (*num)[4])
{
    _Bool alive=0;
    srand((unsigned)time(NULL));
    uint8_t amount=0;
    uint16_t *empty[16];
    for(uint8_t i0=0; i0<4; i0++)
        for(uint8_t i1=0; i1<4; i1++)
            if(*(*(num+i0)+i1)==0)
            {
                empty[amount] = *(num+i0)+i1;
                amount++;
            }
    if(amount)
    {
        uint8_t RandomNum = rand()%4+1;
        if(RandomNum==1 || RandomNum==3)
            RandomNum = 4;
        *empty[rand()%amount] = RandomNum;
        if(amount==1)
        {
            for(uint8_t i1=0; i1<4; i1++)
                for(uint8_t i2=0; i2<3; i2++)
                    if(*(*(num+i1)+i2)==*(*(num+i1)+i2+1))
                        alive=1;
            for(uint8_t i1=0; i1<4; i1++)
                for(uint8_t i2=0; i2<3; i2++)
                    if(*(*(num+i2)+i1)==*(*(num+i2+1)+i1))
                        alive=1;
        }
        else
            alive=1;

    }
    else
    {
        puts("Oops! No empty place!");
        alive=0;
    }
    return alive;
}

_Bool Up(uint16_t (*num)[4], uint32_t *scores)
{
    _Bool done=0;
    for(uint8_t i0=0; i0<3; i0++)
        for(uint8_t i1=0; i1<4; i1++)
            for(uint8_t i2=0; i2<3; i2++)
                if(*(*(num+i2)+i1)==0 && *(*(num+i2+1)+i1)!=0)
                {
                    *(*(num+i2)+i1) = *(*(num+i2+1)+i1);
                    *(*(num+i2+1)+i1) = 0;
                    done = 0;
                }
    for(uint8_t i1=0; i1<4; i1++)
        for(uint8_t i2=0; i2<3; i2++)
            if(*(*(num+i2)+i1)==*(*(num+i2+1)+i1) && *(*(num+i2)+i1)!=0)
            {
                *(*(num+i2)+i1) *= 2;
                *scores += *(*(num+i2)+i1);
                *(*(num+i2+1)+i1) = 0;
                done = 1;
            }
    for(uint8_t i0=0; i0<3; i0++)
        for(uint8_t i1=0; i1<4; i1++)
            for(uint8_t i2=0; i2<3; i2++)
                if(*(*(num+i2)+i1)==0 && *(*(num+i2+1)+i1)!=0)
                {
                    *(*(num+i2)+i1) = *(*(num+i2+1)+i1);
                    *(*(num+i2+1)+i1) = 0;
                }
    return done;
}

_Bool Down(uint16_t (*num)[4], uint32_t *scores)
{
    _Bool done=0;
    for(uint8_t i0=0; i0<4; i0++)
        for(uint8_t i1=0; i1<4; i1++)
            for(uint8_t i2=3; i2>0; i2--)
                if(*(*(num+i2)+i1)==0 && *(*(num+i2-1)+i1)!=0)
                {
                    *(*(num+i2)+i1) = *(*(num+i2-1)+i1);
                    *(*(num+i2-1)+i1) = 0;
                    done = 1;
                }
    for(uint8_t i1=0; i1<4; i1++)
        for(uint8_t i2=3; i2>0; i2--)
            if(*(*(num+i2)+i1)==*(*(num+i2-1)+i1) && *(*(num+i2)+i1)!=0)
            {
                *(*(num+i2)+i1) *= 2;
                *scores += *(*(num+i2)+i1);
                *(*(num+i2-1)+i1) = 0;
                done = 1;
            }
    for(uint8_t i0=0; i0<4; i0++)
        for(uint8_t i1=0; i1<4; i1++)
            for(uint8_t i2=3; i2>0; i2--)
                if(*(*(num+i2)+i1)==0 && *(*(num+i2-1)+i1)!=0)
                {
                    *(*(num+i2)+i1) = *(*(num+i2-1)+i1);
                    *(*(num+i2-1)+i1) = 0;
                }
    return done;
}

_Bool Left(uint16_t (*num)[4], uint32_t *scores)
{
    _Bool done=0;
    for(uint8_t i0=0; i0<4; i0++)
        for(uint8_t i1=0; i1<4; i1++)
            for(uint8_t i2=0; i2<3; i2++)
                if(*(*(num+i1)+i2)==0 && *(*(num+i1)+i2+1)!=0)
                {
                    *(*(num+i1)+i2) = *(*(num+i1)+i2+1);
                    *(*(num+i1)+i2+1) = 0;
                    done = 1;
                }
    for(uint8_t i1=0; i1<4; i1++)
        for(uint8_t i2=0; i2<3; i2++)
            if(*(*(num+i1)+i2)==*(*(num+i1)+i2+1) && *(*(num+i1)+i2)!=0)
            {
                *(*(num+i1)+i2) *= 2;
                *scores += *(*(num+i2)+i1);
                *(*(num+i1)+i2+1) = 0;
                done = 1;
            }
    for(uint8_t i0=0; i0<4; i0++)
        for(uint8_t i1=0; i1<4; i1++)
            for(uint8_t i2=0; i2<3; i2++)
                if(*(*(num+i1)+i2)==0 && *(*(num+i1)+i2+1)!=0)
                {
                    *(*(num+i1)+i2) = *(*(num+i1)+i2+1);
                    *(*(num+i1)+i2+1) = 0;
                }
    return done;
}

_Bool Right(uint16_t (*num)[4], uint32_t *scores)
{
    _Bool done=0;
    for(uint8_t i0=0; i0<4; i0++)
        for(uint8_t i1=0; i1<4; i1++)
            for(uint8_t i2=3; i2>0; i2--)
                if(*(*(num+i1)+i2)==0 && *(*(num+i1)+i2-1)!=0)
                {
                    *(*(num+i1)+i2) = *(*(num+i1)+i2-1);
                    *(*(num+i1)+i2-1) = 0;
                    done = 1;
                }
    for(uint8_t i1=0; i1<4; i1++)
        for(uint8_t i2=3; i2>0; i2--)
            if(*(*(num+i1)+i2)==*(*(num+i1)+i2-1) && *(*(num+i1)+i2)!=0)
            {
                *(*(num+i1)+i2) *=2;
                *scores += *(*(num+i2)+i1);
                *(*(num+i1)+i2-1) = 0;
                done = 1;
            }
    for(uint8_t i0=0; i0<4; i0++)
        for(uint8_t i1=0; i1<4; i1++)
            for(uint8_t i2=3; i2>0; i2--)
                if(*(*(num+i1)+i2)==0 && *(*(num+i1)+i2-1)!=0)
                {
                    *(*(num+i1)+i2) = *(*(num+i1)+i2-1);
                    *(*(num+i1)+i2-1) = 0;
                }
    return done;
}

void Print(uint16_t (*num)[4], uint32_t *scores)
{
    for(uint8_t i0=0; i0<4; i0++)
    {
        for(uint8_t i1=0; i1<4; i1++)
            printf("%5d", *(*(num+i0)+i1));
        putchar('\n');
    }
    printf("Your scores: %d\n", *scores);
}

_Bool Save(uint16_t (*num)[4], uint32_t *scores)
{
    char *BlockStr = malloc(6*sizeof(char));
    char *out = malloc(4*sizeof(char));
    FILE *file;
    file = fopen("data1", "w");
    if(file == NULL)
    {
        free(BlockStr);
        free(out);
        puts("Error opening file");
        return 1;
    }
    for(uint8_t i0=0; i0<4; i0++)
    {
        for(uint8_t i1=0; i1<4; i1++)
        {
            sprintf(BlockStr, "%5d", *(*(num+i0)+i1));
            for(uint8_t i2=0; i2<5; i2++)
            {
                sprintf(out, "%3d", *(BlockStr+i2));
                fputs(out, file);
            }
        }
    }
    free(BlockStr);
    fclose(file);
    file = fopen("data0", "w");
    if(file == NULL)
    {
        free(out);
        puts("Error opening file");
        return 1;
    }
    char *ScoresStr = malloc(8* sizeof(char));
    sprintf(ScoresStr, "%7d", *scores);
    for(uint8_t i0=0; i0<7; i0++)
    {
        sprintf(out, "%3d" ,*(ScoresStr+i0));
        fputs(out, file);
    }
    fclose(file);
    free(out);
    free(ScoresStr);
    return 0;
}

uint32_t Highest(_Bool look)
{
    FILE *file;
    if(!(file = fopen("data2", "r")))
    {
        puts("Not existed!");
        return 0;
    }
    char c;
    char *in = malloc(4 * sizeof(char));
    uint32_t highest;
    char *NumStr = malloc(8 * sizeof(char));
    fgets(NumStr, 8, file);
    highest = atoi(NumStr);
    if(!highest || highest%2)
    {
        fopen("data2", "w");
        puts("No highest record!");
        return 0;
    }
    if(look)
    {
        printf("%d", highest);
        return 0;
    }
    else
    {
        return highest;
    }
}