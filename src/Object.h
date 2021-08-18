#pragma once

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Structures.h"

#define GL_NULL 42069

class Object {

private:
	Mesh mesh;
	
	glm::vec3 scales, rotations, translations;

	glm::mat4 model;
	glm::mat4* view;
	glm::mat4* projection;

	Material material;

	GLuint vao, vertex_buffer, index_buffer;

	GLuint u_model, u_view, u_projection, u_col;
	GLuint u_ambient, u_diffuse, u_specular, u_shininess;

	GLuint shader_program;

public:
	Object();
	Object(Mesh m, Material ma, GLuint s);
	~Object();

	void attachShader(GLuint shader); //*
	void setMesh(Mesh m); //*

	void setColor(glm::vec4 col);

	void setModel(glm::mat4 m);
	void setView(glm::mat4* v); 
	void setProjection(glm::mat4* p); 
	void setViewProjection(glm::mat4* v, glm::mat4* p); //*

	void setMaterial(float a, float d, float s, int shiny);
	void setMaterial(Material m);

	void scale(float val);
	void scale(float x, float y, float z);
	void rotate(float x, float y, float z);
	void setRotations(float x, float y, float z);
	void translate(float x, float y, float z);
	void translate(glm::vec3 v);
	void setPosition(float x, float y, float z);
	void setPosition(glm::vec3 pos);

	void printTranslations();

	void refreshModel();

	void draw();
};
