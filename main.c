#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

struct Item
{
    char name[50];
    int attack;
    int defence;
};

struct Player
{
    char name[50];
    int maxHealth;
    int health;
    int level;
    int experience;
    int difficulty;
    int attack;
    int defence;
    struct Item itemOffensive;
    struct Item itemDefensive;
};

struct Enemy
{
    char name[50];
    int maxHealth;
    int health;
    int level;
    int attack;
    int defence;
};

void mainMenu();
void newGame();
void loadGame();
void gameMenu();
void saveGame();
void printPlayerStats();
void printEnemyStats();
void printPrologue();
void randomEvent();
void eventNothing();
void eventItem();
void eventEnemy();
void eventBoss();
void battleTurn();
void gainExperience();
void endGame();
void gameOver();

int main(int argc, char* argv[])
{
    setlocale(LC_CTYPE, ".1250");

    //wyswietlenie menu g��wnego
    mainMenu(argc, argv);

    return 0;
}

void mainMenu(int argc, char** argv[])
{
    //inicializacja generatora liczb losowych
    srand(time(NULL));

    //alokacja struktury gracza i przeciwnika
    struct Player* player;
    player = (struct Player*)malloc(sizeof(struct Player));
    struct Enemy* enemy;
    enemy = (struct Enemy*)malloc(sizeof(struct Enemy));

    int choose;
    do
    {
        printf("\n");
        printf("      ____________________  ________ \n");
        printf("  ____\\______   \\______   \\/  _____/ \n");
        printf("_/ ___\\|       _/|     ___/   \\  ___ \n");
        printf("\\  \\___|    |   \\|    |   \\    \\_\\  \\\n");
        printf(" \\___  >____|_  /|____|    \\______  /\n");
        printf("     \\/       \\/                  \\/ \n");
        printf("\n");
        printf("1. Nowa gra\n");
        printf("2. Wczytaj gr�\n");
        printf("9. Wyjd� z gry\n");
        scanf("%d", &choose);

        switch (choose)
        {
        case 1:
            newGame(&player, &enemy);
            break;
        case 2:
            loadGame(&player, &enemy, argc, argv);
            break;
        case 9:
            break;
        default:
            break;
        }
    } while (choose != 9);
}

void newGame(struct Player** player, struct Enemy** enemy)
{
    printf("\n");
    printf("Podaj nazw� gracza:\n");
    scanf("%127s", (*player)->name);

    printf("\n");
    do
    {
        printf("Wybierz poziom trudno�ci:\n");
        printf("1. �atwy\n");
        printf("2. Normalny\n");
        printf("3. Trudny\n");
        scanf("%d", &(*player)->difficulty);
    } while ((*player)->difficulty != 1 && (*player)->difficulty != 2 && (*player)->difficulty != 3);

    (*player)->attack = 5;
    (*player)->defence = 5;
    (*player)->maxHealth = (*player)->health = 100;
    (*player)->level = 1;
    (*player)->experience = 0;

    strcpy((*player)->itemOffensive.name, "");
    (*player)->itemOffensive.attack = 0;
    (*player)->itemOffensive.defence = 0;

    strcpy((*player)->itemDefensive.name, "");
    (*player)->itemDefensive.attack = 0;
    (*player)->itemDefensive.defence = 0;

    printPrologue();
    gameMenu(player, enemy);
}

