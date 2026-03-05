/* dataset.c - Application 1 (search by age)
 * Data structure: unsorted array + ageCount[] for maxAgeGap.
 * O(1): create, destroy, insertion, maxAgeGap. O(n): searchAge, deletion.
 */

#include <stdio.h>
#include <stdlib.h>

#define MIN_AGE 18
#define MAX_AGE 30
#define AGE_RANGE (MAX_AGE - MIN_AGE + 1)

typedef struct student {
    int id;
    int age;
} STUDENT;

typedef struct dataSet {
    int count;                /* how many students are in the array */
    int capacity;             /* max students allowed (so we know when full) */
    STUDENT *students;        /* array of student records */
    int ageCount[AGE_RANGE];  /* count per age so maxAgeGap can run in O(1) without scanning all n students */
} DATASET;

DATASET *createDataSet(int maxStudents)
{
    int i;
    DATASET *ds = (DATASET *)malloc(sizeof(DATASET));  /* allocate the struct so we can return a handle to the data set */
    if (ds == NULL) {
        return NULL;
    }

    ds->count = 0;                    /* start with no students */
    ds->capacity = maxStudents;       /* remember the max so insertion can check before adding */

    ds->students = (STUDENT *)malloc(sizeof(STUDENT) * maxStudents);  /* allocate space for all students up to max */
    if (ds->students == NULL) {
        free(ds);
        return NULL;
    }

    for (i = 0; i < AGE_RANGE; i++) {
        ds->ageCount[i] = 0;          /* must init to 0 so we can safely increment on each insertion */
    }

    return ds;
}

void destroyDataSet(DATASET *ds)
{
    if (ds != NULL) {
        free(ds->students);           /* free the array first; if we freed ds first we'd lose the pointer to it */
        free(ds);                     /* then free the struct itself */
    }
}

void insertion(DATASET *ds, int id, int age)
{
    if (ds == NULL) {
        return;
    }

    if (ds->count >= ds->capacity) {  /* array is full; adding would write past the end */
        return;
    }

    ds->students[ds->count].id = id;    /* append at end so we don't shift—gives O(1) insertion */
    ds->students[ds->count].age = age; /* same slot holds both fields for this student */
    ds->count++;                       /* one more student; next insertion will use this index */

    if (age >= MIN_AGE && age <= MAX_AGE) {
        ds->ageCount[age - MIN_AGE]++; /* keep count so maxAgeGap can find min/max age without scanning array */
    }
}

void searchAge(DATASET *ds, int age)
{
    int i;
    int found = 0;

    printf("searchAge: searching for students with Age %d\n", age);

    if (ds == NULL) {
        printf("searchAge: data set is NULL\n");
        return;
    }

    for (i = 0; i < ds->count; i++) {           /* must check every slot—array is unsorted by age */
        if (ds->students[i].age == age) {        /* match: this student has the requested age */
            printf("searchAge: found student (ID %d, Age %d)\n",
                   ds->students[i].id, ds->students[i].age);
            found = 1;
        }
    }

    if (!found) {
        printf("searchAge: no student with Age %d found\n", age);
    }
}

void deletion(DATASET *ds, int age)
{
    int deletedAny = 0;
    int i;

    if (ds == NULL) {
        return;
    }

    printf("deletion: attempting to delete all students with Age %d\n", age);

    i = 0;
    while (i < ds->count) {
        if (ds->students[i].age == age) {       /* found one to delete */
            printf("deletion: deleting student (ID %d, Age %d)\n",
                   ds->students[i].id, ds->students[i].age);

            if (age >= MIN_AGE && age <= MAX_AGE) {
                ds->ageCount[age - MIN_AGE]--;  /* keep count in sync so maxAgeGap stays correct */
            }

            ds->students[i] = ds->students[ds->count - 1];  /* copy last into this slot so we don't shift; O(1) per delete */
            ds->count--;                        /* logical size shrinks; last slot is now unused */

            deletedAny = 1;
            /* don't increment i: the element we moved might also have this age, so we must check it again */
        } else {
            i++;
        }
    }

    if (deletedAny) {
        printf("deletion: completed deletion for Age %d\n", age);
    } else {
        printf("deletion: no student with Age %d found to delete\n", age);
    }
}

int maxAgeGap(DATASET *ds)
{
    int i;
    int minAge = -1;
    int maxAge = -1;
    int gap = 0;

    if (ds == NULL) {
        return 0;
    }

    if (ds->count <= 1) {                      /* gap needs at least 2 different ages */
        printf("maxAgeGap: not enough students, maximum age gap is 0\n");
        return 0;
    }

    for (i = 0; i < AGE_RANGE; i++) {
        if (ds->ageCount[i] > 0) {             /* first index with count > 0 is smallest age present */
            minAge = MIN_AGE + i;
            break;
        }
    }

    for (i = AGE_RANGE - 1; i >= 0; i--) {
        if (ds->ageCount[i] > 0) {             /* last index with count > 0 is largest age present */
            maxAge = MIN_AGE + i;
            break;
        }
    }

    if (minAge != -1 && maxAge != -1) {
        gap = maxAge - minAge;                 /* gap is the difference; we use ageCount so this is O(13) not O(n) */
    }

    printf("maxAgeGap: maximum age gap is %d\n", gap);
    return gap;
}
