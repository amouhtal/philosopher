#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

void ft(int*i)
{
    puts("Her");
    *i = 25;
}

void fs(char *str)
{
    puts("Her");
    str = "D";
}

int main()
{
    char *str;
    int *i;


    *i = 965555;
    str= "c";

    printf("%d\n", *i);
    ft(i);
    printf("%d\n", *i);
    printf("%s\n", str);
    fs(str);
    printf("%s\n", str);

}