void loadGame(struct Player** player, struct Enemy** enemy, int argc, char** argv[])
{
    char filePath[128];

    //sprawdzenie czy przy uruchamianiu programu podany zosta� jaki� argument
    if (argc > 1) 
    {
        strcpy(filePath, argv[1]);
    }
    else 
    {
        printf("Podaj �cie�k� do pliku z zapisanym stanem gry (standatdowo savegame.csv): \n");
        scanf("%127s", &filePath);
    }

    FILE* file;
    file = fopen(filePath, "r");

    if (!file)
    {
        printf("Podany plik nie istnieje!\n");
    }
    else
    {
        char line[1024];
        fgets(line, 1024, file);

        char* sub;

        //wczytywanie informacji o graczu
        sub = strtok(line, ",");
        strcpy((*player)->name, sub);
        sub = strtok(NULL, ",");
        (*player)->health = atoi(sub);
        sub = strtok(NULL, ",");
        (*player)->maxHealth = atoi(sub);
        sub = strtok(NULL, ",");
        (*player)->level = atoi(sub);
        sub = strtok(NULL, ",");
        (*player)->experience = atoi(sub);
        sub = strtok(NULL, ",");
        (*player)->attack = atoi(sub);
        sub = strtok(NULL, ",");
        (*player)->defence = atoi(sub);
        sub = strtok(NULL, ",");
        (*player)->difficulty = atoi(sub);

        //wczytywanie informacji o przedmiocie ofensywnym
        sub = strtok(NULL, ",");
        strcpy((*player)->itemOffensive.name, sub);
        sub = strtok(NULL, ",");
        (*player)->itemOffensive.attack = atoi(sub);
        sub = strtok(NULL, ",");
        (*player)->itemOffensive.defence = atoi(sub);

        //wczytywanie informacji o przedmiocie defensywnym
        sub = strtok(NULL, ",");
        strcpy((*player)->itemDefensive.name, sub);
        sub = strtok(NULL, ",");
        (*player)->itemDefensive.attack = atoi(sub);
        sub = strtok(NULL, ",");
        (*player)->itemDefensive.defence = atoi(sub);

        gameMenu(player, enemy);
    }
}

void gameMenu(struct Player** player, struct Enemy** enemy)
{
    int choose;

    do
    {
        printf("\n");
        printPlayerStats(player);
        printf("\n");
        printf("Co chcesz teraz zrobi�?\n");
        printf("1. Id� dalej\n");
        printf("2. Zapisz gr�\n");
        printf("9. Wyjd� do menu\n");
        scanf("%d", &choose);

        switch (choose)
        {
        case 1:
            randomEvent(player, enemy);
            break;
        case 2:
            saveGame(player);
            break;
        case 9:
            break;
        default:
            break;
        }
    } while (choose != 9);
}

void randomEvent(struct Player** player, struct Enemy** enemy)
{
    int choose;
    int r = rand() % 10;
    switch (r)
    {
    case 0:
    case 1:
        eventNothing();
        break;
    case 2:
    case 3:
        eventItem(player);
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        eventEnemy(player, enemy);
        break;
    case 9:
        do
        {
            printf("\n");
            printf("Na horyzoncie majaczy ogromna posta�. Wygl�da na trudnego przeciwnika. Czy czujesz si� gotowy, aby si� z nim zmierzy�?\n");
            printf("1. Tak\n");
            printf("2. Nie\n");
            scanf("%d", &choose);
        } while (choose != 1 && choose != 2);
        if (choose == 1)
        {
            eventBoss(player, enemy);
        }
        else
        {
            printf("Oddalasz si� od wroga.");
        }
        break;
    }
}

void eventNothing()
{
    int r = rand() % 10; //liczba wymy�lonych zdarze�
    switch (r)
    {
    case 0:
        printf("\n");
        printf("Nic si� nie sta�o.");
        printf("\n");
        printf("Wci�nij dowolny klawisz, aby kontynuowa�...");
        getch();
        break;
    case 1:
        printf("\n");
        printf("Te� si� nie sta�o.");
        printf("\n");
        printf("Wci�nij dowolny klawisz, aby kontynuowa�...");
        getch();
        break;
    case 2:
        printf("\n");
        printf("Jeszcze bardziej nic si� nie sta�o.");
        printf("\n");
        printf("Wci�nij dowolny klawisz, aby kontynuowa�...");
        getch();
        break;
    }
}

