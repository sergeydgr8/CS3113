/*

CS-UY 3113 - Intro to Game Programming
Prof. Ivan Safrin

Sergey Smirnov
sas1030 / N15186921
smirnov@nyu.edu

Assignment 1 - Simple 2D Scene

*/

#ifdef _WINDOWS
    #include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"

#ifdef _WINDOWS
    #define RESOURCE_FOLDER ""
#else
    #define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window *displayWindow;

// texture loading
GLuint LoadTexture(const char *image_path)
{
    SDL_Surface *surface = IMG_Load(image_path);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(surface);

    return textureID;
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Assignment 1 - Sergey Smirnov", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    #ifdef _WINDOWS
        glewInit();
    #endif

    // setup
    glViewport(0, 0, 640, 360);
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

    Matrix projection_matrix;
    Matrix model_matrix;
    Matrix view_matrix;

    projection_matrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);

    GLuint player_texture = LoadTexture(RESOURCE_FOLDER"pieceGreen_multi00.png");
    GLuint plane_texture = LoadTexture(RESOURCE_FOLDER"pieceGreen_border15.png");
    GLuint car_texture = LoadTexture(RESOURCE_FOLDER"pieceGreen_border13.png");

    float plane_position = 0.0f;
    float car_position = 0.0f;
    float angle = 0.0f;

    glEnable(GL_BLEND);

    float last_frame_ticks = 0.0f;

    SDL_Event event;
    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }

        float ticks = (float)SDL_GetTicks() / 1000.0f;
        float elapsed = ticks - last_frame_ticks;
        last_frame_ticks = ticks;

        angle += (3.141592653f / 256);

        car_position += elapsed;
        plane_position += (elapsed * 1.5f);

        program.setModelMatrix(model_matrix);
        program.setProjectionMatrix(projection_matrix);
        program.setViewMatrix(view_matrix);

        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(program.programID);

        // building the player
        model_matrix.identity();
        model_matrix.Rotate(angle);

        program.setModelMatrix(model_matrix);

        float player_vertices[] = { 1.25f, 1.25f, -1.25f, 1.25f, -1.25f, -1.25f, -1.25f, -1.25f, 1.25f, -1.25f, 1.25f, 1.25f };
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, player_vertices);
        glEnableVertexAttribArray(program.positionAttribute);

        float player_tex_coords[] = { 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f };
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, player_tex_coords);
        glEnableVertexAttribArray(program.texCoordAttribute);

        glBindTexture(GL_TEXTURE_2D, player_texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(program.positionAttribute);
        glDisableVertexAttribArray(program.texCoordAttribute);


        // building the car
        model_matrix.identity();
        model_matrix.Translate(car_position, 0.0f, 0.0f);

        program.setModelMatrix(model_matrix);

        float car_vertices[] = { -3.05f, 0.5f, -3.55f, 0.5f, -3.55f, 0.0f, -3.55f, 0.0f, -3.05f, 0.0f, -3.05f, 0.5f };
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, car_vertices);
        glEnableVertexAttribArray(program.positionAttribute);

        float car_tex_coords[] = { 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f };
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, car_tex_coords);
        glEnableVertexAttribArray(program.texCoordAttribute);

        glBindTexture(GL_TEXTURE_2D, car_texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(program.positionAttribute);
        glDisableVertexAttribArray(program.texCoordAttribute);


        // building the plane
        model_matrix.identity();
        model_matrix.Translate(plane_position, 0.0f, 0.0f);

        program.setModelMatrix(model_matrix);

        float plane_vertices[] = { -3.05f, 2.0f, -3.55f, 2.0f, -3.55f, 1.5f, -3.55f, 1.5f, -3.05f, 1.5f, -3.05f, 2.0f };
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, plane_vertices);
        glEnableVertexAttribArray(program.positionAttribute);

        float plane_tex_coords[] = { 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f };
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, plane_tex_coords);
        glEnableVertexAttribArray(program.texCoordAttribute);

        glBindTexture(GL_TEXTURE_2D, plane_texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(program.positionAttribute);
        glDisableVertexAttribArray(program.texCoordAttribute);

        
        SDL_GL_SwapWindow(displayWindow);
    }

    SDL_Quit();
    return 0;
}
