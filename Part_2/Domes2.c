#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUCKETS 16

#define TEMP values[0]
#define PHOSPHATE values[1]
#define SILICATE values[2]
#define NITRITE values[3]
#define NITRATE values[4]
#define SALINITY values[5]
#define OXYGEN values[6]

typedef struct
{
    int day;
    int month;
    int year;
} Date;

typedef struct Data
{
    Date date;
    float values[7];
}
Data;

typedef struct AVLnode
{
    Date* date;
    double* temperature;
    int mult;
    struct AVLnode *father;
    struct AVLnode *leftChild;
    struct AVLnode *rightChild;
    int height;
    int hb;
} AVLnode;

typedef struct node{
    struct node * next;
    Date date;
    float temp;
}
Node;

typedef struct{
    Node * first;
    Node * last;
}
Bucket;

int compareDates(const Date *d1, const Date *d2);
void menuOption(int* option, int lower, int upper);
Date inputDate(void);

// CSV FUNCTIONS
int* fileLinesSize(char* filename);
char* readLine(FILE* file, int linesize);
Data makeData(char* line);
Data* readCsv(char* filename, int maxsize, int linesize);
void exportToCsv(const Data* data, char* filename, int maxsize);

// PRINTING FUNCTIONS
void printData(const Data* data);
void printAllData(const Data* data, int maxsize);

int compareDates(const Date *d1, const Date *d2)
{
    if (d1->year > d2->year)
        return 1;

    if (d1->year < d2->year)
        return -1;

    if (d1->month > d2->month)
        return 1;

    if (d1->month < d2->month)
        return -1;

    if (d1->day > d2->day)
        return 1;

    if (d1->day < d2->day)
        return -1;

    return 0;
}

void menuOption(int* option, int lower, int upper)
{
    *option = lower;

    do
    {
        if ((*option < lower)||(*option > upper))
            printf("Invalid input.\n");

        printf("\nYour input: ");
        scanf("%d", option);
    }
    while ((*option < lower)||(*option > upper));
}

Date inputDate(void)
{
    int day,month,year;

    printf("Please input a date:\n");

    do
    {
        printf("\nDay: ");
        scanf("%d", &day);

        if ((day < 1)||(day > 31))
            printf("Day must be between 1 and 31!\n");
    }
    while((day < 1)||(day > 31));

    do
    {
        printf("\nMonth: ");
        scanf("%d", &month);

        if ((month < 1)||(month > 12))
            printf("Month must be between 1 and 12!\n");
    }
    while((month < 1)||(month > 12));

    do
    {
        printf("\nYear: ");
        scanf("%d", &year);

        if ((year < 2000)||(year > 2019))
            printf("Year must be between 2000 and 2019!\n");
    }
    while((year < 2000)||(year > 2019));

    Date newDate = {day,month,year};

    return newDate;
}

//=========================================================================================

/*
fileLinesSize
------------------------
Accepts the name of the file and returns an array of two cells:

    >array[0] -> the number of structs needed to store the .csv file. Expected value for ocean.csv: 1405
    >array[1] -> the maximum number of characters in a line (including newline character). Expected value for ocean.csv: 51

WARNING: THE FILE MUST HAVE AN EMPTY LINE AT THE END
*/

int *fileLinesSize(char *filename)
{
    FILE *file;

    if ((file = fopen(filename, "r")) == NULL)
    {
        printf("%s", "ERROR: COULD NOT READ FILE");
        exit(-1);
    }

    int *sizeCounter = (int *)malloc(2 * sizeof(int));
    sizeCounter[0]=sizeCounter[1]=0;
    char c;
    int maxLenCounter = 0;

    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n')
        {
            sizeCounter[0]++;

            //New maximum
            if (maxLenCounter > sizeCounter[1])
                sizeCounter[1] = maxLenCounter;

            maxLenCounter = 0;
        }

        maxLenCounter++;
    }

    sizeCounter[1]++;
    sizeCounter[0]--;
    fclose(file);

    return sizeCounter;
}

//=========================================================================================

/*
readLine
------------------------
Accepts a file pointer and the maximum number of characters in a line.
Returns a string of the next line in the file
*/

char* readLine(FILE* file, int linesize)
{
    //Allocating enough space for (Max characters + newline) + \0
    char* str = (char*)malloc((linesize+1)*sizeof(char));

    if (fgets(str, linesize+1, file) != NULL)
    {
        //Removing the newline character at the end of the line and resizing the allocated buffer
        str[strlen(str)-1] = '\0';
        str  = (char*)realloc(str, (strlen(str)+1)*sizeof(char*));
        return str;
    }
}

//=========================================================================================

