

#ifndef UTILS_H
#define UTILS_H

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "Matrix.h"
#include "ShaderProgram.h"

#include <vector>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

enum GameState
{
    TITLE_SCREEN,
    GAME,
    GAME_OVER
};

namespace Utilities
{
    class Utils
    {
    public:
        Utils();
        GLuint LoadRGBTexture(const char *img_path);
        GLuint LoadRGBATexture(const char *img_path);
        void DrawText(ShaderProgram *program, GLuint fontTexture, std::string text, float size, float spacing);
    };

    class SheetSprite
    {
    private:
        float size;
        unsigned int texture_id;
        float u;
        float v;
        float width;
        float height;

    public:
        SheetSprite();
        SheetSprite(unsigned int texture_id, float u, float v, float width, float height, float size);
        void Draw(ShaderProgram *program, Matrix &model_matrix, float x, float y);
    };
}

#endif