#include <stdio.h>
#include <stdlib.h>
#include "Domes2.c"
#include <string.h>

#define COUNT 10

//AVL FUNCTIONS
void readDateandTemp(char *filename, AVLnode **tree, int treeType);

AVLnode *dateSearch(AVLnode *tree, Date date, int *found);
AVLnode *tempSearch(AVLnode *tree, double temp, int *found);

void insertDateAVLnode(AVLnode **tree, double temp, Date date);
void insertTempAVLnode(AVLnode **tree, double temp, Date date);

void deleteAVLnode(AVLnode **tree, Date date);

AVLnode *leftRotation(AVLnode *node);
AVLnode *rightRotation(AVLnode *node);
AVLnode *leftRightRotation(AVLnode *node);
AVLnode *rightLeftRotation(AVLnode *node);

void minTemp(AVLnode* tree);
void maxTemp(AVLnode* tree);

AVLnode *balanceFinder(AVLnode *foundNode, int functionUse);
void deleteRotation(AVLnode *node1, AVLnode *node2, AVLnode **newRoot);
int max(AVLnode *node1, AVLnode *node2);

void inOrder(AVLnode *tree);
void freeTree(AVLnode **tree);

//HASHING FUNCTIONS
Date getDate();
char * getKey(Date date);
int hash(char * key);
void insert(Data data, Bucket * bucket);
void insertAll(Data * data, int maxsize, Bucket * hashtable);
Node * searchNode(Date date, Bucket * hashtable);
void editNode(Node * node, float temp);
void deleteNode(Date date, Bucket * hashtable);

