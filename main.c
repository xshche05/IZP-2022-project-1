#include <stdio.h>
#include <stdlib.h>

/**
 * BASE SETTINGS:
 *
 * MAX_LENGTH - Maximální počet znaků pro vstup a pro řádek seznamů
 * CONTACT_LIST_MAX_LENGTH - Maximální počet kontaktů ve seznamů kontaktů (2x je pocet řadku ve seznam filu)
 * CONTACT_LIST_FILE_NAME - Jmeno seznam filu
 */
#define MAX_LENGTH                  100
#define CONTACT_LIST_MAX_LENGTH     100

/**
 * KONSTANTY
 */
#define CASE_CHANGE_NUM             32
#define LETTER_TO_NUM               97

/**
 * ADV. SETTINGS
 *
 * CHECK_WITH_ERRORS - Define jestli potřebuješ funkce ignorovani input chyb (Priority funkcionality 3.)
 *
 * NUMBER_OF_ERRORS_IN_INPUT - Kolik maximalně muže byt úseků pro ignorovaní
 */
#define CHECK_WITH_ERRORS
#ifdef CHECK_WITH_ERRORS
    #define NUMBER_OF_ERRORS_IN_INPUT   1
#endif

/**
 * CONSOLE OUTPUT SETTINGS
 *
 * ERROR_MESSAGE_COLOR - Barva zprávy o chybě (Standrt je červená barva)
 * COLOR_RESET - Default barva consolu
 */
#define ERROR_MESSAGE_COLOR     "\x1b[31m"
#define COLOR_RESET             "\x1b[0m"

/**
 * CONTACT STRUKTURA
 *
 * filled - Jestli se rovná 1, struktura obsahuje datu, 2 - kontakt je už ověřen
 * name - Jmeno kontaktů
 * phoneNumber - Telefonní číslo kontaktů
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
 * @param input - Kontakt pro ověření
 * @param inputNum - Zadané vchodné číslo
 * @return
 * 1 - kontakt stačí\n
 * 0 - kontakt nestačí
 */
int checkContactPriorityOne(const char *str1, const char *str2)
{
    int found = 0;
    int k = 0;
    int i = k;
    int j = 0;
    #ifdef CHECK_WITH_ERRORS
    int errors = 0;                 // Pocet chbnych useku
    int errorsStack = 0;            // Pocet znaku mezi dvema spravni znaky
    int prev_j = -1;
    #endif
    int was_broke;
    while (str1[i] != '\0' && str2[j] != '\0')
    {
        was_broke = -1;
        if (str1[i] != str2[j])
        {
            #ifdef CHECK_WITH_ERRORS
            if (i == k)
            {
                was_broke = 1;
                break;
            }
            if (errors < NUMBER_OF_ERRORS_IN_INPUT)
            {
                errorsStack++;
                j--;
            }
            else if (errors == NUMBER_OF_ERRORS_IN_INPUT)
            {
                errorsStack++;
                j = prev_j - 1;
                errors = 0;
            }
            else
            {
                was_broke = 1;
                break;
            }
            #else
            was_broke = 1;
            break;
            #endif
        }
        #ifdef CHECK_WITH_ERRORS
        else
        {
            if (errorsStack > 0)
            {
                errors++;
                prev_j = j;
                errorsStack = 0;
            }
        }
        #endif
        i++;
        j++;
    }
    #ifdef CHECK_WITH_ERRORS
    if (errorsStack != 0)
    {
        found = 0;
    }
    else if (was_broke != 1 && str2[j] == '\0')
    {
        found = 1;
    }
    #else
    if (was_broke != 1 && str2[j] == '\0')
    {
        found = 1;
    }
    #endif
    return found;
}

int strContainsStr(char const *str1, char const *str2)
{
    int found;
    for (int k = 0; str1[k] != '\0'; k++)
    {
        found = 0;
        int i = k;
        int j = 0;
        #ifdef CHECK_WITH_ERRORS
        int errors = 0;                 // Pocet chbnych useku
        int errorsStack = 0;            // Pocet znaku mezi dvema spravni znaky
        int prev_j = -1;
        #endif
        int was_broke;
        while (str1[i] != '\0' && str2[j] != '\0')
        {
            was_broke = -1;
            if (str1[i] != str2[j])
            {
                #ifdef CHECK_WITH_ERRORS
                if (i == k)
                {
                    was_broke = 1;
                    break;
                }
                if (errors < NUMBER_OF_ERRORS_IN_INPUT)
                {
                    errorsStack++;
                    j--;
                }
                else if (errors == NUMBER_OF_ERRORS_IN_INPUT)
                {
                    errorsStack++;
                    j = prev_j - 1;
                    j--;
                    errors = 0;
                }
                else
                {
                    was_broke = 1;
                    break;
                }
                #else
                was_broke = 1;
                break;
                #endif
            }
            #ifdef CHECK_WITH_ERRORS
            else
            {
                if (errorsStack > 0)
                {
                    errors++;
                    prev_j = j;
                    errorsStack = 0;
                }
            }
            #endif
            i++;
            j++;
        }
        #ifdef CHECK_WITH_ERRORS
        if (errorsStack != 0)
        {
            found = 0;
        }
        else if (was_broke != 1 && str2[j] == '\0')
        {
            found = 1;
            break;
        }
        #else
        if (was_broke != 1 && str2[j] == '\0')
        {
            found = 1;
            break;
        }
        #endif
    }
    return found;
}

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

