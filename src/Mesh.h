
#include <fstream>
#include "linalg.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
    vec3 position;
    vec3 normal;
    vec3 texture;
};

class Mesh {

public:
    Mesh(int count, Vertex vertices[]) : position(0.0, 0.0, 0.0), rotation(0.0, vec3(1.0, 0.0, 0.0)), extent(1.0, 1.0, 1.0) {
        GLuint VBO;

        vertexCount = count;

        // generate buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        // bind buffers
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // store vertices on gpu
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);
        
        // store information about position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), 0);

        // store information about normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (const void*) sizeof(vec3));

        glBindVertexArray(0);
    }
    
    static Mesh fromFile(const char* file) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate);

        aiMesh *mesh = scene->mMeshes[0]; // ignore subsequent objects in scene
        std::vector<Vertex> vertices;

        for(int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            if(face.mNumIndices != 3)
                continue;

            for(int j = 0; j < 3; ++j) {
                Vertex v;

                int idx = face.mIndices[j];
                aiVector3D position = mesh->mVertices[idx]; 
                aiVector3D normal = mesh->mNormals[idx]; 

                v.position = vec3(position.x, position.y, position.z);
                v.normal = vec3(normal.x, normal.y, normal.z);

                vertices.push_back(v);
            }
        }

        std::cout << "faces " << mesh->mNumFaces << "\n";
        std::cout << "verts " << vertices.size() << std::endl;
        return Mesh(vertices.size(), &vertices[0]);
    }

    void setExtent(vec3 extent) { this->extent = extent; }

    void rot(double dtheta, vec3 axis) { rotation = rotation * quaternion(dtheta, axis); }
    void scl(vec3 factor) { extent = factor % extent; }

    mat4 transform() {
        return translate(position) * rotation.toMatrix() * scale(extent.x, extent.y, extent.z);
    }

    void render() {

        glBindVertexArray( VAO );
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        glBindVertexArray( 0 );

    }

private:

    GLuint VAO;
    int vertexCount;

    vec3 position;
    quaternion rotation;
    vec3 extent; // wanted to call scale, but can't because of the linalg namespace

};