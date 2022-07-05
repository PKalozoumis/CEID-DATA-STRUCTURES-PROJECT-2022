#include<stdio.h>
#include"Domes.c"

void swap(Data *a,Data *b);
void quicksort(Data arr[],int low,int high);

void quicksort(Data arr[],int low,int high)
{
    if(low==high)
        return;

    int pivot_location=(high-low+1)/2;
    Data pivot=arr[pivot_location+low];
    swap(&arr[pivot_location+low],&arr[low]);
    int left=low+1;
    int right=high;


    while(left<right)
    {
        if(!(pivot.TEMP>arr[left].TEMP || (pivot.TEMP==arr[left].TEMP&&compareDates(&pivot.date, &arr[left].date) != -1))&&!(pivot.TEMP<arr[right].TEMP || (pivot.TEMP==arr[right].TEMP&&compareDates(&pivot.date, &arr[right].date) != 1)))
        {
            swap(&arr[left],&arr[right]);
        }
        if(pivot.TEMP>arr[left].TEMP || (pivot.TEMP==arr[left].TEMP&&compareDates(&pivot.date, &arr[left].date) != -1))
        {
            left++;
        }
        if(pivot.TEMP<arr[right].TEMP || (pivot.TEMP==arr[right].TEMP&&compareDates(&pivot.date, &arr[right].date) != 1))
        {
            right--;
        }
    }

    if(arr[right].TEMP<pivot.TEMP || (arr[right].TEMP == pivot.TEMP && compareDates(&arr[right].date, &pivot.date) == -1))
    {
        swap(&arr[low],&arr[right]);
        pivot_location=right;
    }
    else if(right!=low)
    {
        swap(&arr[low],&arr[right-1]);
        pivot_location=right-1;
    }

    if(pivot_location-1>low)
        quicksort(arr,low,pivot_location-1);
    if(pivot_location+1<high)
        quicksort(arr,pivot_location+1,high);

}


void swap(Data *a,Data *b)
{
    Data tmp1=*a;

    *a=*b;
    *b=tmp1;
}