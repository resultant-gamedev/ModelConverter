#ifndef MYIMPORTER_H
#define MYIMPORTER_H

#include "mymodel.h"
#include "MyInternalFormat/mystack.h"

#include <fstream>

#include <iostream>

class MyImporter
{
public:
    MyImporter(std::string filename, MyModel& model);
    ~MyImporter();

private:
    void importModel();

    void importMesh(MyStack* stack);
    void importBones(MyStack* stack);
    void importConnections(MyStack* stack);
    void importAnimations(MyStack* stack);

    void loadStacks();

    std::ifstream file;
    MyModel& model;

    std::vector<MyStack*> stacks;
};

#endif // MYIMPORTER_H
