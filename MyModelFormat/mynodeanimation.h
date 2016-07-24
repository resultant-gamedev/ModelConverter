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

    void addTransformation(glm::vec3 scale,
                           glm::vec3 translate,
                           glm::vec3 rotate,
                           float deadline);

private:
    MyNodeAnimation();
    void connectNode(MyNode* node);

    std::vector<Transformation> transformations;
    MyNode* node;
};

#endif // MYNODEANIMATION_H
