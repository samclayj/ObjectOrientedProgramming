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
 a. This class is used to hold each of the attributes for a data record.
 
 Records are in the form:
 
 INT CHAR[8] INT DOUBlE
 KEY DESCRIPTION COUNT NEXTPART
 
 VARIABLE DICTIONARY:
 
 key - int, contains the primary key for the record.
 name - char[8], an 8 character text name for the record.
 code - int, the code associated with the record.
 cost - double, the cost associated with the record.
 -----------------------------------------------------------------------
 */


#ifndef ___31_Project4__DataRecord__
#define ___31_Project4__DataRecord__

#include <stdio.h>
#include <iostream>

class DataRecord {
private:
    int key;
    char name[8];
    int code;
    double cost;
public:
    DataRecord();
    DataRecord(int key, const char *name, int code, double cost);
    
    //***key***//
    void setKey(int key);
    int getKey() const;
    
    //***name***//
    void setName(const char *name);
    std::string getName();
    
    //***code***//
    void setCode(int code);
    int getCode() const;
    
    //***cost***//
    void setCost(double cost);
    double getCost() const;
    
    void printRecord();
};

#endif /* defined(___31_Project4__DataRecord__) */
