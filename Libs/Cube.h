#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Shaders/Shader.h"
#include "./Camera.h"
#include <filesystem>
#include <iostream>
#include <vector>

using namespace std;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 9.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

string getPath(string filename);


class Cube{
    public:
    glm::vec4 worldspace_center;
    glm::vec3* center;
    // unsigned int VBO, VAO; // IDs para los buffers de vértices
    unsigned int textures[6]; // IDs para las texturas de cada lado
    glm::mat4 projection;
    Shader* ourShader;
    glm::mat4 model;

    Cube(float x,float y,float z){
        //center
        center = new glm::vec3(x,y,z);
        ourShader = new Shader(getPath("Shaders/shader.vs").c_str(), getPath("Shaders/shader.fs").c_str());
        model = glm::mat4(1.0f); //matriz identidad
        // model = glm::scale(model, glm::vec3(2.0f,2.0f,2.0f)); //matriz de traslacion 
        model = glm::translate(model, *center); //matriz de traslacion 
        worldspace_center = model * glm::vec4(center->x,center->y,center->z,1);
        // center = new glm::vec3(worldspace_center.x,worldspace_center.y,worldspace_center.z);
    }

    void configShader_Textures(unsigned int* textures_){
        std::memcpy(textures, textures_, sizeof(textures));
        setProjection();
    }

    void draw(){
    
        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader->setMat4("view", view);

        // calculate the model matrix for each object and pass it to shader before drawing
        // glm::mat4 model = glm::mat4(1.0f);

       
        ourShader->setMat4("model", model);
        // Imprimir los elementos de la matriz model
        // cout<<"\nModel:\n";
        // cout<< glm::to_string(model[0])<<endl;
        // cout<< glm::to_string(model[1])<<endl;
        // cout<< glm::to_string(model[2])<<endl;
        // cout<< glm::to_string(model[3])<<endl;
        // for (int i = 0; i < 4; ++i) {
        //     for (int j = 0; j < 4; ++j) {
        //         std::cout << model[i][j] << " ";
        //     }
        //     std::cout << std::endl;
        // }
        // //apply our model matrix to the center
 
        // cout<<"\nCEnter:\n"<<center->x<<" , "<<center->y<<" , "<<center->z;

        // bind textures on corresponding texture units
         for (int i = 0; i < 6; i++) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures[i]);
            ourShader->setInt("texture1", i);
            glDrawArrays(GL_TRIANGLES, i * 6, 6);
        }
    }

    void setProjection(){
        // activate shader
        ourShader->use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader->setMat4("projection", projection);

        // glBindVertexArray(VAO);
   
    }

   

    void rotateHorizontal(float angle, const glm::vec3& center_) {
        // model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));

        // Trasladar al centro de rotación
        model = glm::translate(model, -center_);

        // Realizar la rotación horizontal en funcion al centro dado 
        glm::mat4 Rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
        model = Rotation * model;

        // Regresar a la posición original
        model = glm::translate(model, center_);
    }

    void rotateVertical(float angle, const glm::vec3& center_) {
        // model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

        // Trasladar al centro de rotación
        model = glm::translate(model, -center_);

        // Realizar la rotación horizontal en funcion al centro dado 
        glm::mat4 Rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        model = Rotation * model;

        // Regresar a la posición original
        model = glm::translate(model, center_);
    }


};



string getPath(string filename){

    std::filesystem::path filePath = std::filesystem::current_path() / filename;
    std::string fullPath = filePath.string();

    std::string undesiredPart = "\\build\\mak.mingw.x64";
    size_t pos = fullPath.find(undesiredPart);
    if (pos != std::string::npos) {
        fullPath.replace(pos, undesiredPart.length(), "");
    }


    std::string escapedPath;
    for (char c : fullPath) {
        if (c == '\\') {
            escapedPath += "\\\\";
        } else {
            escapedPath += c;
        }
    }

    // std::cout << "El path es: " << escapedPath << std::endl;
    return escapedPath;
}
