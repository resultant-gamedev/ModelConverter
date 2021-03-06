#include "myparser.h"

#include <iostream>

int main(int argc, char *argv[])
{
    MyParser imp;

    if(argc != 3)
    {
        imp.importFromFile("data/WholeBody.fbx");
        imp.exportToFile("data/body");
    }
    else
    {
        imp.importFromFile(argv[1]);
        imp.exportToFile(argv[2]);
    }

    std::cout << "successfully converted" << std::endl;

    return 0;
}
