/* dataset.c - Application 2 (search by ID)
 * Author: Tyler Cary
 * Data structure: hash table with direct addressing. h(k) = k % TABLE_SIZE. Linear probing.
 * Time complexity: create O(1), destroy O(1); insertion, searchID, deletion: O(1) average, O(m) worst-case (m = table size).
 */

#include <stdio.h>
#include <stdlib.h>

/* One student record: ID and Age. */
typedef struct student {
    int id;
    int age;
} STUDENT;

/* Table size must be prime. 3001 is prime and >= max students (3000). */
#define TABLE_SIZE 3001

/* Data set ADT for App2: hash table with direct addressing.
 * table[i] holds a student; id == 0 means the slot is empty.
 * count = number of students currently in the table.
 */
typedef struct dataSet {
    int count;              /* current number of students */
    int capacity;           /* max students allowed */
    STUDENT *table;         /* hash table: table[index].id is 0 if empty */
} DATASET;

/* createDataSet: Allocate struct and hash table of size TABLE_SIZE. O(1). */
DATASET *createDataSet(int maxStudents)
{
    int i;

    DATASET *ds = (DATASET *)malloc(sizeof(DATASET));  /* allocate the data set struct */
    if (ds == NULL) {
        return NULL;                                    /* allocation failed */
    }

    ds->count = 0;                                      /* no students yet */
    ds->capacity = maxStudents;                          /* set max capacity */

    ds->table = (STUDENT *)malloc(sizeof(STUDENT) * TABLE_SIZE);  /* allocate the hash table array */
    if (ds->table == NULL) {
        free(ds);                                       /* clean up if table allocation failed */
        return NULL;
    }

    /* Mark all slots empty (id = 0). */
    for (i = 0; i < TABLE_SIZE; i++) {
        ds->table[i].id = 0;                            /* 0 means this slot is empty */
        ds->table[i].age = 0;
    }

    return ds;                                           /* return the new data set */
}

/* destroyDataSet: Free table then struct. O(1). */
void destroyDataSet(DATASET *ds)
{
    if (ds != NULL) {
        free(ds->table);                                /* free the hash table array */
        free(ds);                                       /* free the struct itself */
    }
}

/* Hash function: direct addressing. h(k) = k % TABLE_SIZE. */
static int hash(int id)
{
    int h = id % TABLE_SIZE;                            /* compute index using modulo */
    if (h < 0) {
        h += TABLE_SIZE;                                /* handle negative result */
    }
    return h;                                           /* return the hash index */
}

/* insertion: Add (id, age) to the hash table. Linear probing if collision. O(1) average. */
void insertion(DATASET *ds, int id, int age)
{
    int i;
    int start;

    if (ds == NULL) {
        return;                                         /* nothing to insert into */
    }

    if (ds->count >= ds->capacity) {
        return;                                         /* table is full */
    }

    if (id <= 0) {
        return;                                         /* invalid id */
    }

    start = hash(id);                                   /* get the starting index */
    i = start;

    do {
        if (ds->table[i].id == 0) {
            /* empty slot: store here */
            ds->table[i].id = id;                       /* save the student id */
            ds->table[i].age = age;                     /* save the student age */
            ds->count++;                                /* increment student count */
            return;
        }
        if (ds->table[i].id == id) {
            /* already present: update age */
            ds->table[i].age = age;
            return;
        }
        /* linear probing: next slot */
        i = (i + 1) % TABLE_SIZE;                      /* wrap around to beginning if needed */
    } while (i != start);                               /* stop if we loop back to start */

    /* table full (should not happen if count < capacity and TABLE_SIZE >= capacity) */
}

/* searchID: Find student with given id. Direct hash then linear probe. O(1) average. */
void searchID(DATASET *ds, int id)
{
    int i;
    int start;

    printf("searchID: searching for student with ID %d\n", id);

    if (ds == NULL) {
        printf("searchID: data set is NULL\n");
        return;                                         /* nothing to search */
    }

    if (id <= 0) {
        printf("searchID: student with ID %d not found\n", id);
        return;                                         /* invalid id */
    }

    start = hash(id);                                   /* hash to get starting index */
    i = start;

    do {
        if (ds->table[i].id == 0) {
            /* empty slot means the student is not in the table */
            printf("searchID: student with ID %d not found\n", id);
            return;
        }
        if (ds->table[i].id == id) {
            /* found the matching student */
            printf("searchID: found student (ID %d, Age %d)\n",
                   ds->table[i].id, ds->table[i].age);
            return;
        }
        i = (i + 1) % TABLE_SIZE;                      /* linear probe to next slot */
    } while (i != start);                               /* stop if we checked every slot */

    printf("searchID: student with ID %d not found\n", id);
}

/* deletion: Remove the one student with the given id (if present). O(1) average. */
void deletion(DATASET *ds, int id)
{
    int i;
    int start;

    if (ds == NULL) {
        return;                                         /* nothing to delete from */
    }

    printf("deletion: attempting to delete student with ID %d\n", id);

    if (id <= 0) {
        printf("deletion: student with ID %d not found, nothing deleted\n", id);
        return;                                         /* invalid id */
    }

    start = hash(id);                                   /* hash to get starting index */
    i = start;

    do {
        if (ds->table[i].id == 0) {
            /* empty slot means the student is not in the table */
            printf("deletion: student with ID %d not found, nothing deleted\n", id);
            return;
        }
        if (ds->table[i].id == id) {
            /* found the student to delete */
            printf("deletion: deleting student (ID %d, Age %d)\n",
                   ds->table[i].id, ds->table[i].age);
            ds->table[i].id = 0;                        /* clear the id to mark slot empty */
            ds->table[i].age = 0;                       /* clear the age */
            ds->count--;                                /* decrement student count */
            printf("deletion: student with ID %d has been deleted\n", id);
            return;
        }
        i = (i + 1) % TABLE_SIZE;                      /* linear probe to next slot */
    } while (i != start);                               /* stop if we checked every slot */

    printf("deletion: student with ID %d not found, nothing deleted\n", id);
}
