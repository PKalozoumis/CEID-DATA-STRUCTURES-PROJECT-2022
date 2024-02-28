#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Domes.c"
#define YEARZERO 2000 //The year relative to which the unique value of a date is calculated.

int sqroot(int num); //Calculates the integer part of the square root of num.
int binpow(int num); //Calculates 2 to the power of num.
int dateValue(Date date); //Calculates the unique value of a date.
int * bis(Data * data, Date date, int maxsize); //Returns a pointer to an array containing the positions of all matching instances, preceded by the size of the array.
int * altbis(Data * data, Date date, int maxsize); //Alternative BIS that uses 2^i instead of i multiplied by the square root of the size.
void swap(Data *a,Data *b);
void heapify(Data arr[],int size,int tmpFather);
void heapSort(Data arr[],int size);

int main(){
    Data* array;
    int* c = fileLinesSize("ocean.csv");
    array = readCsv("ocean.csv", c[0], c[1]);

    heapSort(array, c[0]);
    exportToCsv(array, "sortedDates.csv", c[0]);
    printAllData(array, c[0]);

    int day, month, year;
    int option;

    do
    {
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

        printf("\n");

        clock_t t1 = clock();
        int * pos = bis(array, newDate, c[0]);
        t1 = clock() - t1;

        clock_t t2 = clock();
        int * altpos = altbis(array, newDate, c[0]);
        t2 = clock() - t2;

        if (pos == NULL)
            printf("There is no data available for the requested date\n\n");
        else
        {
            for(int i = 1; i < pos[0]; i++) printf("BIS - Temperature: %.3f\nPhosphate: %.3f\n", array[pos[i]].TEMP, array[pos[i]].PHOSPHATE);
            printf("\nBIS Search Time: %lf\n\n", (double)t1/CLOCKS_PER_SEC);
        }

        if (altpos == NULL)
            printf("There is no data available for the requested date\n\n");
        else
        {
            for(int i = 1; i < pos[0]; i++) printf("Alt BIS - Temperature: %.3f\nPhosphate: %.3f\n", array[altpos[i]].TEMP, array[altpos[i]].PHOSPHATE);
            printf("\nAlt BIS Search Time: %lf\n\n", (double)t2/CLOCKS_PER_SEC);
        }

        printf("What will you do?\n1) Input another date\n2) Exit\n");

        do
        {
            printf("\nYour option: ");
            scanf("%d", &option);

            if ((option != 1)&&(option != 2))
                printf("Invalid option!\n");
        }
        while ((option != 1)&&(option != 2));
    }
    while (option != 2);

    free(array);
    free(c);

    return 0;
}

int sqroot(int num){
    int root = 0;
    while(1){
        root++;
        if(root*root > num) return root - 1;
    }
}

int binpow(int num){
    if(num < 0) return 0;
    int pow = 1;
    for(int i = 0; i < num; i++) pow *= 2;
    return pow;
}

int dateValue(Date date){
    int value = date.day;
    for(int i = 1; i < date.month; i++){
        if(i == 4 || i == 6 || i == 9 || i == 11) value += 30;
        else if(i == 2){
            if(date.year % 400 == 0 || (date.year % 100 != 0 && date.year % 4 == 0)) value += 29;
            else value += 28;
        }
        else value += 31;
    }
    for(int i = YEARZERO; i < date.year; i++){
        if(date.year % 400 == 0 || (date.year % 100 != 0 && date.year % 4 == 0)) value += 366;
        else value += 365;
    }
    for(int i = YEARZERO; i > date.year; i--){
        if(date.year % 400 == 0 || (date.year % 100 != 0 && date.year % 4 == 0)) value -= 366;
        else value -= 365;
    }
    return value;
}

int * bis(Data * sortedData, Date date, int maxsize){
    int left = 0;
    int right = maxsize - 1;
    int size = maxsize;
    if(dateValue(date) < dateValue(sortedData[left].date) || dateValue(date) > dateValue(sortedData[right].date)) return NULL; //Check if the date can be within the array.
    int next = size * (dateValue(date) - dateValue(sortedData[left].date)) / (dateValue(sortedData[right].date) - dateValue(sortedData[left].date));
    if(next > right) next = right; //In the case that the wanted date is at sortedData[right], next is out of bounds, so next is returned within bounds.
    while(dateValue(date) != dateValue(sortedData[next].date) && right > left){
        int i = 0;
        if(dateValue(date) > dateValue(sortedData[next].date)){
            while(dateValue(date) > dateValue(sortedData[next + sqroot(i * i * size)].date)){
                i++;
                if(next + sqroot(i * i * size) > right) break; //If this value goes out of bounds there is no reason to continue checking and might cause error.
            }
            left = next + 1 + sqroot((i - 1) * (i - 1) * size);
            if(next + sqroot(i * i * size) < right){ //Only alter right if the new value is within bounds.
                right = next + sqroot(i * i * size); //Otherwise the old value of right is better.
            }
        }
        else{
            while(dateValue(date) < dateValue(sortedData[next - sqroot(i * i * size)].date)){
                i++;
                if(next - sqroot(i * i * size) < left) break; //If this value goes out of bounds there is no reason to continue checking and might cause error.
            }
            if(next - sqroot(i * i * size) > left){ //Only alter left if the new value is within bounds.
                left = next - sqroot(i * i * size); //Otherwise the old value of left is better.
            }
            right = next - 1 - sqroot((i - 1) * (i - 1) * size);
        }
        size = right - left + 1;
        if(dateValue(sortedData[left].date) == dateValue(sortedData[right].date)){ //If the left/right values are identical, the normal procedure divides by 0.
            next = left; //Therefore left is assigned to next as, since left/right are identical and the array is sorted, all dates within bounds are identical.
        }
        else next = left + size * (dateValue(date) - dateValue(sortedData[left].date)) / (dateValue(sortedData[right].date) - dateValue(sortedData[left].date));
        if(next > right) next = right; //In the case that the wanted date is at sortedData[right], next is out of bounds, so next is returned within bounds.
    }
    if(dateValue(date) == dateValue(sortedData[next].date)){ //In the case that one instance has been found we need to find all other acceptable instances.
        int left = next, right = next;
        while(1){
            left--;
            if(left < 0 || dateValue(date) != dateValue(sortedData[left].date)) break;
        }
        while(1){
            right++;
            if(right > maxsize - 1 || dateValue(date) != dateValue(sortedData[right].date)) break;
        } //From next, a net is expanded to the left and to the right that contains all the instances that have the matching dates.
        int * pos = (int *)malloc(sizeof(int)*(right - left));
        pos[0] = right - left; //The size of the array is assigned to the first slot.
        for(int i = 1; i < right - left; i++){
            pos[i] = left + i; //Followed by all instances with matching dates.
        }
        return pos;
    }
    else{
        return NULL;
    }
}

