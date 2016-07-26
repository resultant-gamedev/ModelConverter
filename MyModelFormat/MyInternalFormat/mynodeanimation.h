#ifndef MYNODEANIMATION_H
#define MYNODEANIMATION_H

#include <vector>
#include <string>
#include <cassert>

#include "extern/glm/glm.hpp"

struct Transformation
{
    Transformation(glm::vec3 s,
                   glm::vec3 t,
                   glm::vec3 r,
                   float d)
        : scale(s)
        , translate(t)
        , rotate(r)
        , deadLine(d){}

    glm::vec3 scale;
    glm::vec3 translate;
    glm::vec3 rotate;
    float deadLine;
};

class MyAnimation;
class MyNode;
class MyExporter;

/**
 * @brief The MyNodeAnimation class
 * Saves all transformations of one node from one animation
 */
class MyNodeAnimation
{
    friend class MyAnimation;
    friend class MyNode;
    friend class MyExporter;

public:
    ~MyNodeAnimation();

    void addTransformation(glm::vec3 scale,
                           glm::vec3 translate,
                           glm::vec3 rotate,
                           float deadline);

private:
    MyNodeAnimation(MyNode* node);

    std::vector<Transformation> transformations;
    MyNode* node;
};

#endif // MYNODEANIMATION_H
