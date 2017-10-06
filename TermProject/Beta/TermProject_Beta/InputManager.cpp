//InputManager 

#include <iostream>
#include <fstream>

#include "IndexListManager.h"
#include "ProductManager.h"

using namespace std;

IndexListManager *indexManager;
ProductManager *productManager;

int parseInput(const char* arguments[], int numberOfArguments);

void handleSearchForPrimaryKey(int key);
void handleSearchForSecondaryKey(int key);
void handleSearchForDataValue(string dataValue);

void handleAdd(string record);
void handleDeletePrimary(int key);
void handleDeleteSecondary(int code);
void handleModify(int key, string dataLabel, string newData);
int getFlagForDataLabel(string dataLabel);

bool checkDouble(string checkString);
int convertStringToInt(std::string intString);
double convertStringToDouble(std::string doubleString);

void welcomeMessage();

int main(int argc, const char * argv[]) {
    ifstream textDataFile;
    fstream primaryIndexFile;
    fstream secondaryIndexFile;
    fstream binaryDataFile;

    productManager = new ProductManager(binaryDataFile);
    indexManager = new IndexListManager(primaryIndexFile, secondaryIndexFile);
    
    binaryDataFile.open("BinaryDataFile.dat", fstream::ate | fstream::binary| fstream::out | fstream::in);
    if (!binaryDataFile.is_open()) {
        welcomeMessage();
        
        //Open the text data file
        string textFilePath;
        if (argc == 2) {
            textFilePath = argv[1];
        } else {
            cout << "Enter a path to the data file: ";
            cin >> textFilePath;
        }
        
        cout << "CREATING NEW FILE" << endl;
        ofstream binCreate("BinaryDataFile.dat");
        binCreate.close();
        binaryDataFile.open("BinaryDataFile.dat", fstream::binary| fstream::out | fstream::in);
        
        textDataFile.open(textFilePath.c_str(), ios::in);
        productManager->createBinaryRecordFile(textDataFile);
        textDataFile.close();
    }
    
    //Open a binary file
    primaryIndexFile.open("PrimaryKey.ixp", fstream::ate |fstream::out | fstream::in | fstream::binary);
    secondaryIndexFile.open("SecondaryKey.ixp", fstream::ate |fstream::out | fstream::in | fstream::binary);
    if (!primaryIndexFile.is_open()) {
        cout << "CREATING NEW FILE" << endl;
        ofstream indexCreate("PrimaryKey.ixp");
        indexCreate.close();
        ofstream secondaryCreate("SecondaryKey.ixp");
        secondaryCreate.close();
        
        secondaryIndexFile.open("SecondaryKey.ixp", fstream::ate |fstream::out | fstream::in | fstream::binary);
        primaryIndexFile.open("PrimaryKey.ixp", fstream::binary| fstream::out | fstream::in);
        
        indexManager->populateKeyListFromDataRecordFile(binaryDataFile);
        indexManager->printKeyList();
        indexManager->savePrimaryKeyListToFile();
        indexManager->saveSecondaryKeyListToFile();
    } else {
        indexManager->populateKeyListFromIndexRecordFile();
        indexManager->populateSecondaryKeyListFromFile();
    }
    
    parseInput(argv, argc);
    
    binaryDataFile.flush();
    binaryDataFile.close();
    
    primaryIndexFile.flush();
    primaryIndexFile.close();
    
    return 0;
}

