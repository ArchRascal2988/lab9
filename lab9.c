#include <stdio.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType* record;
	struct HashType* next;
};

// Compute the hash function
int hash(int x, int sz)
{
	return x % sz;
}

// parses input file to an integer array
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
		// Implement parse data block
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

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType **pHashArray, int hashSz)
{
	int i;
	printf("\nHash Records:\n");
	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		struct HashType* temp = pHashArray[i];

		while(temp->record != NULL){
			printf("%d %c %d\n", temp->record->id, temp->record->name, temp->record->order);
			if(temp->next == NULL)
				break;
			
			temp = temp->next;
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("./input.txt", &pRecords);
	printRecords(pRecords, recordSz);

	// Your hash implementation
	FILE* inFile = fopen("./input.txt", "r");
	int dataSz;
	int i, n1, n2, index;
	char c;
	struct HashType **hashTable;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		int tblSz= dataSz * dataSz;
		hashTable = (struct HashType**) malloc(sizeof(struct HashType) * tblSz);
		
		
		if (hashTable == NULL){
			printf("Cannot allocate memory\n");
			exit(-1);
		}

		for(i = 0; i < tblSz; i++){
			hashTable[i] = (struct HashType*) malloc(sizeof(struct HashType));
			hashTable[i]->record = NULL;
			hashTable[i]->next = NULL;
		}

		for (i = 0; i < dataSz; ++i)
		{
			fscanf(inFile, "%d %c %d", &n1, &c, &n2);
			index = hash(n1, tblSz);
			if(hashTable[index]->record != NULL){
				struct HashType* temp = (struct HashType*) malloc(sizeof(struct HashType));
				temp->next = hashTable[index];
				hashTable[index] = temp;
			} 
			
			hashTable[index]->record = (struct RecordType*) malloc(sizeof(struct RecordType));
			hashTable[index]->record->id = n1;
			hashTable[index]->record->name = c;
			hashTable[index]->record->order = n2;
		}

		displayRecordsInHash(hashTable, tblSz);
		fclose(inFile);
	}
}