int main()
{
    printf("Welcome!\n");

    int mainMenuOpt;
    do
    {
        printf("\nWhat would you like to do:\n\t1. Load file into AVL tree\n\t2. Load file using hashing\n\t3. Exit application\n");
        menuOption(&mainMenuOpt, 1, 3);

        switch(mainMenuOpt)
        {
            case 1: //AVL
            {
                int avlOption;
                do
                {
                    printf("\nChoose the type of AVL tree:\n\t1. Date-based\n\t2. Temperature-based\n\t3. Go back\n");
                    menuOption(&avlOption, 1, 3);

                    switch(avlOption)
                    {
                        case 1: //AVL -> Date-based AVL
                        {
                            AVLnode *tree = NULL;
                            readDateandTemp("ocean.csv", &tree, 0);
                            printf("\nCreated AVL tree\n");

                            int dateAvlOption;
                            do
                            {
                                printf("\nWhat would you like to do:\n\t1. Print tree in-order\n\t2. Search temperatures for given date\n\t3. Modify temperatures for given date\n\t4. Delete node\n\t5. Go back\n");

                                menuOption(&dateAvlOption, 1, 5);

                                switch(dateAvlOption)
                                {
                                    case 1: //AVL -> Date-based AVL -> In order
                                    {
                                        printf("\n===================================");
                                        inOrder(tree);
                                        printf("===================================\n");
                                    }
                                    break;

                                    case 2: //AVL -> Date-based AVL -> Search temp
                                    {
                                        printf("\n");
                                        Date date = inputDate();

                                        printf("\n====================================");

                                        int found = 0;
                                        AVLnode* foundNode = dateSearch(tree, date, &found);

                                        if (found)
                                        {
                                            if (foundNode->mult == 1)
                                                printf("\nTemperature: ");
                                            else printf("\nTemperatures: ");

                                            for (int i = 0; i < foundNode->mult; i++)
                                            {
                                                printf("%06.3lf", foundNode->temperature[i]);

                                                if (i < foundNode->mult-1)
                                                    printf(", ");
                                                else printf("\n");
                                            }
                                        }
                                        else printf("\nCould not find an entry with the given date\n");

                                        printf("====================================\n");
                                    }
                                    break;

                                    case 3: //AVL -> Date-based AVL -> Modify temp
                                    {
                                        printf("\n");
                                        Date date = inputDate();

                                        printf("\n====================================");

                                        int found = 0;
                                        AVLnode* foundNode = dateSearch(tree, date, &found);

                                        if (found)
                                        {
                                            int mult;

                                            printf("\nHow many temperatures do you want to insert? (between 1 and 12)\n");
                                            menuOption(&mult, 1, 12);

                                            double* temp = (double*)malloc(mult*sizeof(double));
                                            if (temp == NULL)
                                            {
                                                printf("\nERROR: COULD NOT ALLOCATE THE REQUESTED MEMORY\n");
                                                exit(-1);
                                            }

                                            for (int i = 0; i < mult; i++)
                                            {
                                                printf("\nInput temperature #%d: ", i+1);
                                                scanf("%lf", temp+i);
                                            }

                                            free(foundNode->temperature);
                                            foundNode->temperature = temp;
                                            foundNode->mult = mult;
                                        }
                                        else printf("\nCould not find an entry with the given date\n");

                                        printf("====================================\n");
                                    }
                                    break;

                                    case 4: //AVL -> Date-based AVL -> Delete node
                                    {
                                        printf("\n");
                                        Date date = inputDate();

                                        printf("\n====================================");
                                        deleteAVLnode(&tree, date);
                                        printf("====================================\n");
                                    }
                                    break;
                                }
                            }
                            while(dateAvlOption != 5);

                            //Runs when choosing "Go back"
                            freeTree(&tree);
                            printf("\nDeleted AVL tree from memory\n");
                        }
                        break;

                        case 2: //AVL -> Temp-based AVL
                        {

                            AVLnode *tree = NULL;
                            readDateandTemp("ocean.csv", &tree, 1);
                            printf("\nCreated AVL tree\n");

                            int tempAvlOption;
                            do
                            {
                                printf("\nWhat would you like to do:\n\t1. Find days with minimum temperature\n\t2. Find days with maximum temperature\n\t3. Go back\n");

                                menuOption(&tempAvlOption, 1, 3);


                                switch(tempAvlOption)
                                {
                                    case 1:
                                    {
                                        printf("\n===================================\n");
                                        minTemp(tree);
                                        printf("===================================\n");
                                    }
                                    break;

                                    case 2:
                                    {
                                        printf("\n===================================\n");
                                        maxTemp(tree);
                                        printf("===================================\n");
                                    }
                                    break;
                                }
                            }
                            while(tempAvlOption != 3);

                            //Runs when choosing "Go back"
                            freeTree(&tree);
                            printf("\nDeleted AVL tree from memory\n");
                        }
                        break;
                    }
                }
                while(avlOption != 3);
            }
            break;

            case 2: //Hashing
            {
                Data * array;
                int * c = fileLinesSize("ocean.csv");
                array = readCsv("ocean.csv", c[0], c[1]);

                Bucket * hashtable = (Bucket *)malloc(sizeof(Bucket) * BUCKETS);
                for(int i = 0; i < BUCKETS; i++){
                    hashtable[i].first = NULL;
                    hashtable[i].last = NULL;
                }

                insertAll(array, c[0], hashtable);

                printf("\nCreated hash table\n");

                float temp;
                char answer[5];
                char * key;
                Date date;
                Node * node;

                int hashOption;
                do
                {
                    printf("\nWhat would you like to do:\n\t1. Search temperature\n\t2. Modify temperature\n\t3. Delete temperature\n\t4. Go back\n");
                    menuOption(&hashOption, 1, 4);

                    switch(hashOption)
                    {
                        case 1: //Hashing -> Search
                            printf("\n");
                            date = getDate();
                            node = searchNode(date, hashtable);
                            if(node != NULL)
                                {
                                    printf("Search Found\n");
                                    printf("Date: %04d-%02d-%02d, Temp: %.2f\n\n", node->date.year, node->date.month, node->date.day, node->temp);
                                }
                            else
                                {
                                    printf("No data was found for date: %04d-%02d-%02d\n\n", date.year, date.month, date.day);
                                }
                            break;
                        case 2: //Hashing -> Modify
                            printf("\n");
                            date = getDate();
                            node = searchNode(date, hashtable);
                            if(node != NULL)
                                {
                                    printf("Search Found\n");
                                    printf("Date: %04d-%02d-%02d, Temp: %.2f\n", node->date.year, node->date.month, node->date.day, node->temp);
                                    printf("Would you like to modify?\n");
                                    printf("yes or no\n");
                                    fgets(answer, 5, stdin);
                                    if(strcmp(answer, "yes\n") == 0)
                                        {
                                          printf("\nEnter a new temperature: ");
                                          scanf("%f", &temp);
                                          fflush(stdin);
                                          editNode(node, temp);
                                          printf("\n-New values-\nDate: %04d-%02d-%02d, Temp: %.2f\n\n", node->date.year, node->date.month, node->date.day, node->temp);
                                        }
                                    else if(strcmp(answer, "no\n") == 0)
                                        {
                                          printf("Returning back to Menu\n\n");
                                        }
                                    else
                                        {
                                          printf("Invalid answer\n\n");
                                        }
                                    break;
                                 }
                            else
                                 {
                                    printf("No data was found for date: %04d-%02d-%02d\n\n", date.year, date.month, date.day);
                                    break;
                                 }
                            break;
                        case 3: //Hashing -> Delete
                            printf("\n");
                            date = getDate();
                            node = searchNode(date, hashtable);
                            if(node != NULL)
                                {
                                    printf("Search Found\n");
                                    printf("Date: %04d-%02d-%02d, Temp: %.2f\n", node->date.year, node->date.month, node->date.day, node->temp);
                                    printf("Would you like to delete?\n");
                                    printf("yes or no\n");
                                    fgets(answer, 5, stdin);
                                    if(strcmp(answer, "yes\n") == 0)
                                        {
                                          deleteNode(date, hashtable);
                                        }
                                    else if(strcmp(answer, "no\n") == 0)
                                        {
                                          printf("Returning back to Menu\n\n");
                                        }
                                    else
                                        {
                                          printf("Invalid answer\n\n");
                                        }
                                    break;
                                 }
                            else
                                 {
                                    printf("No data was found for date: %04d-%02d-%02d\n\n", date.year, date.month, date.day);
                                    break;
                                 }
                            break;
                   }
                }
                while (hashOption != 4);

                //Runs when choosing "Go back"

                free(array);
                free(c);
                for(int i = 0; i < BUCKETS; i++){
                    Node * next;
                    for(Node * j = hashtable[i].first; j != NULL; j = next){
                        next = j->next;
                        free(j);
                    }
                }
                free(hashtable);

                printf("\nDeleted hash table\n");
            }
            break;
        }
    }
    while(mainMenuOpt != 3);

    return 0;
}

