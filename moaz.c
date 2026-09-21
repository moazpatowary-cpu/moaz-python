#include<stdio.h>
int main()
{
    char name[100];
    printf("enter your name: ");
    scanf("%99s", name);

    if (name[0] == '\0') {
        printf("name cannot be invalid");
    }

    printf("hi %s how you doing?\n", name);
    return 0;
}