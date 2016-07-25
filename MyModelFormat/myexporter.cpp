#include "myexporter.h"

MyExporter::MyExporter(std::string filename,
                       MyModel &model)
    : file(filename, std::ios_base::out | std::ios_base::trunc)
    , model(model)
{
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

    file << std::endl;
}

void MyExporter::exportMesh()
{
    std::string prefix("");

    file << "Mesh {" << std::endl;
    prefix += '\t';

        file << prefix << "Positions {" << std::endl;
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

        file << prefix << "Data {" << std::endl;
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

    file << "Bones {" << std::endl;
    prefix += '\t';

        for(MyNode& bone : model.getBones())
        {
            file << prefix << bone.getName() << " {" << std::endl;
            prefix += '\t';

                // print bindPose matrix
                file << prefix << "BindPose {" << std::endl;
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
                file << prefix << "BoneWeight {" << std::endl;
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