void readDateandTemp(char *filename, AVLnode **tree, int treeType)
{
    FILE *file;

    if ((file = fopen(filename, "r")) == NULL)
    {
        printf("%s", "ERROR: COULD NOT READ FILE"); // red tape :)
        exit(-1);
    }

    int *infArray = fileLinesSize(filename);
    int linesize = infArray[1];
    int size = infArray[0];
    int counter = 0, i = 0;
    double temp;
    Date date;
    char *dateString;

    free(readLine(file, linesize)); // Get rid of the first line before reading the other ones

    while (i < size)
    {
        char *line = readLine(file, linesize);
        char *token = strtok(line, ","); // it seperates the arraym and the seperator is the ',' character

        while (counter < 2)
        {
            if (counter == 0)
            {
                dateString = (char *)malloc((strlen(token) + 1) * sizeof(char)); // given the length of the line tha we raed ,we allocate memory for the string object
                strcpy(dateString, token);                                       // and then we copy it
            }
            else
                temp = atof(token); // save the other data in the array

            token = strtok(NULL, ","); // line of code that exists for strtok to work properly :)(basically works in the same string )
            counter++;                 // counter for the data of the array
        }
        i++;

        counter = 0;

        token = strtok(dateString, "/"); // same procidure like above but now we want ot store the values of the date
        while (token)
        {
            if (counter == 0) // for these lines we make the values double and save them in the wright spot of the array
                date.month = atoi(token);
            else if (counter == 1)
                date.day = atoi(token);
            else
                date.year = atoi(token);

            token = strtok(NULL, "/");
            counter++;
        }
        counter = 0;

        if (treeType == 0)
            insertDateAVLnode(tree, temp, date);
        else insertTempAVLnode(tree, temp, date);
    }

    free(dateString); //freeing the allocated memory :)
    fclose(file);
}


