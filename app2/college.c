/* college.c - Application 2 (search by ID) */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct dataSet DATASET;

DATASET *createDataSet(int maxStudents);
void destroyDataSet(DATASET *ds);
void insertion(DATASET *ds, int id, int age);
void searchID(DATASET *ds, int id);
void deletion(DATASET *ds, int id);

/* returns 1 or 2 randomly */
int randomOneOrTwo(void)
{
    return (rand() % 2) + 1;
}

int main(void)
{
    int maxStudents = 3000;
    int numberOfStudents = 1000;
    int i;
    int currentID;
    DATASET *ds;

    srand((unsigned int)time(NULL));  /* different seed each run so test is different each time */

    ds = createDataSet(maxStudents);
    if (ds == NULL) {
        printf("Failed to create data set.\n");
        return 1;
    }

    currentID = randomOneOrTwo();      /* first ID 1 or 2 per spec */
    for (i = 0; i < numberOfStudents; i++) {
        int age = (rand() % 13) + 18; /* 18..30 per spec */
        insertion(ds, currentID, age);
        currentID += randomOneOrTwo(); /* next ID = prev + 1 or 2 so IDs stay unique; insertion keeps array sorted */
    }

    {
        int randomID = (rand() % 2000) + 1;   /* 1..2000; App2 searches by ID */
        int randomAge = (rand() % 13) + 18;

        (void)randomAge;

        searchID(ds, randomID);        /* demo search by ID as required */
    }

    {
        int idToDelete = (rand() % 2000) + 1; /* pick random ID; delete that one record if present (App2 rule) */
        deletion(ds, idToDelete);
    }

    destroyDataSet(ds);   /* free all memory */

    return 0;
}
