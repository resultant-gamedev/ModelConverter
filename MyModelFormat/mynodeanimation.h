#ifndef MYNODEANIMATION_H
#define MYNODEANIMATION_H

#include <vector>
#include <string>
#include <cassert>

#include "extern/glm/glm.hpp"

struct Transformation
{
    Transformation(glm::vec3 v, float d)
        : vector(v)
        , deadLine(d){}

    glm::vec3 vector;
    float deadLine;
};

class MyAnimation;
class MyNode;

/**
 * @brief The MyNodeAnimation class
 * Saves all transformations of one node from one animation
 */
class MyNodeAnimation
{
public:
    friend class MyAnimation;
    friend class MyNode;

    ~MyNodeAnimation();

    void addTranslations(glm::vec3 vector, float deadline);
    void addRotation(glm::vec3 vector, float deadline);
    void addScaling(glm::vec3 vector, float deadline);

private:
    MyNodeAnimation();

    void connectNode(MyNode* node);

    std::vector<Transformation> scalings;
    std::vector<Transformation> rotations;
    std::vector<Transformation> translations;

    MyNode* node;
};

#endif // MYNODEANIMATION_H