AVLnode *dateSearch(AVLnode *tree, Date date, int *found)
{
    if (tree != NULL)
    {
        *found = 0;
        while (tree->leftChild != NULL)
        {
            if (compareDates(&date, tree->date) <= 0)
            {
                tree = tree->leftChild;
            }
            else
            {
                tree = tree->rightChild;
            }
        }

        if (compareDates(tree->date, &date) == 0)
        {
            *found = 1;
        }
    }
    else printf("\nBut, there was nothing to search for.\n");

    return tree;
}

AVLnode *tempSearch(AVLnode *tree, double temp, int *found)
{
    if (tree != NULL)
    {
        *found = 0;
        while (tree->leftChild != NULL)
        {
            if (temp <= tree->temperature[0])
            {
                tree = tree->leftChild;
            }
            else
            {
                tree = tree->rightChild;
            }
        }

        if (tree->temperature[0] == temp)
        {
            *found = 1;
        }
    }
    else printf("\nBut, there was nothing to search for.\n");

    return tree;
}

void insertDateAVLnode(AVLnode **tree, double temp, Date date)
{
    if (*tree == NULL)
    {
        *tree = (AVLnode *)malloc(sizeof(AVLnode));
        (*tree)->height = (*tree)->hb = 0;
        (*tree)->mult = 1;
        (*tree)->temperature = (double*)malloc(sizeof(double));
        (*tree)->temperature[0] = temp;
        (*tree)->date = (Date*)malloc(sizeof(Date));
        (*tree)->date[0] = date;
        (*tree)->leftChild = (*tree)->rightChild = (*tree)->father = NULL;
    }
    else
    {
        int found = 0;
        AVLnode *foundNode = dateSearch(*tree, date, &found);
        if (!found)
        {
            AVLnode *leftNode = (AVLnode *)malloc(sizeof(AVLnode));
            AVLnode *rightNode = (AVLnode *)malloc(sizeof(AVLnode));

            leftNode->leftChild = leftNode->rightChild = rightNode->leftChild = rightNode->rightChild = NULL;
            leftNode->father = rightNode->father = foundNode;
            leftNode->height = leftNode->hb = rightNode->height = rightNode->hb = 0;

            leftNode->date = (Date*)malloc(sizeof(Date));
            rightNode->date = (Date*)malloc(sizeof(Date));

            foundNode->leftChild = leftNode;
            foundNode->rightChild = rightNode;


            if (compareDates(&date, foundNode->date) == -1)
            {
                rightNode->date[0] = foundNode->date[0];
                foundNode->date[0] = date;
                leftNode->date[0] = date;

                rightNode->mult = foundNode->mult;
                rightNode->temperature = foundNode->temperature;

                foundNode->mult = 1;
                foundNode->temperature = (double*)malloc(sizeof(double));
                foundNode->temperature[0] = temp;

                leftNode->mult = 1;
                leftNode->temperature = (double*)malloc(sizeof(double));
                leftNode->temperature[0] = temp;
            }
            else
            {
                leftNode->date[0] = foundNode->date[0];
                rightNode->date[0] = date;

                leftNode->mult = foundNode->mult;
                leftNode->temperature = foundNode->temperature;

                foundNode->mult = 1;
                foundNode->temperature = (double*)malloc(sizeof(double));
                foundNode->temperature[0] = leftNode->temperature[0];

                rightNode->mult = 1;
                rightNode->temperature = (double*)malloc(sizeof(double));
                rightNode->temperature[0] = temp;
            }

            AVLnode *node = balanceFinder(foundNode, 1);

            if (node->father == NULL)
                *tree = node;
        }
        else
        {
            foundNode->mult++;
            foundNode->temperature = (double*)realloc(foundNode->temperature,foundNode->mult*sizeof(double));
            foundNode->temperature[foundNode->mult-1] = temp;
        }
    }
}

