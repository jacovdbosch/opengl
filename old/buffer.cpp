#include <assert.h>

#include "buffer.h"
#include "glad/glad.h"
#include "stb_image.h"

unsigned int createFloatBuffer(void* data, int size) {
    unsigned int VBO;
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    
    return VBO;
}

void assignVec2AttributeLayout(int location, int stride, int offset = 0) {
    assignFloatAttributeLayout(location, 2, stride, offset);
}

void assignVec3AttributeLayout(int location, int stride, int offset = 0) {
    assignFloatAttributeLayout(location, 3, stride, offset);
}

void assignFloatAttributeLayout(int location, int length, int stride, int offset = 0) {
    glVertexAttribPointer(
                          location, 
                          length, 
                          GL_FLOAT, 
                          GL_FALSE, 
                          stride * sizeof(float), 
                          (void*)(offset * sizeof(float)));
    
    glEnableVertexAttribArray(location);
}

struct Texture {
    unsigned int id;
    unsigned int channel; 
}

Texture createTexture(const char* path, TextureType type, const char* shaderName, int channel) {
    assert(textureChannel >= 0 && textureChannel < 16);
    
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    
    if (data) {
        GLenum type = GL_RGB;
        
        if (type == TEXTURE_RGBA) {
            type = GL_RGBA;
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, type, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else {
        stbi_image_free(data);
        return 0;
    }
    
    
    return Texture { texture, channel };
}

void bindTexture(Texture* texture) {
    glActivateTexture(GL_TEXTURE0 + texture.channel);
    glBindTexture(GL_TEXTURE_2D, texture.id);
}

unsigned int createTexture(const char* path, TextureType type) {
    
    return texture;
}