#pragma once
#include <glut.h>
#include "SOIL.h"
#include <random>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct Texture
{
	GLuint texture;
	const char* textureName;
};

struct Model
{
    std::vector<float>* verts = new std::vector<float>;
    std::vector<float>* normals = new std::vector<float>;
    std::vector<float>* texCoords = new std::vector<float>;
    std::vector<int>* indeces = new std::vector<int>;
};

class Utility
{
public:
    Utility(std::vector<float>* vrts, std::vector<float>* nrmls, std::vector<float>* texCrds, std::vector<int>* inds) :
        verts(vrts),
        normals(nrmls),
        texCoords(texCrds),
        indeces(inds)
    {};
	Texture* LoadTexture(const char* filename, const char* textureName) 
	{
		Texture* texture = new Texture;
		textures.push_back(texture);
		textures.back()->textureName = textureName;
		textures.back()->texture = SOIL_load_OGL_texture
		(
			filename,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
		return textures.back();
	};

    void AddTexture(GLuint textureToAdd, const char* textureName)
    {
        Texture* texture = new Texture;
        textures.push_back(texture);

        textures.back()->textureName = textureName;
        textures.back()->texture = textureToAdd;
    }

	GLuint GetTextureByName(const char* textureName) 
	{
		for (int i = 0; i < textures.size(); i++)
		{
			if (textures[i]->textureName == textureName) return textures[i]->texture;
		}
		return 0;
	}

    #define TEX_SIZE 256
    #define TILE_COUNT 8
    #define TILE_SIZE (TEX_SIZE / TILE_COUNT)
    #define BORDER_SIZE 2
    #define MIN_TRANSPARENCY 100  // Alpha values will be randomized between this and 255
    void generatePurpleTileTexture(GLuint* texture) {
        srand(time(NULL));

        GLubyte textureData[TEX_SIZE * TEX_SIZE * 4];

        // Store one color and alpha per tile
        unsigned char tileColors[TILE_COUNT][TILE_COUNT][4];

        for (int ty = 0; ty < TILE_COUNT; ++ty) {
            for (int tx = 0; tx < TILE_COUNT; ++tx) {
                unsigned char purple = 100 + rand() % 156;
                unsigned char red = purple / 2;
                unsigned char alpha = MIN_TRANSPARENCY + rand() % (256 - MIN_TRANSPARENCY);

                tileColors[ty][tx][0] = red;
                tileColors[ty][tx][1] = 0;
                tileColors[ty][tx][2] = purple;
                tileColors[ty][tx][3] = alpha;
            }
        }

        for (int y = 0; y < TEX_SIZE; ++y) {
            for (int x = 0; x < TEX_SIZE; ++x) {
                int tx = x / TILE_SIZE;
                int ty = y / TILE_SIZE;
                int localX = x % TILE_SIZE;
                int localY = y % TILE_SIZE;

                if (localX < BORDER_SIZE || localX >= TILE_SIZE - BORDER_SIZE ||
                    localY < BORDER_SIZE || localY >= TILE_SIZE - BORDER_SIZE) {
                    // Black border
                    textureData[(y * TEX_SIZE + x) * 4 + 0] = 0;
                    textureData[(y * TEX_SIZE + x) * 4 + 1] = 0;
                    textureData[(y * TEX_SIZE + x) * 4 + 2] = 0;
                    textureData[(y * TEX_SIZE + x) * 4 + 3] = 255;
                }
                else {
                    // Fill with the tile's purple color and alpha
                    for (int c = 0; c < 4; ++c)
                        textureData[(y * TEX_SIZE + x) * 4 + c] = tileColors[ty][tx][c];
                }
            }
        }

        glGenTextures(1, texture);
        glBindTexture(GL_TEXTURE_2D, *texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_SIZE, TEX_SIZE, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    Model* loadOBJ(const char* filename) {
        Model* model = new Model;
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Could not open file: " << filename << std::endl;
            return new Model;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream s(line);
            std::string prefix;
            s >> prefix;

            if (prefix == "v") {
                float x, y, z;
                s >> x >> y >> z;
                model->verts->push_back(x);
                model->verts->push_back(y);
                model->verts->push_back(z);
            } else if (prefix == "vt") {
                float x, y;
                s >> x >> y;
                model->texCoords->push_back(x);
                model->texCoords->push_back(y);
            } else if (prefix == "vn") {
                float x, y, z;
                s >> x >> y >> z;
                model->normals->push_back(x);
                model->normals->push_back(y);
                model->normals->push_back(z);
            }
            else  if (prefix == "f") {
                for (int i = 0; i < 3; ++i) {
                    float v, t, n;
                    char slash;
                    s >> v >> slash >> t >> slash >> n;
                    model->indeces->push_back(v);
                    model->indeces->push_back(t);
                    model->indeces->push_back(n);
                }
            }
        }

        file.close();
        return model;
    }

    void SwapArrayPointers(Model* model)
    {
        verts->swap(*model->verts);
        normals->swap(*model->normals);
        texCoords->swap(*model->texCoords);
        indeces->swap(*model->indeces);
    }


private:
	std::vector<Texture*> textures;

    std::vector<float>* verts;
    std::vector<float>* normals;
    std::vector<float>* texCoords;
    std::vector<int>* indeces;
};

