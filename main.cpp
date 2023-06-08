

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./Shader.h"
#include "./Camera.h"
#include <filesystem>
#include <iostream>
#include <vector>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;


///////////////////////////////////////////////

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

    std::cout << "El path es: " << escapedPath << std::endl;
    return escapedPath;
}


class Cube{
    public:

    glm::vec3* center;
    unsigned int VBO, VAO;// id buffer ->id vertices 
    unsigned int texture1, texture2;//ids para las texturas
    glm::mat4 projection;
    Shader* ourShader;
    float vertices[180]; 

    Cube(float x,float y,float z){
        //center
        center = new glm::vec3(x,y,z);
        ourShader = new Shader(getPath("shader.vs").c_str(), getPath("shader.fs").c_str());

        float vertices_[] = {
            // Cara frontal
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Vértice inferior izquierdo
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  // Vértice inferior derecho
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // Vértice superior derecho
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // Vértice superior derecho
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // Vértice superior izquierdo
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Vértice inferior izquierdo

            // Cara posterior
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Vértice inferior izquierdo
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // Vértice inferior derecho
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // Vértice superior derecho
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // Vértice superior derecho
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // Vértice superior izquierdo
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Vértice inferior izquierdo

            // Cara izquierda
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Vértice superior derecho
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Vértice superior izquierdo
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Vértice inferior izquierdo
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Vértice inferior izquierdo
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Vértice inferior derecho
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Vértice superior derecho

            // Cara derecha
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // Vértice superior izquierdo
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // Vértice superior derecho
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  // Vértice inferior derecho
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  // Vértice inferior derecho
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  // Vértice inferior izquierdo
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // Vértice superior izquierdo

            // Cara inferior
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Vértice inferior izquierdo
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  // Vértice inferior derecho
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // Vértice superior derecho
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // Vértice superior derecho
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Vértice superior izquierdo
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Vértice inferior izquierdo

            // Cara superior
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // Vértice inferior izquierdo
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // Vértice inferior derecho
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // Vértice superior derecho
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // Vértice superior derecho
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // Vértice superior izquierdo
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f  // Vértice inferior izquierdo
        };

        for (int i = 0; i < 180; i++)
        {
            vertices[i]= vertices_[i];
        }
        
    }

    

    void configShader_Textures(){
        setShaders();
        setTextures();
    }

    void draw(){
        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, texture2);

        // activate shader
        ourShader->use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader->setMat4("projection", projection);

        glBindVertexArray(VAO);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader->setMat4("view", view);
     
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, *center);
        ourShader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    

    void setShaders(){
        // world space positions of our cubes
        // identificadores
        glGenVertexArrays(1, &VAO); // 1 arreglo de verices  asigna a VAO
        glGenBuffers(1, &VBO);// 1 arreglo de buffer de vertices y asigna a VBO
        //enlace VAO y VBO para configuraciones
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //se envia vertices a VBO
        // atributo de posiciones en el shader
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // atributo de coordenadas de textura de los vertices en el shader
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void setTextures(){
        // Uso de la función loadTexture para cargar las texturas
        texture1 = loadTexture(getPath("blue.jpg"));
        texture2 = loadTexture(getPath("awesomeface.png"));

        // Configuración de los uniformes de las texturas en el shader
        ourShader->use();
        ourShader->setInt("texture1", 0);
        ourShader->setInt("texture2", 1);

        // Pasar la matriz de proyección al shader
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader->setMat4("projection", projection);
    }


    unsigned int loadTexture(const std::string& filePath){
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load image, create texture, and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture: " << filePath << std::endl;
        }
        stbi_image_free(data);

        return texture;
    }

    

    ~Cube() {
        delete[] vertices;
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }


};


class Rubik {
public:

    vector<vector<vector<Cube*>>> cubes;
    int rows,cols,depths;

     Rubik(){
        rows = cols = depths = 3;
        cubes.resize(rows, vector<vector<Cube*>>(cols, vector<Cube*>(depths)));

        float offset = 1.25f;
        int i,j,k;
        i = j = k = 0;
         for (float y = offset; y >= -offset; y -= offset, i++){
             j=0;
             for (float x = -offset; x <= offset; x += offset,  j++){
                  k=0;
                 for (float z = offset; z >= -offset; z -= offset,  k++){
                    cubes[i][j][k] = new Cube(x,y,z);
                 }
             }
         }
    }




};