void eventItem(struct Player** player)
{
    int choose, i;
    int r = rand() % 6; //liczba przedmiot�w do wylosowania

    switch (r)
    {
    case 0:
        //sztylet
        i = (rand() % 10) + 1; //wylosuj statystyk�
        do
        {
            printf("\n");
            printf("Znajdujesz sztylet +%d ataku. Czy chcesz go podnie��?\n", i);
            printf("1. Tak\n");
            printf("2. Nie\n");
            scanf("%d", &choose);
        } while (choose != 1 && choose != 2);

        if (choose == 1)
        {
            strcpy((*player)->itemOffensive.name, "Sztylet");
            (*player)->itemOffensive.attack = i;
        }
        break;
    case 1:
        //miecz
        i = (rand() % 10) + 6; //wylosuj statystyk�
        do
        {
            printf("\n");
            printf("Znajdujesz miecz +%d ataku. Czy chcesz go podnie��?\n", i);
            printf("1. Tak\n");
            printf("2. Nie\n");
            scanf("%d", &choose);
        } while (choose != 1 && choose != 2);

        if (choose == 1)
        {
            strcpy((*player)->itemOffensive.name, "Miecz");
            (*player)->itemOffensive.attack = i;
        }
        break;
    case 2:
        //top�r
        i = (rand() % 10) + 11; //wylosuj statystyk�
        do
        {
            printf("\n");
            printf("Znajdujesz top�r +%d ataku. Czy chcesz go podnie��?\n", i);
            printf("1. Tak\n");
            printf("2. Nie\n");
            scanf("%d", &choose);
        } while (choose != 1 && choose != 2);

        if (choose == 1)
        {
            strcpy((*player)->itemOffensive.name, "Top�r");
            (*player)->itemOffensive.attack = i;
        }
        break;
    case 3:
        //tarcza
        i = (rand() % 10) + 1; //wylosuj statystyk�
        do
        {
            printf("\n");
            printf("Znajdujesz tarcz� +%d obrony. Czy chcesz j� podnie��?\n", i);
            printf("1. Tak\n");
            printf("2. Nie\n");
            scanf("%d", &choose);
        } while (choose != 1 && choose != 2);

        if (choose == 1)
        {
            strcpy((*player)->itemDefensive.name, "Tarcza");
            (*player)->itemDefensive.defence = i;
        }
        break;
    case 4:
        //amulet
        i = (rand() % 10) + 6; //wylosuj statystyk�
        do
        {
            printf("\n");
            printf("Znajdujesz amulet +%d obrony. Czy chcesz go podnie��?\n", i);
            printf("1. Tak\n");
            printf("2. Nie\n");
            scanf("%d", &choose);
        } while (choose != 1 && choose != 2);

        if (choose == 1)
        {
            strcpy((*player)->itemDefensive.name, "Amulet");
            (*player)->itemDefensive.defence = i;
        }
        break;
    case 5:
        //zbroja
        i = (rand() % 10) + 11; //wylosuj statystyk�
        do
        {
            printf("\n");
            printf("Znajdujesz zbroj� +%d obrony. Czy chcesz j� podnie��?\n", i);
            printf("1. Tak\n");
            printf("2. Nie\n");
            scanf("%d", &choose);
        } while (choose != 1 && choose != 2);

        if (choose == 1)
        {
            strcpy((*player)->itemDefensive.name, "Zbroja");
            (*player)->itemDefensive.defence = i;
        }
        break;
    }
}

