/* college.c - Application 1 (search by age)
 * Driver program: creates data set, inserts 1000 random students, runs one search by age,
 * one deletion by age, maxAgeGap, then destroys. All per assignment spec.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Opaque type: main doesn't know the internals of DATASET, only the functions in dataset.c do. */
typedef struct dataSet DATASET;

DATASET *createDataSet(int maxStudents);
void destroyDataSet(DATASET *ds);
void insertion(DATASET *ds, int id, int age);
void deletion(DATASET *ds, int age);
void searchAge(DATASET *ds, int age);
int maxAgeGap(DATASET *ds);

/* Assignment says: random integer generator returns 1 or 2. Used to build unique increasing IDs. */
int randomOneOrTwo(void)
{
    return (rand() % 2) + 1;
}

int main(void)
{
    int maxStudents = 3000;      /* spec: max 3000 */
    int numberOfStudents = 1000; /* spec: generate 1000 records */
    int i;
    int currentID;
    DATASET *ds;

    srand((unsigned int)time(NULL));  /* seed so each run has different random data */

    ds = createDataSet(maxStudents);
    if (ds == NULL) {
        printf("Failed to create data set.\n");
        return 1;
    }

    /* Spec: first ID = random 1 or 2; each next ID = previous + (random 1 or 2). So IDs are unique and increasing. */
    currentID = randomOneOrTwo();
    for (i = 0; i < numberOfStudents; i++) {
        int age = (rand() % 13) + 18; /* spec: age 18..30 */
        insertion(ds, currentID, age);
        currentID += randomOneOrTwo();
    }

    /* Spec: randomly generate a record (ID 1..2000, age 18..30) and search. App1 searches by age only. */
    {
        int randomID = (rand() % 2000) + 1;
        int randomAge = (rand() % 13) + 18;
        (void)randomID;  /* only randomAge is used for searchAge */
        searchAge(ds, randomAge);
    }

    /* Spec (App1): randomly generate an age; delete ALL students with that age. */
    {
        int ageToDelete = (rand() % 13) + 18;
        deletion(ds, ageToDelete);
    }

    /* Spec (App1): call maxAgeGap and print. Printing is done inside maxAgeGap. */
    maxAgeGap(ds);

    destroyDataSet(ds);
    return 0;
}
