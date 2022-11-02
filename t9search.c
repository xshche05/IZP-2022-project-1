/**
 * IZP project 1 (FIT VUT) - 1 semester 2022/2023
 * Author:  Kirill Shchetiniuk aka Spagetik, aka xshche05
 * Date:    31.10.2022
 * Description:  This program is a search engine for the T9 dictionary.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Basic settings
#define MAX_LENGTH                  100                 // Maximalni pocet znaku pro vstup a pro radku seznamu
#define CONTACT_LIST_MAX_LENGTH     50                  // Maximalni pocet kontaktu ve seznamu

// Kostanty
#define CASE_CHANGE_NUM             32                  // Konstanta pro zmenu velikosti pismen
#define LETTER_TO_NUM               97                  // Konstanta pro prevod pismen na jich poradi v abecede
#define MAX_ARGUMENTS               5                   // Maximalni pocet argumentu

// Konsolove barvy
#define ERROR_MESSAGE_COLOR     "\x1b[31m"               // Barva chyboveho hlaseni
#define COLOR_RESET             "\x1b[0m"                // Reset barvy

// Chybove hlaseni
#define ERROR_CODE_ARG_AMOUNT                   101      // Spatny pocet argumentu, musi to byt maximalne MAX_ARGUMENTS
#define ERROR_CODE_ARG_HAS_NOT_ONLY_NUMBERS     102      // Hlavni vstupovy argument (cislo pro hledani) neni pouze cislo
#define ERROR_CODE_DONT_HAVE_IN                 103      // Spatne pouziti programu, -s musi byt prvnim a musi aplikace mit cislo pro hledani argumenty
#define ERROR_CODE_ARG_ORDER                    104      // Spatne poradi argumentu, -s musi byt prvnim argumentem nebo neni cislo pro hledani
#define ERROR_CODE_L_USAGE                      105      // Spatne pouziti programu, po -l musi nasledovat L (nezaporne cele cislo)
#define ERROR_CODE_ARG_LEV                      106      // Spatna hodnota argumentu L, musi byt nezaporne cele cislo
#define ERROR_CODE_ARG_LENGTH                   107      // Hlavni vstupovy argument (cislo pro hledani) je prilis dlouhy
#define ERROR_CODE_HAS_UNUSED_ARG               108      // Program ma nezname argumenty

#define ERROR_CODE_LINE_LENGTH                  111      // Chyba v radku seznamu, radka je delsi nez MAX_LENGTH
#define ERROR_CODE_LIST_LENGTH                  112      // Chyba v seznamu, seznam je delsi nez CONTACT_LIST_MAX_LENGTH
#define ERROR_CODE_NOT_EVEN_NUM_OF_LINES        113      // Chyba v seznamu, seznam neni sudy pocet radku
#define ERROR_CODE_LINE_IS_EMPTY                114      // Chyba v seznamu, radka je prazdna
#define ERROR_CODE_NOT_ONLY_ASCII               115      // Chyba v seznamu, radka obsahuje znak jiny nez ASCII




// ADV. Settings
int ERRORS_IN_INPUT = 0;   // MAX Pocet chybnych mezer mezi dvema spravnymi znaky
int LEVINSTEIN_DISTANCE_CHECK = 0; // MAX Pocet povolenych chybnych znaku
int LEVINSTEIN_DISTANCE = 0;
int DONT_HAVE_ANY_IN = 0;

char const transformer[27] = "22233344455566677778889999";    // Transformacni pole, pro prevod pismen na cisla
// "22233344455566677778889999"
// "abcdefghijklmnopqrstuvwxyz"
char userInput[MAX_LENGTH];   // Vstupni cislo pro hledani

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

/**
 * @brief Funkce pro nalezeni minimum z 3 cisel
 *
 * @param a - Prvni cislo
 * @param b - Druhe cislo
 * @param c - Treti cislo
 *
 * @return Minimum z 3 cisel
 */
