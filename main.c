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
#define ERROR_CODE_ARG_AMOUNT                   (-101)
#define ERROR_CODE_ARG_HAS_NOT_ONLY_NUMBERS     (-102)
#define ERROR_CODE_S_USAGE                      (-103)
#define ERROR_CODE_ARG_ORDER                    (-104)
#define ERROR_CODE_L_USAGE                      (-105)

#define ERROR_CODE_LINE_LENGTH                  (-111)
#define ERROR_CODE_LIST_LENGTH                  (-112)
#define ERROR_CODE_NOT_EVEN_NUM_OF_LINES        (-113)
#define ERROR_CODE_LINE_IS_EMPTY                (-114)
#define ERROR_CODE_NOT_ONLY_ASCII               (-115)




// ADV. Settings
int ERRORS_IN_INPUT = 0;   // MAX Pocet chybnych mezer mezi dvema spravnymi znaky
int LEVINSTEIN_DISTANCE_CHECK = 0; // MAX Pocet povolenych chybnych znaku
int LEVINSTEIN_DISTANCE = 0;

char const transformer[27] = "22233344455566677778889999";    // Transformacni pole, pro prevod pismen na cisla
// "22233344455566677778889999"
// "abcdefghijklmnopqrstuvwxyz"

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


int min(int i, int i1, int i2)
{
    if (i < i1 && i < i2)
        return i;
    else if (i1 < i2)
        return i1;
    else
        return i2;
}

int error(int code, char *desc)
{
    code = -code;
    fprintf(stderr, "%s(%d) Error: %s!%s\n", ERROR_MESSAGE_COLOR, code, desc, COLOR_RESET);
    return code;
}

// string ma jen cisla
int checkContainsOnlyNumbers(char const *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            return ERROR_CODE_ARG_HAS_NOT_ONLY_NUMBERS;
        }
    }
    return 1;
}

// overit cislo argumentu pri runu appu
int checkInputArgumentsAmount(int argc)
{
    if (argc > MAX_ARGUMENTS)                                   // Overime pocet argumentu
    {
        return ERROR_CODE_ARG_AMOUNT;       // Pokud je vice nez 3, tak je to vice nez muze byt MAXIMALNE
    }
    return 1;
}

// zmena char v string na jiny char
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

// udelame z velkeho pismena malé
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

// str ma zadany char
int strHasChar(char const *str, char const c, int check_limit)
{
    for (int i = 0; i < check_limit; i++)
    {
        if (str[i] == c)
        {
            return 1;
        }
    }
    return 0;
}

// str1 musi jen obsahovat str2 (obsahuje 3. funkcionalitu)
int checkContactContainsInput(char const *str1, char const *str2)
{
    int i = 0;
    int j = 0;
    for (; str1[i] != '\0'; i++)
    {
        if (str1[i] == str2[j])
        {
            j++;            // Pokud se shoduje, tak zvysime index pro str2
        }
        else if (str2[j] == '\0')
        {
            return 1;       // Pokud je konec str2, tak je to OK
        }
        else if (!ERRORS_IN_INPUT && j != 0)
        {
            i -= j;        // Zpet na puvodni pozici
            j = 0;        // Pokud se nezvysil index pro str2, a musime hledat neprerusenu tak zkusime znovu
        }
    }
    if (str2[j] == '\0')
    {
        return 1;
    }
    return 0;
}

