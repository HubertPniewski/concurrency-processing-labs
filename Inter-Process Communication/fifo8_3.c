#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#define ODCZYT 0
#define ZAPIS 1
#define BUFOR_SIZE 256


int main ()
{
    int file1;
    int file2;

    int fifo1 = mkfifo("fifo1", 0666);
    int fifo2 = mkfifo("fifo2", 0666);

    if (fork())  // II proces
    {
        puts("Proces 1: start.");

        file1 = open("fifo1", O_WRONLY);


        char tekst[BUFOR_SIZE];
        printf("Proces 1: Podaj ciag znakow: \n");
        fgets(tekst, BUFOR_SIZE, stdin);

        write(file1, tekst, strlen(tekst) + 1);
        close(file1);

        wait(NULL);
        wait(NULL);
        puts("Proces 1: koniec.");
    }
    else
    {
        if (fork()) // III proces
        {
            puts("Proces 2: start.");

            char bufor[BUFOR_SIZE];

            file1 = open("fifo1", O_RDONLY);
            read(file1, bufor, BUFOR_SIZE);
            close(file1);

            // Duże
            for (int i = 0; bufor[i] != '\0'; i++) {
                bufor[i] = toupper(bufor[i]);
            }

            file2 = open("fifo2", O_WRONLY);
            write(file2, bufor, BUFOR_SIZE);
            close(file2);

            puts("Proces 2: koniec.");
        } else {
            puts("Proces 3: start.");

            file2 = open("fifo2", O_RDONLY);

            char result[BUFOR_SIZE];

            // Czytamy to, co przysłał Proces 2
            read(file2, result, BUFOR_SIZE);
            close(file2);

            printf("Proces 3: %s", result);

            puts("Proces 3: koniec.");
        }
    }

    return 0;
}