#include <stdio.h>
#include <stdlib.h>

// Basic settings
#define MAX_LENGTH                  100                 // Maximalni pocet znaku pro vstup a pro radku seznamu
#define CONTACT_LIST_MAX_LENGTH     50                  // Maximalni pocet kontaktu ve seznamu

// Kostanty
#define CASE_CHANGE_NUM             32                  // Konstanta pro zmenu velikosti pismen
#define LETTER_TO_NUM               97                  // Konstanta pro prevod pismen na jich poradi v abecede

// ADV. Settings
//#define CHECK_WITH_ERRORS                               // Define pro funkci ignorovani input chyb (Priority funkcionality 3.)
#ifdef CHECK_WITH_ERRORS
    #define NUMBER_OF_ERRORS_IN_INPUT   1               // Pocet chybnych mezer mezi dvema spravnymi znaky
#endif

// Konsolove barvy
#define ERROR_MESSAGE_COLOR     "\x1b[31m"               // Barva chyboveho hlaseni
#define COLOR_RESET             "\x1b[0m"                // Reset barvy

/**
 * @brief Struktura pro uchovani kontaktu
 *
 * @var filled - Zda je kontakt vyplnen, a nebo uz overen a musi byt vypsan
 * @var name - Jmeno kontaktu
 * @var phoneNumber - Telefonni cislo kontaktu
 */
struct contact {
    int filled;
    char name[MAX_LENGTH + 1];
    char phoneNumber[MAX_LENGTH + 1];
};


void error(int exitCode, char *desc)
{
    printf("%sError: %s!%s", ERROR_MESSAGE_COLOR, desc, COLOR_RESET);
    exit(exitCode);
}

/**
 *
 * @param input - String pro ověření
 * @param inputNum - Zadané vchodné číslo
 * @return
 * 1 - kontakt stačí
 * 0 - kontakt nestačí
 */
int checkContactPriorityOne(const char *str1, const char *str2)     // Str1 se musi zacina ze str2 (obsahuje 3. funkcionalitu)
{
    int found = 0;                  // Flag jestli bylo najdeno str2 ve str1, 1 - str2 je ve str1, 0 - ne
    int k = 0;
    int i = k;
    int j = 0;
    #ifdef CHECK_WITH_ERRORS
    int errors = 0;                 // Pocet chbnych useku
    int errorsStack = 0;            // Pocet znaku mezi dvema spravni znaky
    int prev_j = -1;                // Index znáku po 1. chybne mezere
    int correctInLine = 0;          // Pocet spravnych znaku v radku
    #endif
    int was_broke;                  // Flag který se ukazuje jestli cyklus byl zrušen, -1 - ne byl zrušen, 1 - byl zrušen
    while (str1[i] != '\0' && str2[j] != '\0')
    {
        was_broke = -1;
        if (str1[i] != str2[j])
        {
            #ifdef CHECK_WITH_ERRORS
            if (i == k)                                   // jestli prvni znaky str1 a str2 se nerovna
            {
                was_broke = 1;
                break;
            }
            if (errors < NUMBER_OF_ERRORS_IN_INPUT)       // Pocitame cislo chyb v mezere mezi dvema simboly
            {
                errorsStack++;
                correctInLine = 0;
                j--;
            }
            else if (errors == NUMBER_OF_ERRORS_IN_INPUT) // Jestli mame uz jednu chybnu mezeru musime znova zacat hledat znak, ktery byl po prve chybne mezere
            {
                errorsStack++;
                j = prev_j - 1;                           // Index znaky po chybne mezere
                i = i - correctInLine + 1;
                errors = 0;                               // Reset pocetu chybnych mezer
            }
            else                                          // Jeslti mame jine problemy
            {
                was_broke = 1;
                break;
            }
            #else
            was_broke = 1;                                // Rusime cyklus, jeslti mame chybu
            break;
            #endif
        }
        #ifdef CHECK_WITH_ERRORS
        else
        {
            correctInLine++;
            if (errorsStack > 0)                          // Znaky je spravne a overime jestli oni ma chybnu mezeru mezi sebou
            {
                errors++;
                prev_j = j;                               // Sybmbol po skonceni chybne mezery
                errorsStack = 0;                          // Reset pocetu chyb v mezere
            }
        }
        #endif
        i++;
        j++;
    }
    #ifdef CHECK_WITH_ERRORS
    if (errorsStack != 0)                                 // Jeslti mame chyby po skonceni cyklusu neni str2 ve str1
    {
        found = 0;                                        // 0 - neni str2 ve str1
    }
    else if (was_broke != 1 && str2[j] == '\0')           // Jeslti nemame chyb, nebyl cyklus zrusen a dosli jsme do posledniho znaku str2
    {
        found = 1;                                        // 1 - je str2 ve str1
    }
    #else
    if (was_broke != 1 && str2[j] == '\0')                // Jeslti nebyl cyklus zrusen a dosli jsme do posledniho znaku str2
    {
        found = 1;                                        // 1 - je str2 ve str1
    }
    #endif
    return found;                                         // Vratime jestli je str2 ve str1
}

