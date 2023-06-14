// #define GLAD_GL_IMPLEMENTATION
// #include <glad/gl.h>
// #define GLFW_INCLUDE_NONE
// #include <GLFW/glfw3.h>


// #define STB_IMAGE_IMPLEMENTATION
// #include <stb_image.h>

// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

// #include "./Shader.h"
// #include "./libs/Camera.h"
// #include <filesystem>
// #include <iostream>
// #include <vector>

#include "./Libs/Rubik.h"

// using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
//void key_callback (GLFWwindow *window,Rubik* rubik,\
float angle,string& status, int& speed);

// settings
// const unsigned int SCR_WIDTH = 800;
// const unsigned int SCR_HEIGHT = 600;

// camera
// Camera camera(glm::vec3(0.0f, 0.0f, 9.0f));
// float lastX = SCR_WIDTH / 2.0f;
// float lastY = SCR_HEIGHT / 2.0f;
// bool firstMouse = true;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;


///////////////////////////////////////////////




void key_callback(GLFWwindow *window,Rubik* rubik,float angle,string& status, int& speed)
{
    // cout<<"\n\nSTATUS: "<<status<<"  SPEED?: "<<speed;
    

    if( status == "NO"){

        if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
            rubik->reverse = -1.0f * rubik->reverse;
            cout<<"REVERSE: "<<rubik->reverse;
        }
       
        //columns
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
        //Rows
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
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS){
            rubik->RotateDepth(2,angle,speed);
            status = "ROW2";
            speed++;
            cout<<"\nrotate row 2";
        }
        //Depths
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS){
            rubik->RotateDepth(0,angle,speed);
            status = "DEPTH0";
            speed++;
            cout<<"\nrotate depth 0";
        }
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){
            rubik->RotateDepth(1,angle,speed);
            status = "DEPTH1";
            speed++;
            cout<<"\nrotate depth 1";
        }
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS){
            rubik->RotateDepth(2,angle,speed);
            status = "DEPTH2";
            speed++;
            cout<<"\nrotate depth 2";
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
            rubik->RotateDepth(1,angle,speed);
            cout<<"\nrotate row 1";
        }
        if(status == "ROW2"){
            rubik->RotateDepth(2,angle,speed);
            cout<<"\nrotate row 2";
        }
        if(status == "DEPTH0"){
            rubik->RotateDepth(0,angle,speed);
            cout<<"\nrotate row 0";
        }
        if(status == "DEPTH1"){
            rubik->RotateDepth(1,angle,speed);
            cout<<"\nrotate row 1";
        }
        if(status == "DEPTH2"){
            rubik->RotateDepth(2,angle,speed);
            cout<<"\nrotate row 2";
        }




        if(angle*speed == rubik->reverse*90){
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
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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


    float incrementAngle = 0.1f;
    string status = "NO";
    int speed = 1.5;

    while (!glfwWindowShouldClose(window))
    {
        //calcula tiempo transcurrido en cada frame
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);
        
        //cout<<"\n\nSTATUS: "<<status<<"  SPEED?: "<<speed;
        key_callback(window,&rubikCube,rubikCube.reverse*incrementAngle,status,speed);
        
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


