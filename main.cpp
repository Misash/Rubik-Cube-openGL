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
//void key_callback (GLFWwindow *window,Rubik* rubik,\
float angle,string& status, int& speed);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 9.0f));
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

    // std::cout << "El path es: " << escapedPath << std::endl;
    return escapedPath;
}


class Cube{
    public:
    glm::vec4 worldspace_center;
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
        model = glm::mat4(1.0f); //matriz identidad
        // model = glm::scale(model, glm::vec3(2.0f,2.0f,2.0f)); //matriz de traslacion 
        model = glm::translate(model, *center); //matriz de traslacion 
        worldspace_center = model * glm::vec4(center->x,center->y,center->z,1);
        // center = new glm::vec3(worldspace_center.x,worldspace_center.y,worldspace_center.z);
        
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

    void rotateHorizontal_(float angle,glm::vec3 center_) {
        // center = center_;

        // Trasladar al centro de rotación
        model = glm::translate(model, -center_);

        // Realizar la rotación
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));

        // Regresar a la posición original
        model = glm::translate(model, center_);
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

  

    void rotateAroundCenter(float angle, const glm::vec3& center_) {


        // Trasladar al centro de rotación
        model = glm::translate(model, -center_);

        glm::vec3 ratio = *center - center_;

        // Realizar la rotación horizontal en funcion al centro dado gira 45 grados
        glm::mat4 verticalRotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
        model = verticalRotation * model;

        

        // // Realizar la rotación vertical
        // glm::mat4 verticalRotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        // model = verticalRotation * model;

        // Regresar a la posición original
        model = glm::translate(model, center_);

        // cout<<"\ncentro: "<< center->x<<" , "<<center->y<<" , "<<center->z;  
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
    vector<vector<Cube*>> temp;

    Rubik(){
        rows = cols = depths = 3;
        cubes.resize(rows, vector<vector<Cube*>>(cols, vector<Cube*>(depths)));
        temp.resize(rows, vector<Cube*>(cols));
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



    void RotateRow(int col_index,float angle,int speed){

        glm::vec3 RowCenter = *(cubes[col_index][1][1]->center);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cubes[col_index][i][j]->rotateVertical(angle,RowCenter);
                //guardar en temp
                if(speed == 1) temp[i][j] = cubes[col_index][i][j];
            }
        }

        if(speed*angle != 90) return;

        //reasignar posiciones cubos 
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cubes[col_index][2-j][i] = temp[i][j];
            }
        }
    }

    void RotateColumn(int col_index, float angle, int speed) {
        glm::vec3 RowCenter = *(cubes[1][col_index][1]->center);
        

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cubes[i][col_index][j]->rotateHorizontal(angle,RowCenter);
                //guardar en temp en la primera iteracion
                if(speed == 1) temp[i][j] = cubes[i][col_index][j];
            }
        }

        if(speed*angle != 90) return;

        //reasignar posiciones cubos 
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cubes[2-j][col_index][i] = temp[i][j];
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

void key_callback(GLFWwindow *window,Rubik* rubik,float angle,string& status, int& speed)
{
    if( status == "NO"){
        //yikes    
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
            rubik->RotateColumn(0,angle,speed);
            status = "COL0";
            speed++;
            cout<<"\nrotate col 0";
        }
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
            rubik->RotateColumn(1,angle,speed);
            status = "COL1";
            speed++;
            cout<<"\nrotate col 1";
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
            rubik->RotateColumn(2,angle,speed);
            status = "COL2";
            speed++;
            cout<<"\nrotate col 2";
        }
        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS){
            rubik->RotateRow(0,angle,speed);
            status = "ROW0";
            speed++;
            cout<<"\nrotate row 0";
        }
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS){
            rubik->RotateRow(1,angle,speed);
            status = "ROW1";
            speed++;
            cout<<"\nrotate row 1";
        }
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS){
            rubik->RotateRow(2,angle,speed);
            status = "ROW2";
            speed++;
            cout<<"\nrotate row 2";
        }
        }else{

        if(status == "COL0"){
            rubik->RotateColumn(0,angle,speed);
            cout<<"\nrotate col 0";
        }
        if(status == "COL1"){
            rubik->RotateColumn(1,angle,speed);
            cout<<"\nrotate col 1";
        }
        if(status == "COL2"){
            rubik->RotateColumn(2,angle,speed);
            cout<<"\nrotate col 2";
        }
        if(status == "ROW0"){
            rubik->RotateRow(0,angle,speed);
            cout<<"\nrotate row 0";
        }
        if(status == "ROW1"){
            rubik->RotateRow(1,angle,speed);
            cout<<"\nrotate row 1";
        }
        if(status == "ROW2"){
            rubik->RotateRow(2,angle,speed);
            cout<<"\nrotate row 2";
        }

        if(angle*speed >= 90){
            status = "NO";
            speed = 1;
         }else{
            speed++;
        }

    }
   

}

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    Rubik rubikCube;

    rubikCube.load_Shaders_Textures();

    // Cube cube(0.0f, 0.0f, 0.0f);
    // Cube cube2(0.0f, 1.25f, 0.0f);
    // Cube cube3(0.0f, -1.25f, 1.25f);
   
    // cube.configShader_Textures();
    // cube2.configShader_Textures();
    // cube3.configShader_Textures();

    // enum rotatingCubes{
    //     NO,
    //     COL0,
    //     COL1,
    //     COL2,
    //     ROW0,
    //     ROW1,
    //     ROW2
    // }


    float incrementAngle = 30.0f;
    string status = "NO";
    int speed = 1;

    while (!glfwWindowShouldClose(window))
    {
        //calcula tiempo transcurrido en cada frame
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);
        
        cout<<"\n\nSTATUS: "<<status<<"  SPEED?: "<<speed;
        key_callback(window,&rubikCube,incrementAngle,status,speed);
        
        // color para la pantalla de color 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//clear buffer

        // cube.draw();
        // cube2.draw();
        // cube3.draw();

        rubikCube.draw();


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
void processInput(GLFWwindow* window)
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