void welcomeMessage() {
    cout << " _ .-') _     ('-.     .-') _      ('-.     \n( (  OO) )   ( OO ).-.(  OO) )    ( OO ).-. \n \\     .'_   / . --. //     '._   / . --. / \n ,`'--..._)  | \\-.  \\ |'--...__)  | \\-.  \\  \n |  |  \\  '.-'-'  |  |'--.  .--'.-'-'  |  | \n |  |   ' | \\| |_.'  |   |  |    \\| |_.'  | \n |  |   / :  |  .-.  |   |  |     |  .-.  | \n |  '--'  /  |  | |  |   |  |     |  | |  | \n `-------'   `--' `--'   `--'     `--' `--' \n.-. .-')                 .-')     .-')    \n\\  ( OO )               ( OO ).  ( OO ).  \n ;-----.\\  .-'),-----. (_)---\\_)(_)---\\_) \n | .-.  | ( OO'  .-.  '/    _ | /    _ |  \n | '-' /_)/   |  | |  |\\  :` `. \\  :` `.  \n | .-. `. \\_) |  |\\|  | '..`''.) '..`''.) \n | |  \\  |  \\ |  | |  |.-._)   \\.-._)   \\ \n | '--'  /   `'  '-'  '\\       /\\       / \n `------'      `-----'  `-----'  `-----' " << endl;
    cout << "   ____  U _____ u  _____      _      \nU | __\")u\\| ___\"|/ |_ \" _| U  /\"\\  u  \n \\|  _ \\/ |  _|\"     | |    \\/ _ \\/   \n  | |_) | | |___    /| |\\   / ___ \\   \n  |____/  |_____|  u |_|U  /_/   \\_\\  \n _|| \\\\_  <<   >>  _// \\\\_  \\\\    >>  \n(__) (__)(__) (__)(__) (__)(__)  (__)" << endl;
    cout << "WELCOME!\n";
    cout << "Available commands in this alpha version are: " << endl;
    cout << "DISPLAY" << endl;
    cout << "SEARCH FLAG VALUE or DELETE FLAG VALUE" << endl;
    cout << "MOD KEY DATA_LABEL NEW_DATA" << endl;
    cout << "ADD KEY NAME CODE COST" << endl;
}

int parseInput(const char* arguments[], int numberOfArguments) {
    numberOfArguments--;

    if (numberOfArguments == 0) {
        //error
        cout << "ARGUMENTS REQUIRED." << endl;
        return -1;
    }
    
    string command(arguments[1]);

    if (numberOfArguments == 1) {
        //Display
        if (command.compare("DISPLAY") == 0) {
            cout << "DISPLAY" << endl;
            indexManager->printKeyList();
            productManager->traverseFile();
        } else {
            cout << "INVALID COMMAND. Try: DISPLAY" << endl;
        }
    }
    else if (numberOfArguments == 3) {
        string flag = arguments[2];
        string keyString = arguments[3];
        
        //DELETE
        if (command.compare("DELETE") == 0) {
            if (flag.compare("PRIMARY") == 0) {
                //Delete Primary
                cout << "DELETE PRIMARY" << endl;
                handleDeletePrimary(convertStringToInt(keyString));
            } else if (flag.compare("SECONDARY") == 0) {
                //Delete Secondary
                cout << "DELETE SECONDARY" << endl;
                handleDeleteSecondary(convertStringToInt(keyString));
            } else {
                //Invalid command
                cout << "INVALID COMMAND. Try: DELETE PRIMARY KEY or DELETE SECONDARY CODE" << endl;
            }
        }//end if delete
        
        //SEARCH
        else if(command.compare("SEARCH") == 0) {
            if (flag.compare("PRIMARY") == 0) {
                //SEARCH Primary
                cout << "SEARCH PRIMARY" << endl;
                handleSearchForPrimaryKey(convertStringToInt(keyString));
            } else if (flag.compare("SECONDARY") == 0) {
                //SEARCH Secondary
                cout << "SEARCH SECONDARY" << endl;
                handleSearchForSecondaryKey(convertStringToInt(keyString));
            } else if (flag.compare("DATA") == 0) {
                //SEARCH Data File
                cout << "SEARCH DATA FILE" << endl;
                handleSearchForDataValue(keyString);
            } else {
                cout << "INVALID COMMAND. Try: SEARCH PRIMARY KEY or SEARCH SECONDARY CODE or SEARCH DATA VALUE" << endl;
            }
        }
        
        else {
            cout << "INVALID COMMAND." << endl;
        }
    }
    else if (numberOfArguments == 4) {
        //MOD KEY DATA_LABEL NEW_DATA
        if (command.compare("MOD") == 0) {
            cout << "MOD" << endl;
            string keyString = arguments[2];
            string dataLabel = arguments[3];
            string newData = arguments[4];
            
            handleModify(convertStringToInt(keyString), dataLabel, newData);
        } else {
            cout << "INVALID COMMAND. Try: MOD KEY DATA_LABEL NEW_DATA" << endl;
        }
    }
    else if (numberOfArguments == 5) {
        //ADD KEY NAME CODE COST
        if (command.compare("ADD") == 0) {
            cout << "ADD" << endl;
            string record;
            record.append(arguments[2]);
            record.append(" ");
            record.append(arguments[3]);
            record.append(" ");
            record.append(arguments[4]);
            record.append(" ");
            record.append(arguments[5]);
            
            handleAdd(record);
        } else {
            cout << "INVALID COMMAND. Try: ADD KEY NAME CODE COST" << endl;
        }
    }
    else {
        cout << "Invalid number of arguments." << endl;
        return -1;
    }
    
    return 0;
}

