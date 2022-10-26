# T9-Search

### Running application

>
> How to run an application:
> ```bash
> $ ./t9search <options> <INPUT_ARGUMENT> <seznam.txt
> ```
> Options:
> * ***-s*** - program najde i takové kontakty, u který mezi každými dvěma nalezenými znaky může být libovolná posloupnost jiných znaků. Pokud použijete tuto volbu, musí být první.
> 
> 
> * ***-l \<L>*** - Pokud zadal uživatel špatný vyhledávací řetězec (spletl se v čísle), program mu nabídne podobné výsledky. Uvažujte následující chyby uživatele: chybná číslice, chybějící číslice, číslice zadaná navíc.
> L - reprezentuje maximální počet uvažovaných chyb uživatele

### Basic Testing

>
> How to test an application:
> ```bash
> $ python3 ./test.py t9search --bonus <1-2>
> ```
> Test some output and errors.

### Errors and codes descriptions

#### Application has two types of errors:

1. Input arguments errors codes
> ***(-111)*** - Input arguments amount error. Application got more than one *additional* argument.


> ***(-112)*** - Input argument content has other symbols besides numbers, input has to contain only numbers (0-9).

> ***(-113)*** - You cant use -s without <INPUT_ARGUMENT>.

> ***(-114)*** - You have wrong order of arguments or you forgot to add <INPUT_ARGUMENT>. See [this](#running-application) for more information.

> ***(-115)*** - You have to use -l with L number. 
***
2. Seznam file errors codes
> ***(-121)*** - One of the file's line has more symbols than [MAX_LENGTH](https://github.com/xshche05/t9search/blob/86833ec2238c6a4bfbcdc602c28300a8449d9402/main.c#L4).

> ***(-122)*** - Seznam file has more (than [CONTACT_LIST_MAX_LENGTH](https://github.com/xshche05/t9search/blob/86833ec2238c6a4bfbcdc602c28300a8449d9402/main.c#L5)) contacts than program can proceed.

> ***(-123)*** - Seznam file format error, every contact has to look like **that***, file has to contain even amount of lines.
>
> **that***:
> ```text
> Petr Dvorak
> 603123456
> ```

> ***(-124)*** - Seznam file has empty lines, remove all empty lines.

> ***(-125)*** - Data in seznam file has to contain only [ASCII](https://www.asciitable.com/) characters, diacritic isn't allowed.
***