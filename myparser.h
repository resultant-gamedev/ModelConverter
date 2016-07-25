#ifndef MYPARSER_H
#define MYPARSER_H

#include <fbxsdk.h>

#include <string>
#include <vector>
#include <iostream>

#include "extern/glm/glm.hpp"
#include "extern/glm/ext.hpp"

#include "Common/vab.h"

#include "MyModelFormat/mymodel.h"

inline void coutVec3(const glm::vec3& v)
{
    std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
}

class MyParser
{
public:
    MyParser();
    ~MyParser();

    void importFromFile(const std::string& filename);
    void exportToFile(const std::string& filename);

private:
    void loadNode(FbxNode* node);
    void loadMesh(FbxMesh* pMesh);
    void loadNodeKeyframe(FbxNode* node);
    void loadSkelett(FbxMesh* mesh);
    void createAnimations();

    void exportMesh();
    void exportAnimation();

    MyNode* findNode(std::string& name);

    FbxManager* pManager;
    FbxScene* scene;

    MyModel model;
};

#endif // MYPARSER_H
