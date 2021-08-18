#pragma once

#include <glm/glm.hpp>

#include <vector>

class Vec3StateMachine {
private:
	std::vector<glm::vec3> points;
	unsigned int current_index;

public:
	Vec3StateMachine();
	Vec3StateMachine(std::vector<glm::vec3> v);
	~Vec3StateMachine();

	void addVector(glm::vec3 v);
	void addVector(double x, double y, double z);

	unsigned int getIndex();
	glm::vec3 getVec();
	glm::vec3 selectIndex(unsigned int i);

	glm::vec3 next();
	glm::vec3 current();
	glm::vec3 prev();

	glm::vec3 move(glm::vec3 v);
	glm::vec3 move(unsigned int i, glm::vec3 v);
	glm::vec3 move(double x, double y, double z);
	glm::vec3 move(unsigned int i, double x, double y, double z);
};