#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
Texture::Texture() {
}
Texture::Texture(const char* texturePath, TextureType type) {
    path = texturePath;
    Gen();
    Import(texturePath, type);
}
void Texture::Import(const char* texturePath, TextureType type) {
    this->type = type;
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    std::string path = texturePath;
    unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cout << "Failed to load texture: " << path << std::endl;
        data = stbi_load("res/black.png", &width, &height, &nrChannels, 0);
    }

    unsigned int format = GL_RGBA;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

void Texture::Gen() {

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}
void Texture::Bind() {
    glBindTexture(GL_TEXTURE_2D, id);
}
