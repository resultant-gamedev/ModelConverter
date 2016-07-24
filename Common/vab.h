#ifndef VAB_H
#define VAB_H

#include "extern/glm/glm.hpp"
#include "extern/glm/ext.hpp"

#include <string>
#include <vector>

using namespace std;
using namespace glm;

enum class textAlignment{left, right, top, bottom, center};

struct TexturedVertex
{
    TexturedVertex(){}
    TexturedVertex(glm::vec4 pos, glm::vec2 tcs)
        : position(pos), texcoord(tcs){}

    glm::vec4 position;
    glm::vec2 texcoord;
};

struct NormalizedTexturedVertex
{
    NormalizedTexturedVertex(){}
    NormalizedTexturedVertex(glm::vec4 pos, glm::vec2 tcs, glm::vec3 norm)
        : position(pos), texcoord(tcs), normal(norm){}

    glm::vec4 position;
    glm::vec2 texcoord;
    glm::vec3 normal;
};



#endif // VAB_H
