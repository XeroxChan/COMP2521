// Implementation of the Student DB ADT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"
#include "StudentDb.h"
#include "Tree.h"

struct studentDb {
    Tree byZid;
    Tree byName;
};

////////////////////////////////////////////////////////////////////////
// Comparison functions

/**
 * Compares two records by zid only and returns:
 * - A negative number if the first record is less than the second
 * - Zero if the records are equal
 * - A positive number if the first record is greater than the second
 */
int compareByZid(Record r1, Record r2) {
    return RecordGetZid(r1) - RecordGetZid(r2);
}

/**
 * Compares two records by name (family name first) and then by
 * zid if the names are equal, and returns:
 * - A negative number if the first record is less than the second
 * - Zero if the records are equal
 * - A positive number if the first record is greater than the second
 */
int compareByName(Record r1, Record r2) {
    // TODO: Complete this function
    
    //compare by family name
    int cmpF = strcmp(RecordGetFamilyName(r1), RecordGetFamilyName(r2));
    //compare by given name
    int cmpG = strcmp(RecordGetGivenName(r1), RecordGetGivenName(r2));
    
    //return result of strcmp of family name if names not the smae
    if (cmpF != 0) {
        return cmpF;
    }
    //return result of strcmp of given name if names not the smae
    else if (cmpG != 0) {
        return cmpG;
    }
    //same names, so compare zid
    else {
        return compareByZid(r1, r2);
    }
    
    
    
    
}

////////////////////////////////////////////////////////////////////////

StudentDb DbNew(void) {
    StudentDb db = malloc(sizeof(*db));
    if (db == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    db->byZid = TreeNew(compareByZid);
    // TODO: Modify this line to use compareByName once you have
    //       implemented it
    
    //Modified
    db->byName = TreeNew(compareByName);
    return db;
}

void DbFree(StudentDb db) {
    TreeFree(db->byZid, false);
    TreeFree(db->byName, true);
    free(db);
}

////////////////////////////////////////////////////////////////////////

bool DbInsertRecord(StudentDb db, Record r) {
    if (TreeInsert(db->byZid, r)) {
        TreeInsert(db->byName, r);
        return true;
    } else {
        return false;
    }
}

////////////////////////////////////////////////////////////////////////

bool DbDeleteByZid(StudentDb db, int zid) {
    Record dummy = RecordNew(zid, "", "");
    Record r = TreeSearch(db->byZid, dummy);

    if (r != NULL) {
        TreeDelete(db->byZid, r);
        TreeDelete(db->byName, r);
        RecordFree(r);
        RecordFree(dummy);
        return true;
    } else {
        RecordFree(dummy);
        return false;
    }
}

////////////////////////////////////////////////////////////////////////

Record DbFindByZid(StudentDb db, int zid) {
    Record dummy = RecordNew(zid, "", "");
    Record r = TreeSearch(db->byZid, dummy);
    RecordFree(dummy);
    return r;
}

////////////////////////////////////////////////////////////////////////

List DbFindByName(StudentDb db, char *familyName, char *givenName) {
    // TODO: Complete this function
    
    //create new list to store result
    List resultList = ListNew();
    //create dummy to introduce the range for TreeSearchBetween
    //range from MIN_ZID to MAX_ZID
    Record dummy_lower = RecordNew(MIN_ZID, familyName, givenName);
    Record dummy_upper = RecordNew(MAX_ZID, familyName, givenName);
    //Find names using TreeSearchBetween
    resultList = TreeSearchBetween(db->byName, dummy_lower, dummy_upper);
    //free dummy records create
    RecordFree(dummy_lower);
    RecordFree(dummy_upper);
    return resultList;
    
}

////////////////////////////////////////////////////////////////////////

void DbListByZid(StudentDb db) {
    TreeListInOrder(db->byZid);
}

void DbListByName(StudentDb db) {
    // TODO: Complete this function
    TreeListInOrder(db->byName);
}