int min(int a, int b, int c)
{
    if (a < b && a < c)
        return a;
    else if (b < c)
        return b;
    else
        return c;
}

/**
 * @brief Funkce pro vypsani chyboveho hlaseni
 *
 * @param code - Chybovy kod
 * @param desc - Chybove hlaseni
 *
 * @return Chybovy kod
 */
int error(int code, char *desc)
{
    fprintf(stderr, "%s(%d) Error: %s!%s\n", ERROR_MESSAGE_COLOR, code, desc, COLOR_RESET);
    return -code;
}

/**
 * @brief Funkce pro overeni, zda je vstupni argument pouze cisloe
 *
 * @param str - string pro overeni
 *
 * @return 1 - pokud je vstupni argument pouze cislo, jinak 0
 */
int checkContainsOnlyNumbers(char const *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            return 0;
        }
    }
    return 1;
}

/**
 * @brief Funkce pro zmenu oldChar na newChar ve stringu
 *
 * @param str - String, ve kterem se ma zmenit znak
 * @param oldChar - Stary znak
 * @param newChar - Novy znak
 */
void replaceChar(char *str, char oldChar, char newChar)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == oldChar)
        {
            str[i] = newChar;
        }
    }
}

/**
 * @brief Funkce pro zmenu velikosti pismen v stringu na male
 *
 * @param strIn - Vstupni string
 * @param strOut - Vystupni string, kdysi vstupni string, ale s malymi pismeny
 */
void toLowerCase(char const *strIn, char *strOut)
{
    int i = 0;
    while (strIn[i] != '\0')
    {
        if (strIn[i] >= 'A' && strIn[i] <= 'Z')
        {
            strOut[i] = strIn[i] + CASE_CHANGE_NUM;
        }
        else
        {
            strOut[i] = strIn[i];
        }
        i++;
    }
    strOut[i] = '\0';
}

/**
 * @brief Funkce pro overeni, jestli string staci pro zadane argumenty
 * @param string
 * @param inputNum
 * @return 1 - pokud string staci, jinak 0
 */
int checkContactContainsInput(char const *string, char const *inputNum)
{
    int i = 0;
    int j = 0;
    for (; string[i] != '\0'; i++)
    {
        if (string[i] == inputNum[j])
        {
            j++;            // Pokud se shoduje, tak zvysime index pro str2
        }
        else if (inputNum[j] == '\0')
        {
            return 1;       // Pokud je konec str2, tak je to OK
        }
        else if (!ERRORS_IN_INPUT && j != 0)
        {
            i -= j;        // Zpet na puvodni pozici
            j = 0;        // Pokud se nezvysil index pro str2, a musime hledat neprerusenu tak zkusime znovu
        }
    }
    if (inputNum[j] == '\0')
    {
        return 1;
    }
    return 0;
}

/**
 * @brief Funkce pro nacteniseznamu kontaktu ze stdinu
 *
 * @param contactList - pole kontaktu
 * @return pocet nactenych kontaktu
 */
