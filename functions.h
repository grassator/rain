#ifndef RAIN_FUNCTIONS_H
#define RAIN_FUNCTIONS_H

#include "guidelines.h"
#include "lodepng.h"

namespace rain {

enum direction {
  HORIZONTAL = 0,
  VERTICAL
};

// Copies part of the source image to target starting from {fromX, fromY}
// with specified height and width.
// Doesn't check the size of source vector.
void copyImagePart(std::vector<unsigned char> &source, unsigned sourceWidth,
                   std::vector<unsigned char> &target, unsigned targetWidth,
                   unsigned width, unsigned height,
                   unsigned sourceX = 0, unsigned sourceY = 0,
                   unsigned targetX = 0, unsigned targetY = 0)
{
  // It's faster to have y on outer loop because of cache hits
  for(unsigned y = sourceY; y < sourceY + height; ++y)
  {
    for(unsigned x = sourceX; x < sourceX + width; ++x)
    {
      unsigned i = ((targetY + y - sourceY) * targetWidth + targetX + x - sourceX) * 4;
      unsigned j = (y * sourceWidth + x) * 4;
      // I know that loops exist but it's faster that way
      target[i] = source[j];
      target[++i] = source[++j];
      target[++i] = source[++j];
      target[++i] = source[++j];
    }
  }
}

// Very basic implmentation that takes any path and returns
// last path component without extension
std::string basenameWithoutExtension(std::string filename)
{
  // Getting filename from path
  char delimiters[3] = { '/', '\\', ':' };
  for(char i = 0; i < 3; ++i)
  {
    size_t pos = filename.rfind('/');
    if(pos != std::string::npos)
    {
      filename = filename.substr(pos + 1);
    }
  }

  // Removing extension
  size_t pos = filename.rfind('.');
  if(!(pos == std::string::npos || pos == 0))
  {
    filename = filename.substr(0, pos);
  }
  return filename;
}

// Writes raw image data to PNG file
void saveImage(std::vector<unsigned char> &image, unsigned width, 
               unsigned height, std::string filename)
{
  // Creating encoder
  LodePNG::Encoder encoder;
  encoder.getSettings().zlibsettings.windowSize = 2048;

  // Encode and save
  std::vector<unsigned char> ob;
  encoder.encode(ob, image.empty() ? 0 : &image[0], width, height);
  LodePNG::saveFile(ob, filename);
}

// Creates minimized version of image based on guides
// to be used as border-image in css
void saveMinimized(guidelines guides, std::vector<unsigned char> &image,
                   unsigned width, unsigned height, std::string filename)
{
  unsigned outputWidth = guides.getLeft() + guides.getRight() + 1;
  unsigned outputHeight = guides.getTop() + guides.getBottom() + 1;

  // Preparing image output
  std::vector<unsigned char> minimized;
  minimized.resize(outputWidth * outputHeight * 4);

  unsigned sizeX[3] = { guides.getLeft(), 1, guides.getRight() };
  unsigned sizeY[3] = { guides.getTop(), 1, guides.getBottom() };
  unsigned fromX[3] = { 0, guides.getLeft(), width - guides.getRight() };
  unsigned fromY[3] = { 0, guides.getTop(), height - guides.getBottom() };
  unsigned toX[3]   = { 0, guides.getLeft(), guides.getLeft() + 1 };
  unsigned toY[3]   = { 0, guides.getTop(), guides.getTop() + 1 };

  for(char x = 0; x < 3; ++x)
  {
    for(char y = 0; y < 3; ++y)
    {
      rain::copyImagePart(
        image, width,
        minimized, outputWidth,
        sizeX[x], sizeY[y],
        fromX[x], fromY[y],
        toX[x], toY[y]
      );
    }
  }

  saveImage(minimized, outputWidth, outputHeight, filename);
}

// Saves all 9 parts of an image for resizing in both directions
void saveNinePart(guidelines guides, std::vector<unsigned char> &image,
                  unsigned width, unsigned height, std::string basename)
{
  const char * names[3][3] = {
    { "north-west",  "north",  "north-east" },
    { "west",        "middle", "east"       },
    { "south-west",  "south",  "south-east" }
  };
  unsigned sizeX[3] = { guides.getLeft(), 1, guides.getRight() };
  unsigned sizeY[3] = { guides.getTop(), 1, guides.getBottom() };
  unsigned fromX[3] = { 0, guides.getLeft(), width - guides.getRight() };
  unsigned fromY[3] = { 0, guides.getTop(), height - guides.getBottom() };

  for(char x = 0; x < 3; ++x)
  {
    for(char y = 0; y < 3; ++y)
    {
      std::vector<unsigned char> output;
      output.resize(sizeX[x] * sizeY[y] * 4);
      rain::copyImagePart(
        image, width,
        output, sizeX[x],
        sizeX[x], sizeY[y],
        fromX[x], fromY[y],
        0, 0
      );

      saveImage(output, sizeX[x], sizeY[y], basename + "-" + names[y][x] + ".png");
    }
  }
}

// Saves three images either in horizontal or vertical direction
void saveThreePart(guidelines guides, std::vector<unsigned char> &image,
                   unsigned width, unsigned height, std::string basename,
                   direction dir)
{
  if(dir == HORIZONTAL)
  {
    const char * names[3] = { "left",  "middle", "right" };
    unsigned sizeX[3] = { guides.getLeft(),  1, guides.getRight() };
    unsigned fromX[3] = { 0, guides.getLeft(), width - guides.getRight() };
    
    for(char x = 0; x < 3; ++x)
    {
      std::vector<unsigned char> output;
      output.resize(sizeX[x] * height * 4);
      rain::copyImagePart(
        image, width,
        output, sizeX[x],
        sizeX[x], height,
        fromX[x], 0, 0, 0
      );
      saveImage(output, sizeX[x], height, basename + "-" + names[x] + ".png");
    }
  }
  else if(dir == VERTICAL)
  {
    const char * names[3] = { "top",  "middle", "bottom" };
    unsigned sizeY[3] = { guides.getTop(), 1, guides.getBottom() };
    unsigned fromY[3] = { 0, guides.getTop(), height - guides.getBottom() };
    
    for(char y = 0; y < 3; ++y)
    {
      std::vector<unsigned char> output;
      output.resize(width * sizeY[y] * 4);
      rain::copyImagePart(
        image, width,
        output, width,
        width, sizeY[y],
        0, fromY[y], 0, 0
      );
      saveImage(output, width, sizeY[y], basename + "-" + names[y] + ".png");
    }
  }
}

} // end namespace rain

#endif