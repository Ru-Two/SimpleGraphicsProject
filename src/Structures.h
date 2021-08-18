#pragma once

#include <glm/glm.hpp>

#include <iostream>
#include <vector>

//used for bounds struct
#define LENGTH z
#define WIDTH w

struct Vertex {
	float x, y, z; //position
	float nx, ny, nz; //normal
};

struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	glm::vec4 color;

	void translate(float x, float y, float z) {
		for (int i = 0; i < vertices.size(); i++) {
			vertices[i].x += x;
			vertices[i].y += y;
			vertices[i].z += z;
		}
	}

	void scale(float val) {
		for (int i = 0; i < vertices.size(); i++) {
			vertices[i].x *= val;
			vertices[i].y *= val;
			vertices[i].z *= val;
		}
	}
};

struct Material {
	float ambient_effect;
	float diffuse_effect;
	float specular_effect;
	int shininess;
};

//represents any two dimensional bounds as a rectangle
struct Bounds {
	glm::vec2 object_position;
	glm::vec4 bounds;

	//moves object within bounds and returns total movement made
	glm::vec2 moveObject(glm::vec2 shift) {
		glm::vec2 tmp(0.0, 0.0);
		if (object_position.x + shift.x > bounds.x &&
			object_position.x + shift.x < bounds.x + bounds.LENGTH) {
			object_position.x += shift.x;
			tmp.x += shift.x;
		}

		if (object_position.y + shift.y > bounds.y &&
			object_position.y + shift.y < bounds.y + bounds.WIDTH) {
			object_position.y += shift.y;
			tmp.y += shift.y;
		}
		return tmp;
	}

	glm::vec2 moveObject(float move_x, float move_y) {
		glm::vec2 tmp(move_x, move_y);
		return moveObject(tmp);
	}
};
