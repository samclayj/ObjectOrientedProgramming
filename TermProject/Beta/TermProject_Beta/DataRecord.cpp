#include "DataRecord.h"

#pragma mark - Constructors
DataRecord::DataRecord() {
    //Default constructor.
    //Initialize data members to default values.
    setCost(0);
    setCode(0);
    setKey(0);
    
    char blankName[8] = {};
    setName(blankName);
}

DataRecord::DataRecord(int key, const char* name, int code, double cost)
:key(key), code(code), cost(cost)
{
    char blankName[8] = {};
    setName(blankName);
    setName(name);
}

#pragma mark - Getters/Setters
void DataRecord::setKey(int key) {
    //--------------------------------------------------------------------------------//
    //Preconditions: key parameter passed as integer.
    //Postconditions: the key attribute is set to the parameter passed.
    //--------------------------------------------------------------------------------//
    
    DataRecord::key = key;
}

int DataRecord::getKey() const {
    //--------------------------------------------------------------------------------//
    //Postconditions: key is returned to the calling method.
    //--------------------------------------------------------------------------------//
    
    return key;
}

void DataRecord::setName(const char *nameString) {
    //--------------------------------------------------------------------------------//
    //Preconditions: count parameter passed as integer.
    //Postconditions: the count attribute is set to the parameter passed.
    //--------------------------------------------------------------------------------//
    
    char c = nameString[0];
    int count = 0;
    while (isalnum(c)) {
        name[count] = c;
        c = nameString[++count];
    }
    name[count] = '\0';
}

std::string DataRecord::getName() {
    //--------------------------------------------------------------------------------//
    //Preconditions: description string pointer passed as string
    //Postconditions: a string containing the values of the class attribute description
    //is created and returned to the caller.
    //--------------------------------------------------------------------------------//
    
    std::string nameString(name);
    
    return nameString;
}

void DataRecord::setCode(int code) {
    //--------------------------------------------------------------------------------//
    //Preconditions: count parameter passed as integer.
    //Postconditions: the count attribute is set to the parameter passed.
    //--------------------------------------------------------------------------------//
    
    DataRecord::code = code;
}

int DataRecord::getCode() const {
    //--------------------------------------------------------------------------------//
    //Postconditions: count is returned to the calling method.
    //--------------------------------------------------------------------------------//
    
    return code;
}

void DataRecord::setCost(double cost) {
    //--------------------------------------------------------------------------------//
    //Preconditions: nextPart parameter passed as integer.
    //Postconditions: the nextPart attribute is set to the parameter passed.
    //--------------------------------------------------------------------------------//
    
    DataRecord::cost = cost;
}

double DataRecord::getCost() const {
    //--------------------------------------------------------------------------------//
    //Postconditions: nextPart is returned to the calling method.
    //--------------------------------------------------------------------------------//
    
    return cost;
}

#pragma mark - Print
void DataRecord::printRecord() {
    //--------------------------------------------------------------------------------//
    //Preconditions: none
    //Postconditions: the attributes of the DataRecord instance are printed to the
    //console.
    //--------------------------------------------------------------------------------//
    
    std::cout << key << " " << name << " " << code << " " << cost << std::endl;
}
