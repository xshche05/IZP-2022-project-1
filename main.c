#include <stdio.h>

// Basic settings
#define MAX_LENGTH                  100                 // Maximalni pocet znaku pro vstup a pro radku seznamu
#define CONTACT_LIST_MAX_LENGTH     50                  // Maximalni pocet kontaktu ve seznamu

// Kostanty
#define CASE_CHANGE_NUM             32                  // Konstanta pro zmenu velikosti pismen
#define LETTER_TO_NUM               97                  // Konstanta pro prevod pismen na jich poradi v abecede
#define MAX_ARGUMENTS               3                   // Maximalni pocet argumentu

// Konsolove barvy
#define ERROR_MESSAGE_COLOR     "\x1b[31m"               // Barva chyboveho hlaseni
#define COLOR_RESET             "\x1b[0m"                // Reset barvy

// ADV. Settings
int NUMBER_OF_ERRORS_IN_INPUT = 0;   // MAX Pocet chybnych mezer mezi dvema spravnymi znaky

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


int error(int code, char *desc)
{
//    code -= 600;
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
            return -12;
        }
    }
    return 0;
}

// overit cislo argumentu pri runu appu
int checkInputArgumentsAmount(int argc)
{
    if (argc > MAX_ARGUMENTS)                                   // Overime pocet argumentu
    {
        return -11;       // Pokud je vice nez 3, tak je to vice nez muze byt MAXIMALNE
    }
    return 0;
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

// kopie str z from do to
void copyStrToStr(const char *from, char *to)
{
    int i = 0;
    while (from[i] != '\0')     // Dokud neni konec radku, tak kopirujeme znaky.
    {
        to[i] = from[i];
        i++;
    }
    to[i] = '\0';
}

// str1 se rovna str2
int strEquals(const char *str1, const char *str2)
{
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0')
    {
        if (str1[i] != str2[i])
        {
            return 0;
        }
        i++;
    }
    if (str1[i] == '\0' && str2[i] == '\0')
    {
        return 1;
    }
    return 0;
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
    int found;                          // Flag jestli bylo najdeno str2 ve str1, 1 - str2 je ve str1, 0 - ne
    for (int k = 0; str1[k] != '\0'; k++)
    {
        found = 0;                      // Resetujeme flag
        int i = k;                      // Index pro prochazeni str1
        int j = 0;                      // Index pro prochazeni str2
        int errors = 0;                 // Pocet chbnych useku
        int errorsStack = 0;            // Pocet znaku mezi dvema spravni znaky
        int prev_j = -1;                // Index znáku po 1. chybne mezere
        int correctInLine = 0;          // Pocet spravnych znaku v radku pred chybou
        int was_broke;                  // Flag který se ukazuje jestli cyklus byl zrušen, -1 - ne byl zrušen, 1 - byl zrušen
        while (str1[i] != '\0' && str2[j] != '\0')
        {
            was_broke = -1;             // Resetujeme flag
            if (str1[i] != str2[j])
            {
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
            }
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
            i++;
            j++;
        }
        if (errorsStack != 0)                                   // Jeslti mame chyby po skonceni cyklusu neni str2 ve str1
        {
            found = 0;                                          // 0 - neni str2 ve str1
        }
        else if (was_broke != 1 && str2[j] == '\0')             // Jeslti nemame chyb, nebyl cyklus zrusen a dosli jsme do posledniho znaku str2
        {
            found = 1;                                          // 1 - je str2 ve str1
            break;
        }
    }
    return found;
}

// nacist pole kontaktu z stdin
int readContactList(struct contact *contactList)    // Nacteme seznam kontaktu
{
    char buffer[MAX_LENGTH + 3];        // Buffer pro nacitani znaku, +3 protoze potrebujeme misto pro znak '\n', '\0' a znak pro kontrolu pokud je to vice nez 100
    // a znak pro extra kontrolu radku
    int flag = 1;                       // Flag pro overeni jestli je nacteny radek jmene nebo telefonni cislo
    int i = 0;                          // Pocitadlo pro pocet nactenych kontaktu
    while (fgets(buffer, MAX_LENGTH + 3, stdin) != NULL && i < CONTACT_LIST_MAX_LENGTH)
    {
        replaceChar(buffer, '\n', '\0');    // Nahradime znak '\n' za znak '\0', proto je v seznamu \n je konec radku, ho uz mame
        if (strHasChar(buffer, '\0', MAX_LENGTH + 2) != 1)
            // Overeni jestli je nacteny radek delsi nez MAX_LENGTH, +2 protoze potrebujeme misto pro znak '\n' a '\0',
            // pokud nebude buffer mit '\0', tak radek je delsi
        {
            return -21;     // Radek je delsi nez MAX_LENGTH
        }
        if (buffer[0] == '\n')
        {
            return -24;
        }
        if (flag == 1)  // Nacteny radek je jmeno
        {
            flag = 2;
            copyStrToStr(buffer, contactList[i].name);
        }
        else      // Nacteny radek je telefonni cislo
        {
            flag = 1;
            copyStrToStr(buffer, contactList[i].phoneNumber);
            contactList[i].filled = 1;    // Nastavime ze je kontakt plny
            i++;
        }
    }
    if (fgets(buffer, MAX_LENGTH, stdin) != NULL)        // Overime jestli neni jeste neco v stdin
    {
        return -22;        // Pokud ano, tak je to spatny pocet kontaktu v filu
    }
    if (flag == 2)     // Jestli flag je 2 tak nacteni bylo ukonceno nactenim jmena
    {
        return -23;        // Pokud ano, tak je to spatny pocet radku v filu, musi byt sude
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
            return -25;
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

// hledame kontakty
void checkAllIfContainsInput(struct contact *contactList, char *inputNum, int *output, int *found, int contactListLen)    // Hledame vsechny kontakty ktere obsahuji zadane cislo
{
    for(int i = 0; i < contactListLen; i++)
    {
        if (contactList[i].filled == 1 && (checkContactContainsInput(contactList[i].phoneNumber, inputNum) == 1
                                           || checkContactContainsInput(contactList[i].name, inputNum) == 1))
        {
            output[*found] = i;    // Ulozime index kontaktu
            (*found)++;      // Zvysime pocet nalezenych kontaktu
            contactList[i].filled = 2;   // Nastavime flag na 2, kontakt uz byl nalezen
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
    if (checkCode < 0) return error(checkCode, "Input has too many arguments");
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

    if (strEquals(argv[argMove], "-s"))       // Pokud je prvni argument -s
    {
        NUMBER_OF_ERRORS_IN_INPUT = 1;          // Nastavime pocet moznych chyb v inputu na 1
        argMove++;                            // Preskocime jeden argument
        if (argc == argMove)
        {
            return error(-13, "You cant use -s without <INPUT_ARGUMENT>");
        }
    }
    if (argc-1 != argMove) return error(-14, "You have wrong order of arguments"); // argc - 1 protoze musime overit kolokost optional argumentu
    checkCode = checkContainsOnlyNumbers(argv[argMove]);        // Overime, ze uzivatelsky vstup obsahuje pouze cislice
    if (checkCode < 0) return error(checkCode, "Input has to contain only numbers");
    char userInput[MAX_LENGTH + 1];                 // Uzivatelsky vstup
    copyStrToStr(argv[argMove], userInput);               // Zkopirujeme argument do promenne pro uzivatelsky vstup
    struct contact contactListTransformed[contactListLen];    // Seznam kontaktu v transformovanem formatu
    int outList[contactListLen];           // Seznam indexu nalezenych kontaktu
    int found = 0;                                  // Pocet nalezenych kontaktu
    checkCode = transformContactList(contactList, contactListTransformed, contactListLen);     // Prevod seznamu kontaktu na format pro hledani
    if (checkCode < 0) return error(checkCode, "Data in seznam file has to contain only ASCII characters");
    checkAllIfContainsInput(contactListTransformed, userInput, outList, &found, contactListLen);      // Hledame vsechny kontakty ktere obsahuji zadane cislo
    printFoundContacts(contactList, outList, found);    // Vypiseme vsechno co jsme nasli
    return 0;
}
