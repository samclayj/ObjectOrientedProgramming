//
//  KeyRecord.h
//  TermProject_Beta
//
//  Created by Sam Jentsch on 11/12/14.
//  Copyright (c) 2014 Sam Jentsch. All rights reserved.
//

#ifndef __TermProject_Beta__KeyRecord__
#define __TermProject_Beta__KeyRecord__

#include <stdio.h>

class KeyRecord {
protected:
    int key;
public:
    //***key***//
    void setKey(int key);
    int getKey() const;
    
    
    //***Overloaded Constructors***//
    bool operator< (const KeyRecord &rec) const;
    bool operator> (const KeyRecord &rec) const;
    bool operator== (const KeyRecord &rec) const;
};



#endif /* defined(__TermProject_Beta__KeyRecord__) */
