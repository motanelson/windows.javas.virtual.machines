#include <windows.h>  // For MessageBox API
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 256
#define LOCALS_SIZE 256

int stack[STACK_SIZE];
int sp = 0;

int locals[LOCALS_SIZE];

void push(int x)
{
    int s=0;
    s=stack[sp];
    printf("#push %d\n",s);
    stack[sp] = x;
    ++sp;
}

int pop()
{ 
    int s=0;
    s=stack[--sp];
    printf("#pop %d\n",s);

    return s;
}

void iadd()
{
    int b = pop();
    int a = pop();
    push(a + b);
}

void isub()
{
    int b = pop();
    int a = pop();
    push(a - b);
}

void imul()
{
    int b = pop();
    int a = pop();
    push(a * b);
}

void idiv()
{
    int b = pop();
    int a = pop();

    push(a / b);
}

void invokevirtual(char *line)
{
    //printf("%s\n",line);
    int result =0;
    char ccc[256];
    if (strstr(line, "PrintStream.println"))
    {
        sprintf(ccc,"%d", pop());
        int result = MessageBox(
        NULL,                       // No owner window
        ccc,        // Message text
        ccc, // Title
        MB_OKCANCEL | MB_ICONINFORMATION // Buttons + icon
    );

    }
}

void execute(char code[][256], int size)
{
    int pc;

    for (pc = 0; pc < size; pc++)
    {
        char line[256];

        strcpy(line, code[pc]);

        if (strstr(line, ";"))
            line[strlen(line)-1] = 0;
        
        //-------------------------------------

        if (strstr(line, "iconst_0")){
            printf("%s\n",line);
            push(0);}

        else if (strstr(line, "iconst_1")){
            printf("%s\n",line);
            push(1);}

        else if (strstr(line, "iconst_2")){
            printf("%s\n",line);
            push(2);}

        else if (strstr(line, "iconst_3")){
            printf("%s\n",line);
            push(3);}

        else if (strstr(line, "iconst_4")){
            printf("%s\n",line);
            push(4);}

        else if (strstr(line, "iconst_5")){
            printf("%s\n",line);
            push(5);}

        //-------------------------------------

        else if (strstr(line, "bipush"))
        {
            int x;
            printf("%s\n",line);
            char *p;

            p = strstr(line,"bipush");

            p += strlen("bipush");

            int value = atoi(p);


            
            
            push(value);
        }

        //-------------------------------------

        else if (strstr(line, "iload_"))
        {
            
            printf("%s\n",line);
            char *p;

            p = strstr(line,"iload_");

            p += strlen("iload_");

            int n = atoi(p);

            

            push(locals[n]);

        }

        //-------------------------------------

        else if (strstr(line, "istore_"))
        {
            
            printf("%s\n",line);
            char *p;

            p = strstr(line,"istore_");

            p += strlen("istore_");

            int n = atoi(p);

    

            locals[n]=pop();
        }

        //-------------------------------------

        else if (strstr(line, "iadd")){
            printf("%s\n",line);
            iadd();}

        else if (strstr(line, "isub")){
            printf("%s\n",line);
            isub();}

        else if (strstr(line, "imul")){
            printf("%s\n",line);
            imul();}

        else if (strstr(line, "idiv")){
            printf("%s\n",line);
            idiv();}

        //-------------------------------------

        else if (strstr(line, "invokevirtual"))
        {
            
            invokevirtual(line);
        }

        //-------------------------------------

        else if (strstr(line, "return"))
        {
            printf("%s\n",line);
            printf("\nprogram finished.\n");
            return;
        }
    }
}


int main()
{
    char filename[256];
    char classname[256];
    char command[512];
    char jasmname[256];
    printf("\033c\033[47;30m\n");
    printf("give me file .jasm ?\n\n");

    scanf("%s", jasmname);

    

    
    FILE *f = fopen(jasmname, "r");

    if (!f)
    {
        printf("cannot open jasm file\n");
        return 1;
    }

    char line[256];

    char code[1024][256];
    int count = 0;
    int a=0;
    int inside_main = 0;

    while (fgets(line, sizeof(line), f))
    {
        if (strstr(line, "main"))
        {
            inside_main = 1;
            continue;
        }

        if (inside_main)
        {
            if (strstr(line, "}"))
                break;

            if (strstr(line, "{"))
                continue;

            if (strstr(line, "stack"))
                continue;

            if (strlen(line) < 3)
                continue;

            line[strcspn(line, "\n")] = 0;

            strcpy(code[count++], line);
        }
    }

    fclose(f);
    sp=0;
    for(a=0;a<256;a++)locals[a]=0;
    for(a=0;a<256;a++)stack[a]=0;

    
    printf("\n----- EXECUTION -----\n\n");

    execute(code, count);

    return 0;
}