int checkContactPriorityTwo(char const *str1, char const *str2)     // Dela tez same co i checkContactPriorityOne ale str1 musi jen obsahovat str2 (obsahuje 3. funkcionalitu)
{
    int found;                          // Flag jestli bylo najdeno str2 ve str1, 1 - str2 je ve str1, 0 - ne
    for (int k = 0; str1[k] != '\0'; k++)
    {
        found = 0;                      // Resetujeme flag
        int i = k;                      // Index pro prochazeni str1
        int j = 0;                      // Index pro prochazeni str2
        #ifdef CHECK_WITH_ERRORS
        int errors = 0;                 // Pocet chbnych useku
        int errorsStack = 0;            // Pocet znaku mezi dvema spravni znaky
        int prev_j = -1;                // Index znáku po 1. chybne mezere
        int correctInLine = 0;          // Pocet spravnych znaku v radku pred chybou
        #endif
        int was_broke;                  // Flag který se ukazuje jestli cyklus byl zrušen, -1 - ne byl zrušen, 1 - byl zrušen
        while (str1[i] != '\0' && str2[j] != '\0')
        {
            was_broke = -1;             // Resetujeme flag
            if (str1[i] != str2[j])
            {
                #ifdef CHECK_WITH_ERRORS
                if (i == k)                                     // jestli prvni znaky str1 a str2 se nerovna
                {
                    was_broke = 1;
                    break;
                }
                if (errors < NUMBER_OF_ERRORS_IN_INPUT)         // Pocitame cislo chyb v useku
                {
                    errorsStack++;
                    correctInLine = 0;
                    j--;
                }
                else if (errors == NUMBER_OF_ERRORS_IN_INPUT)   // Jestli mame uz jednu chybnu mezeru musime znova zacat hledat znak, ktery byl po prve chybne mezere
                {
                    errorsStack++;
                    j = prev_j - 1;                             // Index znaky po chybne mezere
                    i = i - correctInLine + 1;                  // Index se ktere zacneme hledat znak v str1
                    errors = 0;                                 // Reset pocetu chybnych mezer
                }
                else                                            // Jeslti mame jine problemy
                {
                    was_broke = 1;
                    break;
                }
                #else
                was_broke = 1;                                  // Rusime cyklus, jeslti mame chybu
                break;
                #endif
            }
            #ifdef CHECK_WITH_ERRORS
            else
            {
                correctInLine++;                                // Pocitame spravne znaky v radku
                if (errorsStack > 0)
                {
                    errors++;                                   // Pocitame chybne useky
                    prev_j = j;                                 // Index znaku po skonceni chybne mezery
                    errorsStack = 0;                            // Reset pocetu chyb v mezere
                }
            }
            #endif
            i++;
            j++;
        }
        #ifdef CHECK_WITH_ERRORS
        if (errorsStack != 0)                                   // Jeslti mame chyby po skonceni cyklusu neni str2 ve str1
        {
            found = 0;                                          // 0 - neni str2 ve str1
        }
        else if (was_broke != 1 && str2[j] == '\0')             // Jeslti nemame chyb, nebyl cyklus zrusen a dosli jsme do posledniho znaku str2
        {
            found = 1;                                          // 1 - je str2 ve str1
        }
        #else
        if (was_broke != 1 && str2[j] == '\0')                  // Jeslti nebyl cyklus zrusen a dosli jsme do posledniho znaku str2
        {
            found = 1;                                          // 1 - je str2 ve str1
            break;
        }
        #endif
    }
    return found;
}

