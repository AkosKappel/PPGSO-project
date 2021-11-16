#include <iostream>
#include <utility>

#include "texture.h"

ppgso::Texture::Texture(int width, int height) : image{width, height} {
  initGL();
  update();
}

ppgso::Texture::Texture(Image&& image) : image{std::move(image)} {
  initGL();
  update();
}

ppgso::Texture::Texture(Image&& image, std::vector<ppgso::Image> facesPictures2) : image{std::move(image)}{
    facesPictures = std::move(facesPictures2);
    initGLCube();
    updateCube();
}

ppgso::Texture::~Texture() {
  glDeleteTextures(1, &texture);
}

void ppgso::Texture::initGL() {
  // Create new texture object
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // Reserve texture storage
  glTexStorage2D(GL_TEXTURE_2D, 3, GL_RGB8, image.width, image.height);

  // Set up mipmapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  // Update texture with data from image framebuffer
  update();
}

void ppgso::Texture::initGLCube() {
    // Create new texture object
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // Update texture with data from image framebuffer
    updateCube();
}

void ppgso::Texture::update() {
  bind();
  // Upload texture to GPU
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width, image.height, GL_RGB, GL_UNSIGNED_BYTE, image.getFramebuffer().data());

  // Re-generate mipmaps
  glGenerateMipmap(GL_TEXTURE_2D);
}

void ppgso::Texture::updateCube() {
    bindCube();
    // Upload texture to GPU
    for(unsigned int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, facesPictures[i].width, facesPictures[i].height, 0, GL_RGB, GL_UNSIGNED_BYTE, facesPictures[i].getFramebuffer().data());
    }
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void ppgso::Texture::bind(int id) const {
  glActiveTexture((GLenum) (GL_TEXTURE0 + id));
  glBindTexture(GL_TEXTURE_2D, texture);
}

void ppgso::Texture::bindCube(int id) const {
    glActiveTexture((GLenum) (GL_TEXTURE0 + id));
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}

GLuint ppgso::Texture::getTexture() {
  return texture;
}
