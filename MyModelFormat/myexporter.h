#ifndef MYEXPORTER_H
#define MYEXPORTER_H

#include "mymodel.h"
#include "MyInternalFormat/myfiletype.h"
#include "MyInternalFormat/myidentifier.h"

#include <fstream>
#include <iostream>

class MyExporter
{
public:
    MyExporter(std::string filename, MyModel& model);
    ~MyExporter();

private:
    void exportModel();

    void exportMesh();
    void exportBones();
    void exportConnections();
    void exportAnimations();
    void exportNodeAnims();

    std::ofstream file;
    MyModel& model;
};

#endif // MYEXPORTER_H
