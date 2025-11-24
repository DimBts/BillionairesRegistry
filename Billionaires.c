#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct birthDate {
    int day, month, year;
} Bdate;

typedef struct birthTime {
    int hour, minutes;
} Btime;

typedef struct Billionaire {
    char name[50];
    char cityCountry[100];
    char activity[50];
    char originCountry[50];
    char gender[10];
    Bdate birthdate;
    Btime birthtime;
    float netWorth;
    char accountNumber[15];
    struct Billionaire *next, *prev;
} billionaire;

billionaire *head = NULL, *tail = NULL;

void initList() {
    head = NULL;
    tail = NULL;
}

int isValidDate(int day, int month, int year) {
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
            return day <= 29;
        } else {
            return day <= 28;
        }
    }
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        return day <= 30;
    }
    return 1;
}

int isValidTime(int hour, int minutes) {
    if (hour < 0 || hour > 23) return 0;
    if (minutes < 0 || minutes > 59) return 0;
    return 1;
}

char* generateAccountNumber() {
    static char accountNumber[15];
    int digits[10];
    int sum, checkDigit;
    
    for (int i = 0; i < 10; i++) {
        digits[i] = rand() % 10;
    }

    sum = 0;
    for (int i = 0; i < 10; i++) {
        int digit = digits[i];
        if (i % 2 == 0) {
            digit *= 2;
            if (digit > 9) digit -= 9;
        }
        sum += digit;
    }
    checkDigit = (sum * 9) % 10;

    snprintf(accountNumber, sizeof(accountNumber), "%d%d%d%d%d-%d%d%d%d%d-%d",
             digits[0], digits[1], digits[2], digits[3], digits[4],
             digits[5], digits[6], digits[7], digits[8], digits[9],
             checkDigit);

    return accountNumber;
}

void capitalize(char *str) {
    int i = 0;
    int capitalizeNext = 1;
    while (str[i]) {
        if (capitalizeNext && isalpha((unsigned char)str[i])) {
            str[i] = toupper((unsigned char)str[i]);
            capitalizeNext = 0;
        } else {
            str[i] = tolower((unsigned char)str[i]);
        }
        if (str[i] == ' ' || str[i] == '/' || str[i] == ',') {
            capitalizeNext = 1;
        }
        i++;
    }
}

char* strcasestr_custom(const char *haystack, const char *needle) {
    const char *p;
    if (!*needle) return (char *) haystack;
    for (p = haystack; *p; p++) {
        if (tolower((unsigned char)*p) == tolower((unsigned char)*needle)) {
            const char *h, *n;
            for (h = p, n = needle; *h && *n; h++, n++) {
                if (tolower((unsigned char)*h) != tolower((unsigned char)*n)) break;
            }
            if (!*n) return (char *) p;
        }
    }
    return NULL;
}

void replaceUnknown(char *str) {
    if (strcmp(str, "?") == 0) {
        strcpy(str, "Unknown");
    }
}

