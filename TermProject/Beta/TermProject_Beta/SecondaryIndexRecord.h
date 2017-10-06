//
//  SecondaryIndexRecord.h
//  TermProject_Beta
//
//  Created by Sam Jentsch on 11/12/14.
//  Copyright (c) 2014 Sam Jentsch. All rights reserved.
//

#ifndef __TermProject_Beta__SecondaryIndexRecord__
#define __TermProject_Beta__SecondaryIndexRecord__

#include <stdio.h>
#include <vector>
#include <iostream>

#include "KeyRecord.h"

class SecondaryIndexRecord: public KeyRecord {
private:
    int binarySearch(int key, std::vector<int> searchList);
public:
    std::vector<int> primaryKeys;
    
    SecondaryIndexRecord();
    SecondaryIndexRecord(int key);
    
    void addPrimaryKey(int primaryKey);
    void removePrimaryKey(int primaryKey);
    void printPrimaryKeys();
    
};

#endif /* defined(__TermProject_Beta__SecondaryIndexRecord__) */
