#include "myimporter.h"
#include "MyInternalFormat/myidentifier.h"
#include "extern/glm/ext.hpp"

#include <sstream>
#include <algorithm>
#include <functional>

MyImporter::MyImporter(std::string filename,
                       MyModel& model)
    : file(filename)
    , model(model)
{
    loadStacks();

    file.close();

    importModel();
}

MyImporter::~MyImporter()
{
    for(MyStack* stack : stacks)
    {
        delete stack;
    }
}

void MyImporter::importModel()
{
    auto findCorrectStack = [&](std::string name, std::function<void(MyStack*)> getData)
    {
        for(MyStack* stack : stacks)
        {
            if(name == stack->getName())
            {
                getData(stack);
                return;
            }
        }
    };

    using std::placeholders::_1;

    findCorrectStack(MESH, std::bind(&MyImporter::importMesh, this, _1));
    findCorrectStack(BONES, std::bind(&MyImporter::importBones, this, _1));
    findCorrectStack(BONECONNECTIONS, std::bind(&MyImporter::importConnections, this, _1));
    findCorrectStack(ANIMATIONS, std::bind(&MyImporter::importAnimations, this, _1));
}

void MyImporter::importMesh(MyStack *stack)
{
    for(MyStack* child : stack->getChildren())
    {
        if(MESHDATA == child->getName())
        {
            glm::vec3 n;
            glm::vec2 u;
            uint32_t i;
            char d;

            std::istringstream sstream(child->getContent());

            do
            {
                sstream >> n.x >> d >> n.y >> d >> n.z >> d
                        >> u.x >> d >> u.y >> d
                        >> i;

                model.getNormals().emplace_back(n);
                model.getUVs().emplace_back(u);
                model.getIndices().emplace_back(i);

            } while(sstream >> d);
        }
        else if(POSITION == child->getName())
        {
            glm::vec4 pos;
            char dummy;

            std::istringstream sstream(child->getContent());

            do
            {
                sstream >> pos.x >> dummy
                        >> pos.y >> dummy
                        >> pos.z >> dummy
                        >> pos.w;

                model.getPositions().emplace_back(pos);

            } while(sstream >> dummy);
        }
    }
}

void MyImporter::importBones(MyStack* stack)
{
    for(MyStack* boneStack : stack->getChildren())
    {
        model.getBones().emplace_back(boneStack->getName());
        MyNode& currentBone = model.getBones().back();

        for(MyStack* boneInfoStack : boneStack->getChildren())
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

                currentBone.setBindPose(glm::make_mat4(bindPose));
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

                    currentBone.addBoneDep(i, w);

                } while(sstream >> dummy);
            }
        }
    }
}

void MyImporter::importConnections(MyStack* stack)
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

        MyNode* parent = model.findBone(name1);
        MyNode* child = model.findBone(name2);

        assert(parent);
        assert(child);

        parent->addChild(child);
        child->setParent(parent);
    }
}

void MyImporter::importAnimations(MyStack* stack)
{
    for(MyStack* aniStack : stack->getChildren())
    {
        float duration;

        for(MyStack* durationChild : aniStack->getChildren())
        {
            if(durationChild->getName() == ANIMATIONDURATION)
            {
                std::istringstream sstream(durationChild->getContent());

                sstream >> duration;
            }
        }

        model.getAnimations().emplace_back(new MyAnimation(aniStack->getName(), duration));
        MyAnimation* animation = model.getAnimations().back();

        for(MyStack* nodeAnimStack : aniStack->getChildren())
        {
            if(nodeAnimStack->getName() == NODEANIMATION)
            {
                for(MyStack* nodeAnimChild : nodeAnimStack->getChildren())
                {
                    MyNode* bone = model.findBone(nodeAnimChild->getName());
                    assert(bone);

                    animation->addNodeAnimation(bone);
                    MyNodeAnimation* nodeAnim = animation->getLastNodeAnim();

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

void MyImporter::loadStacks()
{
    MyStack* currentStack = NULL;
    std::string line;

    while(std::getline(file, line))
    {
        line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());

        if(line[line.size()-1] == '{')
        {
            std::string name = line.substr(0, line.size()-2);

            if(!currentStack)
            {
                stacks.emplace_back(new MyStack(name));
                currentStack = stacks.back();
            }
            else
            {
                MyStack* tmpStack = new MyStack(name, currentStack);
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
            else
                std::cout << line << std::endl;
        }
    }
}
