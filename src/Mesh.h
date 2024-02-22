
#include <fstream>
#include "linalg.h"

struct Vertex {
    vec3 position;
    vec3 normal;
    vec3 texture;
};

class Mesh {

public:
    Mesh(int count, Vertex vertices[]) : position(0.0, 0.0, 0.0), rotation(0.0, vec3(1.0, 0.0, 0.0)), extent(1.0, 1.0, 1.0) {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        GLuint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), 0);
    }
    
    static Mesh fromWavefront(const char* file) {

        std::ifstream fs;
        fs.open(file);

        if(!fs)
            throw;


        std::vector<vec3> vertices;
        std::vector<vec3> normals;
        std::vector<vec3> textures;

        std::vector<Vertex> faces;

        while(fs.good()) {
            std::string token;
            fs >> token;

            if(token == "Sphere")
                break;

            // read vertex
            if(token == "v") {
                
                vec3 v(0, 0, 0);

                fs >> v.x;
                fs >> v.y;
                fs >> v.z;

                vertices.push_back(v);

            }

            // read normal
            else if(token == "vn") {

                vec3 n(0, 0, 0);

                fs >> n.x;
                fs >> n.y;
                fs >> n.z;

                normals.push_back(n);

            }

            // read texture co-ordinate
            else if(token == "vt") {

                vec3 t(0, 0, 0);

                fs >> t.x;
                fs >> t.y;

                textures.push_back(t);
                
            }

            // read face
            else if(token == "f") {

                // TODO handle more complex face syntax
                // also requires parsing quads into triangles
                for(int i = 0; i < 3; ++i) {
                    Vertex v;
                    int vidx;
                    fs >> vidx;
                    v.position = vertices[vidx-1];
                    faces.push_back(v);
                }
                
            }
        }

        std::cout << "Loaded " << vertices.size() << " vertices and " << faces.size()/3 << " faces." << std::endl;
        return Mesh(faces.size(), &faces[0]);
    }

    void setExtent(vec3 extent) { this->extent = extent; }

    void rot(double dtheta, vec3 axis) { rotation = rotation * quaternion(dtheta, axis); }
    void scl(vec3 factor) { extent = factor % extent; }

    mat4 transform() {
        return translate(position) * rotation.toMatrix() * scale(extent.x, extent.y, extent.z);
    }

    void render() {

        glBindVertexArray( VAO );
        glDrawArrays( GL_TRIANGLES, 0, 36 );
        glBindVertexArray( 0 );

    }

private:

    GLuint VAO;

    vec3 position;
    quaternion rotation;
    vec3 extent; // wanted to call scale, but can't because of the linalg namespace

};