/*
makeData
------------------------
Accepts a string representing a line from a .csv file.
The string is broken into tokens, using the comma as a delimiter.
The tokens are then converted and stored in a Data struct.
*/
Data makeData(char* line) //function that makes a Data Object
{
    Data newData;
    int i = 0;
    char* dateString;

    char* token = strtok(line, ","); //it seperates the arraym and the seperator is the ',' character
    while(token)
    {
        if (i == 0)
        {
            dateString = (char*)malloc((strlen(token)+1)*sizeof(char)); //given the length of the line tha we raed ,we allocate memory for the string object
            strcpy(dateString, token); //and then we copy it
        }
        else newData.values[i-1] = atof(token); //save the other data in the array

        token = strtok(NULL, ",");//line of code that exists for strtok to work properly :)(basically works in the same string )
        i++; //counter for the data of the array
    }

    i = 0;

    token = strtok(dateString, "/");// same procidure like above but now we want ot store the values of the date
    while(token)
    {
        if (i == 0) // for these lines we make the values double and save them in the wright spot of the array
            newData.date.month = atoi(token);
        else if (i == 1)
            newData.date.day = atoi(token);
        else newData.date.year = atoi(token);

        token = strtok(NULL, "/");
        i++;
    }

    free(dateString); //freeing the allocated memory :)

    return newData;
}

//=========================================================================================

/*
readCsv
------------------------
Accepts the name of a .csv file and returns an array of Data structs
*/
Data* readCsv(char* filename, int maxsize, int linesize)
{
    Data* array;
    FILE* newFile;

    if ((newFile = fopen(filename, "r")) == NULL)
    {
        printf("%s", "ERROR: COULD NOT READ FILE");
        exit(-1);
    }

    array = (Data*)malloc(maxsize*sizeof(Data));
    if (array == NULL)
    {
        printf("%s", "ERROR: COULD NOT ALLOCATE THE REQUESTED MEMORY");
        exit(-1);
    }

    free(readLine(newFile, linesize)); //Get rid of the first line before reading the other ones

    for (int i = 0; i < maxsize; i++)
    {
        char* s2 = readLine(newFile,linesize);
        array[i] = makeData(s2);
        free(s2);
    }

    fclose(newFile);

    return array;
}

//=========================================================================================

/*
exportToCsv
------------------------
Saves the contents of a Data array to an external .csv file
*/
void exportToCsv(const Data* data, char* filename, int maxsize)
{
    FILE* newFile;

    if ((newFile = fopen(filename, "w")) == NULL)
    {
        printf("%s", "ERROR: COULD NOT CREATE FILE");
        exit(-1);
    }

    if (fprintf(newFile,"Date\t\tT_degC\tPO4uM\tSiO3uM\tNO2uM\tNO3uM\tSalnty\tO2ml_L\n") < 0)
    {
        printf("%s", "ERROR: COULD NOT PRINT TO FILE");
        exit(-1);
    }

    for (int i = 0; i < maxsize; i++)
    {
        if (fprintf(newFile, "%02d/%02d/%02d,\t%06.3f,\t%06.3f,\t%06.3f,\t%06.3f,\t%06.3f,\t%06.3f,\t%06.3f\n",
                    data[i].date.month,
                    data[i].date.day,
                    data[i].date.year,
                    data[i].values[0],
                    data[i].values[1],
                    data[i].values[2],
                    data[i].values[3],
                    data[i].values[4],
                    data[i].values[5],
                    data[i].values[6]) < 0)
        {
            printf("%s", "ERROR: COULD NOT PRINT TO FILE");
            exit(-1);
        }
    }

    fclose(newFile);
}

//=========================================================================================

/*
printData
------------------------
Prints the contents of a Data struct
*/
void printData(const Data* data)
{
    printf("%s: %02d/%02d/%d\n","Date", data->date.day, data->date.month, data->date.year);
    printf("%s: %.3f\n", "Temperature", data->TEMP);
    printf("%s: %.3f\n", "Phosphate", data->PHOSPHATE);
    printf("%s: %.3f\n", "Silicate", data->SILICATE);
    printf("%s: %.3f\n", "Nitrite", data->NITRITE);
    printf("%s: %.3f\n", "Nitrate", data->NITRATE);
    printf("%s: %.3f\n", "Salinity", data->SALINITY);
    printf("%s: %.3f\n", "Oxygen", data->OXYGEN);
}

//=========================================================================================

/*
printAllData
------------------------
Prints all the contents of a Data struct array
*/
void printAllData(const Data* data, int maxsize)
{
    printf("1\tDate\t\tT_degC\tPO4uM\tSiO3uM\tNO2uM\tNO3uM\tSalnty\tO2ml_L\n");

    for (int i = 0; i < maxsize; i++)
    {
        printf("%d", i+2);
        printf("\t%02d/%02d/%d\t", data[i].date.day, data[i].date.month, data[i].date.year);
        printf("%06.3f\t", data[i].TEMP);
        printf("%06.3f\t", data[i].PHOSPHATE);
        printf("%06.3f\t", data[i].SILICATE);
        printf("%06.3f\t", data[i].NITRITE);
        printf("%06.3f\t", data[i].NITRATE);
        printf("%06.3f\t", data[i].SALINITY);
        printf("%06.3f", data[i].OXYGEN);

        printf("\n");
    }
}

//=========================================================================================
