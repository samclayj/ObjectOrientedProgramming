//
//  KeyRecord.cpp
//  TermProject_Beta
//
//  Created by Sam Jentsch on 11/12/14.
//  Copyright (c) 2014 Sam Jentsch. All rights reserved.
//

#include "KeyRecord.h"


#pragma mark - Getters/Setters
void KeyRecord::setKey(int key) {
    //--------------------------------------------------------------------------------//
    //Preconditions: key parameter passed as integer.
    //Postconditions: the key attribute is set to the parameter passed.
    //--------------------------------------------------------------------------------//
    KeyRecord::key = key;
}

int KeyRecord::getKey() const {
    //--------------------------------------------------------------------------------//
    //Postconditions: key is returned to the calling method.
    //--------------------------------------------------------------------------------//
    return key;
}

#pragma mark - Overridden Operators
//Operators are overridden to compare the key class attribute.
bool KeyRecord::operator< (const KeyRecord &rec) const {
    return key < rec.getKey();
}

bool KeyRecord::operator> (const KeyRecord &rec) const {
    return key > rec.getKey();
}

bool KeyRecord::operator== (const KeyRecord &rec) const {
    return key == rec.getKey();
}
