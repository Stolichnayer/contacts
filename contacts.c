/*
 ============================================================================
 Name        : contacts
 Author      : Alexandros Perrakis
 Description : Binary contacts' database in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 100

typedef struct person
{
	int nameLen;
	char name[MAX_LEN];
	int mobileNum;
	struct person * next;
}	person_t;

person_t * current;
person_t * start;
person_t * prev;
person_t * tmpNext;

void resetf()
{
/* Deletes the list */
    person_t * next;
    current = start;
    while (current != NULL)
    {
       next = current->next;
       free(current);
       current = next;
    }
    start = NULL;
}

void sortf()
{
/* Sorts the list with bubblesort algorithm */
    int counter=0;
    int i,j;
    int tmpnameLen;
    int tmpmobileNum;
    char tmpname[MAX_LEN];

    current = start;
    while(current->next != NULL)
    {
        counter++;
        current = current->next;
    }

    for(i = 0; i < counter; i++)
    {
        current = start;
        for(j = 0; j < counter-1; j++)
        {
            if(strcmp(current->name, current->next->name) > 0)
            {
                tmpnameLen = current->nameLen;
                strcpy(tmpname, current->name);
                tmpmobileNum = current->mobileNum;

                current->nameLen   = current->next->nameLen;
                strcpy(current->name, current->next->name);
                current->mobileNum = current->next->mobileNum;

                current->next->nameLen   = tmpnameLen;
                strcpy( current->next->name, tmpname);
                current->next->mobileNum = tmpmobileNum;
            }
            current = current->next;
        }
    }
}

void updatef(char name[MAX_LEN], int mobileNum)
{
/* Updates mobile number of the first found record */
    current = start;
    while(strcmp(current->name, name))
    {
        current = current->next;
    }
    current->mobileNum = mobileNum;
}

void searchf(char name[MAX_LEN])
{
/* Prints all contact names that contain the string argument */
    current = start;
    while(current->next != NULL)
    {
        if(strstr(current->name, name))
        {
            printf("%s %i\n", current->name, current->mobileNum);
        }
        current = current->next;
    }
}

void deletef(char name[MAX_LEN])
{
/* Deletes the first found contact */
    if(start == NULL)
        return;
    prev = NULL;
    current = start;

    while(strcmp(current->name, name))
    {
        prev = current;
        current = current->next;
        if(current->next == NULL)
        {
            printf("This contact does not exist.\n");
            return;
        }

    }
    if(prev == NULL)
        start = start->next;
    else
        prev->next = current->next;

    current = NULL;
}

void insertf(char name[MAX_LEN], int mobileNum)
{
/* Adds a contact in the list */
    if(start == NULL)
    {
        start = malloc(sizeof(person_t));

        start->nameLen = strlen(name)+1;
        strcpy(start->name, name);
        start->mobileNum = mobileNum;

        start->next = NULL;
        return;
    }

    current = start;
    while (current->next != NULL)
    {
        current = current->next;
    }

    current->next = malloc(sizeof(person_t));

    current->nameLen = strlen(name)+1;
	strcpy(current->name, name);
	current->mobileNum = mobileNum;

    current = current->next;
    current->next = NULL;
}

void list_all()
{
/* prints all contacts */
    if(start == NULL)
        return;

    if(start->next == NULL)
    {
        printf("%s %i\n", start->name, start->mobileNum);
        return;
    }

	current = start;
	while(current->next != NULL)
	{
        printf("%s %i\n", current->name, current->mobileNum);
		current = current->next;
	}
}

void memoryToFile()
{
/* Prints to file the contacts that are in memory */

	FILE * f = fopen("contacts.dat", "wb");

    if(f == (FILE *) NULL)
    {
        fprintf(stderr, "Cannot open file\n");
        exit(-1);
    }

	current = start;
    if(start == NULL)
     {
         fclose(f);
         return;
     }

    if(start->next == NULL)
    {
        fwrite(&start->nameLen, sizeof(start->nameLen), 1, f);
		fwrite(&start->name, start->nameLen, 1, f);
		fwrite(&start->mobileNum, sizeof(start->mobileNum), 1, f);
		return;
    }


	while(current->next != NULL)
	{
		fwrite(&current->nameLen, sizeof(current->nameLen), 1, f);
		fwrite(&current->name, current->nameLen, 1, f);
		fwrite(&current->mobileNum, sizeof(current->mobileNum), 1, f);

		current = current->next;
	}

	fclose(f);
}

void createBinary()
{
/* Creating some contact samples */
	int i,j;
	FILE * f = fopen("contacts.dat", "wb");
	i=11; j=695132;
	fwrite(&i, sizeof(i), 1, f);
	fwrite("alexandros", sizeof("alexandros"), 1, f);
	fwrite(&j, sizeof(j), 1, f);
	i=10; j=707070;
	fwrite(&i, sizeof(i), 1, f);
	fwrite("alexandra", sizeof("alexandra"), 1, f);
	fwrite(&j, sizeof(j), 1, f);
	i=12; j=717171;
	fwrite(&i, sizeof(i), 1, f);
	fwrite("alexandross", sizeof("alexandross"), 1, f);
	fwrite(&j, sizeof(j), 1, f);
	i=12; j=717171;
	fwrite(&i, sizeof(i), 1, f);
	fwrite("alexandross", sizeof("alexandross"), 1, f);
	fwrite(&j, sizeof(j), 1, f);
	i=13; j=717171;
	fwrite(&i, sizeof(i), 1, f);
	fwrite("alexandrossa", sizeof("alexandrossa"), 1, f);
	fwrite(&j, sizeof(j), 1, f);
	i=12; j=717171;
	fwrite(&i, sizeof(i), 1, f);
	fwrite("alexandrozz", sizeof("alexandrozz"), 1, f);
	fwrite(&j, sizeof(j), 1, f);

	fclose(f);
}

void filetoMemory()
{
/* Reads from file and creates a linked-list of contacts in memory */
	int len;

	FILE * f = fopen("contacts.dat", "rb");

	if(f == NULL)
        return;

	fseek(f, 0, SEEK_END);
	len = ftell(f);

	if(len == 0)
	{
	    fclose(f);
	    return;
	}
	rewind(f);

	start = malloc(sizeof(person_t));
	current = start;

	while (1)
	{
		fread(&current->nameLen, sizeof(current->nameLen), 1, f);
		if(feof(f))
			break;
		fread(&current->name, current->nameLen, 1, f);
		fread(&current->mobileNum, sizeof(current->mobileNum), 1, f);

		current->next = malloc(sizeof(person_t));
		current = current->next;
	}

	free(current->next);
	current->next = NULL;
	fclose(f);
}


int main(int argc, char * argv[])
{

    if (argc != 2 && argc != 3 && argc != 4)
        return -1;


    filetoMemory();

    if(!strcmp(argv[1], "--reset"))
	resetf();

    if(!strcmp(argv[1], "--insert"  ))
        insertf(argv[2], atoi(argv[3]));

    if(!strcmp(argv[1], "--delete"  ))
        deletef(argv[2]);

    if(!strcmp(argv[1], "--list-all"))
        list_all();

    if(!strcmp(argv[1], "--search"  ))
        searchf(argv[2]);

    if(!strcmp(argv[1], "--update"  ))
        updatef(argv[2], atoi(argv[3]));

    if(!strcmp(argv[1], "--sort"    ))
        sortf();

    memoryToFile();


	return 0;
}
