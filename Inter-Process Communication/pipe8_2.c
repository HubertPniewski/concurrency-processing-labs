#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#define ODCZYT 0
#define ZAPIS 1
#define BUFOR_SIZE 256


int main ()
{
    int potok1 [2];  // 1 -> 2
    int potok2 [2];  // 2 -> 3

    pipe (potok1);
    pipe (potok2);

    if (fork())  // II proces
    {
        puts("Proces 1: start.");

        close(potok1[ODCZYT]);
        close(potok2[ODCZYT]);
        close(potok2[ZAPIS]);

        char tekst[BUFOR_SIZE];
        printf("Proces 1: Podaj ciag znakow: \n");
        fgets(tekst, BUFOR_SIZE, stdin);

        write(potok1[ZAPIS], tekst, strlen(tekst) + 1);
        close(potok1[ZAPIS]);

        wait(NULL);
        wait(NULL);
        puts("Proces 1: koniec.");
    }
    else
    {
        if (fork()) // III proces
        {
            puts("Proces 2: start.");

            close(potok1[ZAPIS]);
            close(potok2[ODCZYT]);

            char bufor[BUFOR_SIZE];

            read(potok1[ODCZYT], bufor, BUFOR_SIZE);
            close(potok1[ODCZYT]);

            // Duże
            for (int i = 0; bufor[i] != '\0'; i++) {
                bufor[i] = toupper(bufor[i]);
            }

            write(potok2[ZAPIS], bufor, BUFOR_SIZE);
            close(potok2[ZAPIS]);

            puts("Proces 2: koniec.");
        } else {
            puts("Proces 3: start.");
            close(potok1[ODCZYT]);
            close(potok1[ZAPIS]);
            close(potok2[ZAPIS]);

            char result[BUFOR_SIZE];

            // Czytamy to, co przysłał Proces 2
            read(potok2[ODCZYT], result, BUFOR_SIZE);
            close(potok2[ODCZYT]);

            printf("Proces 3: %s", result);

            puts("Proces 3: koniec.");
        }
    }

    return 0;
}