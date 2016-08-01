#ifndef MYEXPORTER_H
#define MYEXPORTER_H

#include "mymodel.h"
#include "MyInternalFormat/myfiletype.h"
#include "MyInternalFormat/myidentifier.h"

#include <fstream>

namespace MyModelFormat
{

class MyExporter
{
public:
    MyExporter(std::string filename, MyModelFormat::MyModel& model);
    ~MyExporter();

private:
    void exportModel();

    void exportMesh();
    void exportBones();
    void exportConnections();
    void exportAnimations();
    void exportNodeAnims();

    void deleteSpaceInNames();

    std::ofstream file;
    MyModelFormat::MyModel& model;
};

}

#endif // MYEXPORTER_H
