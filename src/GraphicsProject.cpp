/*

    Ruturaj Darji
    Computer Graphics Final Project

    Lighting 3D models

*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <ctime>

#include "Object.h"
#include "Utility.h"
#include "Structures.h"
#include "Vec3StateMachine.h"
#include "MyTimer.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
const char WINDOW_NAME[] = "Final Project";

#define LIGHT_SOURCE_1 0
#define LIGHT_SOURCE_2 1
#define LIGHT_SOURCE_3 2

// main
int main(void) {
    GLFWwindow* window;

    //init glfw
    if (!glfwInit()) return -1;

    //use version 3.3 with core profile to use vaos
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //create window
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    //init glew
    if (glewInit() != GLEW_OK) return -1;

    glMatrixMode(GL_PROJECTION);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    //done initializing opengl

    //shader programs
    GLuint shader_program = createShader("resource/vertex_shader.txt", "resource/fragment_shader.txt");
    
    //light positions
    glm::vec3 light_source_1(3, 15, -3);
    glm::vec3 light_source_2(0, 13, 10);
    glm::vec3 light_source_3(-10, 13, 0);

    glm::vec3* selected_light_source = &light_source_1;

    //eye positions and lookat
    Vec3StateMachine eye_position;
    eye_position.addVector(-17, 18, 17);
    eye_position.addVector(0, 13, 7);
    eye_position.addVector(-4, 15, -3);
    eye_position.addVector(-20, 14, -2);
    eye_position.addVector(10, 14, 11);
    eye_position.addVector(-7, 34, 7);
    glm::vec3 current_eye_position = eye_position.current();

    Vec3StateMachine eye_lookat;
    eye_lookat.addVector(10, 2.5, -10);
    eye_lookat.addVector(0, 5, 0);
    eye_lookat.addVector(4, 12, -6);
    eye_lookat.addVector(5, 7, -5);
    eye_lookat.addVector(5, 9.5, -6);
    eye_lookat.addVector(8, 0, -6);
    glm::vec3 current_eye_lookat = eye_lookat.current();

    //view projection matrices
    glm::mat4 projection = glm::perspective(glm::radians(50.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.001f, 100.0f);
    glm::mat4 view = getView(current_eye_position, current_eye_lookat);

    //Materials
    
    float ambience = 0.2;
    
    Material floor_material{ ambience, 0.3f, 0.2f, 5 };
    Material walls_material{ ambience, 0.2f, 0.15f, 50 };
    Material container_material{ ambience, 0.3f, 0.35f, 50 };
    Material table_material{ ambience, 0.4f, 0.3f, 50 };
    Material mug_material{ ambience, 0.6f, 0.8f, 400 };
    Material laptop_material{ ambience, 0.6f, 0.6f, 300 };
    Material phone_material{ ambience, 0.6f, 0.75f, 50 };
    Material curtain_material{ ambience, 0.2f, 0.1f, 50 };
    Material chair_material{ ambience, 0.4f, 0.4f, 50 };
    Material cabinet_material{ ambience, 0.4f, 0.2f, 50 };
    Material teapot_material{ ambience, 0.6f, 0.75f, 300 };

    Material clock_frame_material{ ambience, 0.7f, 0.5f, 300 };
    Material clock_background_material{ ambience, 1.0f, 0.75f, 250 };
    Material clock_hand_material{ ambience, 0.5f, 0.6f, 75 };

    Material light_ball_material{ 1.0f, 1.0f, 1.0f, 50 };

    //Colors
    glm::vec4 wall_color(0.7, 0.7, 0.9, 1.0);
    glm::vec4 floor_color(0.580, 0.345, 0.141, 1.0);
    glm::vec4 container_color(0.494, 0.274, 0.125, 1.0);
    glm::vec4 table_color(0.623, 0.454, 0.196, 1.0);
    glm::vec4 mug_color(0.917, 0.901, 0.901, 1.0);
    glm::vec4 laptop_color(0.6, 0.6, 0.6, 1.0);
    glm::vec4 phone_color(0.862, 0.756, 0.850, 1.0);
    glm::vec4 curtain_color(0.505, 0.219, 0.541, 1.0);
    glm::vec4 chair_color(0.882, 0.329, 0.176, 1.0);
    glm::vec4 cabinet_color(0.466, 0.313, 0.172, 1.0);
    glm::vec4 teapot_color(0.498, 0.470, 0.4, 1.0);

    glm::vec4 clock_frame_color(0.1, 0.1, 0.1, 1.0);
    glm::vec4 clock_background_color(1.0, 1.0, 1.0, 1.0);
    glm::vec4 clock_second_hand_color(0.811, 0.164, 0.164, 1.0);
    glm::vec4 clock_minute_hand_color(0.164, 0.360, 0.811, 1.0);
    glm::vec4 clock_hour_hand_color(0.168, 0.168, 0.168, 1.0);

    glm::vec4 light_ball_color(1.0, 1.0, 0.3, 0.5);
    glm::vec4 selected_light_ball_color(1.0, 0.0, 0.3, 0.5);

    //meshes and colors
    Mesh floor_mesh = loadMesh("resource/floor.ply");
    floor_mesh.color = floor_color;

    Mesh walls_mesh = loadMesh("resource/walls.ply");
    walls_mesh.color = wall_color;

    Mesh container_mesh = loadMesh("resource/box.ply");
    container_mesh.color = container_color;

    Mesh table_mesh = loadMesh("resource/table.ply");
    table_mesh.color = table_color;

    Mesh mug_mesh = loadMesh("resource/mug.ply");
    mug_mesh.translate(1.8f, 0.0f, 1.8f);
    mug_mesh.color = mug_color;

    Mesh laptop_mesh = loadMesh("resource/laptop.ply");
    laptop_mesh.translate(-0.13f, 0.0f, -0.13f);
    laptop_mesh.color = laptop_color;

    Mesh phone_mesh = loadMesh("resource/phone.ply");
    phone_mesh.color = phone_color;
    phone_mesh.translate(1.4f, 0.0f, -1.1f);

    Mesh curtain_mesh = loadMesh("resource/curtain.ply");
    curtain_mesh.color = curtain_color;

    Mesh chair_mesh = loadMesh("resource/chair.ply");
    chair_mesh.color = chair_color;

    Mesh cabinet_mesh = loadMesh("resource/cabinet.ply");
    cabinet_mesh.color = cabinet_color;

    Mesh teapot_mesh = loadMesh("resource/teapot.ply");
    teapot_mesh.color = teapot_color;

    Mesh clock_frame_mesh = loadMesh("resource/clock_frame.ply");
    clock_frame_mesh.color = clock_frame_color;

    Mesh clock_background_mesh = loadMesh("resource/clock_background.ply");
    clock_background_mesh.color = clock_background_color;

    Mesh clock_second_hand_mesh = loadMesh("resource/clock_second_hand.ply");
    clock_second_hand_mesh.color = clock_second_hand_color;

    Mesh clock_minute_hand_mesh = loadMesh("resource/clock_minute_hand.ply");
    clock_minute_hand_mesh.color = clock_minute_hand_color;

    Mesh clock_hour_hand_mesh = loadMesh("resource/clock_hour_hand.ply");
    clock_hour_hand_mesh.color = clock_hour_hand_color;
   
    Mesh sphere_mesh = loadMesh("resource/sphere.ply");
    sphere_mesh.scale(0.5);
    sphere_mesh.color = light_ball_color;

    //objects 

    //floor
    Object floor(floor_mesh, floor_material, shader_program);

    //wall
    Object walls(walls_mesh, walls_material, shader_program);
    walls.scale(1.5f);
    walls.translate(-0.02f, 9.0f, -15.0f);

    //box containing objects
    Object container(container_mesh, container_material, shader_program);
    container.scale(0.7f);
    container.translate(0.0f, 5.0f, 0.0f);
    
    //table
    Object table(table_mesh, table_material, shader_program);
    table.scale(6.0f);

    //mug in box
    Object mug(mug_mesh, mug_material, shader_program);
    mug.scale(0.5f);
    mug.translate(0.0f, 5.5f, 0.0f);

    //laptop in box
    Object laptop(laptop_mesh, laptop_material, shader_program);
    laptop.scale(3.75f);
    laptop.translate(0.0f, 5.35f, 0.0f);

    //phone in box
    Object phone(phone_mesh, phone_material, shader_program);
    phone.translate(0.0f, 6.18f, 0.0f);

    //curtain
    Object curtain(curtain_mesh, curtain_material, shader_program);
    curtain.scale(19.5f, 9.5f, 9.5f);
    curtain.rotate(0.0f, -PI / 2.0f, 0.0f);
    curtain.translate(14.3f, 11.0f, 0.0f);

    //chair 1
    Object chair1(chair_mesh, chair_material, shader_program);
    chair1.scale(9.0f);
    chair1.translate(-0.3f, 0.0f, -4.3f);

    //chair 2
    Object chair2(chair_mesh, chair_material, shader_program);
    chair2.scale(9.0f);
    chair2.rotate(0.0f, -PI / 2.0f, 0.0f);
    chair2.translate(5.45f, 0.0f, 0.3f);

    //chair 3
    Object chair3(chair_mesh, chair_material, shader_program);
    chair3.scale(9.0f);
    chair3.rotate(0.0f, PI / 2.0f, 0.0f);
    chair3.translate(-5.5f, 0.0f, 0.0f);

    //cabinet
    Object cabinet(cabinet_mesh, cabinet_material, shader_program);
    cabinet.scale(9.5f);
    cabinet.translate(-7.4f, 5.0f, -12.4f);

    Object teapot(teapot_mesh, teapot_material, shader_program);
    teapot.translate(-7.35f, 10.0f, -12.15f);

    //clock items
    Object clock_frame(clock_frame_mesh, clock_frame_material, shader_program);
    Object clock_background(clock_background_mesh, clock_background_material, shader_program);
    Object clock_second_hand(clock_second_hand_mesh, clock_hand_material, shader_program);
    Object clock_minute_hand(clock_minute_hand_mesh, clock_hand_material, shader_program);
    Object clock_hour_hand(clock_hour_hand_mesh, clock_hand_material, shader_program);

    //object for lights
    Object light_ball_1(sphere_mesh, light_ball_material, shader_program);
    light_ball_1.setPosition(light_source_1);
    light_ball_1.setColor(selected_light_ball_color);

    Object light_ball_2(sphere_mesh, light_ball_material, shader_program);
    light_ball_2.setPosition(light_source_2);

    Object light_ball_3(sphere_mesh, light_ball_material, shader_program);
    light_ball_3.setPosition(light_source_3);

    Object* selected_light_ball = &light_ball_1;
    
    //sets of objects
    std::vector<Object*> box_items;
    std::vector<Object*> clock_items;

    std::vector<Object*> all_objects;
    std::vector<Object*> all_light_balls;
    std::vector<Object*> everything;
    
    box_items.push_back(&container);
    box_items.push_back(&mug);
    box_items.push_back(&laptop);
    box_items.push_back(&phone);

    clock_items.push_back(&clock_frame);
    clock_items.push_back(&clock_background);
    clock_items.push_back(&clock_second_hand);
    clock_items.push_back(&clock_minute_hand);
    clock_items.push_back(&clock_hour_hand);

    for (Object* o : clock_items) { 
        o->scale(2.2f);
        o->translate(6.4f, 13.0f, -14.55f); 
    }

    all_objects.push_back(&floor);
    all_objects.push_back(&walls);
    all_objects.push_back(&container);
    all_objects.push_back(&table);
    all_objects.push_back(&mug);
    all_objects.push_back(&laptop);
    all_objects.push_back(&phone);
    all_objects.push_back(&curtain);
    all_objects.push_back(&chair1);
    all_objects.push_back(&chair2);
    all_objects.push_back(&chair3);
    all_objects.push_back(&cabinet);
    all_objects.push_back(&teapot);
    all_objects.push_back(&clock_frame);
    all_objects.push_back(&clock_background);
    all_objects.push_back(&clock_second_hand);
    all_objects.push_back(&clock_minute_hand);
    all_objects.push_back(&clock_hour_hand);

    all_light_balls.push_back(&light_ball_1);
    all_light_balls.push_back(&light_ball_2);
    all_light_balls.push_back(&light_ball_3);

    everything.insert(std::end(everything), std::begin(all_objects), std::end(all_objects));
    everything.insert(std::end(everything), std::begin(all_light_balls), std::end(all_light_balls));

    //lighting for groups of objects
    for (Object* o : everything) { o->setViewProjection(&view, &projection); }

    //window variables
    bool light_1_flipped = true;
    bool light_2_flipped = true;
    bool light_3_flipped = true;

    glm::vec3 far_away(9999,9999,9999);

    Bounds box_bounds{ glm::vec2(0.0, 0.0), glm::vec4(-4.14, -2.95, 8.065, 5.99) };

    //light travel distance
    Vec3StateMachine attenuation_distance_values;
    attenuation_distance_values.addVector(1.0, 0.7, 1.8);
    attenuation_distance_values.addVector(1.0, 0.35, 0.44);
    attenuation_distance_values.addVector(1.0, 0.22, 0.2);
    attenuation_distance_values.addVector(1.0, 0.14, 0.07);
    attenuation_distance_values.addVector(1.0, 0.09, 0.032);
    attenuation_distance_values.addVector(1.0, 0.07, 0.017);
    attenuation_distance_values.addVector(1.0, 0.045, 0.0075);
    attenuation_distance_values.addVector(1.0, 0.027, 0.0028);
    attenuation_distance_values.addVector(1.0, 0.022, 0.0019);
    attenuation_distance_values.addVector(1.0, 0.014, 0.0007);
    attenuation_distance_values.addVector(1.0, 0.007, 0.0002);
    attenuation_distance_values.addVector(1.0, 0.0014, 0.000007);

    glm::vec3 attenuation_distance = attenuation_distance_values.selectIndex(7);
    
    //global uniforms
    GLuint u_attenuation_values = glGetUniformLocation(shader_program, "u_attenuation_values");
    GLuint u_light_position_1   = glGetUniformLocation(shader_program, "u_light_position_1");
    GLuint u_light_position_2   = glGetUniformLocation(shader_program, "u_light_position_2");
    GLuint u_light_position_3   = glGetUniformLocation(shader_program, "u_light_position_3");
    GLuint u_eye_position       = glGetUniformLocation(shader_program, "u_eye_position");

    glUseProgram(shader_program);

    glUniform3fv(u_attenuation_values, 1, &attenuation_distance[0]);
    glUniform3fv(u_light_position_1,   1, &light_source_1[0]);
    glUniform3fv(u_light_position_2,   1, &light_source_2[0]);
    glUniform3fv(u_light_position_3,   1, &light_source_3[0]);
    glUniform3fv(u_eye_position,       1, &current_eye_position[0]);

    //timers
    MyTimer camera_shift_timer, attenuation_change_timer, light_switch_timer;

    //debug variables
    Object* tmp = &teapot;
    glm::vec3 tmp_pos(0.0, 0.0, 0.0);

    //start window until closed
    while (!glfwWindowShouldClose(window)) {

        //clear screen and depth buffer
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 shift(0.0, 0.0, 0.0);
        
        //movement of selected light source
        if (pressed_or(window, 
            { GLFW_KEY_D, GLFW_KEY_A, GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_SPACE })){

            if (pressed(window, GLFW_KEY_D)) shift += glm::vec3(0.05, 0.0, 0.0);
            if (pressed(window, GLFW_KEY_A)) shift += glm::vec3(-0.05, 0.0, 0.0);
            if (pressed(window, GLFW_KEY_W)) shift += glm::vec3(0.0, 0.0, -0.05);
            if (pressed(window, GLFW_KEY_S)) shift += glm::vec3(0.0, 0.0, 0.05);
            if (pressed(window, GLFW_KEY_LEFT_SHIFT)) shift += glm::vec3(0.0, -0.05, 0.0);
            if (pressed(window, GLFW_KEY_SPACE)) shift += glm::vec3(0.0, 0.05, 0.0);

            *selected_light_source += shift;
            (*selected_light_ball).translate(shift);

            shift = glm::vec3(0.0, 0.0, 0.0);
        }

        //TAB - switch view
        if (pressed(window, GLFW_KEY_TAB) && camera_shift_timer.ready()) {
            current_eye_position = eye_position.next();
            current_eye_lookat = eye_lookat.next();
            view = getView(current_eye_position, current_eye_lookat);

            glUseProgram(shader_program);
            glUniform3fv(u_eye_position, 1, &current_eye_position[0]);

            //std::cout << "\nCamera Position: " << eye_position.getIndex()+1 << std::endl;

            camera_shift_timer.reset();
        }

        //brightness darker
        if (pressed_or(window, { GLFW_KEY_Z, GLFW_KEY_X }) && attenuation_change_timer.ready()) {

            if (pressed(window, GLFW_KEY_Z)) attenuation_distance = attenuation_distance_values.prev();
            if (pressed(window, GLFW_KEY_X)) attenuation_distance = attenuation_distance_values.next();

            glUseProgram(shader_program);
            glUniform3fv(u_attenuation_values, 1, &attenuation_distance[0]);

            attenuation_change_timer.reset();
        }

        //ENTER - current light switch
        if (pressed(window, GLFW_KEY_ENTER) && light_switch_timer.ready()) {
            if (selected_light_source == &light_source_1) {
                light_1_flipped = !light_1_flipped;
            }
            if (selected_light_source == &light_source_2) {
                light_2_flipped = !light_2_flipped;
            }
            if (selected_light_source == &light_source_3) {
                light_3_flipped = !light_3_flipped;
            }
            light_switch_timer.reset();
        }

        //1 - light 1 select to move
        if (pressed(window, GLFW_KEY_1)) {
            selected_light_source = &light_source_3;
            selected_light_ball = &light_ball_3;

            light_ball_1.setColor(light_ball_color);
            light_ball_2.setColor(light_ball_color);
            light_ball_3.setColor(selected_light_ball_color);
        }
        //2 - light 2 select to move
        if (pressed(window, GLFW_KEY_2)) {
            selected_light_source = &light_source_1;
            selected_light_ball = &light_ball_1;

            light_ball_1.setColor(selected_light_ball_color);
            light_ball_2.setColor(light_ball_color);
            light_ball_3.setColor(light_ball_color);
        }
        //3 - light 3 select to move
        if (pressed(window, GLFW_KEY_3)) {
            selected_light_source = &light_source_2;
            selected_light_ball = &light_ball_2;

            light_ball_1.setColor(light_ball_color);
            light_ball_2.setColor(selected_light_ball_color);
            light_ball_3.setColor(light_ball_color);
        }

        //movement of container and items
        if (pressed_or(window,
            { GLFW_KEY_RIGHT, GLFW_KEY_LEFT, GLFW_KEY_UP, GLFW_KEY_DOWN })){
            glm::vec2 box_shift(0.0, 0.0);

            if (pressed(window, GLFW_KEY_RIGHT)) box_shift += box_bounds.moveObject(0.05, 0.0);
            if (pressed(window, GLFW_KEY_LEFT)) box_shift += box_bounds.moveObject(-0.05, 0.0);
            if (pressed(window, GLFW_KEY_UP)) box_shift += box_bounds.moveObject(0.0, -0.05);
            if (pressed(window, GLFW_KEY_DOWN)) box_shift += box_bounds.moveObject(0.0, 0.05);
            
            shift = glm::vec3(box_shift.x, 0.0, box_shift.y);

            eye_lookat.move(1, shift);

            current_eye_position = eye_position.current();
            current_eye_lookat = eye_lookat.current();

            view = getView(current_eye_position, current_eye_lookat);
            glUseProgram(shader_program);
            glUniform3fv(u_eye_position, 1, &current_eye_position[0]);

            for (Object* o : box_items) o->translate(shift);

            shift = glm::vec3(0.0);
        }

        //rotate container left
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
            for (Object* o : box_items) o->rotate(0.0, -0.03, 0.0);
        }
        //rotate container right
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
            for (Object* o : box_items) o->rotate(0.0, 0.03, 0.0);
        }

        //debug
        if (pressed_or(window,
            { GLFW_KEY_I, GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_L })) {

            if (pressed(window, GLFW_KEY_L)) shift += glm::vec3(0.05, 0.0, 0.0);
            if (pressed(window, GLFW_KEY_J)) shift += glm::vec3(-0.05, 0.0, 0.0);
            if (pressed(window, GLFW_KEY_I)) shift += glm::vec3(0.0, 0.0, -0.05);
            if (pressed(window, GLFW_KEY_K)) shift += glm::vec3(0.0, 0.0, 0.05);

            tmp->translate(shift);
            tmp_pos += shift;

            printVec3(tmp_pos);

            shift = glm::vec3(0.0);
        }

        //update clock hands
        glm::vec3 angles = getTimeAngles();
        clock_second_hand.setRotations(0.0f, 0.0f, angles.SECONDS);
        clock_minute_hand.setRotations(0.0f, 0.0f, angles.MINUTES);
        clock_hour_hand.setRotations(0.0f, 0.0f, angles.HOURS);

        glUseProgram(shader_program);
        
        //sending light positions to shader
        if (light_1_flipped) glUniform3fv(u_light_position_1, 1, &light_source_1[0]);
        else glUniform3fv(u_light_position_1, 1, &far_away[0]);

        if (light_2_flipped) glUniform3fv(u_light_position_2, 1, &light_source_2[0]);
        else glUniform3fv(u_light_position_2, 1, &far_away[0]);

        if (light_3_flipped) glUniform3fv(u_light_position_3, 1, &light_source_3[0]);
        else glUniform3fv(u_light_position_3, 1, &far_away[0]);
        
        //draw all objects
        for (Object* o : everything) o->draw();

        //tick timers
        camera_shift_timer.tick();
        attenuation_change_timer.tick();
        light_switch_timer.tick();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}