#include "IndexRecord.h"

#pragma mark - Constructors
IndexRecord::IndexRecord() {
    //Default constructor
    setKey(0);
    setRelativeOffset(0);
}

IndexRecord::IndexRecord(int key, int relativeOffset) {
    setKey(key);
    setRelativeOffset(relativeOffset);
}

#pragma mark - Getters/Setters

void IndexRecord::setRelativeOffset(int relativeOffset) {
    //--------------------------------------------------------------------------------//
    //Preconditions: relativeOffset parameter passed as integer.
    //Postconditions: the relativeOffset attribute is set to the parameter passed.
    //--------------------------------------------------------------------------------//
    IndexRecord::relativeOffset = relativeOffset;
}

int IndexRecord::getRelativeOffset() const {
    //--------------------------------------------------------------------------------//
    //Postconditions: RRN is returned to the calling method.
    //--------------------------------------------------------------------------------//
    return relativeOffset;
}
