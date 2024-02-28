#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Domes.c"

// DATE FUNCTIONS
int isLeapYear(int year);
int dateDifference(const Date* endDate, const Date* startDate);
void sortDates(Data* data, int maxsize);
Date inputDate(void);

// SEARCHING FUNCTIONS
int binSearch(const Data* data, int* left, int* right, const Date* date, const int maxIndex, int* steps);
int interSearch(const Data* data, int* left, int* right, const Date* date, const int maxIndex, int* steps);

//=========================================================================================

int main(void)
{
    Data* array;
    int* c = fileLinesSize("ocean.csv");
    array = readCsv("ocean.csv", c[0], c[1]);

    sortDates(array, c[0]);
    exportToCsv(array, "sortedDates.csv", c[0]);

    int option;

    do
    {
        Date newDate = inputDate();

        printf("\n");

        int steps1 = 0;
        int left1 = 0, right1 = c[0]-1;
        clock_t t1 = clock();
        int found1 = binSearch(array, &left1, &right1, &newDate, c[0]-1, &steps1);
        t1 = clock() - t1;

        int steps2 = 0;
        int left2 = 0, right2 = c[0]-1;
        clock_t t2 = clock();
        int found2 = interSearch(array, &left2, &right2, &newDate, c[0]-1, &steps2);
        t2 = clock() - t2;

        printf("============= BINARY SEARCH RESULTS =============\n\n");
        if (found1 == 0)
            printf("There is no data available for the requested date\n\n");
        else
        {
            for (int i = left1; i <= right1; i++)
                printf("Temperature: %.3f\nPhosphate: %.3f\n\n", array[i].TEMP, array[i].PHOSPHATE);

            printf("Binary Search Time: %lf (%d steps)\n", (double)t1/CLOCKS_PER_SEC, steps1);

        }
        printf("\n==================================================\n\n\n");

        printf("========== INTERPOLATION SEARCH RESULTS ==========\n\n");
        if (found2 == 0)
            printf("There is no data available for the requested date\n\n");
        else
        {
            for (int i = left2; i <= right2; i++)
                printf("Temperature: %.3f\nPhosphate: %.3f\n\n", array[i].TEMP, array[i].PHOSPHATE);

            printf("Interpolation Search Time: %lf (%d steps)\n", (double)t2/CLOCKS_PER_SEC, steps2);

        }
        printf("\n==================================================\n\n");

        printf("What will you do?\n\t1.Input another date\n\t2.Exit\n");

        do
        {
            printf("\nYour input: ");
            scanf("%d", &option);

            if ((option != 1)&&(option != 2))
                printf("Invalid input!\n");
        }
        while ((option != 1)&&(option != 2));

        printf("\n");
    }
    while (option != 2);

    free(array);
    free(c);

    return 0;
}

//=========================================================================================

/*
isLeapYear
------------------------
Returns 1 if the given year is a leap year, 0 otherwise.
*/
int isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

//=========================================================================================

/*
dateDifference
------------------------
Calculates the difference in days between two given dates
(not including the final day)
*/
int dateDifference(const Date* endDate, const Date* startDate)
{
    static int daysOf[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int days;
    int monthDif = endDate->month - startDate->month;
    int yearDif = endDate->year - startDate->year;

    if (endDate->year != startDate->year)
    {
        /*The calculation is split into three parts:
            >endDate - 1/1/endDate->year
            >calculating the days of all intermediate years
            >31/12/startDate->year - startDate
        */

        days = endDate->day; //Would be -1 (for the 1st of January), but we add back the 31st of December of endDate->year - 1

        //Adding the days since the start of the year
        for (int i = 1; i <= endDate->month-1; i++)
        {
            days += daysOf[endDate->month - i - 1];
            if (endDate->month - i - 1 == 1)
            {
                days += isLeapYear(endDate->year);
            }
        }

        //Adding the days until we reach the start year
        for (int i = 1; i < yearDif; i++)
        {
            days += 365 + isLeapYear(endDate->year - i);
        }

        //Calculating 31/12/startDate->year - startDate
        days += 31 - startDate->day;
        for (int i = 1; i <= 12-startDate->month; i++)
        {
            days += daysOf[11 - i];
            if (11 - i == 1)
            {
                days += isLeapYear(startDate->year);
            }
        }
    }
    else
    {
        days = endDate->day - startDate->day;
        for (int i = 1; i <= monthDif; i++)
        {
            days += daysOf[endDate->month - i - 1];
            if (endDate->month - i - 1 == 1)
            {
                days += isLeapYear(endDate->year);
            }
        }
    }

    return days;
}

//=========================================================================================

/*
sortDates
------------------------
Sorts the contents of a Data array by dates using insertion sort.
The contents are already almost sorted, so insertion sort is ideal.
*/
void sortDates(Data* data, int maxsize)
{
    int wall;
    Data temp;

    for (int i = 1; i < maxsize; i++)
    {
        wall = i-1;
        temp = data[i];

        while ((wall >= 0)&&(compareDates(&(temp.date), &(data[wall].date)) == -1))
        {
            data[wall+1] = data[wall];
            wall--;
        }

        data[wall+1] = temp;
    }
}

//=========================================================================================

int binSearch(const Data* data, int* left, int* right, const Date* date, const int maxIndex, int* steps)
{
    *steps = *steps + 1;

    if (*left > *right || compareDates(date, &data[*left].date) == -1 || compareDates(date, &data[*right].date) == 1)
    {
        return 0;
    }

    int arrayMid = (*left + *right)/2;

    switch(compareDates(date, &data[arrayMid].date))
    {
        case -1:
        {
            *right = arrayMid - 1;
            binSearch(data, left, right, date, maxIndex, steps);
        }
        break;

        case 0:
        {
            //Left and right will now become the limits of the sub-array where every element is the date we want

            *left = arrayMid;
            *right = arrayMid;

            while((*left > 0) && (compareDates(date, &data[*left-1].date) == 0))
                *left = *left - 1;

            while((*right < maxIndex) && (compareDates(date, &data[*right+1].date) == 0))
                *right = *right + 1;

            return 1;
        }
        break;

        case 1:
        {
            *left = arrayMid + 1;
            return binSearch(data, left, right, date, maxIndex, steps);
        }
        break;
    }
}

//=========================================================================================

int interSearch(const Data* data, int* left,int* right, const Date* date, const int maxIndex, int* steps)
{
    *steps = *steps + 1;

    if (*left > *right || compareDates(date, &data[*left].date) == -1 || compareDates(date, &data[*right].date) == 1)
        return 0;

    int next = ((double)(dateDifference(date, &data[*left].date))/(dateDifference(&data[*right].date, &data[*left].date)))*(*right-*left) + *left;

    switch(compareDates(date, &data[next].date))
    {
        case -1:
        {
            *right = next-1;
            return interSearch(data, left, right, date, maxIndex, steps);
        }
        break;

        case 0:
        {
            //Left and right will now become the limits of the sub-array where every element is the date we want

            *left = next;
            *right = next;

            while((*left > 0) && (compareDates(date, &data[*left-1].date) == 0))
                *left = *left - 1;

            while((*right < maxIndex) && (compareDates(date, &data[*right+1].date) == 0))
                *right = *right + 1;

            return 1;
        }
        break;

        case 1:
        {
            *left = next+1;
            return interSearch(data, left, right, date, maxIndex, steps);
        }
        break;
    }
}

//=========================================================================================

/*
inputDate
------------------------
Allows the user to input a date. Returns a struct with the date
*/
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
