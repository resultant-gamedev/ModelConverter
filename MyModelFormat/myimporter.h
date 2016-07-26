#ifndef MYIMPORTER_H
#define MYIMPORTER_H

#include "mymodel.h"
#include "MyInternalFormat/mystack.h"

#include <fstream>

namespace MyModelFormat
{

class MyImporter
{
public:
    MyImporter(std::string filename,
               MyModelFormat::MyModel& model);
    ~MyImporter();

private:
    void importModel();

    void importMesh(MyModelFormat::MyStack* stack);
    void importBones(MyModelFormat::MyStack* stack);
    void importConnections(MyModelFormat::MyStack* stack);
    void importAnimations(MyModelFormat::MyStack* stack);

    void loadStacks();

    std::ifstream file;
    MyModelFormat::MyModel& model;

    std::vector<MyModelFormat::MyStack*> stacks;
};

}

#endif // MYIMPORTER_H
