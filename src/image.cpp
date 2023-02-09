// Copyright 2021, Aline Normoyle, alinen

/**
 * This program defines the methods of the Image class
 * which allows for loading, saving images. It also
 * supports many image manipulation methods. This
 * program loads each image with three channels.
 * 
 * Current Supported Image Manipulation Methods:
 * - Flip Horizontal
 * - Replace
 * - SubImage
 * - Resize
 * - Greyscale
 * - Gamma Correction
 * - Alpha Blend
 * 
 * @author David Dinh
 * @version Feb 2, 2023
 * 
*/

#include "image.h"
#include <cassert>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <algorithm>
#include <cstring>

#define NUM_CHANNELS 3 // assumes that there will only be three components in an image

namespace agl {

enum Color { RED = 0, GREEN, BLUE };

// Function to clamp value
int clamp(int value, int low, int hi) {
  return std::min(std::max(value, low), hi);
}

Image::Image() {
  this->myData= nullptr;
}

Image::Image(int width, int height): myWidth(width), myHeight(height) {
  this->myData= new unsigned char[width * height * NUM_CHANNELS];
  this->totalBytes= width * height * NUM_CHANNELS;
  this->totalPixels= width * height;
}


Image::Image(const Image& orig) {
  this->myData= nullptr;
  this->set(orig.width(), orig.height(), orig.data());
}

Image& Image::operator=(const Image& orig) {
  if (&orig == this) {
    return *this;
  }
  this->set(orig.width(), orig.height(), orig.data());

  return *this;
}

Image::~Image() {
  if (this->myData != nullptr) delete[] this->myData;
}

int Image::width() const {
  return this->myWidth;
}

int Image::height() const {
  return this->myHeight;
}

unsigned char* Image::data() const {
  return this->myData;
}

int Image::bytes() const {
  return this->totalBytes;
}

int Image::pixelCount() const {
  return this->totalPixels;
}

void Image::set(int width, int height, unsigned char* data) {
  assert(sizeof(data) != width * height * NUM_CHANNELS);
  this->myWidth= width;
  this->myHeight= height;
  this->totalBytes= this->myWidth * this->myHeight * NUM_CHANNELS;
  this->totalPixels= this->myWidth * this->myHeight;

  // Assures that we clean up the data we are replacing to avoid leaks
  if (this->myData != nullptr) {
    delete[] this->myData;
    this->myData= nullptr;
  }
  this->myData= new unsigned char[this->totalBytes];
  std::memcpy(this->myData, data, this->totalBytes);
}

// Assumes that flip is false for now
bool Image::load(const std::string& filename, bool flip) {
  const char* file= filename.c_str();
  unsigned char* data= stbi_load(file, &this->myWidth, &myHeight, nullptr, 3); // force it to have 4 channels

  bool success= data != nullptr;

  this->set(this->myWidth, this->myHeight, data);

  stbi_image_free(data);

  return success;
}

// Assumes that flip is false for now
bool Image::save(const std::string& filename, bool flip) const {
  const char* file= filename.c_str();
  int success= stbi_write_png(file, this->myWidth, this->myHeight, NUM_CHANNELS, 
    this->myData, this->myWidth * NUM_CHANNELS);

  return success == 1;
}

Pixel Image::get(int row, int col) const {
  this->inImageCheck(row, col);

  int idx= (row * this->myWidth + col) * NUM_CHANNELS;
  //                  red                 green                 blue
  return Pixel{ this->myData[idx + RED], this->myData[idx + GREEN], this->myData[idx + BLUE] };
}

void Image::set(int row, int col, const Pixel& color) {
  this->inImageCheck(row, col);

  int idx= (row * this->myWidth + col) * NUM_CHANNELS;

  this->myData[idx + RED]= color.r;
  this->myData[idx + GREEN] = color.g;
  this->myData[idx + BLUE] = color.b;
}

Pixel Image::get(int i) const
{
  assert(i >= 0 && i < this->totalPixels);
  int idx= i * NUM_CHANNELS;

  return Pixel{ this->myData[idx + RED], this->myData[idx + GREEN], this->myData[idx + BLUE] };
}

void Image::set(int i, const Pixel& c)
{
  assert(i >= 0 && i < this->totalPixels);
  int idx= i * NUM_CHANNELS;
  this->myData[idx + RED]= c.r;
  this->myData[idx + GREEN]= c.g;
  this->myData[idx + BLUE]= c.b;
}

Image Image::resize(int w, int h) const {
  Image result(w, h);
  int i_1;
  int j_1;
  for (int i_2= 0; i_2 < h; i_2++) {
    for (int j_2= 0; j_2 < w; j_2++) {
      float rowRatio_2= (float) i_2 / (float) (h-1);
      float colRatio_2= (float) j_2 / (float) (w-1);

      i_1= rowRatio_2 * (this->myHeight - 1);
      j_1= colRatio_2 * (this->myWidth - 1);
      
      result.set(i_2, j_2, this->get(i_1, j_1));

    }
  }
  return result;
}

Image Image::flipHorizontal() const {
  Image result(this->myWidth, this->myHeight);

  for (int i_start= 0; i_start < this->myHeight; i_start++) {

    // corresponding index of the pixel on the other side of the middle line
    int i_end= this->myHeight - 1 - i_start;
    for (int j= 0; j < this->myWidth; j++) {
      
      result.set(i_start, j, this->get(i_end, j));
    }
  }

  return result;

}

Image Image::flipVertical() const {
  Image result(0, 0);
  return result;
}

Image Image::flipPositiveDiagonal() const {
  // switch dimensions to actually flip them correctly by swapping i,j -> j,i
  Image result(this->myHeight, this->myWidth);

  // invariant is that we only traverse the bottom triangle
  for (int i= 0; i < this->myHeight; i++) {
    for (int j= 0; j < this->myWidth; j++) {
      Pixel curPixel= this->get(i, j);
      result.set(j, i, curPixel); // place the pixel in mirrored position
    }
  }

  return result;
}

Image Image::rotate90() const {
  Image flippedHorizontally= this->flipHorizontal();
  Image result= flippedHorizontally.flipPositiveDiagonal();
  
  return result;
}

Image Image::subimage(int startx, int starty, int w, int h) const {
  // assures that the sub image is actually a subimage
  assert(startx + w < this->myWidth && starty + h < this->myHeight);
  Image sub(w, h);

  for (int i= 0; i < h; i++) {
    for (int j= 0; j < w; j++) {
      sub.set(i, j, this->get(starty + i, startx + j));
    }
  }

  return sub;
}

void Image::replace(const Image& image, int startx, int starty) {

  // loop condition protects against index out of bounds error
  for (int i= 0; i < image.height() && starty + i < this->myHeight; i++) {
    for (int j= 0; j < image.width() && startx + j < this->myWidth; j++) {

      this->set(startx + i, starty + j, image.get(i, j));
    }
  }  
}

Image Image::swirl() const {
  Image result(this->myWidth, this->myHeight);

  for (int i= 0; i < this->myHeight; i++) {
    for (int j= 0; j < this->myWidth; j++) {
      Pixel pixel= this->get(i, j);
      unsigned char tempRed= pixel.r;
      pixel.r= pixel.b;
      pixel.b= pixel.g;
      pixel.g= tempRed;

      result.set(i, j, pixel);
    }
  }
  return result;
}

Image Image::add(const Image& other) const {
  Image result(this->myWidth, this->myHeight);

  for (int i= 0; i < this->totalPixels; i++) {
    Pixel pixel1= this->get(i);
    Pixel pixel2= other.get(i);
    Pixel resultPixel;
    resultPixel.r= std::min(pixel1.r + pixel2.r, 255);
    resultPixel.g= std::min(pixel1.g + pixel2.g, 255);
    resultPixel.b= std::min(pixel1.b + pixel2.b, 255);
    result.set(i, resultPixel);
  }
  
  return result;
}

Image Image::subtract(const Image& other) const {
  Image result(this->myWidth, this->myHeight);

  for (int i= 0; i < this->totalPixels; i++) {
    Pixel pixel1= this->get(i);
    Pixel pixel2= other.get(i);
    Pixel resultPixel;
    resultPixel.r= std::max(pixel1.r - pixel2.r, 0);
    resultPixel.g= std::max(pixel1.g - pixel2.g, 0);
    resultPixel.b= std::max(pixel1.b - pixel2.b, 0);
    result.set(i, resultPixel);
  }
   
  return result;
}

Image Image::multiply(const Image& other) const {
  Image result(this->myWidth, this->myHeight);

  for (int i= 0; i < this->totalPixels; i++) {
    Pixel pixel1= this->get(i);
    Pixel pixel2= other.get(i);
    Pixel resultPixel;
    resultPixel.r= std::min(pixel1.r * pixel2.r, 255);
    resultPixel.g= std::min(pixel1.g * pixel2.g, 255);
    resultPixel.b= std::min(pixel1.b * pixel2.b, 255);
    result.set(i, resultPixel);
  }
   
  return result;
}

Image Image::difference(const Image& other) const {
  Image result(this->myWidth, this->myHeight);

  for (int i= 0; i < this->totalPixels; i++) {
    Pixel pixel1= this->get(i);
    Pixel pixel2= other.get(i);
    Pixel resultPixel;
    resultPixel.r= std::abs(pixel1.r - pixel2.r);
    resultPixel.g= std::abs(pixel1.g - pixel2.g);
    resultPixel.b= std::abs(pixel1.b - pixel2.b);
    result.set(i, resultPixel);
  }
  
  return result;
}

Image Image::lightest(const Image& other) const {
  Image result(this->myWidth, this->myHeight);

  for (int i= 0; i < this->totalPixels; i++) {
    Pixel pixel1= this->get(i);
    Pixel pixel2= other.get(i);
    Pixel resultPixel;
    resultPixel.r= std::max(pixel1.r, pixel2.r);
    resultPixel.g= std::max(pixel1.g, pixel2.g);
    resultPixel.b= std::max(pixel1.b, pixel2.b);
    result.set(i, resultPixel);
  }
  
  return result;
}

Image Image::darkest(const Image& other) const {
  Image result(this->myWidth, this->myHeight);

  for (int i= 0; i < this->totalPixels; i++) {
    Pixel pixel1= this->get(i);
    Pixel pixel2= other.get(i);
    Pixel resultPixel;
    resultPixel.r= std::min(pixel1.r, pixel2.r);
    resultPixel.g= std::min(pixel1.g, pixel2.g);
    resultPixel.b= std::min(pixel1.b, pixel2.b);
    result.set(i, resultPixel);
  }
  
  return result;
}

Image Image::gammaCorrect(float gamma) const {
  Image result(this->myWidth, this->myHeight);
   

  for (int i= 0; i < this->totalPixels; i++) {
    Pixel pixel= this->get(i);

    pixel.r= std::pow(pixel.r/255.0f, 1.0f/gamma) * 255;
    pixel.g= std::pow(pixel.g/255.0f, 1.0f/gamma) * 255;
    pixel.b= std::pow(pixel.b/255.0f, 1.0f/gamma) * 255;

    result.set(i, pixel);
  }
  
  return result;
}

Image Image::alphaBlend(const Image& other, float alpha) const {
  // assumes that images have the same dimensions
  assert(this->myWidth == other.width() && this->myHeight == other.height());
  Image result(this->myWidth, this->myHeight);

  for (int i= 0; i < this->myHeight; i++) {
    for (int j= 0; j < this->myWidth; j++) {
      Pixel blendedPixel= Pixel { 0, 0, 0 };
      Pixel pixel1= this->get(i, j);
      Pixel pixel2= other.get(i, j);

      blendedPixel.r= (float) pixel1.r * (1 - alpha) + (float) pixel2.r * alpha;
      blendedPixel.g= (float) pixel1.g * (1 - alpha) + (float) pixel2.g * alpha;
      blendedPixel.b= (float) pixel1.b * (1 - alpha) + (float) pixel2.b * alpha;

      result.set(i, j, blendedPixel);
    } 
  }

  return result;
}

Image Image::invert() const {
  Image image(this->myWidth, this->myHeight);

  for (int i= 0; i < this->totalPixels; i++) {
    Pixel pixel= this->get(i);

    pixel.r= 255 - pixel.r;
    pixel.g= 255 - pixel.g;
    pixel.b= 255 - pixel.b;    

    image.set(i, pixel);

  }

  return image;
}

Image Image::grayscale() const {
  Image result(this->myWidth, this->myHeight);
  Pixel pixel;
  unsigned char intensity;

  for (int i= 0; i < this->totalPixels; i++) {
    pixel= this->get(i);

    // Hardcoded values to make the greyscale intensity to look pleasing to human eye
    intensity= (float) pixel.r * 0.3f + (float) pixel.g * 0.59f + (float) pixel.b * 0.11f;
    
    pixel.r= intensity;
    pixel.g= intensity;
    pixel.b= intensity;

    result.set(i, pixel);
  }

  return result;
}

Image Image::colorJitter(int size) const {
  Image image(0, 0);
 
  return image;
}

Image Image::bitmap(int size) const {
  Image image(this->myWidth, this->myHeight);

   
  return image;
}

void Image::fill(const Pixel& c) {

}

Image Image::sharpen() const {
  int kernel[] {0, -1, 0,
               -1, 5, -1,
                0, -1, 0};
  
  Image result= this->convolute(kernel, 1, 3);

  return result;

}

Image Image::identity() const {
  int kernel[] {0, 0, 0,
                0, 1, 0,
                0, 0, 0};

  Image result= this->convolute(kernel, 1, 3);
  return result;
}

Image Image::gaussianBlur() const {
  int kernel[] {1, 2, 1,
                2, 4, 2,
                1, 2, 1};
  float scale= 1.0f/16.0f;

  Image result= this->convolute(kernel, scale, 3);

  return result;
}

Image Image::boxBlur() const {
  int kernel[] {1, 1, 1,
                1, 1, 1,
                1, 1, 1};
  float scale= 1.0f/9.0f;

  Image result= this->convolute(kernel, scale, 3);

  return result;
}

Image Image::ridgeDetection() const {
  //int kernel[] {-1, -1, -1, -1, 8, -1, -1, -1, -1};
  int kernel[] {0, -1, 0,
               -1, 4, -1,
                0, -1, 0}; 
  Image result= this->convolute(kernel, 1, 3);

  return result;
}

Image Image::unsharpMasking() const {
  int kernel[] {1, 4, 6, 4, 1,
                4, 16, 24, 16, 4,
                6, 24, -476, 24, 6,
                4, 16, 24, 16, 4,
                1, 4, 6, 4, 1};
  float scale= -1/256.0f;

  Image result= this->convolute(kernel, scale, 5);
  return result;
}

Image Image::sobel() const {
  int kernel1[] {-1, 0, 1,
                  -2, 0, 2,
                  -1, 0, 1};
  int kernel2[] {1, 2, 1,
                 0, 0, 0,
                 -1, -2, -1};
      
  Image G1= this->convolute(kernel1, 1, 3);
  Image G2= this->convolute(kernel2, 1, 3);

  Image result(this->myWidth, this->myHeight);

  for (int i= 0; i < this->totalPixels; i++) {
    Pixel pixel1= G1.get(i);
    Pixel pixel2= G2.get(i);

    unsigned char r= clamp(std::sqrt((float) pixel1.r * (float) pixel1.r + (float) pixel2.r * (float) pixel2.r), 0, 255);
    unsigned char g= clamp(std::sqrt((float) pixel1.g * (float) pixel1.g + (float) pixel2.g * (float) pixel2.g), 0, 255);
    unsigned char b= clamp(std::sqrt((float) pixel1.b * (float) pixel1.b + (float) pixel2.b * (float) pixel2.b), 0, 255);

    result.set(i, Pixel{r, g, b});
  }
  
  return result;
}

void Image::inImageCheck(int row, int col) const {
  assert(row >= 0 && row < this->myHeight);
  assert(col >= 0 && col < this->myWidth);
  assert(this->myData != nullptr);
}


Image Image::convolute(int kernel[], float kernelScale, int sideLength) const {
  Image result(this->myWidth, this->myHeight);

  for (int i= 0; i < this->myHeight; i++) {
    for (int j= 0; j < this->myWidth; j++) {
      Pixel accumulator= {0, 0, 0};
      float accumulatorRed= 0;
      float accumulatorGreen= 0;
      float accumulatorBlue= 0;

      // convolute operator
      for (int k_i= 0; k_i < sideLength; k_i++) {
        for (int k_j= 0; k_j < sideLength; k_j++) {
          int i_offset= k_i - 1; // so we get -1, 0, or 1
          int j_offset= k_j - 1;

          int pixel_i= clamp(i + i_offset, 0, this->myHeight - 1);
          int pixel_j= clamp(j + j_offset, 0, this->myWidth - 1);


          Pixel pixel= this->get(pixel_i, pixel_j);

          // convolution operator requires us to multiply the index
          // mirrored to the pixel aka (m-i-1, n-j-1)
          int kernel_idx= (sideLength - 1 - k_i) * sideLength + (sideLength - 1 - k_j);
          accumulatorRed += kernelScale * kernel[kernel_idx] * pixel.r;
          accumulatorGreen += kernelScale * kernel[kernel_idx] * pixel.g;
          accumulatorBlue += kernelScale * kernel[kernel_idx] * pixel.b;
        }
      }

      accumulator.r= clamp(accumulatorRed, 0, 255);
      accumulator.g= clamp(accumulatorGreen, 0, 255);
      accumulator.b= clamp(accumulatorBlue, 0, 255);
      
      result.set(i, j, accumulator);

    }
  }

  return result;
}

}  // namespace agl

