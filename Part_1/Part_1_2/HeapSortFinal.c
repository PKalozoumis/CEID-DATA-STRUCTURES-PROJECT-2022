#include<stdio.h>
#include"Domes.c"

void swap(Data *a,Data *b);
void heapify(Data arr[],int size,int tmpFather);
void heapSort(Data arr[],int size);

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
                if(arr[tmpChild].PHOSPHATE<arr[tmpChild+1].PHOSPHATE)
                    tmpChild++;
                else if(compareDates(&arr[tmpChild].date,&arr[tmpChild+1].date)==-1&&arr[tmpChild].PHOSPHATE==arr[tmpChild+1].PHOSPHATE)
                    tmpChild++;
            }
            if((tmp.PHOSPHATE<arr[tmpChild].PHOSPHATE)||(compareDates(&arr[tmpChild].date,&tmp.date)==1&&arr[tmpChild].PHOSPHATE==tmp.PHOSPHATE))
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
