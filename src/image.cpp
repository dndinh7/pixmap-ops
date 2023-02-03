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

Image::Image() {
  this->myData= nullptr;
}

Image::Image(int width, int height): myWidth(width), myHeight(height) {
  this->myData= new unsigned char[width * height * NUM_CHANNELS];
  this->totalBytes= width * height * NUM_CHANNELS;
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

void Image::set(int width, int height, unsigned char* data) {
  assert(sizeof(data) != width * height * NUM_CHANNELS);
  this->myWidth= width;
  this->myHeight= height;
  this->totalBytes= this->myWidth * this->myHeight * NUM_CHANNELS;

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
  assert(i >= 0 && i < this->myWidth * this->myHeight);
  int idx= i * NUM_CHANNELS;

  return Pixel{ this->myData[idx + RED], this->myData[idx + GREEN], this->myData[idx + BLUE] };
}

void Image::set(int i, const Pixel& c)
{
  assert(i >= 0 && i < this->myWidth * this->myHeight);
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

Image Image::rotate90() const {
  Image result(0, 0);
  
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
  Image result(0, 0);
  return result;
}

Image Image::add(const Image& other) const {
  Image result(0, 0);
  
  return result;
}

Image Image::subtract(const Image& other) const {
  Image result(0, 0);
   
  return result;
}

Image Image::multiply(const Image& other) const {
  Image result(0, 0);
   
  return result;
}

Image Image::difference(const Image& other) const {
  Image result(0, 0);
  
  return result;
}

Image Image::lightest(const Image& other) const {
  Image result(0, 0);
  
  return result;
}

Image Image::darkest(const Image& other) const {
   Image result(0, 0);
  
   return result;
}

Image Image::gammaCorrect(float gamma) const {
  Image result(this->myWidth, this->myHeight);
   

  for (int i= 0; i < this->myWidth * this->myHeight; i++) {
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
   Image image(0, 0);
   
   return image;
}

Image Image::grayscale() const {
  Image result(this->myWidth, this->myHeight);
  Pixel pixel;
  unsigned char intensity;

  for (int i= 0; i < this->myWidth * this->myHeight; i++) {
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
  Image image(0, 0);
   
  return image;
}

void Image::fill(const Pixel& c) {
  }

void Image::inImageCheck(int row, int col) const {
  assert(row >= 0 && row < this->myHeight);
  assert(col >= 0 && col < this->myWidth);
  assert(this->myData != nullptr);
}

}  // namespace agl

