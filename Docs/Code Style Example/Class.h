/*
 * A description of the class might be written here if required
 */

#ifndef CLASS_H
#define CLASS_H

#define VAR 100     // Defines are written like this

#include <lib.h>    // External files and libs are included like this

#include "myfiles.h"// Files that we have written are included like this

 
class Class : public Parent { // Name and the class it "extends"

public:

    // Constructors and destructors
    Class() {};
    Class(int, float, std::string);
    ~Class();

    // Memberfunctions
    void publicFunction(int, float);

    // Getters, one line in the header file to keep crap from the .cpp file
    int getNumber() { return mNum; };

    // Setters, one line in the header file to keep crap from the .cpp file
    void setNumber(int n) { mNum = n; };

protected:

    // Variables and functions that only inherrited classes can access

private:

    // Variables and functions that only this class can access directly
    // Instance variables should be named with an "m" before its name.
    int mNum;
    float mFloat;
    std::string mString;

    std::vector<float> mFloats;
};

#endif // CLASS_H