// nacist pole kontaktu z stdin
int readContactList(struct contact *contactList)    // Nacteme seznam kontaktu
{
    char buffer[MAX_LENGTH + 3];        // Buffer pro nacitani znaku, +3 protoze potrebujeme misto pro znak '\n', '\0' a znak pro kontrolu pokud je to vice nez 100
    // a znak pro extra kontrolu radku
    int flag = 0;                       // Flag pro overeni jestli je nacteny radek jmene nebo telefonni cislo
    int i = 0;                          // Pocitadlo pro pocet nactenych kontaktu
    while (fgets(buffer, MAX_LENGTH + 3, stdin) != NULL && i < CONTACT_LIST_MAX_LENGTH)
    {
        replaceChar(buffer, '\n', '\0');    // Nahradime znak '\n' za znak '\0', proto je v seznamu \n je konec radku, ho uz mame
        if (strHasChar(buffer, '\0', MAX_LENGTH + 1) != 1)
            // Overeni jestli je nacteny radek delsi nez MAX_LENGTH, +2 protoze potrebujeme misto pro znak '\n' a '\0',
            // pokud nebude buffer mit '\0', tak radek je delsi
        {
            return ERROR_CODE_LINE_LENGTH;     // Radek je delsi nez MAX_LENGTH
        }
        if (buffer[0] == '\0')                 // RAdek je prazdny
        {
            return ERROR_CODE_LINE_IS_EMPTY;
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
        return ERROR_CODE_LIST_LENGTH;        // Pokud ano, tak je to spatny pocet kontaktu v filu
    }
    if (flag == 1)     // Jestli flag je 2 tak nacteni bylo ukonceno nactenim jmena
    {
        return ERROR_CODE_NOT_EVEN_NUM_OF_LINES;        // Pokud ano, tak je to spatny pocet radku v filu, musi byt sude
    }
    return i;     // Vratime pocet nactenych kontaktu
}

// zmenenit pismena na t9 cisla
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
            return ERROR_CODE_NOT_ONLY_ASCII;
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

// znenit pole kontaktu na pole pro hledani
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

void substring(char *str, char *subStr, int first_sym, int len){
    int c = 0;
    while (c < len) {
        subStr[c] = str[first_sym+c];
        c++;
    }
    subStr[c] = '\0';
}

// levenshtein delka pro str1 a str2
int levenshteinDistance(char *str1, char *str2)    // Funkce pro vypocet Levenshteinovy vzdalenosti
{
    int len1 = (int) strlen(str1);
    int len2 = (int) strlen(str2);
    int distance[len1 + 1][len2 + 1];   // Matice pro vypocet Levenshteinovy vzdalenosti
    for (int i = 0; i <= len1; i++)
    {
        distance[i][0] = i;
    }
    for (int j = 0; j <= len2; j++)
    {
        distance[0][j] = j;
    }
    for (int i = 1; i <= len1; i++)
    {
        for (int j = 1; j <= len2; j++)
        {
            int cost = 0;
            if (str1[i - 1] != str2[j - 1])
            {
                cost = 1;
            }
            distance[i][j] = min(distance[i - 1][j] + 1, distance[i][j - 1] + 1, distance[i - 1][j - 1] + cost);
        }
    }
    return distance[len1][len2];
}

// overit jestli ma str1 podretezec ktery je na L distance od zadaneho cisla
int checkIfSubDifByL(char *str, char *inputNum)
{
    char subStr[MAX_LENGTH];
    int i, j, len = MAX_LENGTH + 1;
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

// hledame kontakty
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

// vypiseme nalezeny kontakty
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


int main(int argc, char *argv[]) {
    int argMove = 1;        // Pocet argumentu, ktere se maji preskocit do user inputu
    int checkCode = checkInputArgumentsAmount(argc);
    if (checkCode < 0) return error(checkCode, "Too many arguments");
    struct contact contactList[CONTACT_LIST_MAX_LENGTH];        // Seznam kontaktu
    int contactListLen = readContactList(contactList);         // Nacteme seznam kontaktu a ulozime pocet nactenych kontaktu
    checkCode = contactListLen;
    if (checkCode < 0) return error(checkCode, "Seznam file has a problem");
    if (argc < 2)                                   // Pocet argumentu je mensi nez 2
    {
        for (int i = 0; i < contactListLen; i++)    // Nic ne hledame, vypiseme vsechne kontakty
        {
            printf("%s, %s\n", contactList[i].name, contactList[i].phoneNumber);
        }
        return 0;
    }
    if (strcmp(argv[argMove], "-s") == 0)       // Pokud je prvni argument -s
    {
        ERRORS_IN_INPUT = 1;          // Nastavime pocet moznych chyb v inputu na 1
        argMove++;                            // Preskocime jeden argument
        if (argc == argMove)
        {
            return error(ERROR_CODE_S_USAGE, "You cant use -s without <INPUT_ARGUMENT>");
        }
    }
    if (strcmp(argv[argMove], "-l") == 0)       // Pokud je prvni argument -f
    {
        LEVINSTEIN_DISTANCE_CHECK = 1;
        argMove++;                            // Preskocime jeden argument
        if (argc == argMove)
        {
            return error(ERROR_CODE_L_USAGE, "You cant use -l without <L>");
        }
        else
        {
            LEVINSTEIN_DISTANCE = atoi(argv[argMove]);   // Nacteme hodnotu L
            argMove++;
        }
    }
    if (argc-1 != argMove) return error(ERROR_CODE_ARG_ORDER, "You have wrong order of arguments or dont have <INPUT_ARGUMENT>"); // argc - 1 protoze musime overit kolokost optional argumentu
    checkCode = checkContainsOnlyNumbers(argv[argMove]);        // Overime, ze uzivatelsky vstup obsahuje pouze cislice
    if (checkCode < 0) return error(checkCode, "Input has to contain only numbers");
    char userInput[MAX_LENGTH + 1];                 // Uzivatelsky vstup
    strcpy(userInput, argv[argMove]);               // Zkopirujeme argument do promenne pro uzivatelsky vstup
    struct contact contactListTransformed[contactListLen];    // Seznam kontaktu v transformovanem formatu
    int outList[contactListLen];           // Seznam indexu nalezenych kontaktu
    int found = 0;                                  // Pocet nalezenych kontaktu
    checkCode = transformContactList(contactList, contactListTransformed, contactListLen);     // Prevod seznamu kontaktu na format pro hledani
    if (checkCode < 0) return error(checkCode, "Data in seznam file has to contain only ASCII characters");
    checkAll(contactListTransformed, userInput, outList, &found, contactListLen);      // Hledame vsechny kontakty ktere obsahuji zadane cislo
    printFoundContacts(contactList, outList, found);    // Vypiseme vsechno co jsme nasli
    return 0;
}