void insertTempAVLnode(AVLnode **tree, double temp, Date date)
{
    if (*tree == NULL)
    {
        *tree = (AVLnode *)malloc(sizeof(AVLnode));
        (*tree)->height = (*tree)->hb = 0;
        (*tree)->mult = 1;
        (*tree)->temperature = (double *)malloc(sizeof(double));
        (*tree)->temperature[0] = temp;
        (*tree)->date = (Date *)malloc(sizeof(Date));
        (*tree)->date[0] = date;
        (*tree)->leftChild = (*tree)->rightChild = (*tree)->father = NULL;
    }
    else
    {
        int found = 0;
        AVLnode *foundNode = tempSearch(*tree, temp, &found);
        if (!found)
        {
            AVLnode *leftNode = (AVLnode *)malloc(sizeof(AVLnode));
            AVLnode *rightNode = (AVLnode *)malloc(sizeof(AVLnode));

            leftNode->leftChild = leftNode->rightChild = rightNode->leftChild = rightNode->rightChild = NULL;
            leftNode->father = rightNode->father = foundNode;
            leftNode->height = leftNode->hb = rightNode->height = rightNode->hb = 0;

            leftNode->temperature = (double *)malloc(sizeof(double));
            rightNode->temperature = (double *)malloc(sizeof(double));

            foundNode->leftChild = leftNode;
            foundNode->rightChild = rightNode;

            if (temp < (*foundNode->temperature))
            {
                rightNode->temperature[0] = foundNode->temperature[0];
                foundNode->temperature[0] = temp;
                leftNode->temperature[0] = temp;

                rightNode->mult = foundNode->mult;
                rightNode->date = foundNode->date;

                foundNode->mult = 1;
                foundNode->date = (Date *)malloc(sizeof(Date));
                foundNode->date[0] = date;

                leftNode->mult = 1;
                leftNode->date = (Date *)malloc(sizeof(Date));
                leftNode->date[0] = date;
            }
            else
            {
                leftNode->temperature[0] = foundNode->temperature[0];
                rightNode->temperature[0] = temp;

                leftNode->mult = foundNode->mult;
                leftNode->date = foundNode->date;

                foundNode->mult = 1;
                foundNode->date = (Date *)malloc(sizeof(Date));
                foundNode->date[0] = leftNode->date[0];

                rightNode->mult = 1;
                rightNode->date = (Date *)malloc(sizeof(Date));
                rightNode->date[0] = date;
            }

            AVLnode *node = balanceFinder(foundNode, 1);

            if (node->father == NULL)
                *tree = node;
        }
        else
        {
            foundNode->mult++;
            foundNode->date = (Date *)realloc(foundNode->date, foundNode->mult * sizeof(Date));
            foundNode->date[foundNode->mult - 1] = date;
        }
    }
}

AVLnode *balanceFinder(AVLnode *foundNode, int functionUse)
{
    AVLnode *currentNode = foundNode;
    currentNode->height = 1;
    currentNode->hb = 0;
    AVLnode *originNode;
    AVLnode *newRoot = foundNode;

    while (currentNode->father != NULL)
    {
        if (currentNode->leftChild == NULL) //Used in deletions, because we pass the leaf as the argument (unlike insertions)
        {
            currentNode->height = 0;
            currentNode->hb = 0;
        }

        originNode = currentNode;
        currentNode = currentNode->father;

        currentNode->height = max(currentNode->leftChild, currentNode->rightChild) + 1;

        currentNode->hb = currentNode->rightChild->height - currentNode->leftChild->height;

        // Choose the type of rotation (or no rotation)to perform
        if (functionUse == 1)
        {
            if (currentNode->hb == 0)
            {
                newRoot = currentNode;
                break;
            }

            if (currentNode->hb == 2)
            {
                if (originNode->hb == 1)
                    newRoot = leftRotation(currentNode);
                else if (originNode->hb == -1)
                    newRoot = leftRightRotation(currentNode);

                break;
            }

            if (currentNode->hb == -2)
            {
                if (originNode->hb == -1)
                    newRoot = rightRotation(currentNode);
                else if (originNode->hb == 1)
                    newRoot = rightLeftRotation(currentNode);

                break;
            }
        }
    }
    return newRoot;
}

AVLnode *leftRotation(AVLnode *node)
{
    AVLnode *nodeChild = node->rightChild;
    AVLnode *nodeFather = NULL;

    if (node->father != NULL)
    {
        nodeFather = node->father;

        if (node == nodeFather->leftChild)
            nodeFather->leftChild = nodeChild;
        else if (node == nodeFather->rightChild)
            nodeFather->rightChild = nodeChild;

        nodeChild->father = nodeFather;
    }
    else
    {
        nodeChild->father = nodeFather;
    }

    node->father = nodeChild;

    node->rightChild = nodeChild->leftChild;
    nodeChild->leftChild->father = node;

    nodeChild->leftChild = node;

    //nodeChild is now the new root of the subtree, and node is its left child
    //Now we update the heights and height balances, starting from the lower node (node)

    //Height of node
    if (node->leftChild->height >= node->rightChild->height)
        node->height = node->leftChild->height + 1;
    else node->height = node->rightChild->height + 1;

    node->hb = node->rightChild->height - node->leftChild->height;

    //Height of nodeChild
    if (nodeChild->leftChild->height >= nodeChild->rightChild->height)
        nodeChild->height = nodeChild->leftChild->height + 1;
    else nodeChild->height = nodeChild->rightChild->height + 1;

    nodeChild->hb = nodeChild->rightChild->height - nodeChild->leftChild->height;

    return nodeChild;
}

