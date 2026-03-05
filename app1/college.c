/* college.c - Application 1 (search by age) */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct dataSet DATASET;

DATASET *createDataSet(int maxStudents);
void destroyDataSet(DATASET *ds);
void insertion(DATASET *ds, int id, int age);
void deletion(DATASET *ds, int age);
void searchAge(DATASET *ds, int age);
int maxAgeGap(DATASET *ds);

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

    srand((unsigned int)time(NULL));  /* seed with time so each run gives different random IDs/ages */

    ds = createDataSet(maxStudents);  /* create with max 3000 as required by assignment */
    if (ds == NULL) {
        printf("Failed to create data set.\n");
        return 1;
    }

    currentID = randomOneOrTwo();      /* first ID 1 or 2 per spec */
    for (i = 0; i < numberOfStudents; i++) {
        int age = (rand() % 13) + 18; /* 18..30 per spec */
        insertion(ds, currentID, age);
        currentID += randomOneOrTwo(); /* next ID = prev + 1 or 2 so IDs stay unique and increasing */
    }

    {
        int randomID = (rand() % 2000) + 1;   /* 1..2000 (App1 searches by age only, so ID unused) */
        int randomAge = (rand() % 13) + 18;  /* 18..30; we search for everyone with this age */

        (void)randomID;

        searchAge(ds, randomAge);      /* demo search by age as required */
    }

    {
        int ageToDelete = (rand() % 13) + 18; /* pick an age; we delete all students with that age (App1 rule) */
        deletion(ds, ageToDelete);
    }

    maxAgeGap(ds);                   /* required for App1: print largest age gap */

    destroyDataSet(ds);              /* free all memory when done */

    return 0;
}