void handleSearchForPrimaryKey(int key) {
    //Offset is -1 if record not found in key list.
    int offset = indexManager->getRecordOffsetForKey(key);
    if (offset == -1) {
        cout << "RECORD NOT FOUND IN SEARCH." << endl;
    } else {
        DataRecord recordRead = productManager->getDataRecordAtOffset(offset);
        cout << "FOUND: ";
        recordRead.printRecord();
    }
}

void handleSearchForSecondaryKey(int key) {
    vector<int> primaryKeys = indexManager->getPrimaryKeysWithCode(key);
    
    if (primaryKeys.size() == 0) {
        cout << "SECONDARY KEY NOT FOUND IN SEARCH." << endl;
        return;
    }
    
    int RRN = -1;
    DataRecord record;
    cout << "RECORDS WITH CODE: " << key << endl;
    for (int i = 0; i < primaryKeys.size(); i++) {
        RRN = indexManager->getRecordOffsetForKey(primaryKeys.at(i));
        record = productManager->getDataRecordAtOffset(RRN);
        record.printRecord();
    }
}

void handleSearchForDataValue(string data) {
    //Description OR Cost
    bool isDouble = checkDouble(data);
    
    if (isDouble) {
        //Handle search of double value cost
        double cost = convertStringToDouble(data);
        DataRecord foundRecord = productManager->searchFileForCost(cost);
        if (foundRecord.getKey() != -1) {
            cout << "FOUND RECORD WITH COST: " << cost << endl;
            foundRecord.printRecord();
        } else {
            cout << "COULD NOT FIND RECORD WITH COST: " << cost << endl;
        }
    } else {
        //Handle search for description
        DataRecord foundRecord = productManager->searchFileForDescription(data);
        if (foundRecord.getKey() != -1) {
            cout << "FOUND RECORD WITH NAME: " << data << endl;
            foundRecord.printRecord();
        } else {
            cout << "COULD NOT FIND RECORD WITH NAME: " << data << endl;
        }
     
    }
}

void handleAdd(string record) {
    DataRecord newRecord = productManager->getDataRecordForString(record);
    int RRN = productManager->addDataRecordToBinaryFile(newRecord);
    indexManager->addDataRecordToKeyList(newRecord, RRN);
    indexManager->addDataRecordToInvertedList(newRecord);
    indexManager->savePrimaryKeyListToFile();
    indexManager->saveSecondaryKeyListToFile();
}

void handleDeletePrimary(int key) {
    int offset = indexManager->getRecordOffsetForKey(key);
    if (offset == -1) {
        cout << "RECORD NOT FOUND TO DELETE." << endl;
    } else {
        DataRecord deleteRecord = productManager->getDataRecordAtOffset(offset);
        
        //Delete Secondary Index Entry for Primary Key deletion
        int code = deleteRecord.getCode();
        int primaryKey = deleteRecord.getKey();
        cout << "DELETE RECORD WITH CODE AND PRIMARY KEY: " << code << " " << primaryKey << endl;
        indexManager->removePrimaryIndexFromSecondaryKeyWithCode(code, primaryKey);
        
        //Delete Primary Index Entry for Primary Key deletion
        indexManager->deleteIndexRecordWithKey(key);
        
        //Delete Record Entry for Primary Key deletion
        productManager->deleteRecordAtOffset(offset);
        
        indexManager->savePrimaryKeyListToFile();
        indexManager->saveSecondaryKeyListToFile();
    }
}

