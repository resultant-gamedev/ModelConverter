#ifndef MYPARSER_H
#define MYPARSER_H

#include <fbxsdk.h>

#include <string>
#include <vector>

#include "extern/glm/glm.hpp"
#include "extern/glm/ext.hpp"

#include "Common/vab.h"

#include "MyModelFormat/mymodel.h"
#include "MyModelFormat/myexporter.h"
#include "MyModelFormat/myimporter.h"

#define MAX_BONES_PER_VERTEX 4

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
    void createLinks();
    void calcMaxBonesPerVertex();

    bool checkFileType(const std::string& filename);

    FbxManager* pManager;
    FbxScene* scene;

    MyModelFormat::MyModel model;
};

#endif // MYPARSER_H