int * altbis(Data * sortedData, Date date, int maxsize){
    int left = 0;
    int right = maxsize - 1;
    int size = maxsize;
    if(dateValue(date) < dateValue(sortedData[left].date) || dateValue(date) > dateValue(sortedData[right].date)) return NULL; //Check if the date can be within the array.
    int next = size * (dateValue(date) - dateValue(sortedData[left].date)) / (dateValue(sortedData[right].date) - dateValue(sortedData[left].date));
    if(next > right) next = right; //In the case that the wanted date is at sortedData[right], next is out of bounds, so next is returned within bounds.
    while(dateValue(date) != dateValue(sortedData[next].date) && right > left){
        int i = -1;
        if(dateValue(date) > dateValue(sortedData[next].date)){
            while(dateValue(date) > dateValue(sortedData[next + sqroot(binpow(2 * i) * size)].date)){
                i++;
                if(next + sqroot(binpow(2 * i) * size) > right) break; //If this value goes out of bounds there is no reason to continue checking and might cause error.
            }
            left = next + 1 + sqroot(binpow(2 * (i - 1)) * size);
            if(next + sqroot(binpow(2 * i) * size) < right){ //Only alter right if the new value is within bounds.
                right = next + sqroot(binpow(2 * i) * size); //Otherwise the old value of right is better.
            }
        }
        else{
            while(dateValue(date) < dateValue(sortedData[next - sqroot(binpow(2 * i) * size)].date)){
                i++;
                if(next - sqroot(binpow(2 * i) * size) < left) break; //If this value goes out of bounds there is no reason to continue checking and might cause error.
            }
            if(next - sqroot(binpow(2 * i) * size) > left){ //Only alter left if the new value is within bounds.
                left = next - sqroot(binpow(2 * i) * size); //Otherwise the old value of left is better.
            }
            right = next - 1 - sqroot(binpow(2 * (i - 1)) * size);
        }
        size = right - left + 1;
        if(dateValue(sortedData[left].date) == dateValue(sortedData[right].date)){ //If the left/right values are identical, the normal procedure divides by 0.
            next = left; //Therefore left is assigned to next as, since left/right are identical and the array is sorted, all dates within bounds are identical.
        }
        else next = left + size * (dateValue(date) - dateValue(sortedData[left].date)) / (dateValue(sortedData[right].date) - dateValue(sortedData[left].date));
        if(next > right) next = right; //In the case that the wanted date is at sortedData[right], next is out of bounds, so next is returned within bounds.
    }
    if(dateValue(date) == dateValue(sortedData[next].date)){ //In the case that one instance has been found we need to find all other acceptable instances.
        int left = next, right = next;
        while(1){
            left--;
            if(left < 0 || dateValue(date) != dateValue(sortedData[left].date)) break;
        }
        while(1){
            right++;
            if(right > maxsize - 1 || dateValue(date) != dateValue(sortedData[right].date)) break;
        } //From next, a net is expanded to the left and to the right that contains all the instances that have the matching dates.
        int * pos = (int *)malloc(sizeof(int)*(right - left));
        pos[0] = right - left; //The size of the array is assigned to the first slot.
        for(int i = 1; i < right - left; i++){
            pos[i] = left + i; //Followed by all instances with matching dates.
        }
        return pos;
    }
    else{
        return NULL;
    }
}

void heapSort(Data arr[],int size)
{
    int father=size/2;
    int SortCounter=size-1;

    while(SortCounter>=1)
    {
        if(father>0)
        {
            father--;
        }
        else
        {
            swap(arr,&arr[SortCounter]);
            SortCounter--;
            father=0;
        }

        heapify(arr,SortCounter,father);

    }
}

void heapify(Data arr[],int size,int tmpFather)
{
   Data tmp=arr[tmpFather];
   int tmpChild=0;

   while(2*tmpFather+1<=size)
        {
            tmpChild=2*tmpFather+1;

            if(tmpChild<size)
            {
                if(dateValue(arr[tmpChild].date)<dateValue(arr[tmpChild+1].date))
                    tmpChild++;
            }
            if(dateValue(tmp.date)<dateValue(arr[tmpChild].date))
            {
                arr[tmpFather]=arr[tmpChild];
                tmpFather=tmpChild;
            }
            else
            {
                break;
            }
        }
    arr[tmpFather]=tmp;
}


void swap(Data *a,Data *b)
{
    Data tmp1=*a;

    *a=*b;
    *b=tmp1;
}
