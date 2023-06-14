#include "./Cube.h"



class Rubik {
public:

    int rows,cols,depths;
    vector<vector<vector<Cube*>>> cubes;
    vector<vector<Cube*>> temp;
    float vertices[180];
    unsigned int VBO, VAO; // IDs para los buffers de vértices
    unsigned int textures[6]; // IDs para las texturas de cada lado

    Rubik(){
        rows = cols = depths = 3;
        cubes.resize(rows, vector<vector<Cube*>>(cols, vector<Cube*>(depths)));
        temp.resize(rows, vector<Cube*>(cols));
        float offset = 1.25f;
        int i,j,k;
        i = j = k = 0;

        //create cubes
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


    void load_Shaders_Textures(){

        setShaders();
        setTextures();

        for (int i = 0; i < rows ; i++)
        {
            for(int j = 0; j < cols ; j++){

                for(int k = 0; k < depths ; k++){

                    cubes[i][j][k]->configShader_Textures(textures);
                }
            }
        }
        
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
            loadTexture(getPath("Textures/" + fileNames[i]),textures[i]);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(VAO);
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