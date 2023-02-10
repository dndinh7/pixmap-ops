// Copyright 2021, Aline Normoyle, alinen

#include <iostream>
#include "image.h"
using namespace std;
using namespace agl;

int main(int argc, char** argv)
{
   Image image;
   if (!image.load("../images/feep.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }

   for (int i = 0; i < image.height(); i++) {
      for (int j = 0; j < image.width(); j++) {
         Pixel c = image.get(i, j);
         std::cout << "(" << (int)c.r << "," << (int)c.g << "," << (int)c.b << ") ";
      }
      std::cout << std::endl;
   }
   image.save("feep-test-save.png"); // should match original
   
   // should print 4 4
   cout << "loaded feep: " << image.width() << " " << image.height() << endl;

   // test: copy constructor
   cout << "copy constructor" << endl;
   Image copy = image; 
   copy.save("feep-test-copy.png"); // should match original and load into gimp

   // test: assignment operator
   cout << "assignment operator" << endl;
   copy = image; 
   copy.save("feep-test-assignment.png"); // should match original and load into gimp

   // should print r,g,b
   cout << "printing r,g,b at 1,1" << endl;
   Pixel pixel = image.get(1, 1);
   cout << (int) pixel.r << " " << (int) pixel.g << " " << (int) pixel.b << endl;

   // test: setting a color
   cout << "setting color" << endl;
   pixel.r = 255;
   image.set(1, 1, pixel);
   image.save("feep-test-newcolor.png");

   // test a non-trivial image
   cout << "loading earth" << endl;
   if (!image.load("../images/earth.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }


   // should print 400 400
   cout << "loaded earth: " << image.width() << " " << image.height() << endl;

   // resize
   cout << "resizing earth" << std::endl;
   Image resize = image.resize(200,300);
   resize.save("earth-200-300.png");

   // grayscale
   cout << "grayscaling earth" << std::endl;
   Image grayscale = image.grayscale(); 
   grayscale.save("earth-grayscale.png");

   // flip horizontal
   cout << "flipping earth horizontally" << std::endl;
   Image flip = image.flipHorizontal(); 
   flip.save("earth-flip.png"); 

   // sub image
   cout << "subimage earth" << std::endl;
   Image sub = image.subimage(200, 200, 100, 100); 
   sub.save("earth-subimage.png"); 

   // gamma correction
   cout << "gamma correction earth" << std::endl;
   Image gamma = image.gammaCorrect(0.6f); 
   gamma.save("earth-gamma-0.6.png"); 

   gamma = image.gammaCorrect(2.2f);
   gamma.save("earth-gamma-2.2.png"); 

   // alpha blend
   cout << "blending earth and soup" << std::endl;
   Image soup;
   soup.load("../images/soup.png");

   // replacing
   cout << "replacing a part of earth with soup" << std::endl;
   Image replaced_earth= image;
   replaced_earth.replace(soup, 0, 0);
   replaced_earth.save("earth-soup-replaced.png");

   // replacing, but second image does not fit completely
   cout << "replacing a part of earth with soup, but out of bounds" << std::endl;
   Image replaced_earth_out= image;
   replaced_earth_out.replace(soup, 300, 300);
   replaced_earth_out.save("earth-soup-replaced-out-of-bounds.png");

   // replacing, but weird coordinates
   cout << "replacing a part of earth with soup, but unequal coordinates" << std::endl;
   Image replaced_earth_unequal= image;
   replaced_earth_unequal.replace(soup, 0, 150);
   replaced_earth_unequal.save("earth-soup-replaced-unequal.png");


   int y = (int) (0.5f * (image.width() - soup.width()));
   int x = (int) (0.5f * (image.height() - soup.height()));
   Image background = image.subimage(x, y, soup.width(), soup.height());
   background.save("background-test.png");
   Image blend = background.alphaBlend(soup, 0.5f);
   image.replace(blend, x, y);
   image.save("earth-blend-0.5.png");
   
   /*
   Image yin_yang; 
   if (!yin_yang.load("../images/yin_and_yang.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }

   cout << "Rotating 90 degrees" << endl;
   Image rotated_yy= yin_yang.rotate90();
   rotated_yy.save("rotated_yin_yang.png");
   */

   Image earth;
   // test a non-trivial image
   cout << "loading earth" << endl;
   if (!earth.load("../images/earth.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }

   cout << "Rotating 90 degrees" << endl;
   Image rotated_earth= earth.rotate90();
   rotated_earth.save("rotated_earth.png");

   cout << "Bitmap earth" << endl;
   Image bitmap_earth= earth.bitmap(8);
   bitmap_earth.save("bitmap_earth.png");

   //cout << "Identity earth" << endl;
   //Image identity_earth= earth.identity();
   //identity_earth.save("identity_earth.png");

   Image squirrel;
   if (!squirrel.load("../images/squirrel.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }
   cout << "sharpening squirrel" << endl;
   Image sharpened_squirrel= squirrel.sharpen();
   sharpened_squirrel.save("sharpened_squirrel.png"); 

   cout << "box blurring squirrel" << endl;
   Image box_blur_squirrel= squirrel.boxBlur();
   box_blur_squirrel.save("box_blur_squirrel.png");

   cout << "guassian blurring squirrel" << endl;
   Image gauss_blur_squirrel= squirrel.gaussianBlur();
   gauss_blur_squirrel.save("gaussian_blurred_squirrel.png");

   cout << "ridge detection squirrel" << endl;
   Image ridge_squirrel= squirrel.ridgeDetection();
   ridge_squirrel.save("ridge_squirrel.png");

   cout << "unsharp masking squirrel" << endl;
   Image unsharp_masking_squirrel= squirrel.unsharpMasking();
   unsharp_masking_squirrel.save("unsharp_masking_squirrel.png");

   cout << "sobel operator on squirrel" << endl;
   Image sobel_squirrel= squirrel.sobel();
   sobel_squirrel.save("sobel_squirrel.png");

   Image valve;
   if (!valve.load("../images/valve.png")) {
      std::cout << "Error: Cannot load image! Exiting...\n";
      exit(0);
   }

   cout << "sobel operator on valve" << endl;
   Image sobel_valve= valve.sobel();
   sobel_valve.save("sobel_valve.png");

   cout << "invert operator on squirrel" << endl;
   Image invert_squirrel= squirrel.invert();
   invert_squirrel.save("invert_squirrel.png");


   return 0;
}

