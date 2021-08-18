#include "Vec3StateMachine.h"

Vec3StateMachine::Vec3StateMachine(){
	points = {};
	current_index = 0;
}

Vec3StateMachine::Vec3StateMachine(std::vector<glm::vec3> v) {
	points = v;
	current_index = 0;
}

Vec3StateMachine::~Vec3StateMachine() {}

//adds vector to state machine
void Vec3StateMachine::addVector(glm::vec3 v){
	points.push_back(v);
}

//adds vector to state machine
void Vec3StateMachine::addVector(double x, double y, double z){
	points.push_back(glm::vec3(x, y, z));
}

//gets index of current selected vector
unsigned int Vec3StateMachine::getIndex(){
	return current_index;
}

//gets current vector
glm::vec3 Vec3StateMachine::getVec(){
	if (points.empty() || current_index >= points.size() || current_index < 0) return glm::vec3(0.0, 0.0, 0.0);

	return points[current_index];
}

//selects index of vector in state machine
glm::vec3 Vec3StateMachine::selectIndex(unsigned int i){
	if (i < points.size() && i >= 0) current_index = i;

	return getVec();
}

//transfers current index to the next. If index is at the tail, return index to 0
glm::vec3 Vec3StateMachine::next(){
	current_index++;
	if (current_index >= points.size()) current_index = 0;

	return getVec();
}

//returns current selected vector
glm::vec3 Vec3StateMachine::current(){
	return getVec();
}

//transfers current index to the previous. If index is at the head, return index to tail
glm::vec3 Vec3StateMachine::prev() {
	if (current_index == 0) current_index = points.size() - 1;
	else current_index--;

	return getVec();
}

//translates current vector by given values
glm::vec3 Vec3StateMachine::move(glm::vec3 v){
	if (points.empty()) return glm::vec3(0.0);

	points[current_index].x += v.x;
	points[current_index].y += v.y;
	points[current_index].z += v.z;

	return points[current_index];
}

//translates current vector by given values
glm::vec3 Vec3StateMachine::move(double x, double y, double z){
	if (points.empty()) return glm::vec3(0.0);

	points[current_index].x += x;
	points[current_index].y += y;
	points[current_index].z += z;

	return points[current_index];
}

//translates vector at given index by given values
glm::vec3 Vec3StateMachine::move(unsigned int i, glm::vec3 v) {
	if (points.empty()) return glm::vec3(0.0);

	points[i].x += v.x;
	points[i].y += v.y;
	points[i].z += v.z;

	return points[i];
}

//translates vector at given index by given values
glm::vec3 Vec3StateMachine::move(unsigned int i, double x, double y, double z) {
	if (points.empty()) return glm::vec3(0.0);

	points[i].x += x;
	points[i].y += y;
	points[i].z += z;

	return points[current_index];
}