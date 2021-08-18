#include "Object.h"

/*
	Object class to store each object and its data
*/

Object::Object() {
	model = glm::mat4(1.0);
	view = nullptr;
	projection = nullptr;

	scales = glm::vec3(1.0, 1.0, 1.0);
	rotations = glm::vec3(0.0, 0.0, 0.0);
	translations = glm::vec3(0.0, 0.0, 0.0);

	material = { 0.1f, 0.5f, 0.7f, 50 };

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vertex_buffer);
	glGenBuffers(1, &index_buffer);

	shader_program = GL_NULL;

	u_model = GL_NULL;
	u_view = GL_NULL;
	u_projection = GL_NULL;
	u_col = GL_NULL;
	u_ambient = GL_NULL;
	u_diffuse = GL_NULL;
	u_specular = GL_NULL;

	setMesh(Mesh{{}, {}, glm::vec4(1.0, 1.0, 1.0, 1.0)});
}

Object::Object(Mesh m, Material ma, GLuint s) {
	model = glm::mat4(1.0);
	view = nullptr;
	projection = nullptr;

	scales = glm::vec3(1.0, 1.0, 1.0);
	rotations = glm::vec3(0.0, 0.0, 0.0);
	translations = glm::vec3(0.0, 0.0, 0.0);

	material = ma;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vertex_buffer);
	glGenBuffers(1, &index_buffer);

	shader_program = GL_NULL;

	u_model = GL_NULL;
	u_view = GL_NULL;
	u_projection = GL_NULL;
	u_col = GL_NULL;
	u_ambient = GL_NULL;
	u_diffuse = GL_NULL;
	u_specular = GL_NULL;

	attachShader(s);
	setMesh(m);
}

Object::~Object() {

}

//give this object a shader program
void Object::attachShader(GLuint shader){
	shader_program = shader;

	u_model = glGetUniformLocation(shader_program, "u_model");
	u_view = glGetUniformLocation(shader_program, "u_view");
	u_projection = glGetUniformLocation(shader_program, "u_projection");
	u_col = glGetUniformLocation(shader_program, "u_col");
	u_ambient = glGetUniformLocation(shader_program, "u_ambient");
	u_diffuse = glGetUniformLocation(shader_program, "u_diffuse");
	u_specular = glGetUniformLocation(shader_program, "u_specular");
	u_shininess = glGetUniformLocation(shader_program, "u_shininess");	
}

//set this object's mesh and buffer data
void Object::setMesh(Mesh m){
	mesh = m;

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, x));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, nx));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);
}

//setters
void Object::setColor(glm::vec4 col){
	mesh.color = col;
}

void Object::setModel(glm::mat4 m) {
	model = m;
}

void Object::setView(glm::mat4* v) {
	view = v;
}

void Object::setProjection(glm::mat4* p) {
	projection = p;
}

void Object::setViewProjection(glm::mat4* v, glm::mat4* p){
	view = v;
	projection = p;
}

void Object::setMaterial(float a, float d, float s, int shiny){
	material = Material{a, d, s, shiny};
}

void Object::setMaterial(Material m) {
	material = m;
}

//scale's object by given value
void Object::scale(float val){
	scales = scales * val;

	refreshModel();
}

//scale's object by given values
void Object::scale(float x, float y, float z){
	scales = scales * glm::vec3(x, y, z);

	refreshModel();
}

//rotates object
void Object::rotate(float x, float y, float z){
	rotations = rotations + glm::vec3(x, y, z);

	refreshModel();
}

//sets total rotations
void Object::setRotations(float x, float y, float z){
	rotations.x = x;
	rotations.y = y;
	rotations.z = z;

	refreshModel();
}

//translates object 
void Object::translate(float x, float y, float z){
	translations = translations + glm::vec3(x, y, z);
	
	refreshModel();
}

//translates object 
void Object::translate(glm::vec3 v){
	translations = translations + v;

	refreshModel();
}

//sets object translation
void Object::setPosition(float x, float y, float z){
	translations = glm::vec3(x, y, z);

	refreshModel();
}

//sets object translation
void Object::setPosition(glm::vec3 pos) {
	translations = pos;

	refreshModel();
}

//updates model matrix with current matrix data
void Object::refreshModel() {
	glm::mat4 sca = glm::scale(glm::mat4(1.0), scales);
	glm::mat4 rotx = glm::rotate(glm::mat4(1.0), (float)rotations.x, glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 roty = glm::rotate(glm::mat4(1.0), (float)rotations.y, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rotz = glm::rotate(glm::mat4(1.0), (float)rotations.z, glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 tra = glm::translate(glm::mat4(1.0), translations);

	model = tra * rotz * roty * rotx * sca;
}

//draws object using GL_TRIANGLES
void Object::draw() {
	if (shader_program != GL_NULL) {
		glUseProgram(shader_program);

		//update uniforms
		glUniform4f(u_col, mesh.color.x, mesh.color.y, mesh.color.z, mesh.color.w);

		glUniformMatrix4fv(u_model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(u_view, 1, GL_FALSE, &((*view)[0][0]));
		glUniformMatrix4fv(u_projection, 1, GL_FALSE, &((*projection)[0][0]));

		glUniform1f(u_ambient, material.ambient_effect);
		glUniform1f(u_diffuse, material.diffuse_effect);
		glUniform1f(u_specular, material.specular_effect);
		glUniform1i(u_shininess, material.shininess);

		//bind arrays and draw
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
	}
}

//printing for debug
void Object::printTranslations() {
	std::cout << "( " << translations.x << " , " << translations.y << " , " << translations.z << " )\n";
}