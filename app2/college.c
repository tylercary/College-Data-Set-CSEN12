/* college.c - Application 2 (search by ID)
 * Author: Tyler Cary
 * Driver: create data set (hash table), insert 1000 students, search by ID, deletion by ID, destroy. Per spec.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct dataSet DATASET;

DATASET *createDataSet(int maxStudents);
void destroyDataSet(DATASET *ds);
void insertion(DATASET *ds, int id, int age);
void searchID(DATASET *ds, int id);
void deletion(DATASET *ds, int id);

/* Spec: random 1 or 2 for building unique increasing IDs. */
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

    srand((unsigned int)time(NULL));

    ds = createDataSet(maxStudents);
    if (ds == NULL) {
        printf("Failed to create data set.\n");
        return 1;
    }

    /* Same ID rule: first 1 or 2, then prev + 1 or 2. insertion() stores in hash table. */
    currentID = randomOneOrTwo();
    for (i = 0; i < numberOfStudents; i++) {
        int age = (rand() % 13) + 18;
        insertion(ds, currentID, age);
        currentID += randomOneOrTwo();
    }

    /* Spec: randomly generate ID (1..2000) and search by ID. App2 only searches by ID. */
    {
        int randomID = (rand() % 2000) + 1;
        int randomAge = (rand() % 13) + 18;
        (void)randomAge;
        searchID(ds, randomID);
    }

    /* Spec (App2): randomly generate ID; delete that one record if found. */
    {
        int idToDelete = (rand() % 2000) + 1;
        deletion(ds, idToDelete);
    }

    destroyDataSet(ds);
    return 0;
}
