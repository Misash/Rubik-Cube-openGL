

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
    unsigned int VBO, VAO; // IDs para los buffers de vértices
    unsigned int textures[6]; // IDs para las texturas de cada lado
    glm::mat4 projection;
    Shader* ourShader;
    float vertices[180];
    glm::mat4 model;

    Cube(float x,float y,float z){
        //center
        center = new glm::vec3(x,y,z);
        ourShader = new Shader(getPath("shader.vs").c_str(), getPath("shader.fs").c_str());
        model = glm::mat4(1.0f); 
        model = glm::translate(model, *center);

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

        
         std::memcpy(vertices, vertices_, sizeof(vertices));
        
    }

    
    void configShader_Textures(){
        setShaders();
        setTextures();
        setProjection();
    }



    void draw(){
    
             // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader->setMat4("view", view);

        // calculate the model matrix for each object and pass it to shader before drawing
        // glm::mat4 model = glm::mat4(1.0f);
       
        ourShader->setMat4("model", model);

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

        glBindVertexArray(VAO);
   
    }

    void setShaders(){
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Coordenadas de posición del vértice
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Coordenadas de textura del vértice
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void setTextures(){
        // Cargar las texturas
        glGenTextures(6, textures);
        string fileNames[] = {"green_.jpg","pink.jpg","yellow.jpg","white.jpg","blue.jpg","red.jpg"};
        for (int i = 0; i < 6; i++) {
            loadTexture(getPath(fileNames[i]),textures[i]);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }


    void loadTexture(const std::string& filePath,unsigned int texture){

        glBindTexture(GL_TEXTURE_2D, texture);
        int width, height, nrChannels;
        unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Error al cargar la textura" << std::endl;
        }
        stbi_image_free(data);

        // Configurar opciones de la textura
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void rotateX(float angle) {
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    // ~Cube() {
    //     delete[] vertices;
    //     glDeleteVertexArrays(1, &VAO);
    //     glDeleteBuffers(1, &VBO);
    // }


};


class Rubik {
public:
    int rows,cols,depths;
    vector<vector<vector<Cube*>>> cubes;
    

    Rubik(){
        rows = cols = depths = 3;
        cubes.resize(rows, vector<vector<Cube*>>(cols, vector<Cube*>(depths)));

        float offset = 1.25f;
        int i,j,k;
        i = j = k = 0;
        for (float y = offset; y >= -offset && i < rows; y -= offset, i++){
             j=0;
             for (float x = -offset; x <= offset && j < cols; x += offset,  j++){
                  k=0;
                 for (float z = offset; z >= -offset && k < depths; z -= offset,  k++){
                    // row col depth
                    cubes[i][j][k] = new Cube(x,y,z);
                 }
             }
         }
    }


    void load_Shaders_Textures(){

        for (int i = 0; i < rows ; i++)
        {
            for(int j = 0; j < cols ; j++){

                for(int k = 0; k < depths ; k++){

                    cubes[i][j][k]->configShader_Textures();
                }
            }
        }
        
    }


    void draw(){
        for (int i = 0; i < rows ; i++)
        {
            for(int j = 0; j < cols ; j++){

                for(int k = 0; k < depths ; k++){

                    cubes[i][j][k]->draw();
                }
            }
        }
    }


    ~Rubik() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < depths; k++) {
                delete cubes[i][j][k];
            }
        }
    }
}




};


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// void processInput(GLFWwindow *window);
void processInput(GLFWwindow *window, Cube* cube);

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



    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------

    // Rubik rubikCube;

    // rubikCube.load_Shaders_Textures();

    Cube cube(0.0f, 0.0f, 0.0f);
    Cube cube2(0.0f, 1.25f, 1.25f);
    Cube cube3(0.0f, -1.25f, 1.25f);
   
    cube.configShader_Textures();
    cube2.configShader_Textures();
    cube3.configShader_Textures();

    while (!glfwWindowShouldClose(window))
    {
        //calcula tiempo transcurrido en cada frame
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window,&cube);

        // color para la pantalla de color 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//clear buffer


        
        cube.draw();
        cube2.draw();
        cube3.draw();

        // rubikCube.draw();


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
void processInput(GLFWwindow *window,Cube* cube)
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
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
         cube->rotateX(45.0f);
        cout<<"\nrotate x";
    }
       
       
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


