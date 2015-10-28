//
//  Utils.cpp
//  NYUCodebase
//

#include "Utils.h"

namespace Utilities
{

    // GENERAL UTILITIES

    GLuint Utils::LoadRGBTexture(const char *img_path)
    {
        SDL_Surface* surface = IMG_Load(img_path);

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        SDL_FreeSurface(surface);

        return textureID;
    }

    GLuint Utils::LoadRGBATexture(const char *img_path)
    {
        SDL_Surface* surface = IMG_Load(img_path);

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        SDL_FreeSurface(surface);

        return textureID;
    }

    void Utils::DrawText(ShaderProgram *program, GLuint fontTexture, std::string text, float size, float spacing)
    {
        float texture_size = 1.0 / 16.0f;
        std::vector<float> vertexData;
        std::vector<float> texCoordData;
        for (int i = 0; i < text.size(); i++) {
            float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
            float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
            vertexData.insert(vertexData.end(), {
                ((size + spacing) * i) + (-0.5f * size), 0.5f * size,
                ((size + spacing) * i) + (-0.5f * size), -0.5f * size,
                ((size + spacing) * i) + (0.5f * size), 0.5f * size,
                ((size + spacing) * i) + (0.5f * size), -0.5f * size,
                ((size + spacing) * i) + (0.5f * size), 0.5f * size,
                ((size + spacing) * i) + (-0.5f * size), -0.5f * size,
            });
            texCoordData.insert(texCoordData.end(), {
                texture_x, texture_y,
                texture_x, texture_y + texture_size,
                texture_x + texture_size, texture_y,
                texture_x + texture_size, texture_y + texture_size,
                texture_x + texture_size, texture_y,
                texture_x, texture_y + texture_size,
            });
        }
        glUseProgram(program->programID);
        glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
        glEnableVertexAttribArray(program->positionAttribute);
        glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
        glEnableVertexAttribArray(program->texCoordAttribute);
        glBindTexture(GL_TEXTURE_2D, fontTexture);
        glDrawArrays(GL_TRIANGLES, 0, text.size() * 6.0f);
        glDisableVertexAttribArray(program->positionAttribute);
        glDisableVertexAttribArray(program->texCoordAttribute);
    }

    // SHEETSPRITE

    SheetSprite::SheetSprite() {}

    SheetSprite::SheetSprite(unsigned int texture_id, float u, float v, float width, float height, float size) :
        texture_id(texture_id), u(u), v(v), width(width), height(height), size(size) {}

    void SheetSprite::Draw(ShaderProgram *program, Matrix &model_matrix, float x = 0.0f, float y = 0.0f)
    {
        glBindTexture(GL_TEXTURE_2D, texture_id);

        GLfloat tex_coords[] =
        {
            u, (v + height),
            (u + width), v,
            u, v,
            (u + width), v,
            u, (v + width),
            (u + width), (v + height)
        };

        float aspect = width / height;
        float vertices[] =
        {
            (-0.5f * size * aspect), (-0.5f * size),
            (0.5f * size * aspect), (0.5f * size),
            (-0.5f * size * aspect), (0.5f * size),
            (0.5f * size * aspect), (0.5f * size),
            (-0.5f * size * aspect), (-0.5f * size),
            (0.5f * size * aspect), (-0.5f * size)
        };

        // draw our arrays
        glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program->positionAttribute);

        glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, tex_coords);
        glEnableVertexAttribArray(program->texCoordAttribute);

        model_matrix.identity();
        model_matrix.Translate(x, y, 0.0f);
        program->setModelMatrix(model_matrix);

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(program->positionAttribute);
        glDisableVertexAttribArray(program->texCoordAttribute);
    }
}