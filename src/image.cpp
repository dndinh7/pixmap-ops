// Copyright 2021, Aline Normoyle, alinen

#include "image.h"

#include <cassert>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <algorithm>

namespace agl {

enum Color { RED = 0, GREEN, BLUE };

Image::Image() {
   this->myData= nullptr;
}

Image::Image(int width, int height): myWidth(width), myHeight(height) {
   this->myData= new unsigned char[width * height * this->myChannels];
}

// copy constructor, not needed to free
Image::Image(const Image& orig) {
  this->myWidth= orig.width();
  this->myHeight= orig.height();
  this->myData= orig.data();

}

Image& Image::operator=(const Image& orig) {
  if (&orig == this) {
    return *this;
  }   

  this->myWidth= orig.width();
  this->myHeight= orig.height();
  
  // free the data before reassigning its value
  if (!this->myData) delete[] this->myData;
  this->myData= orig.data();
  this->myChannels= orig.channel();

  return *this;
}

Image::~Image() {
   delete[] this->myData;
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

int Image::channel() const {
  return this->myChannels;
}

void Image::set(int width, int height, unsigned char* data) {
  assert(sizeof(data) != width * height * this->myChannels);
  this->myWidth= width;
  this->myHeight= height;

  if (!this->myData) delete[] this->myData;
  this->myData= data;
}

bool Image::load(const std::string& filename, bool flip) {
  if (!this->myData) delete[] this->myData;
  const char* file= filename.c_str();
  this->myData= stbi_load(file, &myWidth, &myHeight, &myChannels, 3); // force it to have 4 channels
  
  return this->myData != nullptr;
}

bool Image::save(const std::string& filename, bool flip) const {
  const char* file= filename.c_str();
  int success= stbi_write_png(file, this->myWidth, this->myHeight, this->myChannels, this->myData, this->myWidth * this->myChannels);
  return success == 1;
}

Pixel Image::get(int row, int col) const {
  this->inImageCheck(row, col);

  int idx= (row * this->myWidth + col) * this->myChannels;
  //                  red                 green                 blue
  return Pixel{ this->myData[idx + RED], this->myData[idx + GREEN], this->myData[idx + BLUE] };
}

void Image::set(int row, int col, const Pixel& color) {
  this->inImageCheck(row, col);

  int idx= (row * this->myWidth + col) * this->myChannels;
  this->myData[idx + RED]= color.r;
  this->myData[idx + GREEN] = color.g;
  this->myData[idx + BLUE] = color.b;
}

Pixel Image::get(int i) const
{
  assert(i >= 0 && i < this->myWidth * this->myHeight);
  int idx= i * this->myChannels;

  return Pixel{ this->myData[idx + RED], this->myData[idx + GREEN], this->myData[idx + BLUE] };
}

void Image::set(int i, const Pixel& c)
{
  assert(i >= 0 && i < this->myWidth * this->myHeight);
  int idx= i * this->myChannels;
  this->myData[idx + RED]= c.r;
  this->myData[idx + GREEN]= c.g;
  this->myData[idx  + BLUE]= c.r;
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
  unsigned char* data= this->data();

  for (int i_start= 0; i_start < this->myHeight/2; i_start++) {
    int i_end= this->myHeight - 1 - i_start;

    // move the bottom row of the original image to the top row of the new image
    memmove(data + (i_start * this->myWidth), this->myData + (i_end * this->myWidth), this->myWidth);

    // move the top row of the original image to the bottom row of the new image
    memmove(data + (i_end * this->myWidth), this->myData + (i_start * this->myWidth), this->myWidth);
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

  Pixel pixel;
  for (int i= 0; i < this->myWidth * this->myHeight; i++) {
    pixel= this->get(i);
    pixel.r= std::pow(pixel.r, 1.0f/gamma);
    pixel.g= std::pow(pixel.g, 1.0f/gamma);
    pixel.b= std::pow(pixel.b, 1.0f/gamma);

    result.set(i, pixel);
  }

  return result;
}

Image Image::alphaBlend(const Image& other, float alpha) const {
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