AVLnode *rightRotation(AVLnode *node)
{
    AVLnode *nodeChild = node->leftChild;
    AVLnode *nodeFather = NULL;

    if (node->father != NULL)
    {
        nodeFather = node->father;

        if (node == nodeFather->leftChild)
            nodeFather->leftChild = nodeChild;
        else if (node == nodeFather->rightChild)
            nodeFather->rightChild = nodeChild;

        nodeChild->father = nodeFather;
    }
    else
    {
        nodeChild->father = nodeFather;
    }

    node->father = nodeChild;

    node->leftChild = nodeChild->rightChild;
    nodeChild->rightChild->father = node;

    nodeChild->rightChild = node;

    //nodeChild is now the new root of the subtree, and node is its right child
    //Now we update the heights and height balances, starting from the lower node (node)

    //Height of node
    if (node->leftChild->height >= node->rightChild->height)
        node->height = node->leftChild->height + 1;
    else node->height = node->rightChild->height + 1;

    node->hb = node->rightChild->height - node->leftChild->height;

    //Height of nodeChild
    if (nodeChild->leftChild->height >= nodeChild->rightChild->height)
        nodeChild->height = nodeChild->leftChild->height + 1;
    else nodeChild->height = nodeChild->rightChild->height + 1;

    nodeChild->hb = nodeChild->rightChild->height - nodeChild->leftChild->height;

    return nodeChild;
}

AVLnode *leftRightRotation(AVLnode *node)
{
    AVLnode *nodeChild = node->rightChild;

    rightRotation(nodeChild);

    return leftRotation(node);
}

AVLnode *rightLeftRotation(AVLnode *node)
{
    AVLnode *nodeChild = node->leftChild;

    leftRotation(nodeChild);

    return rightRotation(node);
}

void inOrder(AVLnode *tree)
{
    if (tree != NULL)
    {
        inOrder(tree->leftChild);

        printf("\nDate: %d/%d/%d\n", tree->date->day, tree->date->month, tree->date->year);

        if (tree->mult == 1)
            printf("Temperature: ");
        else printf("Temperatures: ");

        for (int i = 0; i < tree->mult; i++)
        {
            printf("%06.3lf", tree->temperature[i]);

            if (i < tree->mult-1)
                printf(", ");
            else printf("\n");
        }

        inOrder(tree->rightChild);
    }
}

void freeTree(AVLnode **tree)
{
    if (*tree != NULL)
    {
        freeTree(&((*tree)->leftChild));
        freeTree(&((*tree)->rightChild));
        free((*tree)->temperature);
        free((*tree)->date);
        free(*tree);
        *tree = NULL;
    }
}

void minTemp(AVLnode* tree)
{
    if (tree != NULL)
    {
        AVLnode* node = tree;

        while(node->leftChild != NULL)
            node = node->leftChild;

        printf("Minimum temperature of %06.3lf found in %d dates:\n\n", node->temperature[0], node->mult);

        for (int i = 0; i < node->mult; i++)
            printf("%d/%d/%d\n", node->date[i].day, node->date[i].month, node->date[i].year);
    }
    else printf("But, there was no tree to find the minimum of\n");
}

void maxTemp(AVLnode* tree)
{
    if (tree != NULL)
    {
        AVLnode* node = tree;

        while(node->rightChild != NULL)
            node = node->rightChild;

        printf("Maximum temperature of %06.3lf found in %d dates:\n\n", node->temperature[0], node->mult);

        for (int i = 0; i < node->mult; i++)
            printf("%d/%d/%d\n", node->date[i].day, node->date[i].month, node->date[i].year);
    }
    else printf("But, there was no tree to find the maximum of\n");
}

