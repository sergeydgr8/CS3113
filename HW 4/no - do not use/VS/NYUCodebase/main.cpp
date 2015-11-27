#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Assignment 4 - Sergey Smirnov", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif

    glViewport(0, 0, 1280, 720);

    ShaderProgram *program = new ShaderProgram(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
    Matrix projection_matrix;
    Matrix model_matrix;
    Matrix view_matrix;

    projection_matrix.setOrthoProjection(-8.0f, 8.0f, 4.5f, 4.5f, -1.0f, 1.0f);
    
    glUseProgram(program->programID);

	SDL_Event event;
	bool done = false;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
        glClearColor(0.4f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		SDL_GL_SwapWindow(displayWindow);

        model_matrix.identity();
        program->setModelMatrix(model_matrix);
        program->setProjectionMatrix(projection_matrix);
        program->setViewMatrix(view_matrix);


        float vertices[] = { 2.0f, -2.0f, 0.0f, 2.0f, -2.0f, -2.0f };
        glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program->positionAttribute);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(program->positionAttribute);


	}

	SDL_Quit();
	return 0;
}
