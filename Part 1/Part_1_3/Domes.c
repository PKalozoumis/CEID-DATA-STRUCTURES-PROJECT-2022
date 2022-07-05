#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
}Date;

typedef struct
{
    Date date;
    float values[7];
}Data;

int compareDates(const Date* d1, const Date* d2);

// CSV FUNCTIONS
int* fileLinesSize(char* filename);
char* readLine(FILE* file, int linesize);
Data makeData(char* line);
Data* readCsv(char* filename, int maxsize, int linesize);
void exportToCsv(const Data* data, char* filename, int maxsize);

//DATE FUNCTIONS
int compareDates(const Date* d1, const Date* d2);

//PRINTING FUNCTIONS
void printData(const Data* data);
void printAllData(const Data* data, int maxsize);

/*
fileLinesSize
------------------------
Accepts the name of the file and returns an array of two cells:

    >array[0] -> the number of structs needed to store the .csv file. Expected value for ocean.csv: 1405
    >array[1] -> the maximum number of characters in a line (including newline character). Expected value for ocean.csv: 51

WARNING: THE FILE MUST HAVE AN EMPTY LINE AT THE END
*/
int *fileLinesSize(char *filename) //function that returns the numbers of lines of a file and
{                                  //and the number of characters of the longest line
    FILE *file;

    if ((file = fopen(filename, "r")) == NULL)
    {
        printf("%s", "ERROR: COULD NOT READ FILE"); //red tape :)
        exit(-1);
    }

    int *sizeCounter = (int *)malloc(2 * sizeof(int)); //in order to return the array we use malloc,so we can return the pointer
    sizeCounter[0]=sizeCounter[1]=0; //the first objects is the counter of the lines and the second of the max characters
    char c;
    int maxLenCounter = 0; //secondary counter helping the process
    while ((c = fgetc(file)) != EOF) //while the character is not the end of the file
    {
        if (c == '\n')
        {
            sizeCounter[0]++; //every time we meet a new line we increase the counter

            if (maxLenCounter > sizeCounter[1])
                sizeCounter[1] = maxLenCounter; //when we find a line with more characters we save it

            maxLenCounter = 0; //making the secondary counter zero so he can count the next line's characters
        }

        maxLenCounter++; //counting the characters of the next line
    }

    sizeCounter[1]++; //including the /0 character
    sizeCounter[0]--; //the first line isnt information
    fclose(file); //closing the file :)

    return sizeCounter; //return the pointer
}

/*
readLine
------------------------
Accepts a file pointer and the maximum number of characters in a line.
Returns a string of the next line in the file
*/
char* readLine(FILE* file, int linesize)
{
    char* str = (char*)malloc((linesize+1)*sizeof(char)); //Max characters + newline + \0

    if (fgets(str, linesize+1, file) != NULL)
    {
        if (str[strlen(str)-1] == '\n')
        {
            str[strlen(str)-1] = '\0';
        }
        str  = (char*)realloc(str, (strlen(str)+1)*sizeof(char*));
        return str;
    }
}

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

/*
compareDates
------------------------
Compares two given dates d1 and d2. Returns:
    -1 if d1 is older than d2
    0 if d1 is the same as d2
    1 if d1 is more recent than d2
*/
int compareDates(const Date* d1, const Date* d2)
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