void deleteAVLnode(AVLnode **tree, Date date)
{
    if (*tree != NULL)
    {
        int found = 0;
        AVLnode *foundNode = dateSearch(*tree, date, &found);

        if (found)
        {
            AVLnode *nodeFather = foundNode->father;

            // if only the root is left
            if (nodeFather == NULL)
            {
                free(foundNode->temperature);
                free(foundNode->date);
                free(foundNode);
                *tree = NULL;
                printf("\nEntry deleted successfully and the tree is now empty\n");
                return;
            }

            // If the father is the root
            AVLnode *nodeGrandpa = nodeFather->father;

            if (nodeGrandpa == NULL)
            {
                if (foundNode == nodeFather->leftChild)
                {
                    *tree = nodeFather->rightChild;
                    nodeFather->rightChild->father = NULL;
                }
                else
                {
                    *tree = nodeFather->leftChild;
                    nodeFather->leftChild->father = NULL;
                }

                free(foundNode->temperature);
                free(foundNode->date);
                free(foundNode);

                free(nodeFather->temperature);
                free(nodeFather->date);
                free(nodeFather);

                printf("\nEntry deleted successfully (one node remaining)\n");

                return;
            }

            // When found node is any other leaf

            AVLnode *nodeBro;

            if (foundNode == nodeFather->leftChild)
                nodeBro = nodeFather->rightChild;
            else if (foundNode == nodeFather->rightChild)
                nodeBro = nodeFather->leftChild;

            nodeBro->father = nodeGrandpa;

            if (nodeFather == nodeGrandpa->leftChild)
                nodeGrandpa->leftChild = nodeBro;
            else if (nodeFather == nodeGrandpa->rightChild)
                nodeGrandpa->rightChild = nodeBro;

            free(foundNode->temperature);
            free(foundNode->date);
            free(foundNode);

            free(nodeFather->temperature);
            free(nodeFather->date);
            free(nodeFather);

            AVLnode *nodePrev = nodeBro;
            AVLnode *newRoot;

            deleteRotation(nodeGrandpa, nodePrev, &newRoot);

            if (newRoot->father == NULL)
                *tree = newRoot;

            printf("\nEntry deleted successfully\n");
        }
        else
            printf("\nCould not find an entry with the given date\n");
    }
    else printf("\nBut, there was nothing to delete.\n");
}

void deleteRotation(AVLnode *node1, AVLnode *node2, AVLnode **newRoot)
{
    *newRoot = node1;

    if (node1 == NULL)
    {
        *newRoot = node2;
        return;
    }

    AVLnode *nodeGrandpa = node1;
    AVLnode *nodePrev = node2;

    while (nodeGrandpa != NULL)
    {
        if (nodePrev->leftChild != NULL)
        {
            nodePrev->height = max(nodePrev->leftChild, nodePrev->rightChild) + 1;
            nodePrev->hb = nodePrev->rightChild->height - nodePrev->leftChild->height;
        }
        else
        {
            nodePrev->height = 0;
            nodePrev->hb = 0;
        }

        if (nodeGrandpa->hb == 0)
            break;
        else if (nodePrev == nodeGrandpa->leftChild && nodeGrandpa->hb == 1)
            break;
        else if (nodePrev == nodeGrandpa->rightChild && nodeGrandpa->hb == -1)
            break;

        nodePrev = nodeGrandpa;
        nodeGrandpa = nodeGrandpa->father;
    }

    if (nodeGrandpa == NULL)
    {
        nodePrev->height = max(nodePrev->leftChild, nodePrev->rightChild) + 1;
        nodePrev->hb = nodePrev->rightChild->height - nodePrev->leftChild->height;
        return;
    }

    if (nodeGrandpa->hb == 0)
    {
        nodeGrandpa->height = max(nodeGrandpa->leftChild, nodeGrandpa->rightChild) + 1;
        nodeGrandpa->hb = nodeGrandpa->rightChild->height - nodeGrandpa->leftChild->height;

        return;
    }

    AVLnode *nodeKid;

    if (nodePrev == nodeGrandpa->leftChild)
        nodeKid = nodeGrandpa->rightChild;
    else if (nodePrev == nodeGrandpa->rightChild)
        nodeKid = nodeGrandpa->leftChild;

    if (nodeGrandpa->hb == 1)
    {
        nodeGrandpa->height = max(nodeGrandpa->leftChild, nodeGrandpa->rightChild) + 1;
        nodeGrandpa->hb = nodeGrandpa->rightChild->height - nodeGrandpa->leftChild->height;


        if (nodeKid->hb == 1)
        {
            *newRoot = leftRotation(nodeGrandpa);
            deleteRotation((*newRoot)->father, *newRoot, newRoot);
        }
        else if (nodeKid->hb == -1)
        {
            *newRoot = leftRightRotation(nodeGrandpa);
            deleteRotation((*newRoot)->father, *newRoot, newRoot);
        }
        else
            *newRoot = leftRotation(nodeGrandpa);
    }

    if (nodeGrandpa->hb == -1)
    {
        nodeGrandpa->height = max(nodeGrandpa->leftChild, nodeGrandpa->rightChild) + 1;
        nodeGrandpa->hb = nodeGrandpa->rightChild->height - nodeGrandpa->leftChild->height;

        if (nodeKid->hb == -1)
        {
            *newRoot = rightRotation(nodeGrandpa);
            deleteRotation((*newRoot)->father, *newRoot, newRoot);
        }
        else if (nodeKid->hb == 1)
        {
            *newRoot = rightLeftRotation(nodeGrandpa);
            deleteRotation((*newRoot)->father, *newRoot, newRoot);
        }
        else
            *newRoot = rightRotation(nodeGrandpa);
    }
}