// copy str from source to destination
void copyStrToStr(const char *from, char *to)
{
    int i = 0;
    while (from[i] != '\0' && from[i] != '\n')
    {
        to[i] = from[i];
        i++;
    }
    to[i] = '\0';
}

// str has char
int strHasChar(char const *str, char const c, int max_length)
{
    for (int i = 0; i < max_length; i++)
    {
        if (str[i] == c)
        {
            return 1;
        }
    }
    return 0;
}

// replace char in str
void replaceCharInStr(char *str, char const c, char const replace)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == c)
        {
            str[i] = replace;
        }
    }
}

// str contains only numbers
void checkContainsOnlyNumbers(char const *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            error(4, "Input have to be only numbers");
        }
    }
}

// reading list from stdin
int readContactList(struct contact *contactList)
{
    char buffer[MAX_LENGTH + 1];        // Buffer pro nacitani znaku
    int flag = 1;                       // Flag pro overeni jestli je nacteny radek jmene nebo telefonni cislo
    int i = 0;                          // Pocitadlo pro pocet nactenych kontaktu
    while (fgets(buffer, MAX_LENGTH+2, stdin) != NULL && i < CONTACT_LIST_MAX_LENGTH)
    {
        if (strHasChar(buffer, '\0', MAX_LENGTH+1) != 1)            // Overeni jestli je nacteny radek delsi nez MAX_LENGTH
        {
            error(2, "Contact name or number in seznam is too long");
        }
        if (flag == 1) {                                                            // Nacteny radek je jmeno
            flag = 2;
            copyStrToStr(buffer, contactList[i].name);
        }
        else {                                                                      // Nacteny radek je telefonni cislo
            flag = 1;
            copyStrToStr(buffer, contactList[i].phoneNumber);
            contactList[i].filled = 1;                                              // Nastavime ze je kontakt plny
            i++;
        }
    }
    if (fgets(buffer, MAX_LENGTH, stdin) != NULL)                  // Overime jestli neni jeste neco v stdin
    {
        error(3, "Wrong number of contacts in seznam file");           // Pokud ano, tak je to spatny pocet kontaktu v filu
    }
    if (flag == 2)                                                                  // Jestli flag je 2 tak nacteni bylo ukonceno nactenim jmena
    {
        error(5, "File has to have even number of lines");             // Pokud ano, tak je to spatny pocet radku v filu, musi byt sude
    }
    return i;                                                                       // Vratime pocet nactenych kontaktu
}

// Uppercase letters to lowercase letters
void toLowerCase(char const *input, char *output)
{
    int i = 0;
    while (input[i] != '\0')
    {
        if (input[i] >= 'A' && input[i] <= 'Z')
        {
            output[i] = input[i] + CASE_CHANGE_NUM;
        }
        else
        {
            output[i] = input[i];
        }
        i++;
    }
    output[i] = '\0';
}

void transformName(char const *inputName, char *transformedName)                // Funkce pro prevod jmena na format pro hledani
{
    char transformer[27] = "22233344455566677778889999";                        // Transformacni pole
    //    "22233344455566677778889999"
    //    "abcdefghijklmnopqrstuvwxyz"
    char name[MAX_LENGTH + 1];
    toLowerCase(inputName, name);                                   // Prevod jmena na mala pismena
    int i = 0;
    while (name[i] != '\0')
    {
        if (name[i] >= 'a' && name[i] <= 'z'){                                  // Pokud je znak pismeno
            transformedName[i] = transformer[name[i]-LETTER_TO_NUM];            // Prevod pismena na cislo
        }
        else                                                                    // Pokud neni znak pismeno
        {
            if (name[i] == '+')                                                 // Pokud je znak +
            {
                transformedName[i] = '0';                                       // Nastavime cislo 0
            }
            else                                                                // Pokud neni znak +
            {
                transformedName[i] = name[i];                                   // Nastavime znak
            }
        }
        i++;
    }
    transformedName[i] = '\0';                                                  // Ukonceni retezce
}