int readContactList(struct contact *contactList)    // Nacteme seznam kontaktu
{
    char buffer[MAX_LENGTH + 2];        // Buffer pro nacitani znaku, +2 protoze potrebujeme misto pro znak '\n', '\0'
    // a znak pro extra kontrolu radku
    int flag = 0;                       // Flag pro overeni jestli je nacteny radek jmene nebo telefonni cislo
    int i = 0;                          // Pocitadlo pro pocet nactenych kontaktu
    while (fgets(buffer, MAX_LENGTH + 2, stdin) != NULL && i < CONTACT_LIST_MAX_LENGTH)
    {
        replaceChar(buffer, '\n', '\0');    // Nahradime znak '\n' za znak '\0', proto je v seznamu \n je konec radku, ho uz mame
        if (strlen(buffer) > MAX_LENGTH)
        {
            return error(ERROR_CODE_LINE_LENGTH, "Seznam line is too long");     // Radek je delsi nez MAX_LENGTH
        }
        if (buffer[0] == '\0')                 // RAdek je prazdny
        {
            return error(ERROR_CODE_LINE_IS_EMPTY, "Seznam has empty line");
        }
        if (flag == 0)  // Nacteny radek je jmeno
        {
            flag = 1;
            strcpy(contactList[i].name, buffer);
        }
        else      // Nacteny radek je telefonni cislo
        {
            flag = 0;
            strcpy(contactList[i].phoneNumber, buffer);
            contactList[i].filled = 1;    // Nastavime ze je kontakt plny
            i++;
        }
    }
    if (fgets(buffer, MAX_LENGTH, stdin) != NULL)        // Overime jestli neni jeste neco v stdin
    {
        return error(ERROR_CODE_LIST_LENGTH, "Seznam has more contacts than max allowed contact list len");        // Pokud ano, tak je to spatny pocet kontaktu v filu
    }
    if (flag == 1)     // Jestli flag je 2 tak nacteni bylo ukonceno nactenim jmena
    {
        return error(ERROR_CODE_NOT_EVEN_NUM_OF_LINES, "Seznam has not even amount of lines");        // Pokud ano, tak je to spatny pocet radku v filu, musi byt sude
    }
    return i;     // Vratime pocet nactenych kontaktu
}

/**
 * @brief Funkce pro transformaci znaky do cisla T9 klavesnice
 *
 * @param inputStr - vstupni string
 * @param transformedStr - vystupni string, ktery obsahuje cisla T9
 * @return 0 - pokud je vse OK, jinak error kod ktery je zaporny
 */
int transformElement(char const *inputStr, char *transformedStr)     // Funkce pro prevod jmena na format pro hledani
{
    char lowerCaseStr[MAX_LENGTH + 1];
    toLowerCase(inputStr, lowerCaseStr);  // Prevod jmena na mala pismena
    int i = 0;
    while (lowerCaseStr[i] != '\0')
    {
        if (lowerCaseStr[i] >= 'a' && lowerCaseStr[i] <= 'z')       // Pokud je znak pismeno
        {
            // ('a' - LETTER_TO_NUM = 0; transformedStr[0] = '2')
            transformedStr[i] = transformer[lowerCaseStr[i] - LETTER_TO_NUM];   // Prevod pismena na cislo
        }
        else if (lowerCaseStr[i] < 0)
        {
            return error(ERROR_CODE_NOT_ONLY_ASCII, "Seznam has not only ASCII characters");
        }
        else  // Pokud neni znak pismeno
        {
            if (lowerCaseStr[i] == '+')    // Pokud je znak +
            {
                transformedStr[i] = '0';     // Nastavime cislo 0
            }
            else   // Pokud neni znak +
            {
                transformedStr[i] = lowerCaseStr[i];  // Nastavime znak
            }
        }
        i++;
    }
    transformedStr[i] = '\0';   // Ukonceni retezce
    return 0;
}

/**
 * @brief Funkce pro transformaci celeho seznamu kontaktu
 *
 * @param input - pole kontaktu ktere chceme transformovat
 * @param output - vystupni pole kontaktu
 * @param contactListLen - pocet kontaktu v seznamu
 * @return 0 - poku je vse OK, jinak error kod ktery je zaporny
 */
int transformContactList(struct contact *input, struct contact *output, int contactListLen)    // Funkce pro prevod seznamu kontaktu na format pro hledani
{
    int checkflag = 0;
    for (int i = 0; i < contactListLen; i++)
    {
        checkflag = transformElement(input[i].name, output[i].name);     // Prevod jmena
        if (checkflag < 0) return checkflag;
        checkflag = transformElement(input[i].phoneNumber, output[i].phoneNumber);  // Prevod tel cisla
        if (checkflag < 0) return checkflag;
        output[i].filled = input[i].filled;   // Kopirovani flagu
    }
    return checkflag;
}

