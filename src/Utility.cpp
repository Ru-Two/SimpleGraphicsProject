#include "Utility.h"

//takes a file path as a string and returns the contents of that file back as a string
std::string fileToString(const std::string file_path) {
    std::ifstream input_file(file_path);
    std::string file_contents = "", tmp;

    while (std::getline(input_file, tmp)) { //read and concatenate
        file_contents = file_contents + tmp + "\n";
    }

    input_file.close();

    return file_contents;
}

/*
    compileShader : THIS FUNCTION (ALONG WITH createShader()) WAS DERIVED FROM THIS VIDEO BY THE CHERNO: https://www.youtube.com/watch?v=71BLZwRGUJE
                    I needed a good way to learn modern opengl, so I learned with this person's youtube series.
                    I am clarifying so I don't get hit with a plagiarism claim

                  * Takes in the type of shader to compile and the source string of the shader. It creates a shader
                    of the specified type and string, and returns the number that references that shader
*/
GLuint compileShader(GLuint shader_type, const std::string& shader_source) {
    GLuint shader_id = glCreateShader(shader_type);

    const char* src = &shader_source[0];

    glShaderSource(shader_id, 1, &src, nullptr); //give shader source code and compile
    glCompileShader(shader_id);

    return shader_id;
}

/*
    compileShader : THIS FUNCTION (ALONG WITH compileShader()) WAS DERIVED FROM THIS VIDEO BY THE CHERNO: https://www.youtube.com/watch?v=71BLZwRGUJE
                    I needed a good way to learn modern opengl, so I learned with this person's youtube series.
                    I am clarifying so I don't get hit with a plagiarism claim

                  * Takes in the file names of a vertex shader and fragment shader. Creates the program which the shaders
                    get attached to and returns that program back to function call.
*/
GLuint createShader(const std::string vertex_shader_filepath, const std::string fragment_shader_filepath) {
    //create program and shaders
    GLuint program = glCreateProgram();

    std::string vertex_shader = fileToString(vertex_shader_filepath);
    std::string fragment_shader = fileToString(fragment_shader_filepath);

    GLuint vs = compileShader(GL_VERTEX_SHADER, vertex_shader);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragment_shader);

    //attach shaders
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    //link and validate created program to this process
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

//given an edited ply file, this function will take the model from that file and return a mesh with that data
Mesh loadMesh(std::string file_path) {
    Mesh m{ {}, {}, glm::vec4(1.0, 1.0, 1.0, 1.0) };

    std::ifstream input_file(file_path);

    int vertex_count;
    input_file >> vertex_count;

    int index_count, tmp;
    input_file >> index_count;

    //vertices
    for (int i = 0; i < vertex_count; i++) {
        Vertex vertex;
        input_file >> vertex.x;
        input_file >> vertex.y;
        input_file >> vertex.z;
        input_file >> vertex.nx;
        input_file >> vertex.ny;
        input_file >> vertex.nz;
        m.vertices.push_back(vertex);
    }

    //indices
    for (int i = 0; i < index_count; i++) {
        input_file >> tmp;
        for (int j = 0; j < tmp; j++){
            unsigned int index;
            input_file >> index;
            m.indices.push_back(index);
        }
    }

    input_file.close();

    return m;
}

//given eye position and look at position, returns a view matrix
glm::mat4 getView(glm::vec3 eye_position, glm::vec3 looking_at){
    glm::mat4 view = glm::lookAt(glm::vec3(eye_position.x, eye_position.y, eye_position.z), // Camera is at this point
                                glm::vec3(looking_at.x, looking_at.y, looking_at.z),  // looking at this point
                                glm::vec3(0, 1, 0)); //look upside
    return view;
}

//converts given degree to radians
float degToRad(int deg) {
    return (((float)deg / 360.0f) * (2 * PI));
}

//function to return a vec3 containing the rotations needed for the clock object
//to rotate with each time increment
glm::vec3 getTimeAngles(){

    std::time_t time = std::time(NULL);

    int seconds = time % 60;
    int minutes = time % (60 * 60) / 60;
    int hours = time % (60 * 60 * 24) / (60 * 60) - 4;
    if (hours >= 12) hours -= 12; //12h format

    glm::vec3 angles(0.0, 0.0, 0.0);

    angles.SECONDS = degToRad(6 * seconds - 90); // 6 degrees per tick on the clock - 360/60 = 6
    
    angles.MINUTES = degToRad(6 * minutes - 90);
    
    angles.HOURS = degToRad((30 * hours - 90) + (minutes / 2)); // 30 deg per hour tick - 6 (seconds ricks) * 5 (ticks between each hour) = 30

    angles *= -1;
    angles -= PI / 2.0;

    return angles;
}

//checks if a key from given window is pressed
bool pressed(GLFWwindow* window, int key) {
    return (glfwGetKey(window, key) == GLFW_PRESS);
}

//checks if any key from the given list of keys is pressed within the given window
bool pressed_or(GLFWwindow* window, std::vector<int> keys){
    for (int key : keys) {
        if (glfwGetKey(window, key) == GLFW_PRESS) return true;
    }
    return false;
}

//prints any glm::mat4 type variable (4x4 matrix)
void printMat4(glm::mat4 m) {
    std::cout << "----------------------------------\n";
    for (int i = 0; i < 4; i++) {
        printf("[%2.4f  %2.4f  %2.4f  %2.4f]\n", m[i][0], m[i][1], m[i][2], m[i][3]);
    }
    std::cout << "----------------------------------\n";
}

//prints any vec3
void printVec3(glm::vec3 v){
    std::cout << "( " << v.x << " , " << v.y << " , " << v.z << " )\n";
}
