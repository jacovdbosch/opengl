#ifndef BUFFER_H
#define BUFFER_H

enum TextureType {
    TEXTURE_RGB,
    TEXTURE_RGBA
};

unsigned int createFloatBuffer(void* data, int size);


void assignVec2AttributeLayout(int location, int stride, int offset);
void assignVec3AttributeLayout(int location, int stride, int offset);
void assignFloatAttributeLayout(int location, int length, int stride, int offset);

unsigned int createTexture(const char* path, TextureType type);

#endif