int readContactList(struct contact *contactList)
{
    char buffer[MAX_LENGTH + 1];
    int flag = 1;
    int i = 0;
    while (fgets(buffer, MAX_LENGTH, stdin) != NULL && i < CONTACT_LIST_MAX_LENGTH)
    {
        if (flag == 1) {
            flag = 2;
            copyStrToStr(buffer, contactList[i].name);
        }
        else {
            flag = 1;
            copyStrToStr(buffer, contactList[i].phoneNumber);
            contactList[i].filled = 1;
            i++;
        }
    }
    if (fgets(buffer, MAX_LENGTH, stdin) != NULL)
    {
        error(-1, "Wrong length of line of seznam file");
    }
    return i;
}

void toLowerCase(char const *input, char *output)
{
    int i = 0;
    while (input[i] != '\0')
    {
        if (input[i] >= 65 && input[i] <= 90)
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

void transformName(char const *inputName, char *transformedName)
{
    char transformer[27] = "22233344455566677778889999";
//    "22233344455566677778889999"
//    "abcdefghijklmnopqrstuvwxyz"
    int i = 0;
    char name[MAX_LENGTH + 1];
    toLowerCase(inputName, name);
    while (name[i] != '\0')
    {
        if (name[i] >= 97 && name[i] <= 122){
            transformedName[i] = transformer[name[i]-LETTER_TO_NUM];
        }
        else
        {
            transformedName[i] = name[i];
        }
        i++;
    }
    transformedName[i] = '\0';
}

void transformContactList(struct contact *input, struct contact *output, int contactListLen)
{
    int i = 0;
    while (i < contactListLen){
        output[i].filled = 1;
        copyStrToStr(input[i].phoneNumber, output[i].phoneNumber);
        transformName(input[i].name, output[i].name);
        i++;
    }
}

void getAllPriorityOne(struct contact *contactList, char *inputNum, int *output, int *found)
{
    int i = 0;
    while (contactList[i].filled > 0) {
        if (checkContactPriorityOne(contactList[i].phoneNumber, inputNum) == 1)
        {
            if (contactList[i].filled == 1)
            {
                output[*found] = i;
                *found = *found + 1;
                contactList[i].filled = 2;
            }
        }
        i++;
    }
}

void containsInput(struct contact *contactList, char *inputNum, int *output, int *found, int contactListLen)
{
    int i = 0;
    while (i < contactListLen) {
        if (strContainsStr(contactList[i].phoneNumber, inputNum) == 1)
        {
            if (contactList[i].filled == 1)
            {
                output[*found] = i;
                *found = *found + 1;
                contactList[i].filled = 2;
            }
        }
        else if (strContainsStr(contactList[i].name, inputNum) == 1)
        {
            if (contactList[i].filled == 1)
            {
                output[*found] = i;
                *found = *found + 1;
                contactList[i].filled = 2;
            }
        }
        i++;
    }
}

int main(int argc, char *argv[]) {
    char userInput[MAX_LENGTH + 1];
    if (argc > 2)
    {
        error(-1, "Too many arguments, program need only one additional argument");
    }
    int outList[CONTACT_LIST_MAX_LENGTH];
    int found = 0;
    struct contact contactList[CONTACT_LIST_MAX_LENGTH];
    struct contact contactListTransformed[CONTACT_LIST_MAX_LENGTH];
    int contactListLen = readContactList(contactList);
    if (argc < 2)
    {
        for (int i = 0; i < contactListLen; i++)
        {
            printf("%s, %s\n", contactList[i].name, contactList[i].phoneNumber);
        }
        return 0;
    }
    copyStrToStr(argv[1], userInput);
    transformContactList(contactList, contactListTransformed, contactListLen);
    getAllPriorityOne(contactListTransformed, userInput, outList, &found);
    containsInput(contactListTransformed, userInput, outList, &found, contactListLen);
    if (found == 0)
    {
        printf("Not found");
    }
    else
    {
        for (int i = 0; i < found; i++)
        {
            printf("%s, %s\n", contactList[outList[i]].name, contactList[outList[i]].phoneNumber);
        }
    }
    return 0;
}
