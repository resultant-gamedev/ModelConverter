#include "myparser.h"

#include <iostream>

int main(int argc, char *argv[])
{
    MyParser imp;

    if(argc != 3)
    {
        imp.importFromFile("data/TestBin.fbx");
    }
    else
    {
        imp.importFromFile(argv[1]);
        imp.exportToFile(argv[2]);
    }

    return 0;
}