/**
 * @brief Funkce pro udelani substringu podle zadanych parametrue
 *
 * @param inputStr - vstupni string, ze ktereho chceme udelat substring
 * @param outputStr - vystupni string, do ktereho chceme ulozit substring
 * @param start - index od ktereho chceme udelat substring
 * @param len - delka substringu od startu
 */
void substring(const char *str, char *subStr, int start, int len){
    int c = 0;
    while (c < len) {
        subStr[c] = str[start + c];
        c++;
    }
    subStr[c] = '\0';
}

/**
 * @brief Funkce pro hledani levenshteinovskou vzdalenosti
 *
 * @param str1 - prvni retezec
 * @param str2 - druhy retezec
 * @return levenshteinovska vzdalenost
 */
int levenshteinDistance(char *str1, char *str2)    // Funkce pro vypocet Levenshteinovy vzdalenosti
{
    int len1 = (int) strlen(str1);
    int len2 = (int) strlen(str2);
    int distance[len1 + 1][len2 + 1];   // Matice pro vypocet Levenshteinovy vzdalenosti
    for (int i = 0; i <= len1; i++)
    {
        distance[i][0] = i;             // Prvni radek matice
    }
    for (int j = 0; j <= len2; j++)
    {
        distance[0][j] = j;             // Prvni sloupec matice
    }
    for (int i = 1; i <= len1; i++)
    {
        for (int j = 1; j <= len2; j++)
        {
            int cost = 0;               // Pokud jsou znaky stejne, tak je cost 0, jinak 1
            if (str1[i - 1] != str2[j - 1])
            {
                cost = 1;
            }
            distance[i][j] = min(distance[i - 1][j] + 1, distance[i][j - 1] + 1, distance[i - 1][j - 1] + cost);    // Vypocet Levenshteinovy vzdalenosti, bereme minimum z 3 hodnot
        }
    }
    return distance[len1][len2];        // Vraceni Levenshteinovy vzdalenosti, ktera je v poslednim policku matice a je nejmensi
}

/**
 * @brief Funkce pro hledani kontaktu podle parametru -l
 *
 * @param str - substring, ktery obsahuje kontakt
 * @param inputNum - vstupni pole kontaktu
 * @return 1 - pokud kontakt staci, jinak 0
 */