int main()
{
   
   	// #################################  WINDOW ###################################
	//  glfw: initialize and configure
	//  ------------------------------
	glfwInit();									   // init glw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // max version opengl
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // min version opnegl
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // window cambia

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGL(glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// #################################  END WINDOW #################################


    // ################################ VAO/VBO #########################################

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Cube cube(0.0f, 0.0f, 0.0f);
    Cube cube2(0.0f, 1.25f, 1.25f);
    // Shader ourShader(getPath("shader.vs").c_str(), getPath("shader.fs").c_str());

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
//    float vertices[] = {
//         // Cara frontal
//         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Vértice inferior izquierdo
//         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  // Vértice inferior derecho
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // Vértice superior derecho
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // Vértice superior derecho
//         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // Vértice superior izquierdo
//         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Vértice inferior izquierdo

//         // Cara posterior
//         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Vértice inferior izquierdo
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // Vértice inferior derecho
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // Vértice superior derecho
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // Vértice superior derecho
//         -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // Vértice superior izquierdo
//         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Vértice inferior izquierdo

//         // Cara izquierda
//         -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Vértice superior derecho
//         -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Vértice superior izquierdo
//         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Vértice inferior izquierdo
//         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Vértice inferior izquierdo
//         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Vértice inferior derecho
//         -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Vértice superior derecho

//         // Cara derecha
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // Vértice superior izquierdo
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // Vértice superior derecho
//         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  // Vértice inferior derecho
//         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  // Vértice inferior derecho
//         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  // Vértice inferior izquierdo
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // Vértice superior izquierdo

//         // Cara inferior
//         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Vértice inferior izquierdo
//         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  // Vértice inferior derecho
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // Vértice superior derecho
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // Vértice superior derecho
//         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Vértice superior izquierdo
//         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Vértice inferior izquierdo

//         // Cara superior
//         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // Vértice inferior izquierdo
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // Vértice inferior derecho
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // Vértice superior derecho
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // Vértice superior derecho
//         -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // Vértice superior izquierdo
//         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f  // Vértice inferior izquierdo
//     };

//    glm::vec3 cubePositions[] = {
//         // Vértices superiores
//         // Coordenadas x, y, z
//         glm::vec3(0.0f, 1.25f, 0.0f),        // Vértice 0
//         glm::vec3(1.25f, 1.25f, 0.0f),      // Vértice 1
//         glm::vec3(-1.25f, 1.25f, 0.0f),     // Vértice 2
//         glm::vec3(0.0f, 1.25f, -1.25f),     // Vértice 3
//         glm::vec3(1.25f, 1.25f, -1.25f),    // Vértice 4
//         glm::vec3(-1.25f, 1.25f, -1.25f),   // Vértice 5
//         glm::vec3(0.0f, 1.25f, 1.25f),      // Vértice 6
//         glm::vec3(1.25f, 1.25f, 1.25f),     // Vértice 7
//         glm::vec3(-1.25f, 1.25f, 1.25f),    // Vértice 8

//         // Vértices medios
//         // Coordenadas x, y, z
//         glm::vec3(0.0f, 0.0f, 0.0f),        // Vértice 9 CENTRO
//         glm::vec3(1.25f, 0.0f, 0.0f),       // Vértice 10 
//         glm::vec3(-1.25f, 0.0f, 0.0f),      // Vértice 11
//         glm::vec3(0.0f, 0.0f, -1.25f),      // Vértice 12
//         glm::vec3(1.25f, 0.0f, -1.25f),     // Vértice 13
//         glm::vec3(-1.25f, 0.0f, -1.25f),    // Vértice 14
//         glm::vec3(0.0f, 0.0f, 1.25f),       // Vértice 15
//         glm::vec3(1.25f, 0.0f, 1.25f),      // Vértice 16
//         glm::vec3(-1.25f, 0.0f, 1.25f),     // Vértice 17

//         // Vértices inferiores
//         // Coordenadas x, y, z
//         glm::vec3(0.0f, -1.25f, 0.0f),       // Vértice 18
//         glm::vec3(1.25f, -1.25f, 0.0f),      // Vértice 19
//         glm::vec3(-1.25f, -1.25f, 0.0f),     // Vértice 20
//         glm::vec3(0.0f, -1.25f, -1.25f),     // Vértice 21
//         glm::vec3(1.25f, -1.25f, -1.25f),    // Vértice 22
//         glm::vec3(-1.25f, -1.25f, -1.25f),   // Vértice 23
//         glm::vec3(0.0f, -1.25f, 1.25f),      // Vértice 24
//         glm::vec3(1.25f, -1.25f, 1.25f),     // Vértice 25
//         glm::vec3(-1.25f, -1.25f, 1.25f)     // Vértice 26
//     };




    // ################# VERTICES SHADERS ##########################################

    // // world space positions of our cubes
    // // identificadores
    // unsigned int VBO, VAO;// id buffer ->id vertices 
    // glGenVertexArrays(1, &VAO); // 1 arreglo de verices  asigna a VAO
    // glGenBuffers(1, &VBO);// 1 arreglo de buffer de vertices y asigna a VBO

    // //enlace VAO y VBO para configuraciones
    // glBindVertexArray(VAO);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //se envia vertices a VBO

    // // atributo de posiciones en el shader
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    // // atributo de coordenadas de textura de los vertices en el shader
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);
    // ################# END VERTICES SHADERS ##########################################



    // ################ TEXTURAS ##########################################################
    // // load and create a texture
    // unsigned int texture1, texture2;//ids para las texturas

    // // texture 1 
    // glGenTextures(1, &texture1);//id
    // glBindTexture(GL_TEXTURE_2D, texture1);// se puede hacer  operaciones con textura

    // // set the texture wrapping parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // // set texture filtering parameters SUAVIZADO
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    // // load image, create texture and generate mipmaps
    // int width, height, nrChannels;
    // stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // unsigned char *data = stbi_load(getPath("blue.jpg").c_str(), &width, &height, &nrChannels, 0);
    // if (data)
    // {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    // }
    // else
    // {
    //     std::cout << "Failed to load texture" << std::endl;
    // }
    // stbi_image_free(data);


    // // texture 2
    // glGenTextures(1, &texture2);
    // glBindTexture(GL_TEXTURE_2D, texture2);
    // // set the texture wrapping parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // // set texture filtering parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // // load image, create texture and generate mipmaps
    // data = stbi_load(getPath("awesomeface.png").c_str(), &width, &height, &nrChannels, 0);
    // if (data)
    // {
    //     // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    // }
    // else
    // {
    //     std::cout << "Failed to load texture" << std::endl;
    // }
    // stbi_image_free(data);

    // // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // // -------------------------------------------------------------------------------------------
    // ourShader.use();
    // ourShader.setInt("texture1", 0);//indices de textura
    // ourShader.setInt("texture2", 1);

    // // pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
    // // trasnsforma 3d -> 2D: carga los idetntificadores de textura en la matriz para q se pueda renderizar
    // glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    // ourShader.setMat4("projection", projection);

    //################################# END TEXTURAS ######################################################################

    cube.configShader_Textures();
    cube2.configShader_Textures();

    while (!glfwWindowShouldClose(window))
    {
        //calcula tiempo transcurrido en cada frame
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // color para la pantalla de color 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//clear buffer

        // bind textures on corresponding texture units
        // glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, texture1);
        // glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, texture2);


        // activate shader
        // ourShader.use();

//        // camera/view transformation
//        glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//        float radius = 10.0f;
//        float camX = static_cast<float>(sin(glfwGetTime()) * radius);
//        float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
//        view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//        ourShader.setMat4("view", view);
        // activate shader
        // ourShader.use();

        // ############## MATRICES #######################################################
        // pass projection matrix to shader (note that in this case it could change every frame)
        // glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // ourShader.setMat4("projection", projection);

        // camera/view transformation
        // glm::mat4 view = camera.GetViewMatrix();
        // ourShader.setMat4("view", view);
        //  // ############## END MATRICES ###################################################

        // // ################ CUBES RENDER ###################################################
        // glBindVertexArray(VAO);
        // for (unsigned int i = 0; i < 27; i++)
        // {
        //     // calculate the model matrix for each object and pass it to shader before drawing
        //     glm::mat4 model = glm::mat4(1.0f);
        //     model = glm::translate(model, cubePositions[i]);
        //     //float angle = 20.0f * i;
        //     //model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //     ourShader.setMat4("model", model);

        //     glDrawArrays(GL_TRIANGLES, 0, 36);
        // }

         // ################ END CUBES RENDER ##################################################
        cube.draw();
        cube2.draw();
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}