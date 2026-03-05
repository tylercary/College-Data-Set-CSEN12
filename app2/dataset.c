/* dataset.c - Application 2 (search by ID)
 * Data structure: array kept SORTED by student ID so binary search can be used for searchID.
 * O(1): create, destroy. O(log n): searchID. O(n): insertion, deletion (shift to keep sorted).
 */

#include <stdio.h>
#include <stdlib.h>

/* One student record: ID and Age. Same as App1. */
typedef struct student {
    int id;
    int age;
} STUDENT;

/* Data set ADT for App2:
 * students[] is always sorted by id. That allows binary search for searchID in O(log n).
 * Insertion and deletion do shifting to maintain sorted order.
 */
typedef struct dataSet {
    int count;         /* current number of students */
    int capacity;      /* max allowed */
    STUDENT *students; /* always sorted by id (ascending) */
} DATASET;

/* createDataSet: Allocate struct and array for up to maxStudents. O(1). */
DATASET *createDataSet(int maxStudents)
{
    DATASET *ds = (DATASET *)malloc(sizeof(DATASET));  /* allocate struct so caller gets a handle */
    if (ds == NULL) {
        return NULL;
    }

    ds->count = 0;
    ds->capacity = maxStudents;

    ds->students = (STUDENT *)malloc(sizeof(STUDENT) * maxStudents);  /* array for up to maxStudents records */
    if (ds->students == NULL) {
        free(ds);   /* avoid leak: ds was already allocated */
        return NULL;
    }

    return ds;
}

/* destroyDataSet: Free array then struct. Same as App1. O(1). */
void destroyDataSet(DATASET *ds)
{
    if (ds != NULL) {
        free(ds->students);   /* free array first; after free(ds) the pointer to the array is lost */
        free(ds);
    }
}

/* insertion: Add (id, age) and keep array sorted by id.
 * Find position by scanning from the end: shift every element with id > new id one slot right, then insert.
 * O(n) but insertion is rare in App2; design optimizes for searchID which is frequent. */
void insertion(DATASET *ds, int id, int age)
{
    int i;

    if (ds == NULL) {
        return;
    }

    if (ds->count >= ds->capacity) {  /* no room; would write past end of array */
        return;
    }

    i = ds->count - 1;                 /* start at last element */
    while (i >= 0 && ds->students[i].id > id) {  /* shift right until the position where new id fits is found */
        ds->students[i + 1] = ds->students[i];  /* move element right to make a gap */
        i--;
    }
    /* after loop: students[i].id <= id (or i is -1), so insert at i+1 */
    ds->students[i + 1].id = id;
    ds->students[i + 1].age = age;
    ds->count++;
}

/* searchID: Find student with given id using binary search. O(log n).
 * Compare with middle element; if id < mid search left half, else right half. Repeat until found or range empty. */
void searchID(DATASET *ds, int id)
{
    int low = 0;
    int high;
    int found = 0;

    printf("searchID: searching for student with ID %d\n", id);

    if (ds == NULL) {
        printf("searchID: data set is NULL\n");
        return;
    }

    high = ds->count - 1;

    while (low <= high) {                          /* binary search: narrow range until found or empty */
        int mid = (low + high) / 2;                 /* check middle index */
        int midID = ds->students[mid].id;

        if (midID == id) {
            printf("searchID: found student (ID %d, Age %d)\n",
                   ds->students[mid].id, ds->students[mid].age);
            found = 1;
            break;
        } else if (midID < id) {
            low = mid + 1;                          /* target id is larger, so it's in the right half */
        } else {
            high = mid - 1;                         /* target id is smaller, so it's in the left half */
        }
    }

    if (!found) {
        printf("searchID: student with ID %d not found\n", id);
    }
}

/* deletion: Remove the one student with the given id (if present).
 * Binary search to find index, then shift all elements after it left by one. O(log n) + O(n) = O(n).
 * Index is needed to know where to shift from; same binary search as searchID but stores the index. */
void deletion(DATASET *ds, int id)
{
    int low = 0;
    int high;
    int index = -1;
    int i;

    if (ds == NULL) {
        return;
    }

    printf("deletion: attempting to delete student with ID %d\n", id);

    high = ds->count - 1;

    while (low <= high) {                          /* same binary search as searchID to find index */
        int mid = (low + high) / 2;
        int midID = ds->students[mid].id;

        if (midID == id) {
            index = mid;                            /* index needed so the shift loop knows where to start */
            break;
        } else if (midID < id) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (index == -1) {                             /* not found, nothing to delete */
        printf("deletion: student with ID %d not found, nothing deleted\n", id);
        return;
    }

    printf("deletion: deleting student (ID %d, Age %d)\n",
           ds->students[index].id, ds->students[index].age);

    for (i = index; i < ds->count - 1; i++) {
        ds->students[i] = ds->students[i + 1];    /* shift left: each element takes the place of the next */
    }
    /* after loop: the slot at index is filled, and the old last slot is duplicate; decrement count to "remove" it */
    ds->count--;

    printf("deletion: student with ID %d has been deleted\n", id);
}