void eventEnemy(struct Player** player, struct Enemy** enemy)
{
    int choose, i, j, k, playerLevel;
    int r = rand() % 3; //liczba przeciwnik�w do wylosowania

    switch (r)
    {
    case 0:
        //szczur
        playerLevel = (*player)->level;
        switch ((*player)->difficulty)
        {
        case 1:
            i = (rand() % 2) + 1; //wylosuj statystyk� ataku
            j = (rand() % 2) + 1; //wylosuj statystyk� obrony
            break;

        case 2:
            i = (rand() % 2) + 3; //wylosuj statystyk� ataku
            j = (rand() % 2) + 3; //wylosuj statystyk� obrony
            break;

        case 3:
            i = (rand() % 2) + 5; //wylosuj statystyk� ataku
            j = (rand() % 2) + 5; //wylosuj statystyk� obrony
            break;
        }
        k = (rand() % 3) + playerLevel;
        strcpy((*enemy)->name, "Szczur");
        (*enemy)->attack = i;
        (*enemy)->defence = j;
        (*enemy)->health = 10;
        (*enemy)->maxHealth = 10;
        (*enemy)->level = k;

        printf("\n");
        printf("Na swojej drodze spotykasz szczura, kt�ry chce walczy�!\n", i);

        do
        {
            printPlayerStats(player);
            printEnemyStats(enemy);
            printf("\n");
            printf("Co chcesz zrobi�?\n");
            printf("1. Atakuj\n");
            printf("2. Uciekaj\n");
            scanf("%d", &choose);

            switch (choose)
            {
            case 1:
                battleTurn(player, enemy, 1);
                break;
            case 2:
                battleTurn(player, enemy, 2);
                break;
            }
        } while ((*enemy)->health > 0);

        printf("Szczur pokonany!\n");
        gainExperience(player, enemy);
        break;

    case 1:
        //wilk
        playerLevel = (*player)->level;
        switch ((*player)->difficulty)
        {
        case 1:
            i = (rand() % 2) + 3; //wylosuj statystyk� ataku
            j = (rand() % 2) + 4; //wylosuj statystyk� obrony
            break;

        case 2:
            i = (rand() % 3) + 5; //wylosuj statystyk� ataku
            j = (rand() % 3) + 4; //wylosuj statystyk� obrony
            break;

        case 3:
            i = (rand() % 3) + 5; //wylosuj statystyk� ataku
            j = (rand() % 3) + 4; //wylosuj statystyk� obrony
            break;
        }

        k = (rand() % 3) + playerLevel;
        strcpy((*enemy)->name, "Wilk");
        (*enemy)->attack = i;
        (*enemy)->defence = j;
        (*enemy)->health = 15;
        (*enemy)->maxHealth = 15;
        (*enemy)->level = k;

        printf("\n");
        printf("Na swojej drodze spotykasz wilka, kt�ry chce walczy�!\n", i);

        do
        {
            printPlayerStats(player);
            printEnemyStats(enemy);
            printf("\n");
            printf("Co chcesz zrobi�?\n");
            printf("1. Atakuj\n");
            printf("2. Uciekaj\n");
            scanf("%d", &choose);

            switch (choose)
            {
            case 1:
                battleTurn(player, enemy, 1);
                break;
            case 2:
                battleTurn(player, enemy, 2);
                break;
            }
        } while ((*enemy)->health > 0);

        printf("Wilk pokonany!\n");
        gainExperience(player, enemy);
        break;

    case 2:
        //szczur
        playerLevel = (*player)->level;
        switch ((*player)->difficulty)
        {
        case 1:
            i = (rand() % 3) + 2; //wylosuj statystyk� ataku
            j = (rand() % 3) + 3; //wylosuj statystyk� obrony
            break;

        case 2:
            i = (rand() % 3) + 3; //wylosuj statystyk� ataku
            j = (rand() % 3) + 3; //wylosuj statystyk� obrony
            break;

        case 3:
            i = (rand() % 4) + 3; //wylosuj statystyk� ataku
            j = (rand() % 4) + 3; //wylosuj statystyk� obrony
            break;
        }

        k = (rand() % 3) + playerLevel;
        strcpy((*enemy)->name, "Goblin");
        (*enemy)->attack = i;
        (*enemy)->defence = j;
        (*enemy)->health = 20;
        (*enemy)->maxHealth = 20;
        (*enemy)->level = k;

        printf("\n");
        printf("Na swojej drodze spotykasz goblina, kt�ry chce walczy�!\n");

        do
        {
            printPlayerStats(player);
            printEnemyStats(enemy);
            printf("\n");
            printf("Co chcesz zrobi�?\n");
            printf("1. Atakuj\n");
            printf("2. Uciekaj\n");
            scanf("%d", &choose);

            switch (choose)
            {
            case 1:
                battleTurn(player, enemy, 1);
                break;
            case 2:
                battleTurn(player, enemy, 2);
                break;
            }
        } while ((*enemy)->health > 0);

        printf("Goblin pokonany!\n");
        gainExperience(player, enemy);
        break;
    }
}

