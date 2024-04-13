//Matthew Cardona
//Lab 9
//4/12/24
//COP 3502C

#include <stdio.h>
#include <stdlib.h> // for malloc, free

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order; 
};

// Structure for each node in the linked list
struct Node
{
    struct RecordType data;
    struct Node* next;
};

// Structure for hash table
struct HashType
{
    struct Node* head;
};

// Size of the hash table
#define HASH_SIZE 10

// Compute the hash function
int hash(int x)
{
    return x % HASH_SIZE;
}

// Insert a record into the hash table
void insertRecord(struct HashType* hashTable, struct RecordType record)
{
    int index = hash(record.id);
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (!newNode)
    {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = record;
    newNode->next = hashTable[index].head;
    hashTable[index].head = newNode;
}

// Display records in the hash structure
void displayRecordsInHash(struct HashType *hashTable, int hashSize)
{
    printf("\nRecords in Hash Table:\n");
    for (int i = 0; i < hashSize; ++i)
    {
        printf("Index %d -> ", i);
        struct Node* current = hashTable[i].head;
        while (current)
        {
            printf("(%d, %c, %d) -> ", current->data.id, current->data.name, current->data.order);
            current = current->next;
        }
        printf("NULL\n");
    }
}

// Parse input file to an array of RecordType
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }
        fclose(inFile);
    }

    return dataSz;
}

// Prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    printf("\nRecords:\n");
    for (int i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;
    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Initialize the hash table
    struct HashType hashTable[HASH_SIZE];
    for (int i = 0; i < HASH_SIZE; ++i)
    {
        hashTable[i].head = NULL;
    }

    // Insert records into the hash table
    for (int i = 0; i < recordSz; ++i)
    {
        insertRecord(hashTable, pRecords[i]);
    }

    // Display records in the hash table
    displayRecordsInHash(hashTable, HASH_SIZE);

    // Free allocated memory
    for (int i = 0; i < HASH_SIZE; ++i)
    {
        struct Node* current = hashTable[i].head;
        while (current)
        {
            struct Node* next = current->next;
            free(current);
            current = next;
        }
    }
    free(pRecords);

    return 0;
}
