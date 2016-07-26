#include "myexporter.h"

MyExporter::MyExporter(std::string filename,
                       MyModel &model)
    : model(model)
{
    if(std::string::npos == filename.find(MYFILETYPE))
        filename += MYFILETYPE;

    file.open(filename, std::ios_base::out | std::ios_base::trunc);

    if(!file.is_open())
    {
        std::cerr << "can't open " << filename << " for export" << std::endl;
        return;
    }

    exportModel();

    file.close();
}

MyExporter::~MyExporter()
{

}

void MyExporter::exportModel()
{
    exportMesh();
    exportBones();
    exportConnections();
    exportAnimations();

    file << std::endl;
}

void MyExporter::exportMesh()
{
    std::string prefix("");

    file << MESH << " {" << std::endl;
    prefix += '\t';

        file << prefix << POSITION << " {" << std::endl;
        prefix += '\t';

            for(uint32_t i = 0; i < model.getPositions().size(); i++)
            {
                glm::vec4& pos = model.getPositions()[i];

                file << prefix
                     << pos.x << ","
                     << pos.y << ","
                     << pos.z << ","
                     << pos.w << ((i != model.getPositions().size()-1) ? "," : "")
                     << std::endl;
            }

        prefix.resize(prefix.size()-1);
        file << prefix << "}" << std::endl;

        file << prefix << MESHDATA << " {" << std::endl;
        prefix += '\t';

            uint32_t size = model.getIndices().size();
            for(uint32_t k = 0; k < size; k++)
            {
                glm::vec3& n = model.getNormals()[k];
                glm::vec2& u = model.getUVs()[k];
                uint32_t& i = model.getIndices()[k];

                file << prefix
                     << n.x << "," << n.y << "," << n.z << ","
                     << u.x << "," << u.y << ","
                     << i << ((k != size-1) ? "," : "")
                     << std::endl;
            }

        prefix.resize(prefix.size()-1);
        file << prefix << "}" << std::endl;

    file << "}" << std::endl;
}

void MyExporter::exportBones()
{
    std::string prefix("");

    file << BONES << " {" << std::endl;
    prefix += '\t';

        for(MyNode& bone : model.getBones())
        {
            file << prefix << bone.getName() << " {" << std::endl;
            prefix += '\t';

                // print bindPose matrix
                file << prefix << BINDPOSEMATRIX << " {" << std::endl;
                prefix += '\t';

                    float* m = &bone.getBindPose()[0][0];
                    file << prefix;
                    for(uint32_t i = 0; i < 16; i++)
                    {
                        file << m[i] << ((i != 15) ? "," : "\n");
                    }

                prefix.resize(prefix.size()-1);
                file << prefix << "}" << std::endl;

                // print bone weight with index
                file << prefix << BONEWEIGHT << " {" << std::endl;
                prefix += '\t';

                    for(uint32_t i = 0; i < bone.getBoneDepCount(); i++)
                    {
                        file << prefix
                             << bone.getBoneDep(i).vertexIndex << ","
                             << bone.getBoneDep(i).boneWeight
                             << ((i != bone.getBoneDepCount()-1) ? "," : "")
                             << std::endl;
                    }

                prefix.resize(prefix.size()-1);
                file << prefix << "}" << std::endl;

            prefix.resize(prefix.size()-1);
            file << prefix << "}" << std::endl;
        }

    file << "}" << std::endl;
}

void MyExporter::exportConnections()
{
    std::string prefix("");

    file << BONECONNECTIONS << " {" << std::endl;
    prefix += '\t';

        struct NodeRelation
        {
            NodeRelation(std::string p,
                         std::string c)
                : parent(p)
                , child(c){}

            std::string parent;
            std::string child;
        };

        std::vector<NodeRelation> relations;

        for(MyNode& node : model.getBones())
        {
            for(MyNode* child : node.getChildren())
            {
                relations.emplace_back(node.getName(), child->getName());
            }
        }

        uint32_t count = relations.size();
        for(uint32_t i = 0; i < count; i++)
        {
            file << prefix << relations[i].parent << " : "
                 << relations[i].child << ((i != count-1) ? "," : "")
                 << std::endl;
        }

    file << "}" << std::endl;
}

void MyExporter::exportAnimations()
{
    std::string prefix("");

    file << ANIMATIONS << " {" << std::endl;
    prefix += '\t';

        for(MyAnimation* animation : model.getAnimations())
        {
            file << prefix << animation->getName() << " {" << std::endl;
            prefix += '\t';

                file << prefix << ANIMATIONDURATION << " {" << std::endl;
                prefix += '\t';

                    file << prefix << animation->getDuration() << std::endl;

                prefix.resize(prefix.size()-1);
                file << prefix << "}" << std::endl;

                file << prefix << NODEANIMATION << " {" << std::endl;
                prefix += '\t';

                    for(MyNodeAnimation* nodeAnim : animation->nodeAnimations)
                    {
                        if(nodeAnim->transformations.size())
                        {
                            file << prefix << nodeAnim->node->getName() << " {" << std::endl;
                            prefix += '\t';

                                uint32_t count = nodeAnim->transformations.size();
                                for(uint32_t i = 0; i < count; i++)
                                {
                                    Transformation& trans = nodeAnim->transformations[i];

                                    file << prefix
                                         << trans.scale.x << "," << trans.scale.y << "," << trans.scale.z << ","
                                         << trans.rotate.x << "," << trans.rotate.y << "," << trans.rotate.z << ","
                                         << trans.translate.x << "," << trans.translate.y << "," << trans.translate.z << ","
                                         << trans.deadLine << ((i != count-1) ? "," : "") << std::endl;
                                }

                            prefix.resize(prefix.size()-1);
                            file << prefix << "}" << std::endl;
                        }
                    }

                prefix.resize(prefix.size()-1);
                file << prefix << "}" << std::endl;

            prefix.resize(prefix.size()-1);
            file << prefix << "}" << std::endl;
        }

    file << "}" << std::endl;
}
