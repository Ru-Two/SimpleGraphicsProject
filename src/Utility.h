#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

#include "Structures.h"

#define PI 3.14159265358979323846

#define SECONDS x 
#define MINUTES y
#define HOURS z

std::string fileToString(const std::string file_path);

GLuint compileShader(GLuint shader_type, const std::string& shader_source);
GLuint createShader(const std::string vertex_shader_filepath, const std::string fragment_shader_filepath);

Mesh loadMesh(std::string file_path);

glm::mat4 getView(glm::vec3 eye_position, glm::vec3 looking_at);

float degToRad(int deg);
glm::vec3 getTimeAngles();

bool pressed(GLFWwindow* window, int key);
bool pressed_or(GLFWwindow* window, std::vector<int> keys);

void printMat4(glm::mat4 m);
void printVec3(glm::vec3 v);