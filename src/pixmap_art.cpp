#include <iostream>
#include "image.h"
#include <vector>
#include <string>
using namespace std;
using namespace agl;

/**
 * This program creates a 5x4 grid of the same image,
 * then proceedingly replaces each of the cells with 
 * the same image but manipulated using the methods
 * implemented in image.cpp. It showcases a plethora
 * of methods: 
 * - GridCopy
 * - Gaussian Blur
 * - Box Blur
 * - Unsharp Masking
 * - Sobel Operator
 * - Greyscale
 * - Invert
 * - Bitmap
 * - Ridge Detection
 * - Sharpen
 * - Swirl
 * - Flip/Rotation
 * - Glow
 * - Increase RGB value by already existing ones
 * - Decrease RGB value by already existing ones
 * - Color Jitter
*/

int main(int argc, char** argv)
{
  vector<Image> images;
  vector<string> names;
  Image heimer;
  if (!heimer.load("../images/heimerdinger.png")) {
    std::cout << "ERROR: Cannot load image! Exiting...\n";
    exit(0);
  }

  Image squirrel;
  if (!squirrel.load("../images/squirrel.png")) {
    std::cout << "ERROR: Cannot load image! Exiting...\n";
    exit(0);
  }

  Image psyduck;
  if (!psyduck.load("../images/psyduck.png")) {
    std::cout << "ERROR: Cannot load image! Exiting...\n";
    exit(0);
  }

  Image earth;
  if (!earth.load("../images/earth.png")) {
    std::cout << "ERROR: Cannot load image! Exiting...\n";
    exit(0);
  }

  Image scenery;
  if (!scenery.load("../images/scenery.png")) {
    std::cout << "ERROR: Cannot load image! Exiting...\n";
    exit(0);
  }

  Image jinx;
  if (!jinx.load("../images/jinx.png")) {
    std::cout << "ERROR: Cannot load image! Exiting...\n";
    exit(0);
  }

  images.push_back(heimer);
  names.push_back("heimer");

  images.push_back(squirrel);
  names.push_back("squirrel");

  images.push_back(psyduck);
  names.push_back("psyduck");

  images.push_back(earth);
  names.push_back("earth");

  images.push_back(scenery);
  names.push_back("scenery");

  images.push_back(jinx);
  names.push_back("jinx");

  for (int i= 0; i < images.size(); i++) {
    Image cur_image= images[i];
    string cur_name= names[i];

    // dimensions for each of the subsequent images
    int width= cur_image.width();
    int height= cur_image.height();

    
    cout << "5x4 grid copy " << cur_name << endl;
    Image grid_image= cur_image.gridCopy(5, 4);

    cout << "gaussian blur " << cur_name << endl;
    Image gaussBlur= cur_image.gaussianBlur();
    grid_image.replace(gaussBlur, width, 0);

    cout << "box blur " << cur_name << endl;
    Image boxBlur= cur_image.boxBlur();
    grid_image.replace(boxBlur, 2*width, 0);

    cout << "unsharp mask " << cur_name << endl;
    Image unsharpMask= cur_image.unsharpMasking();
    grid_image.replace(unsharpMask, 3*width, 0);

    cout << "sobel " << cur_name << endl;
    Image sobel= cur_image.sobel();
    grid_image.replace(sobel, 0, height);

    cout << "greyscale " << cur_name << endl;
    Image greyscale= cur_image.grayscale();
    grid_image.replace(greyscale, width, height);

    cout << "invert " << cur_name << endl;
    Image invert= cur_image.invert();
    grid_image.replace(invert, 2*width, height);
    
    cout << "bitmap " << cur_name << endl;
    Image bitmap= cur_image.bitmap(8);
    grid_image.replace(bitmap, 3*width, height);
    
    cout << "ridge detection " << cur_name << endl;
    Image ridge= cur_image.ridgeDetection();
    grid_image.replace(ridge, 0, 2*height);

    cout << "sharpen " << cur_name << endl;
    Image sharpen= cur_image.sharpen();
    grid_image.replace(sharpen, width, 2*height);

    cout << "swirl " << cur_name << endl;
    Image swirl= cur_image.swirl();
    grid_image.replace(swirl, 2*width, 2*height);

    cout << "flipped " << cur_name << endl;
    Image flipped= cur_image.rotate90().rotate90();
    grid_image.replace(flipped, 3*width, 2*height);

    // got a pixel threshold for white using this reference
    // https://tjosh.medium.com/finding-lane-lines-with-colour-thresholds-beb542e0d839
    cout << "glow " << cur_name << endl;
    Image glow= cur_image.glow(Pixel{100, 100, 200}, Pixel{255, 255, 255});
    grid_image.replace(glow, 0, 3*height);

    cout << "redify " << cur_name << endl;
    Image redify= cur_image.extractRed().boxBlur().add(cur_image);
    grid_image.replace(redify, width, 3*height);

    cout << "greenify " << cur_name << endl;
    Image greenify= cur_image.extractGreen().boxBlur().add(cur_image);
    grid_image.replace(greenify, 2*width, 3*height);

    cout << "blueify " << cur_name << endl;
    Image blueify= cur_image.extractBlue().boxBlur().add(cur_image);
    grid_image.replace(blueify, 3*width, 3*height);

    cout << "redless " << cur_name << endl;
    Image redless= cur_image.subtract(cur_image.extractRed().boxBlur());
    grid_image.replace(redless, 0, 4*height);

    cout << "greenless " << cur_name << endl;
    Image greenless= cur_image.subtract(cur_image.extractGreen().boxBlur());
    grid_image.replace(greenless, width, 4*height);

    cout << "blueless " << cur_name << endl;
    Image blueless= cur_image.subtract(cur_image.extractBlue().boxBlur());
    grid_image.replace(blueless, 2*width, 4*height);

    cout << "jitter " << cur_name << endl;
    Image jitter= cur_image.colorJitter(20);
    grid_image.replace(jitter, 3*width, 4*height);

    grid_image.save(cur_name + ".png");
  }

  Image psyduck_extra= psyduck;
  std::cout << "ghosting psyduck" << endl;
  psyduck_extra.replaceAlpha(psyduck, 0.1f, 10, 0);
  psyduck_extra.replaceAlpha(psyduck, 0.1f, 20, 0);
  psyduck_extra.replaceAlpha(psyduck, 0.1f, 30, 0);
  psyduck_extra.save("psyduck_ghost.png");
}

