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
#define CONTACT_LIST_MAX_LENGTH     50
#define CONTACT_LIST_FILE_NAME      "seznam.txt"

/**
 * KONSTANTY
 */
#define CASE_CHANGE_NUM             32

/**
 * ADV. SETTINGS
 *
 * CHECK_WITH_ERRORS - Define jestli potřebuješ funkce ignorovani input chyb (Priority funkcionality 3.)
 *
 * NUMBER_OF_ERRORS_IN_INPUT - Kolik maximalně muže byt úseků pro ignorovaní
 */
//#define CHECK_WITH_ERRORS
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
    else if (was_broke != 1)
    {
        found = 1;
//        break;
    }
    #else
    if (was_broke != 1)
    {
        found = 1;
    }
    #endif
    return found;
//    int i = 0;
//    int j = 0;
//    #ifdef CHECK_WITH_ERRORS
//    int errors = 0;                 // Pocet chbnych useku
//    int errorsStack = 0;            // Pocet znaku mezi dvema spravni znaky
//    int prev_j = -1;
//    #endif
//    while (input[i] != '\0' && inputNum[j] != '\0')
//    {
//        if (input[i] != inputNum[j])
//        {
//            #ifdef CHECK_WITH_ERRORS
//            if (i == 0)
//            {
//                return 0;
//            }
//            if (errors < NUMBER_OF_ERRORS_IN_INPUT)
//            {
//                errorsStack++;
//                j--;
//            }
//            else if (errors == NUMBER_OF_ERRORS_IN_INPUT)
//            {
//                errorsStack++;
//                j = prev_j - 1;
//                errors = 0;
//            }
//            else
//            {
//                return 0;
//            }
//            #else
//            return 0;
//            #endif
//        }
//        #ifdef CHECK_WITH_ERRORS
//        else
//        {
//            if (errorsStack > 0)
//            {
//                errors++;
//                prev_j = j;
//                errorsStack = 0;
//            }
//        }
//        #endif
//        i++;
//        j++;
//    }
//#ifdef CHECK_WITH_ERRORS
//    if (errorsStack != 0)
//    {
//        return 0;
//    }
//    #endif
//    return 1;
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
        else if (was_broke != 1)
        {
            found = 1;
            break;
        }
        #endif
        if (was_broke != 1)
        {
            found = 1;
            break;
        }
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

void readContactList(struct contact *contactList)
{
    FILE* in;
    char buffer[MAX_LENGTH + 1];
    in = fopen(CONTACT_LIST_FILE_NAME, "r");
    if (in == NULL)
    {
        error(1, "File not found");
    }

    int flag = 1;
    int i = 0;
    while (fgets(buffer, MAX_LENGTH, in) != NULL)
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
}

void readInput(char *userInput)
{
    // input for t9search
    scanf("%s", userInput);
    // input has to be 100 or less symbols
    if (userInput[MAX_LENGTH] != '\0')
    {
        error(-1, "Input length is more than MAX_LENGTH");
    }
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
    toLowerCase(inputName, name);;
    while (name[i] != '\0')
    {
        if (name[i] >= 97 && name[i] <= 122){
            transformedName[i] = transformer[name[i]-97];
        }
        else
        {
            transformedName[i] = name[i];
        }
        i++;
    }
}

void transformContactList(struct contact *input, struct contact *output)
{
    int i = 0;
    while (input[i].filled == 1){
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

void containsInput(struct contact *contactList, char *inputNum, int *output, int *found)
{
    int i = 0;
    while (contactList[i].filled > 0) {
        if (strContainsStr(contactList[i].name, inputNum) == 1)
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

int main() {
    while (1 == 1){
        char userInput[MAX_LENGTH + 1];
        int outList[CONTACT_LIST_MAX_LENGTH];
        int found = 0;
        struct contact contactList[CONTACT_LIST_MAX_LENGTH];
        struct contact contactListTransformed[CONTACT_LIST_MAX_LENGTH];
        readInput(userInput);
        readContactList(contactList);
        transformContactList(contactList, contactListTransformed);
        getAllPriorityOne(contactListTransformed, userInput, outList, &found);
        containsInput(contactListTransformed, userInput, outList, &found);
        for (int i = 0; i < found; i++)
        {
            printf("%s, %s\n", contactList[outList[i]].name, contactList[outList[i]].phoneNumber);
            printf("%s, %s\n", contactListTransformed[outList[i]].name, contactListTransformed[outList[i]].phoneNumber);
        }
    }
    return 0;
}
