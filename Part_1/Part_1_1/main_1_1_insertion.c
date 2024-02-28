#include <stdio.h>
#include <string.h>
#include <time.h>
#include "quicksort.c"

void insertionSort(Data arr[],int size);

int main()
{
    char* fname = "ocean.csv";

    int* c = fileLinesSize(fname);

    Data* dataInsert = readCsv(fname, c[0], c[1]);
    Data* dataQuick=(Data*)malloc(c[0]*sizeof(Data));
    memcpy(dataQuick,dataInsert,c[0]*sizeof(Data));

    clock_t insert=clock();
    insertionSort(dataInsert,c[0]);
    double result1=(double)(clock()-insert)/CLOCKS_PER_SEC;

   insert=clock();
   quicksort(dataQuick,0,c[0]-1);
    double result2=(double)(clock()-insert)/CLOCKS_PER_SEC;

    printf("\nInsertion Sort Time: %lf\n", result1);
    printf("Quick Sort Time: %lf\n\n", result2);

    exportToCsv(dataInsert,"InsertionSortResult.csv",c[0]);
    exportToCsv(dataQuick,"QuickSortResult.csv",c[0]);

    free(dataInsert);
    free(dataQuick);
    free(c);
}

void insertionSort(Data arr[],int size)
{
    int i=0,j=0; //the two counters,one for every object of the array and another one to check all the smaller objects
    Data key; //the object tha we want to check

    for( i=1; i<size; i++)
    {
        key=arr[i];  //initializing the key
        j=i-1; //check all smaller objects of the array with the second counter
        while(j>=0&&(key.TEMP<arr[j].TEMP)||(key.TEMP==arr[j].TEMP&&compareDates(&key.date, &arr[j].date)==-1)) //if the temperature value of the key is lower than the other object
        {                                                                                                       //or the key has the same temperature but smaller date
            arr[j+1]=arr[j]; //we move the object one position ahead
            j--; //and check the smaller objects
        }
    arr[j+1]=key; //when the process is finished we put the key in the "blank" spot
    }
}
