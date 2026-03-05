/* dataset.c - Application 2 (search by ID)
 * Data structure: array sorted by ID (binary search).
 * O(1): create, destroy. O(log n): searchID. O(n): insertion, deletion.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct student {
    int id;
    int age;
} STUDENT;

typedef struct dataSet {
    int count;         /* how many students in array */
    int capacity;      /* max allowed so we know when full */
    STUDENT *students; /* kept sorted by id so we can binary search by ID */
} DATASET;

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
        free(ds);   /* avoid leak: we already allocated ds */
        return NULL;
    }

    return ds;
}

void destroyDataSet(DATASET *ds)
{
    if (ds != NULL) {
        free(ds->students);   /* free array first; after free(ds) we can't access ds->students */
        free(ds);
    }
}

void insertion(DATASET *ds, int id, int age)
{
    int i;

    if (ds == NULL) {
        return;
    }

    if (ds->count >= ds->capacity) {  /* no room; would write past end of array */
        return;
    }

    i = ds->count - 1;                 /* start at last element to find insert position */
    while (i >= 0 && ds->students[i].id > id) {  /* shift right to make room; keep array sorted by id */
        ds->students[i + 1] = ds->students[i];
        i--;
    }

    ds->students[i + 1].id = id;       /* insert in the gap we made so order is preserved */
    ds->students[i + 1].age = age;
    ds->count++;
}

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

    while (low <= high) {                          /* binary search: only works because array is sorted by id */
        int mid = (low + high) / 2;                 /* check middle */
        int midID = ds->students[mid].id;

        if (midID == id) {
            printf("searchID: found student (ID %d, Age %d)\n",
                   ds->students[mid].id, ds->students[mid].age);
            found = 1;
            break;
        } else if (midID < id) {
            low = mid + 1;                          /* id is in right half, so search there */
        } else {
            high = mid - 1;                         /* id is in left half */
        }
    }

    if (!found) {
        printf("searchID: student with ID %d not found\n", id);
    }
}

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
            index = mid;                            /* need index so we can shift from here */
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
        ds->students[i] = ds->students[i + 1];    /* shift left to close the gap and keep array sorted */
    }

    ds->count--;                                   /* logical size decreases */

    printf("deletion: student with ID %d has been deleted\n", id);
}