void eventBoss(struct Player** player, struct Enemy** enemy)
{
    int choose, i, j, k;
    int r = rand() % 1; //liczba przeciwnik�w do wylosowania

    switch (r)
    {
    case 0:
        strcpy((*enemy)->name, "Ogr");
        (*enemy)->attack = 10;
        (*enemy)->defence = 10;
        (*enemy)->health = 100;
        (*enemy)->maxHealth = 100;
        (*enemy)->level = 10;

        printf("\n");
        printf("Na swojej drodze spotykasz ogromnego ogra, kt�ry chce walczy�!\n");

        do
        {
            printPlayerStats(player);
            printEnemyStats(enemy);
            printf("\n");
            printf("Co chcesz zrobi�?\n");
            printf("1. Atakuj\n");
            printf("2. Uciekaj\n");
            scanf("%d", &choose);

            switch (choose)
            {
            case 1:
                battleTurn(player, enemy, 1);
                break;
            case 2:
                battleTurn(player, enemy, 2);
                break;
            }
        } while ((*enemy)->health > 0);

        printf("Ogr pokonany!\n");
        endGame();

        break;
    }
}

void battleTurn(struct Player** player, struct Enemy** enemy, int move)
{
    int r, i, playerAttack;
    int playerDamage = 0;

    switch (move)
    {
    case 1:
        i = rand() % 10; //losowanie odchy�u mocy ataku
        switch (i)
        {
        case 0:
            playerAttack = (*player)->attack + ((*player)->attack * 0.3 + (*player)->itemOffensive.attack);
            break;
        case 1:
        case 2:
            playerAttack = (*player)->attack + ((*player)->attack * 0.2 + (*player)->itemOffensive.attack);
            break;
        case 3:
        case 4:
        case 5:
            playerAttack = (*player)->attack + ((*player)->attack * 0.1 + (*player)->itemOffensive.attack);
            break;
        case 6:
        case 7:
        case 8:
        case 9:
            playerAttack = (*player)->attack + (*player)->itemOffensive.attack;
            break;
        }

        if (playerDamage < 0)
        {
            playerDamage = 0;
        }

        r = rand() % 10; //szansa na uderzenie krytyczne
        if (r == 0)
        {
            playerDamage = (2 * playerAttack) - (*enemy)->defence;
        }
        else
        {
            playerDamage = playerAttack - (*enemy)->defence;
        }

        (*enemy)->health -= playerDamage;
        printf("Zadajesz %d obra�e�!\n", playerDamage);

        if ((*enemy)->health > 0)
        {
            int enemyAttack = 0;

            enemyAttack = (*enemy)->attack - ((*player)->defence + (*player)->itemDefensive.defence);

            if (enemyAttack < 0)
            {
                enemyAttack = 0;
            }

            (*player)->health -= enemyAttack;
            printf("Otrzymujesz %d obra�e�!\n", enemyAttack);

            if ((*player)->health <= 0)
            {
                gameOver();
            }
        }
        break;
    case 2:
        printf("Uda�o ci si� uciec!\n");
        gameMenu(player, enemy);
        break;
    }
}