void transformContactList(struct contact *input, struct contact *output, int contactListLen)    // Funkce pro prevod seznamu kontaktu na format pro hledani
{
    for (int i = 0; i < contactListLen; i++)
    {
        transformName(input[i].name, output[i].name);        // Prevod jmena
        copyStrToStr(input[i].phoneNumber, output[i].phoneNumber);          // Kopirovani telefonniho cisla
        replaceCharInStr(output[i].phoneNumber, '+', '0');             // Nahrazeni znaku + za 0
        output[i].filled = input[i].filled;                                         // Kopirovani flagu
    }
}

void getAllPriorityOne(struct contact *contactList, char *inputNum, int *output, int *found)     // Hledame vsechny kontakty s prioritou 1
{
    int i = 0;
    while (contactList[i].filled > 0) {
        if (checkContactPriorityOne(contactList[i].phoneNumber, inputNum) == 1)
        {
            if (contactList[i].filled == 1)                     // Pokud je kontakt plny
            {
                output[*found] = i;                             // Ulozime index kontaktu
                (*found)++;                                     // Zvysime pocet nalezenych kontaktu
                contactList[i].filled = 2;                      // Nastavime flag na 2, kontakt uz byl nalezen
            }
        }
        i++;
    }
}

void containsInput(struct contact *contactList, char *inputNum, int *output, int *found, int contactListLen)  // Hledame vsechny kontakty ktere obsahuji zadane cislo
{
    for(int i = 0; i < contactListLen; i++)
    {
        if (contactList[i].filled == 1 && checkContactPriorityTwo(contactList[i].phoneNumber, inputNum) == 1)
        {
            output[*found] = i;             // Ulozime index kontaktu
            (*found)++;                     // Zvysime pocet nalezenych kontaktu
            contactList[i].filled = 2;      // Nastavime flag na 2, kontakt uz byl nalezen
        }
        else if (contactList[i].filled == 1 && checkContactPriorityTwo(contactList[i].name, inputNum) == 1)
        {
            output[*found] = i;             // Ulozime index kontaktu
            (*found)++;                     // Zvysime pocet nalezenych kontaktu
            contactList[i].filled = 2;      // Nastavime flag na 2, kontakt uz byl nalezen
        }
    }
}

int main(int argc, char *argv[]) {
    char userInput[MAX_LENGTH + 1];                 // Uzivatelsky vstup
    if (argc > 2)                                   // Overime pocet argumentu
    {
        error(1, "Wrong number of arguments, program need only one additional argument");       // Pokud je vice nez 2, tak je to spatny pocet argumentu
    }
    int outList[CONTACT_LIST_MAX_LENGTH];           // Seznam indexu nalezenych kontaktu
    int found = 0;                                  // Pocet nalezenych kontaktu
    struct contact contactList[CONTACT_LIST_MAX_LENGTH];        // Seznam kontaktu
    struct contact contactListTransformed[CONTACT_LIST_MAX_LENGTH];    // Seznam kontaktu v transformovanem formatu
    int contactListLen = readContactList(contactList);         // Nacteme seznam kontaktu a ulozime pocet nactenych kontaktu
    if (argc < 2)                                   // Pocet argumentu je mensi nez 2
    {
        for (int i = 0; i < contactListLen; i++)    // Nic ne hledame, vypiseme vsechne kontakty
        {
            printf("%s, %s\n", contactList[i].name, contactList[i].phoneNumber);
        }
        return 0;
    }
    copyStrToStr(argv[1], userInput);               // Zkopirujeme argument do promenne pro uzivatelsky vstup
    checkContainsOnlyNumbers(userInput);        // Overime, ze uzivatelsky vstup obsahuje pouze cislice
    transformContactList(contactList, contactListTransformed, contactListLen);      // Prevod seznamu kontaktu na format pro hledani
    getAllPriorityOne(contactListTransformed, userInput, outList, &found);      // Hledame vsechny kontakty s prioritou 1
    containsInput(contactListTransformed, userInput, outList, &found, contactListLen);      // Hledame vsechny kontakty ktere obsahuji zadane cislo
    if (found == 0)                        // Pokud nebyl nalezen zadny kontakt
    {
        printf("Not found\n");               // Vypiseme, ze nebyl nalezen zadny kontakt
    }
    else                                            // Pokud byl nalezen alespon jeden kontakt
    {
        for (int i = 0; i < found; i++)             // Vypiseme vsechny nalezeny kontakty
        {
            printf("%s, %s\n", contactList[outList[i]].name, contactList[outList[i]].phoneNumber);
        }
    }
    return 0;
}
