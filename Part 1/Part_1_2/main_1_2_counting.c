#include<stdio.h>
#include<time.h>
#include"HeapSortFinal.c"

Data* countingSort(Data arr[],int size);
int* findRange(Data arr[],int size);

int main()
{
    char* fname = "ocean.csv";

    int* c = fileLinesSize(fname);

    Data* dataCount = readCsv(fname, c[0], c[1]);
    Data* dataHeap=(Data*)malloc(c[0]*sizeof(Data));
    memcpy(dataHeap,dataCount,c[0]*sizeof(Data));

    clock_t insert=clock();
    dataCount = countingSort(dataCount,c[0]);
    double result1=(double)(clock()-insert)/CLOCKS_PER_SEC;

    insert=clock();
    heapSort(dataHeap,c[0]);
    double result2=(double)(clock()-insert)/CLOCKS_PER_SEC;

    printf("\nCounting Sort Time: %lf\n", result1);
    printf("Heap Sort Time: %lf\n\n", result2);

    exportToCsv(dataCount,"CountingSortResult.csv",c[0]);
    exportToCsv(dataHeap,"HeapSortResult.csv",c[0]);

    free(dataCount);
    free(dataHeap);
    free(c);
}

int* findRange(Data arr[],int size)
{
    float min=0, max=0;

    for(int i=0; i<size; i++)
    {
        if(min>arr[i].PHOSPHATE)
            min=arr[i].PHOSPHATE;

        if(max<arr[i].PHOSPHATE)
            max=arr[i].PHOSPHATE;
    }

    int *arr1=(int*)malloc(sizeof(int));

    arr1[0]=min*100;
    arr1[1]=(max-min)*100+1;

    return arr1;

}

Data* countingSort(Data arr[],int size)
{
    int *phosChar=findRange(arr,size);
    int min=phosChar[0];
    int range=phosChar[1];
    int *helper=(int*)calloc(range,sizeof(int));
    //memset(helper, 0, sizeof (int) * range);

    int index=0;
    for(int i=0; i<size; i++)
    {
        index=arr[i].PHOSPHATE*100;
         helper[index-min]++;
    }


    for(int i=1; i<range; i++)
        helper[i]=helper[i]+helper[i-1];

    Data *sortedData=(Data*)malloc(size*sizeof(Data));

    for(int i=size-1; i>=0; i--)
    {
        index=arr[i].PHOSPHATE*100;
        sortedData[helper[index-min]-1]=arr[i];
        helper[index-min]--;
    }

    free(phosChar);
    free(helper);

    return sortedData;

}