int checkIfSubDifByL(char *str, char *inputNum)
{
    char subStr[MAX_LENGTH];
    int i, j;
    int len = MAX_LENGTH + 1;
    int lenInputNum = (int) strlen(inputNum);
    // i - prvni pismeno, j - pocet pismen, len - delka str
    for(i = 0; i < len; i++) // pro kazdy prvni znak
    {
        for(j = 1; j <= len - i; j++) // pro kazdy pocet znaku
        {
            if (abs(lenInputNum - j) <= LEVINSTEIN_DISTANCE) // pokud je LEVINSTEIN_DISTANCE <= rozdil delky inputu a substringu jinak nema smysl hledat distanci
            {
                substring(str, subStr, i, j); // vytvorime substring
                if (levenshteinDistance(inputNum, subStr) <= LEVINSTEIN_DISTANCE) // pokud je LEVINSTEIN_DISTANCE <= vzdalenost inputu a substringu
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/**
 * @brief Funkce pro vsech kontaktu podle zadanych parametru
 *
 * @param contactList - transformovany seznam kontaktu
 * @param inputNum - vstupni retezec, ktery chceme hledat
 * @param output - pole indexu nalezenych kontaktu
 * @param found - pocet nalezenych kontaktu
 * @param contactListLen - pocet kontaktu v seznamu
 */
void checkAll(struct contact *contactList, char *inputNum, int *output, int *found, int contactListLen)    // Hledame vsechny kontakty ktere obsahuji zadane cislo
{
    for(int i = 0; i < contactListLen; i++)
    {
        if (contactList[i].filled == 1){
            if ((checkContactContainsInput(contactList[i].phoneNumber, inputNum) == 1 || checkContactContainsInput(contactList[i].name, inputNum) == 1))
            {
                output[*found] = i;    // Ulozime index kontaktu
                (*found)++;      // Zvysime pocet nalezenych kontaktu
                contactList[i].filled = 2;   // Nastavime flag na 2, kontakt uz byl nalezen
            }
            else if (LEVINSTEIN_DISTANCE_CHECK == 1){
                if (checkIfSubDifByL(contactList[i].phoneNumber, inputNum) == 1 || checkIfSubDifByL(contactList[i].name, inputNum) == 1)
                {
                    output[*found] = i;    // Ulozime index kontaktu
                    (*found)++;      // Zvysime pocet nalezenych kontaktu
                    contactList[i].filled = 2;   // Nastavime flag na 2, kontakt uz byl nalezen
                }
            }
        }
    }
}

/**
 * @brief Vypis vsech nalezenych kontaktu
 *
 * @param contactList - netransformovany seznam kontaktu
 * @param outList - pole indexu nalezenych kontaktu
 * @param found - pocet nalezenych kontaktu
 */
void printFoundContacts(struct contact *contactList, const int *outList, int found)
{
    if (found == 0)    // Pokud nebyl nalezen zadny kontakt
    {
        printf("Not found\n");   // Vypiseme, ze nebyl nalezen zadny kontakt
    }
    else    // Pokud byl nalezen alespon jeden kontakt
    {
        for (int i = 0; i < found; i++)   // Vypiseme vsechny nalezeny kontakty
        {
            printf("%s, %s\n", contactList[outList[i]].name, contactList[outList[i]].phoneNumber);
        }
    }
}

/**
 * @brief Overeni, jestli ma pole stringu zadany string
 *
 * @param array - pole stringu
 * @param arrayLen - delka pole stringu
 * @param string - string, ktery chceme hledat
 * @param index - index nalezeneho stringu
 * @return 1 - pokud ma pole stringu zadany string, jinak 0
 */
int arrayContainsString(char *array[], int arrayLen, char *string, int *index)
{
    for (int i = 0; i < arrayLen; i++)
    {
        if (strcmp(array[i], string) == 0)
        {
            *index = i;
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Funkce pro zpracovani parametru
 *
 * @param argc - pocet parametru
 * @param argv - pole parametru
 * @return 0 - pokud vse probehlo v poradku, jinak error kod ktery je zaporny
 */
int parseArguments(int argc, char *argv[])
{
    if (argc == 1) // Overime jestli mame nejake argumenty
    {
        DONT_HAVE_ANY_IN = 1;   // Nastavime flag, ze nemame zadne argumenty
        return 1;
    }
    int i;
    if (argc > MAX_ARGUMENTS) // Overime jestli mame mene argumentu nez MAX_ARGUMENTS
    {
        return error(ERROR_CODE_ARG_AMOUNT, "Too many arguments");
    }
    int indexes[argc];  // Pole pro indexy nactenych argumentu
    for (int j = 0; j < argc; j++)
    {
        indexes[j] = -1;    // Nastavime vsechny indexy na -1, ze nejsou nacteny
    }
    if (arrayContainsString(argv, argc, "-s", &i) == 1)  // Jestli mame -s
    {
        ERRORS_IN_INPUT = 1; // Nastavime flag na 1, proto musime vypsat kontanty podle 3.
        indexes[i] = 1;  // Nastavime index na 1, protoze je nacteny
        if (i != 1) // Overime jestli je -s na spravnem miste
        {
            return error(ERROR_CODE_ARG_ORDER, "Wrong order of arguments, -s must be first");
        }
    }
    if (arrayContainsString(argv, argc, "-l", &i) == 1) // Jestli mame -l
    {
        LEVINSTEIN_DISTANCE_CHECK = 1; // Nastavime flag na 1, protoze chceme pouzit Levenshteinovu vzdalenost
        indexes[i] = 1; // Nastavime index na 1, protoze je nacteny
        if (i+1 < argc) // Pokud mame jeste neco po -l
        {
            char *in = argv[i+1];   // Nacteme toto
            int check = checkContainsOnlyNumbers(in);   // Overime jestli je to cislo
            if (check == 1)
            {
                LEVINSTEIN_DISTANCE = atoi(in); // Nastavime LEVINSTEIN_DISTANCE na nactene cislo
                indexes[i+1] = 1;   // Nastavime index na 1, protoze je nacteny
            }
            else
            {
                return error(ERROR_CODE_ARG_LEV, "Levenshtein distance is not a number");   // Pokud neni cislo, vratime chybu
            }
        }
        else
        {
            return error(ERROR_CODE_L_USAGE, "-l must be followed by a number");    // Pokud nema po sobe cislo, vratime chybu
        }
    }
    int flag = 0;   // Pomocny flag
    for (int j = 1; j < argc; j++)      // Hledame hlavni argument
    {
        if (indexes[j] == -1)
        {
            flag = 1; // Nastavime flag na 1, protoze jsme nasli hlavni argument
            if (strlen(argv[j]) > MAX_LENGTH)
            {
                return error(ERROR_CODE_ARG_LENGTH, "Argument is too long");  // Pokud je argument delsi nez MAX_LENGTH, vratime chybu
            }
            if (checkContainsOnlyNumbers(argv[j]) == 0)
            {
                return error(ERROR_CODE_ARG_HAS_NOT_ONLY_NUMBERS, "Argument is not a number"); // Pokud argument neni cislo, vratime chybu
            }
            strcpy(userInput, argv[j]); // Nastavime userInput na nacteny argument
            indexes[j] = 1; // Nastavime index na 1, protoze je nacteny
            break;
        }
    }
    if (flag == 0)
    {
        return error(ERROR_CODE_DONT_HAVE_IN, "App doesn't have any main input argument");  // Pokud neni zadany hlavni argument, vratime chybu
    }
    for (int j = 1; j < argc; j++)  // Overime jestli mame jeste neco
    {
        if (indexes[j] == -1)
        {
            return error(ERROR_CODE_HAS_UNUSED_ARG, "App have an unused or unsupported argument"); // Pokud mame jeste neco, vratime chybu, proto ze nemusi to mit jine argumenty
        }
    }
    return 0;
}


int main(int argc, char *argv[]) {
    int checkCode = parseArguments(argc, argv);
    if (checkCode < 0) return -checkCode;
    struct contact contactList[CONTACT_LIST_MAX_LENGTH];        // Seznam kontaktu
    int contactListLen = readContactList(contactList);         // Nacteme seznam kontaktu a ulozime pocet nactenych kontaktu
    checkCode = contactListLen;
    if (checkCode < 0) return -checkCode;
    if (DONT_HAVE_ANY_IN == 1)                                   // Pocet argumentu je mensi nez 2
    {
        for (int i = 0; i < contactListLen; i++)    // Nic ne hledame, vypiseme vsechne kontakty
        {
            printf("%s, %s\n", contactList[i].name, contactList[i].phoneNumber);
        }
        return 0;
    }
    struct contact contactListTransformed[contactListLen];    // Seznam kontaktu v transformovanem formatu
    int outList[contactListLen];           // Seznam indexu nalezenych kontaktu
    int found = 0;                                  // Pocet nalezenych kontaktu
    checkCode = transformContactList(contactList, contactListTransformed, contactListLen);     // Prevod seznamu kontaktu na format pro hledani
    if (checkCode < 0) return -checkCode;
    checkAll(contactListTransformed, userInput, outList, &found, contactListLen);      // Hledame vsechny kontakty ktere obsahuji zadane cislo
    printFoundContacts(contactList, outList, found);    // Vypiseme vsechno co jsme nasli
    return 0;
}
