/* dataset.c - Application 1 (search by age)
 * Author: Tyler Cary
 * Data structure: unsorted array + ageCount[] for maxAgeGap.
 * Time complexity: create O(1), destroy O(1), insertion O(1), searchAge O(n), deletion O(n), maxAgeGap O(1).
 */

#include <stdio.h>
#include <stdlib.h>

/* Age range 18-30 inclusive = 13 possible values. Used for array bounds and ageCount size. */
#define MIN_AGE 18
#define MAX_AGE 30
#define AGE_RANGE (MAX_AGE - MIN_AGE + 1)

/* One student record: ID (unique, never recycled) and Age. */
typedef struct student {
    int id;
    int age;
} STUDENT;

/* Data set ADT for App1:
 * - students[]: unsorted array; append at end so insertion is O(1).
 * - ageCount[]: ageCount[i] = number of students with age (MIN_AGE + i).
 *   Lets us compute max age gap by scanning 13 buckets instead of n students.
 */
typedef struct dataSet {
    int count;                /* current number of students in the array */
    int capacity;             /* max students allowed (used to check when full) */
    STUDENT *students;        /* array of student records */
    int ageCount[AGE_RANGE];  /* count per age 18..30; index 0 = age 18, etc. */
} DATASET;

/* createDataSet: Allocate and initialize a new data set that can hold up to maxStudents.
 * Returns pointer to DATASET, or NULL if allocation fails.
 * Struct and student array allocated once; no realloc. O(1). */
DATASET *createDataSet(int maxStudents)
{
    int i;
    DATASET *ds = (DATASET *)malloc(sizeof(DATASET));  /* allocate the struct to return a handle to the data set */
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
        ds->ageCount[i] = 0;          /* init to 0 so each insertion can safely increment */
    }

    return ds;
}

/* destroyDataSet: Free all memory used by the data set.
 * Must free students[] first, then ds; otherwise the pointer to the array is lost. O(1). */
void destroyDataSet(DATASET *ds)
{
    if (ds != NULL) {
        free(ds->students);           /* free the array first; if ds is freed first the pointer to the array is lost */
        free(ds);                     /* then free the struct itself */
    }
}

/* insertion: Add one student (id, age) to the data set.
 * Append at index count (no shifting), so insertion is O(1).
 * ageCount[age - MIN_AGE] is updated so maxAgeGap can use it later.
 * Sorting by age would make insertion O(n); not needed because search is by linear scan. */
void insertion(DATASET *ds, int id, int age)
{
    if (ds == NULL) {
        return;
    }

    if (ds->count >= ds->capacity) {  /* array is full; adding would write past the end */
        return;
    }

    ds->students[ds->count].id = id;    /* append at end so no shifting - gives O(1) insertion */
    ds->students[ds->count].age = age; /* same slot holds both fields for this student */
    ds->count++;                       /* one more student; next insertion will use this index */

    if (age >= MIN_AGE && age <= MAX_AGE) {
        ds->ageCount[age - MIN_AGE]++; /* keep count so maxAgeGap can find min/max age without scanning array */
    }
}

/* searchAge: Find and print all students with the given age.
 * Whole array is scanned because it is unsorted by age. O(n).
 * Spec requires returning multiple records; each match is printed. */
void searchAge(DATASET *ds, int age)
{
    int i;
    int found = 0;

    printf("searchAge: searching for students with Age %d\n", age);

    if (ds == NULL) {
        printf("searchAge: data set is NULL\n");
        return;
    }

    for (i = 0; i < ds->count; i++) {           /* must check every slot - array is unsorted by age */
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

/* deletion: Remove ALL students with the given age.
 * For each match the slot is overwritten with the last element and count is decremented (no shifting).
 * Index i is not incremented after a delete because the moved element might also have that age. O(n). */
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

            ds->students[i] = ds->students[ds->count - 1];  /* copy last into this slot so no shifting; O(1) per delete */
            ds->count--;                        /* logical size shrinks; last slot is now unused */

            deletedAny = 1;
            /* do not increment i: the element moved here might also have this age and must be checked again */
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

/* maxAgeGap: Return (max age present) - (min age present).
 * ageCount[] only: find first and last index with count > 0. O(13) = O(1).
 * Without ageCount the whole array would need to be scanned to find min/max age. */
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
        if (ds->ageCount[i] > 0) {             /* smallest age present: first bucket with any students */
            minAge = MIN_AGE + i;              /* index i corresponds to age (18 + i) */
            break;
        }
    }

    for (i = AGE_RANGE - 1; i >= 0; i--) {
        if (ds->ageCount[i] > 0) {             /* largest age present: last bucket with any students */
            maxAge = MIN_AGE + i;
            break;
        }
    }

    if (minAge != -1 && maxAge != -1) {
        gap = maxAge - minAge;                 /* gap is the difference; ageCount makes this O(13) not O(n) */
    }

    printf("maxAgeGap: maximum age gap is %d\n", gap);
    return gap;
}
