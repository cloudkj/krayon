// STL
#include <iostream>

// Krayon
#include "SceneParser.h"

#define cimg_display 0
#include "CImg.h"

int main(int argc, char *argv[]) {

    if (argc < 5) {
        std::cout << "usage: krayon sceneFilename width height outputFilename" << std::endl;
        return -1;
    }

    std::string sceneFilename(argv[1]);
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);
    std::string outputFilename(argv[4]);

    std::cout << "Processing " << sceneFilename << " at " << width << "x" << height << std::endl;

    // Parse the scene description file
    SceneParser parser(sceneFilename);
    parser.createScene();
    if (parser.scene.antiAliasing) {
        std::cout << "Anti-aliasing enabled" << std::endl;
    }

    // Trace scene
    std::vector< std::vector<Color> > pixels = parser.scene.trace(width, height);

    // Output image
    cimg_library::CImg<float> image(width, height, 1, 3);
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        float color[3];
        color[0] = pixels[i][j].red * 255;
        color[1] = pixels[i][j].green * 255;
        color[2] = pixels[i][j].blue * 255;
        image.draw_point(j, height-i, color);
      }
    }
    std::cout << "Output: " << outputFilename << std::endl;
    image.save(outputFilename.c_str());
    return 0;
}