void gainExperience(struct Player** player, struct Enemy** enemy)
{
    int r = rand() % 3;
    int gainedExperience = 0;

    switch (r)
    {
    case 0:
        gainedExperience += (*enemy)->level * 3;
        break;
    case 1:
        gainedExperience += (*enemy)->level * 2;
        break;
    case 2:
        gainedExperience += (*enemy)->level * 3;
        break;
    }

    printf("Otrzymujesz %d punkt�w doswiadczenia!\n", gainedExperience);
    (*player)->experience += gainedExperience;

    int lvlUps = 0;

    for ((*player)->experience; (*player)->experience >= 10; (*player)->experience -= 10)
    {
        lvlUps++;
    }

    if (lvlUps != 0)
    {
        (*player)->level += lvlUps;
        (*player)->maxHealth += lvlUps * 10;
        (*player)->health += lvlUps * 10;
        (*player)->attack += lvlUps;
        (*player)->defence += lvlUps;
        printf("Zdobywasz poziom!\n");
    }
}

void endGame()
{
    printf("Pokona�e� g��wnego wroga! Gra zako�czona.");
    getch();
    mainMenu(NULL, NULL);
}

void gameOver()
{
    printf("Niestety poleg�e� w walce. Wcisniej dowolny klawisz, aby wr�ci� do menu...");
    getch();
    mainMenu(NULL, NULL);
}

void saveGame(struct Player** player)
{
    FILE* file;
    file = fopen("savegame.csv", "w+");
    fprintf(file,
        "%s, %d, %d, %d, %d, %d, %d, %d, %s, %d, %d, %s, %d, %d",
        (*player)->name,
        (*player)->health,
        (*player)->maxHealth,
        (*player)->level,
        (*player)->experience,
        (*player)->attack,
        (*player)->defence,
        (*player)->difficulty,
        (*player)->itemOffensive.name,
        (*player)->itemOffensive.attack,
        (*player)->itemOffensive.defence,
        (*player)->itemDefensive.name,
        (*player)->itemDefensive.attack,
        (*player)->itemDefensive.defence);
    fclose(file);
}

void printPlayerStats(struct Player** player)
{
    printf("\n");
    printf("%s: %d/%d\n", (*player)->name, (*player)->health, (*player)->maxHealth);
    printf("Poziom: %d\n", (*player)->level);
    printf("Do�wiadczenie: %d\n", (*player)->experience);
    printf("Atak: %d\n", (*player)->attack);
    printf("Obrona: %d\n\n", (*player)->defence);

    printf("Przedmiot ofensywny\n");
    if ((*player)->itemOffensive.attack != 0)
    {
        printf((*player)->itemOffensive.name);
        printf("\n");
        printf("+%d ataku\n", (*player)->itemOffensive.attack);
    }
    else
    {
        printf("--brak--\n");
    }

    printf("Przedmiot defensywny\n");
    if ((*player)->itemDefensive.defence != 0)
    {
        printf((*player)->itemDefensive.name);
        printf("\n");
        printf("+%d obrony\n", (*player)->itemDefensive.defence);
    }
    else
    {
        printf("--brak--\n");
    }
}

void printEnemyStats(struct Enemy** enemy)
{
    printf("\n");
    printf("%s: %d/%d\n", (*enemy)->name, (*enemy)->health, (*enemy)->maxHealth);
    printf("Poziom: %d\n", (*enemy)->level);
    printf("Atak: %d\n", (*enemy)->attack);
    printf("Obrona: %d\n", (*enemy)->defence);
}

void printPrologue()
{
    printf("\n");
    printf("Za g�rami za lasami bohater postanowi� wej�� do starego zamku zwanego SESJA\n");
    printf("Ciekawe co tam b�dzie na niego czycha�o....\n");
    printf("\n");
    printf("Wci�nij dowolny klawisz, aby kontynuowa�...");
    getch();
}