int max(AVLnode *node1, AVLnode *node2)
{
    if (node1->height > node2->height)
        return node1->height;
    else
        return node2->height;
}

//============================================
Date getDate(){
    int day, month, year;

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
    fflush(stdin);
    return newDate;
}

char * getKey(Date date){
    char * str = (char *)malloc(sizeof(char) * 11);
    sprintf(str, "%04d-%02d-%02d", date.year, date.month, date.day);
    return str;
}

int hash(char * key){
    int sum = 0;
    for(int i = 0; key[i]!= '\0'; i++){
        sum += (int) key[i];
    }
    return sum % BUCKETS;
}

void insert(Data datain, Bucket * bucket){
    Node * node = (Node *)malloc(sizeof(Node));
    node->next = NULL;
    node->date = datain.date;
    node->temp = datain.TEMP;
    if(bucket->first == NULL){
        bucket->first = node;
        bucket->last = node;
        return;
    }
    bucket->last->next = node;
    bucket->last = node;
}

void insertAll(Data * data, int maxsize, Bucket * hashtable){
    for(int i = 0; i < maxsize; i++){
        char * key = getKey(data[i].date);
        insert(data[i], &hashtable[hash(key)]);
        free(key);
    }
}

Node * searchNode(Date date, Bucket * hashtable){
    char * key = getKey(date);
    int index = hash(key);
    free(key);
    Node * i;
    for(i = hashtable[index].first; i != NULL; i = i->next){
        if(i->date.day == date.day && i->date.month == date.month && i->date.year == date.year){
            return i;
        }
    }
    return i;
}

void editNode(Node * node, float temp){
    node->temp = temp;
    /*char * key = getKey(date);
    int index = hash(key);
    free(key);
    Node * i;
    for(i = hashtable[index].first; i != NULL; i = i->next){
        if(i->date.day == date.day && i->date.month == date.month && i->date.year == date.year){
            i->temp = temp;
        }
    }
    printf("No data was found for date: %04d-%02d-%02d", date.year, date.month, date.day);*/
}

void deleteNode(Date date, Bucket * hashtable){
    char * key = getKey(date);
    int index = hash(key);
    free(key);
    Node ** i;
    Node * prev = NULL;
    for(i = &hashtable[index].first; (*i) != NULL; i = &(*i)->next){
        if((*i)->date.day == date.day && (*i)->date.month == date.month && (*i)->date.year == date.year){
            Node * next = (*i)->next;
            if((*i) == hashtable[index].last) hashtable[index].last = prev;
            free(*i);
            (*i) = next;
            return;
        }
        prev = (*i);
    }
    printf("No data was found for date: %04d-%02d-%02d", date.year, date.month, date.day);
}
