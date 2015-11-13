//
//  Utils.hpp
//  NYUCodebase
//

#ifndef Utils_h
#define Utils_h

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


enum EntityType
{
    PLAYER,
    INVADER
};

enum GameState
{
    TITLE_SCREEN,
    GAME,
    GAME_OVER
};

class Utils
{
public:
    GLuint LoadRGBTexture(const char *img_path);
    GLuint LoadRGBATexture(const char *img_path);
    void DrawText(ShaderProgram *program, GLuint fontTexture, std::string text, float size, float spacing);
    void DrawSpriteSheetSprite(ShaderProgram *program, Matrix model_matrix, int index, int spriteCountX, int spriteCountY, int textureID, float x, float y);
};

#endif
