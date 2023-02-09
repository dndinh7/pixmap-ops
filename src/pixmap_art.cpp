#include <iostream>
#include "image.h"
using namespace std;
using namespace agl;

int main(int argc, char** argv)
{
   // todo: make at least one artwork!
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


   return 0;
}