void handleDeleteSecondary(int code) {
    int found = indexManager->deleteSecondaryIndexWithKey(code);
    
    if (found != -1) {
        indexManager->saveSecondaryKeyListToFile();
        cout << "DELETED SECONDARY INDEX ENTRY FOR: " << code << endl;
    } else {
        cout << "COULD NOT FIND SECONDARY INDEX FOR CODE: " << code << endl;
    }
    
}

void handleModify(int key, string dataLabel, string newData) {
    int dataLabelFlag = getFlagForDataLabel(dataLabel);
    
    int modifiedRecordOffset = indexManager->getRecordOffsetForKey(key);
    
    DataRecord modifiedRecord;
    if (modifiedRecordOffset == -1) {
        cout << "RECORD NOT FOUND TO MODIFY." << endl;
    } else {
        modifiedRecord = productManager->getDataRecordAtOffset(modifiedRecordOffset);
    }
    
    if (dataLabelFlag == -1) {
        cout << "INVALID DATA LABEL SPECIFIED." << endl;
    }
    
    if (dataLabelFlag != -1 && modifiedRecordOffset != -1) {
        switch (dataLabelFlag) {
            case 1:
                //KEY modification
                indexManager->removePrimaryIndexFromSecondaryKeyWithCode(modifiedRecord.getCode(), modifiedRecord.getKey());

                modifiedRecord.setKey(convertStringToInt(newData));
                indexManager->deleteIndexRecordWithKey(key);
                indexManager->addDataRecordToKeyList(modifiedRecord, modifiedRecordOffset);
                
                indexManager->addDataRecordToInvertedList(modifiedRecord);
                
                indexManager->savePrimaryKeyListToFile();
                indexManager->saveSecondaryKeyListToFile();
                break;
            case 2:
                //DESCRIPTION modification
                modifiedRecord.setName(newData.c_str());
                break;
            case 3:
                //CODE modification
                //Affects secondary index list
                indexManager->removePrimaryIndexFromSecondaryKeyWithCode(modifiedRecord.getCode(), modifiedRecord.getKey());
                modifiedRecord.setCode(convertStringToInt(newData));
                indexManager->addDataRecordToInvertedList(modifiedRecord);
                indexManager->saveSecondaryKeyListToFile();
                break;
            case 4:
                //COST modification
                modifiedRecord.setCost(convertStringToDouble(newData));
                break;
            default:
                break;
        }//end switch for data type
        
        productManager->writeDataRecordToBinaryFile(modifiedRecord, modifiedRecordOffset);
    }//end else
}//end handle modify

int getFlagForDataLabel(string dataLabel) {
    int dataFlag = -1;
    
    if (dataLabel.compare("KEY") == 0) {
        dataFlag = 1;
    } else if (dataLabel.compare("DESCRIPTION") == 0) {
        dataFlag = 2;
    } else if (dataLabel.compare("CODE") == 0) {
        dataFlag = 3;
    } else if (dataLabel.compare("COST") == 0) {
        dataFlag = 4;
    }
    
    return dataFlag;
}

#pragma mark - Conversion Methods For Parsing

bool checkDouble(string checkString) {
    bool isDouble = true;
    for (int i = 0; i < checkString.size() && isDouble; i++) {
        if (checkString.at(i) != '.' && isalpha(checkString.at(i)))
            isDouble = false;
    }
    
    return isDouble;
}

int convertStringToInt(std::string intString) {
    //--------------------------------------------------------------------------------//
    //Convert the string passed into an integer and return the integer value.
    //Preconditions: string representation of an integer passed as parameter.
    //Postconditions: the string passed is parsed and the integer value of the string
    //is returned to the caller.
    //--------------------------------------------------------------------------------//
    int convertedInteger;
    
    std::istringstream converter(intString);
    converter >> convertedInteger;
    
    return convertedInteger;
}

double convertStringToDouble(std::string doubleString) {
    //--------------------------------------------------------------------------------//
    //Convert the string passed into a double and return the double value.
    //Preconditions: string representation of a double passed as parameter.
    //Postconditions: the string passed is parsed and the double value of the string
    //is returned to the caller.
    //--------------------------------------------------------------------------------//
    double convertedDouble;
    
    std::istringstream converter(doubleString);
    converter >> convertedDouble;
    
    return convertedDouble;
}
