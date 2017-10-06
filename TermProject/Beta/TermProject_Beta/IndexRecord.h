/*
 -----------------------------------------------------------------------
 PROGRAM NAME: Project 5, Key Index File
 PROGRAMMER:   Samuel Jentsch
 CLASS:        CSC 331.001, Fall 2014
 INSTRUCTOR:   Dr. R. Strader
 DATE STARTED: October 25, 2014
 DUE DATE:     October 28, 2014
 REFERENCES:   C++ How to Program
 Paul Deitel & Harvey Deitel
 Dr. Strader: assignment information sheet
 
 PROGRAM PURPOSE:
 a. This class is used to hold an index record referencing a record in a
    data file.
 
 Records are in the form:
 
 INT INT
 KEY RRN
 
 VARIABLE DICTIONARY:
 
 key - int, contains the primary key for the record.
 relativeOffset - int, contains the relative offset for the record
                    referenced by the index record
 -----------------------------------------------------------------------
 */

#ifndef ___31_Project5__IndexRecord__
#define ___31_Project5__IndexRecord__

#include <stdio.h>
#include "KeyRecord.h"

class IndexRecord: public KeyRecord  {
private:
    int relativeOffset;
public:
    IndexRecord();
    IndexRecord(int key, int relativeOffset);
    
    //***relativeOffset***//
    void setRelativeOffset(int relativeOffset);
    int getRelativeOffset() const;
};



#endif /* defined(___31_Project5__IndexRecord__) */
