#include "myimporter.h"
#include "MyInternalFormat/myidentifier.h"
#include "extern/glm/ext.hpp"

#include <sstream>
#include <algorithm>
#include <functional>

MyModelFormat::MyImporter::MyImporter(std::string filename,
                                      MyModelFormat::MyModel& model)
    : file(filename)
    , model(model)
{
    loadStacks();

    file.close();

    importModel();
}

MyModelFormat::MyImporter::~MyImporter()
{
    for(MyModelFormat::MyStack* stack : stacks)
    {
        delete stack;
    }
}

void MyModelFormat::MyImporter::importModel()
{
    auto findCorrectStack = [&](std::string name, std::function<void(MyModelFormat::MyStack*)> getData)
    {
        for(MyModelFormat::MyStack* stack : stacks)
        {
            if(name == stack->getName())
            {
                getData(stack);
                return;
            }
        }
    };

    using std::placeholders::_1;

    findCorrectStack(MESH, std::bind(&MyModelFormat::MyImporter::importMesh, this, _1));
    findCorrectStack(BONES, std::bind(&MyModelFormat::MyImporter::importBones, this, _1));
    findCorrectStack(BONECONNECTIONS, std::bind(&MyModelFormat::MyImporter::importConnections, this, _1));
    findCorrectStack(ANIMATIONS, std::bind(&MyModelFormat::MyImporter::importAnimations, this, _1));
}

void MyModelFormat::MyImporter::importMesh(MyModelFormat::MyStack *stack)
{
    for(MyModelFormat::MyStack* child : stack->getChildren())
    {
        if(MESHDATA == child->getName())
        {
            uint32_t i;
            char d;

            std::istringstream sstream(child->getContent());

            do
            {
                sstream >> i;

                model.getIndices().emplace_back(i);

            } while(sstream >> d);
        }
        else if(POSITION == child->getName())
        {
            glm::vec3 n;
            glm::vec2 u;
            glm::vec4 pos;
            char d;

            std::istringstream sstream(child->getContent());

            do
            {
                sstream >> pos.x >> d >> pos.y >> d >> pos.z >> d >> pos.w >> d
                        >> n.x >> d >> n.y >> d >> n.z >> d
                        >> u.x >> d >> u.y;

                model.getPositions().emplace_back(pos);
                model.getNormals().emplace_back(n);
                model.getUVs().emplace_back(u);

            } while(sstream >> d);
        }
    }
}

void MyModelFormat::MyImporter::importBones(MyModelFormat::MyStack* stack)
{
    for(MyModelFormat::MyStack* boneStack : stack->getChildren())
    {
        MyModelFormat::MyNode* currentBone = new MyNode(boneStack->getName());

        for(MyModelFormat::MyStack* boneInfoStack : boneStack->getChildren())
        {
            if(BINDPOSEMATRIX == boneInfoStack->getName())
            {
                float bindPose[16];
                char dummy;
                uint32_t i = 0;

                std::istringstream sstream(boneInfoStack->getContent());

                do
                {
                    sstream >> bindPose[i];
                    i++;
                } while( (sstream >> dummy) && (i < 16));

                currentBone->setBindPose(glm::make_mat4(bindPose));
            }
            else if(BONEWEIGHT == boneInfoStack->getName())
            {
                uint32_t i;
                float w;
                char dummy;

                std::istringstream sstream(boneInfoStack->getContent());

                do
                {
                    sstream >> i >> dummy >> w;

                    currentBone->addBoneDep(i, w);

                } while(sstream >> dummy);
            }
        }

        model.getBones().emplace_back(currentBone);
    }
}

void MyModelFormat::MyImporter::importConnections(MyModelFormat::MyStack* stack)
{
    std::istringstream sstream(stack->getContent());
    std::string connection;

    while(std::getline(sstream, connection, ','))
    {
        std::string name1;
        std::string name2;
        char dummy;

        std::istringstream constream(connection);
        constream >> name1 >> dummy >> name2;

        MyModelFormat::MyNode* parent = model.findBone(name1);
        MyModelFormat::MyNode* child = model.findBone(name2);

        assert(parent);
        assert(child);

        parent->addChild(child);
        child->setParent(parent);
    }
}

void MyModelFormat::MyImporter::importAnimations(MyModelFormat::MyStack* stack)
{
    for(MyModelFormat::MyStack* aniStack : stack->getChildren())
    {
        float duration;

        for(MyModelFormat::MyStack* durationChild : aniStack->getChildren())
        {
            if(durationChild->getName() == ANIMATIONDURATION)
            {
                std::istringstream sstream(durationChild->getContent());

                sstream >> duration;
            }
        }

        model.getAnimations().emplace_back(new MyModelFormat::MyAnimation(aniStack->getName(), duration));
        MyModelFormat::MyAnimation* animation = model.getAnimations().back();

        for(MyModelFormat::MyStack* nodeAnimStack : aniStack->getChildren())
        {
            if(nodeAnimStack->getName() == NODEANIMATION)
            {
                for(MyModelFormat::MyStack* nodeAnimChild : nodeAnimStack->getChildren())
                {
                    MyModelFormat::MyNode* bone = model.findBone(nodeAnimChild->getName());
                    assert(bone);

                    animation->addNodeAnimation(bone);
                    MyModelFormat::MyNodeAnimation* nodeAnim = animation->getLastNodeAnim();

                    glm::vec3 s, t, r;
                    float d;
                    char dummy;

                    std::istringstream sstream(nodeAnimChild->getContent());

                    do
                    {
                        sstream >> s.x >> dummy >> s.y >> dummy >> s.z >> dummy
                                >> t.x >> dummy >> t.y >> dummy >> t.z >> dummy
                                >> r.x >> dummy >> r.y >> dummy >> r.z >> dummy
                                >> d;

                        nodeAnim->addTransformation(s, t, r ,d);

                    } while(sstream >> dummy);
                }
            }
        }
    }
}

void MyModelFormat::MyImporter::loadStacks()
{
    MyModelFormat::MyStack* currentStack = NULL;
    std::string line;

    while(std::getline(file, line))
    {
        line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());

        if(line[line.size()-1] == '{')
        {
            std::string name = line.substr(0, line.size()-2);

            if(!currentStack)
            {
                stacks.emplace_back(new MyModelFormat::MyStack(name));
                currentStack = stacks.back();
            }
            else
            {
                MyStack* tmpStack = new MyModelFormat::MyStack(name, currentStack);
                currentStack->addChild(tmpStack);
                currentStack = tmpStack;
            }
        }
        else if(line[line.size()-1] == '}')
        {
            currentStack = currentStack->getParent();
        }
        else
        {
            if(currentStack)
                currentStack->addContent(line);
        }
    }
}
