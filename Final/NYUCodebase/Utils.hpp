//
//  Utils.hpp
//  NYUCodebase
//

#ifndef Utils_hpp
#define Utils_hpp

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "Matrix.h"
#include "ShaderProgram.h"

#include <vector>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

enum GameState
{
    TITLE_SCREEN,
    LEVEL1,
    LEVEL2,
    LEVEL3,
    LEVEL4,
    LEVEL5,
    LEVEL6,
    LEVEL7,
    LEVEL8,
    LEVEL9,
    LEVEL10,
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
        float sprite_count_x;
        float sprite_count_y;
        float width;
        float height;
    public:
        SheetSprite();
        SheetSprite(unsigned int texture_id, float sprite_count_x, float sprite_count_y, float width, float height, float size);
        void Draw(ShaderProgram *program, Matrix &model_matrix, int index, float x, float y);
    };
}

#endif /* Utils_hpp */