int isValidGender(const char *gender) {
    const char *validGenders[] = {
        "M", "F", "NB", "TG", "Q", "O"
    };
    int i;
    for (i = 0; i < 6; i++) {
        if (strcasecmp(gender, validGenders[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int checkDuplicate(billionaire *newBil) {
    billionaire *current = head;
    if (!head) return 0;

    do {
        if (strcmp(current->name, newBil->name) == 0 &&
            strcmp(current->cityCountry, newBil->cityCountry) == 0 &&
            strcmp(current->activity, newBil->activity) == 0 &&
            strcmp(current->originCountry, newBil->originCountry) == 0 &&
            strcmp(current->gender, newBil->gender) == 0 &&
            current->birthdate.day == newBil->birthdate.day &&
            current->birthdate.month == newBil->birthdate.month &&
            current->birthdate.year == newBil->birthdate.year &&
            current->birthtime.hour == newBil->birthtime.hour &&
            current->birthtime.minutes == newBil->birthtime.minutes &&
            current->netWorth == newBil->netWorth) {
            printf("Duplicate entry found: %s\n", newBil->name);
            return 1;
        }
        current = current->next;
    } while (current != head);

    return 0;
}

void addBillionaire(char *input) {
    char name[50], cityCountry[100], activity[50], originCountry[50], gender[10];
    int day, month, year, hour, minutes;
    float netWorth;
    billionaire *newBil;

    char *token;
    char *delim = ";";

    // Parse input
    token = strtok(input, delim);
    if (token) strcpy(name, token);

    token = strtok(NULL, delim);
    if (token) strcpy(cityCountry, token);

    token = strtok(NULL, delim);
    if (token) strcpy(activity, token);

    token = strtok(NULL, delim);
    if (token) strcpy(originCountry, token);

    token = strtok(NULL, delim);
    if (token) strcpy(gender, token);

    token = strtok(NULL, delim);
    if (token) {
        sscanf(token, "%d/%d/%d %d:%d", &day, &month, &year, &hour, &minutes);
        if (!isValidDate(day, month, year) || !isValidTime(hour, minutes)) {
            printf("Invalid date or time format.\n");
            return;
        }
    }

    token = strtok(NULL, delim);
    if (token) {
        netWorth = atof(token);
    }

    replaceUnknown(cityCountry);
    replaceUnknown(activity);
    replaceUnknown(originCountry);
    replaceUnknown(gender);

    if (!isValidGender(gender)) {
        printf("Invalid gender. Accepted values are: M, F, NB, TG, Q, O.\n");
        return;
    }

    newBil = (billionaire*)malloc(sizeof(billionaire));
    strcpy(newBil->name, name);
    strcpy(newBil->cityCountry, cityCountry);
    strcpy(newBil->activity, activity);
    strcpy(newBil->originCountry, originCountry);
    strcpy(newBil->gender, gender);
    newBil->birthdate.day = day;
    newBil->birthdate.month = month;
    newBil->birthdate.year = year;
    newBil->birthtime.hour = hour;
    newBil->birthtime.minutes = minutes;
    newBil->netWorth = netWorth;
    strcpy(newBil->accountNumber, generateAccountNumber());

    capitalize(newBil->name);
    capitalize(newBil->cityCountry);
    capitalize(newBil->activity);
    capitalize(newBil->originCountry);
    capitalize(newBil->gender);

    if (checkDuplicate(newBil)) {
        free(newBil);
        return;
    }

    if (head == NULL) {
        head = newBil;
        tail = newBil;
        newBil->next = newBil;
        newBil->prev = newBil;
    } else {
        tail->next = newBil;
        newBil->prev = tail;
        newBil->next = head;
        head->prev = newBil;
        tail = newBil;
    }
    printf("The billionaire %s added successfully.\n", name);
}

void deleteNew() {
    billionaire *toDelete;
    if (head == NULL) {
        printf("The list is empty.\n");
        return;
    }

    toDelete = tail;
    if (head == tail) {
        head = NULL;
        tail = NULL;
    } else {
        tail = toDelete->prev;
        tail->next = head;
        head->prev = tail;
    }
    free(toDelete);
    printf("Recent registration deleted.\n");
}

void deleteOld() {
    billionaire *toDelete;
    if (head == NULL) {
        printf("The list is empty.\n");
        return;
    }

    toDelete = head;
    if (head == tail) {
        head = NULL;
        tail = NULL;
    } else {
        head = toDelete->next;
        head->prev = tail;
        tail->next = head;
    }
    free(toDelete);
    printf("First registration deleted.\n");
}

void saveToFile(char *filename) {
    FILE *file;
    billionaire *current;

    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return;
    }

    current = head;
    if (current == NULL) {
        printf("The list is empty.\n");
        fclose(file);
        return;
    }

    do {
        fprintf(file, "%s;%s;%s;%s;%s;%02d/%02d/%04d %02d:%02d;%.2f;%s\n",
                current->name, current->cityCountry, current->activity,
                current->originCountry, current->gender, current->birthdate.day,
                current->birthdate.month, current->birthdate.year, current->birthtime.hour,
                current->birthtime.minutes, current->netWorth, current->accountNumber);
        current = current->next;
    } while (current != head);

    fclose(file);
    printf("Data saved to file %s.\n", filename);
}

void loadFromFile(int numEntries, char *filename) {
    FILE *file;
    char line[256];
    int count;
    billionaire *newBil;
    char *token;
    char *delim = ";";
    
    char name[50], cityCountry[100], activity[50], originCountry[50], gender[10];
    int day, month, year, hour, minutes;
    float netWorth;
    char accountNumber[15];

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return;
    }

    count = 0;
    while (fgets(line, sizeof(line), file) && count < numEntries) {
        token = strtok(line, delim);
        if (token) strcpy(name, token);

        token = strtok(NULL, delim);
        if (token) strcpy(cityCountry, token);

        token = strtok(NULL, delim);
        if (token) strcpy(activity, token);

        token = strtok(NULL, delim);
        if (token) strcpy(originCountry, token);

        token = strtok(NULL, delim);
        if (token) strcpy(gender, token);

        token = strtok(NULL, delim);
        if (token) {
            sscanf(token, "%d/%d/%d %d:%d", &day, &month, &year, &hour, &minutes);
            if (!isValidDate(day, month, year) || !isValidTime(hour, minutes)) {
                printf("Invalid date or time format in file. Skipping entry.\n");
                continue;
            }
        }

        token = strtok(NULL, delim);
        if (token) {
            netWorth = atof(token);
        }

        token = strtok(NULL, "\n");
        if (token) {
            strcpy(accountNumber, token);
        } else {
            strcpy(accountNumber, generateAccountNumber());
        }

        if (!isValidGender(gender)) {
            printf("Invalid gender found in file: %s. Skipping this entry.\n", gender);
            continue;
        }

        newBil = (billionaire*)malloc(sizeof(billionaire));
        strcpy(newBil->name, name);
        strcpy(newBil->cityCountry, cityCountry);
        strcpy(newBil->activity, activity);
        strcpy(newBil->originCountry, originCountry);
        strcpy(newBil->gender, gender);
        newBil->birthdate.day = day;
        newBil->birthdate.month = month;
        newBil->birthdate.year = year;
        newBil->birthtime.hour = hour;
        newBil->birthtime.minutes = minutes;
        newBil->netWorth = netWorth;
        strcpy(newBil->accountNumber, accountNumber);

        capitalize(newBil->name);
        capitalize(newBil->cityCountry);
        capitalize(newBil->activity);
        capitalize(newBil->originCountry);
        capitalize(newBil->gender);

        if (head == NULL) {
            head = newBil;
            tail = newBil;
            newBil->next = newBil;
            newBil->prev = newBil;
        } else {
            tail->next = newBil;
            newBil->prev = tail;
            newBil->next = head;
            head->prev = newBil;
            tail = newBil;
        }
        count++;
    }

    fclose(file);
    printf("%d entries loaded from file %s.\n", count, filename);
}

void printSum() {
    float sum;
    billionaire *current;

    if (head == NULL) {
        printf("The list is empty.\n");
        return;
    }

    sum = 0;
    current = head;
    do {
        sum += current->netWorth;
        current = current->next;
    } while (current != head);

    printf("Sum of all net worths: %.2f\n", sum);
}

void printBillionaire(billionaire *bil) {
    printf("%-42s %-36s %-30s %-20s %-6s %02d/%02d/%04d   %02d:%02d   %-6.1f   %-15s\n",
           bil->name,bil->cityCountry, bil->activity, bil->originCountry, bil->gender,
           bil->birthdate.day, bil->birthdate.month, bil->birthdate.year,
           bil->birthtime.hour, bil->birthtime.minutes, bil->netWorth,bil->accountNumber);
}

void findBillionaire(char criteria, const char *search) {
    billionaire *current;
    int found;

    if (head == NULL) {
        printf("The list is empty.\n");
        return;
    }

    current = head;
    found = 0;

    do {
        if (*(search) == '*'){
            printBillionaire(current);
            found = 1;
        } else {
            switch (criteria) {
                case 'N':
                    if (strcasestr_custom(current->name, search)) {
                        printBillionaire(current);
                        found = 1;
                    }
                    break;
                case 'L':
                    if ((strcasestr_custom(current->cityCountry, search)) || (strcasestr_custom(current->originCountry, search)))  {
                        printBillionaire(current);
                        found = 1;
                    }
                    break;
                case 'S':
                    if (strcasestr_custom(current->activity, search)) {
                        printBillionaire(current);
                        found = 1;
                    }
                    break;
            }
        }
        
        current = current->next;
    } while (current != head);

    if (!found) {
        printf("No billionaire matches the criteria.\n");
    }
}

void swap(billionaire *a, billionaire *b) {
    char tempName[50], tempCityCountry[100], tempActivity[50], tempOriginCountry[50], tempGender[10], tempAccountNumber[15];
    Bdate tempBirthdate;
    Btime tempBirthtime;
    float tempNetWorth;

    strcpy(tempName, a->name);
    strcpy(tempCityCountry, a->cityCountry);
    strcpy(tempActivity, a->activity);
    strcpy(tempOriginCountry, a->originCountry);
    strcpy(tempGender, a->gender);
    strcpy(tempAccountNumber, a->accountNumber);
    tempBirthdate = a->birthdate;
    tempBirthtime = a->birthtime;
    tempNetWorth = a->netWorth;

    strcpy(a->name, b->name);
    strcpy(a->cityCountry, b->cityCountry);
    strcpy(a->activity, b->activity);
    strcpy(a->originCountry, b->originCountry);
    strcpy(a->gender, b->gender);
    strcpy(a->accountNumber, b->accountNumber);
    a->birthdate = b->birthdate;
    a->birthtime = b->birthtime;
    a->netWorth = b->netWorth;

    strcpy(b->name, tempName);
    strcpy(b->cityCountry, tempCityCountry);
    strcpy(b->activity, tempActivity);
    strcpy(b->originCountry, tempOriginCountry);
    strcpy(b->gender, tempGender);
    strcpy(b->accountNumber, tempAccountNumber);
    b->birthdate = tempBirthdate;
    b->birthtime = tempBirthtime;
    b->netWorth = tempNetWorth;
}

void sortByName() {
    if (!head) return;
    int swapped;
    billionaire *current;
    do {
        swapped = 0;
        current = head;
        do {
            if (strcmp(current->name, current->next->name) > 0) {
                swap(current, current->next);
                swapped = 1;
            }
            current = current->next;
        } while (current != head);
    } while (swapped);
}

void sortByAge() {
    if (!head) return;
    int swapped;
    billionaire *current;
    do {
        swapped = 0;
        current = head;
        do {
            if ((current->birthdate.year > current->next->birthdate.year) ||
                (current->birthdate.year == current->next->birthdate.year && current->birthdate.month > current->next->birthdate.month) ||
                (current->birthdate.year == current->next->birthdate.year && current->birthdate.month == current->next->birthdate.month && current->birthdate.day > current->next->birthdate.day)) {
                swap(current, current->next);
                swapped = 1;
            }
            current = current->next;
        } while (current != head);
    } while (swapped);
}

void sortByNetWorth() {
    if (!head) return;
    int swapped;
    billionaire *current;
    do {
        swapped = 0;
        current = head;
        do {
            if (current->netWorth < current->next->netWorth) {
                swap(current, current->next);
                swapped = 1;
            }
            current = current->next;
        } while (current != head);
    } while (swapped);
}

void printComparison(char comparator, float value) {
    billionaire *current;
    int found;
    int match;

    if (head == NULL) {
        printf("The list is empty.\n");
        return;
    }

    current = head;
    found = 0;

    do {
        match = 0;
        switch (comparator) {
            case '+':
                if (current->netWorth > value) match = 1;
                break;
            case '-':
                if (current->netWorth < value) match = 1;
                break;
            case '=':
                if (current->netWorth == value) match = 1;
                break;
        }

        if (match) {
            printBillionaire(current);
            found = 1;
        }

        current = current->next;
    } while (current != head);

    if (!found) {
        printf("No billionaire matches the criteria.\n");
    }
}

void printDateComparison(char comparator, Bdate date) {
    billionaire *current;
    int found;
    int match;

    if (head == NULL) {
        printf("The list is empty.\n");
        return;
    }

    current = head;
    found = 0;

    do {
        match = 0;
        switch (comparator) {
            case '+':
                if ((current->birthdate.year > date.year) ||
                    (current->birthdate.year == date.year && current->birthdate.month > date.month) ||
                    (current->birthdate.year == date.year && current->birthdate.month == date.month && current->birthdate.day > date.day)) {
                    match = 1;
                }
                break;
            case '-':
                if ((current->birthdate.year < date.year) ||
                    (current->birthdate.year == date.year && current->birthdate.month < date.month) ||
                    (current->birthdate.year == date.year && current->birthdate.month == date.month && current->birthdate.day < date.day)) {
                    match = 1;
                }
                break;
            case '=':
                if (current->birthdate.year == date.year && current->birthdate.month == date.month && current->birthdate.day == date.day) {
                    match = 1;
                }
                break;
        }

        if (match) {
            printBillionaire(current);
            found = 1;
        }

        current = current->next;
    } while (current != head);

    if (!found) {
        printf("No billionaire matches the criteria.\n");
    }
}

void printTimeComparison(char comparator, Btime time) {
    billionaire *current;
    int found;
    int match;

    if (head == NULL) {
        printf("The list is empty.\n");
        return;
    }

    current = head;
    found = 0;

    do {
        match = 0;
        switch (comparator) {
            case '+':
                if ((current->birthtime.hour > time.hour) ||
                    (current->birthtime.hour == time.hour && current->birthtime.minutes > time.minutes)) {
                    match = 1;
                }
                break;
            case '-':
                if ((current->birthtime.hour < time.hour) ||
                    (current->birthtime.hour == time.hour && current->birthtime.minutes < time.minutes)) {
                    match = 1;
                }
                break;
            case '=':
                if (current->birthtime.hour == time.hour && current->birthtime.minutes == time.minutes) {
                    match = 1;
                }
                break;
        }

        if (match) {
            printBillionaire(current);
            found = 1;
        }

        current = current->next;
    } while (current != head);

    if (!found) {
        printf("No billionaire matches the criteria.\n");
    }
}

void menu() {
    char input[256];
    int numEntries;
    char filename[50];
    float value;
    Bdate date;
    Btime time;

    while (1) {
        printf("Enter command: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strncmp(input, "newbil ", 7) == 0) {
            addBillionaire(input + 7);
        } else if (strcmp(input, "deleteN") == 0) {
            deleteNew();
        } else if (strcmp(input, "deleteO") == 0) {
            deleteOld();
        } else if (strncmp(input, "save ", 5) == 0) {
            saveToFile(input + 5);
        } else if (strncmp(input, "load ", 5) == 0) {
            char *token = strtok(input + 5, " ");
            if (token) {
                numEntries = strtol(token, NULL, 10);
                token = strtok(NULL, " ");
                if (token) {
                    strncpy(filename, token, sizeof(filename));
                    loadFromFile(numEntries, filename);
                } else {
                    printf("Invalid load format.\n");
                }
            } else {
                printf("Invalid load format.\n");
            }
        } else if (strcmp(input, "sum") == 0) {
            printSum();
        } else if (strncmp(input, "findN ", 6) == 0) {
            findBillionaire('N', input + 6);
        } else if (strncmp(input, "findL ", 6) == 0) {
            findBillionaire('L', input + 6);
        } else if (strncmp(input, "findS ", 6) == 0) {
            findBillionaire('S', input + 6);
        } else if (strncmp(input, "print+", 6) == 0 || strncmp(input, "print-", 6) == 0 || strncmp(input, "print=", 6) == 0) {
            char comparator = input[5];
            char *valueStr = input + 7;

            char *token = strtok(valueStr, "/");
            if (token) {
                date.day = strtol(token, NULL, 10);
                token = strtok(NULL, "/");
                if (token) {
                    date.month = strtol(token, NULL, 10);
                    token = strtok(NULL, " ");
                    if (token) {
                        date.year = strtol(token, NULL, 10);
                        if (isValidDate(date.day, date.month, date.year)) {
                            if (comparator == '+') {
                                printDateComparison('+', date);
                            } else if (comparator == '-') {
                                printDateComparison('-', date);
                            } else if (comparator == '=') {
                                printDateComparison('=', date);
                            }
                        } else {
                            printf("Invalid date format.\n");
                        }
                        continue;
                    }
                }
            }

            token = strtok(valueStr, ":");
            if (token) {
                time.hour = strtol(token, NULL, 10);
                token = strtok(NULL, " ");
                if (token) {
                    time.minutes = strtol(token, NULL, 10);
                    if (isValidTime(time.hour, time.minutes)) {
                        if (comparator == '+') {
                            printTimeComparison('+', time);
                        } else if (comparator == '-') {
                            printTimeComparison('-', time);
                        } else if (comparator == '=') {
                            printTimeComparison('=', time);
                        }
                    } else {
                        printf("Invalid time format.\n");
                    }
                    continue;
                }
            }

            token = strtok(valueStr, " ");
            if (token) {
                value = strtof(token, NULL);
                if (comparator == '+') {
                    printComparison('+', value);
                } else if (comparator == '-') {
                    printComparison('-', value);
                } else if (comparator == '=') {
                    printComparison('=', value);
                }
                continue;
            }

            printf("Invalid comparison format.\n");
        } else if (strcmp(input, "sortA") == 0) {
            sortByName();
            printf("List sorted alphabetically by name.\n");
        } else if (strcmp(input, "sortT") == 0) {
            sortByAge();
            printf("List sorted by age from oldest to youngest.\n");
        } else if (strcmp(input, "sortW") == 0) {
            sortByNetWorth();
            printf("List sorted by net worth.\n");
        } else if (strcmp(input, "exit") == 0) {
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid command. Try again.\n");
        }
    }
}

int main() {
    srand(time(NULL));
    initList();
    menu();
    return 0;
}
