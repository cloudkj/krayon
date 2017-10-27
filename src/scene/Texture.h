#ifndef INC_TEXTURE_H
#define INC_TEXTURE_H

// TODO: FIXME
#define cimg_display 0
#include "CImg.h"

#include <string>
#include "Primitives.h"

class Texture {

public:

    Texture() {
        isLoaded = false;
    }

    ~Texture() {
        if (isLoaded) {
            delete imageData;
        }
    }

    void load(std::string filename) {
        cimg_library::CImg<float> image(filename.c_str());
        width = image.width();
        height = image.height();

        imageData = new Color[height * width];
        for (int i = 0; i < height; i++) {
          for (int j = 0; j < width; j++) {
            Color c;
            c.red = *image.data(j, i, 0, 1) / 255.0;
            c.green = *image.data(j, i, 0, 2) / 255.0;
            c.blue = *image.data(j, i, 0, 3) / 255.0;
            imageData[i * width + j] = c;
          }
        }
        
        isLoaded = true;
    }

    Color *getPixels() {
        return imageData;
    }

    int getHeight() {
        return height;
    }

    int getWidth() {
        return width;
    }

    bool isLoaded;

// establish private members as abstraction for various
// possible image formats
private:

    Color *imageData;
    int width;
    int height;
};

#endif
