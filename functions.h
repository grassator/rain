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

void getResizedWithOffset(guidelines guides, std::vector<unsigned char> &image,
                          unsigned newWidth, unsigned newHeight,
                          unsigned width, unsigned height,
                          std::vector<unsigned char> &output)
{
  // Forcing new size to be appropriate
  unsigned minWidth = guides.getLeft() + guides.getRight() + 1;
  unsigned minHeight = guides.getTop() + guides.getBottom() + 1;
  newWidth = newWidth < minWidth ? minWidth : newWidth;
  newHeight = newHeight < minHeight ? minHeight : newHeight;

  unsigned repeatX = newWidth - guides.getLeft() - guides.getRight();
  unsigned repeatY = newHeight - guides.getTop() - guides.getBottom();

  // Preparing image output
  output.resize(newWidth * newHeight * 4);

  unsigned sizeX[3] = { guides.getLeft(), 1, guides.getRight() };
  unsigned sizeY[3] = { guides.getTop(), 1, guides.getBottom() };
  unsigned fromX[3] = { 0, guides.getLeft(), width - guides.getRight() };
  unsigned fromY[3] = { 0, guides.getTop(), height - guides.getBottom() };
  unsigned toX[3]   = { 0, guides.getLeft(), guides.getLeft() + repeatX };
  unsigned toY[3]   = { 0, guides.getTop(), guides.getTop() + repeatY };

  for(char x = 0; x < 3; ++x)
  {
    // For middle part there might be more than one iteration
    unsigned countX = (x == 1) ? repeatX - 1 : 0;

    for (unsigned rx = 0; rx <= countX; ++rx)
    {
      for(char y = 0; y < 3; ++y)
      {
        // For middle part there might be more than one iteration
        unsigned countY = (y == 1) ? repeatY - 1 : 0;

        for (unsigned ry = 0; ry <= countY; ++ry)
        {
          rain::copyImagePart(
            image, width,
            output, newWidth,
            sizeX[x], sizeY[y],
            fromX[x], fromY[y],
            toX[x] + rx, toY[y] + ry
          );
        }
      }
    }
  }
}

// Resizes image based on guides doesn't check sizes
void saveResized(guidelines guides, std::vector<unsigned char> &image,
                 unsigned newWidth, unsigned newHeight,
                 unsigned width, unsigned height, std::string filename)
{
  std::vector<unsigned char> resized;
  getResizedWithOffset(
   guides, image, newWidth, newHeight, width, height, resized
  );
  saveImage(resized, newWidth, newHeight, filename);
}

// Creates minimized version of image based on guides
// to be used as border-image in css
void saveMinimized(guidelines guides, std::vector<unsigned char> &image,
                   unsigned width, unsigned height, std::string filename)
{
  unsigned newWidth = guides.getLeft() + guides.getRight() + 1;
  unsigned newHeight = guides.getTop() + guides.getBottom() + 1;
  saveResized(guides, image, newWidth, newHeight, width, height, filename);
}

void fillPixel(std::vector<unsigned char> &image, unsigned width,
               unsigned x,  unsigned y, unsigned char r = 0,
               unsigned char g = 0, unsigned char b = 0, unsigned char a = 0)
{
  unsigned offset = (x + y * width)*4;
  image[offset] = r;
  image[++offset] = g;
  image[++offset] = b;
  image[++offset] = a;
}

// Creates minimized version of image based on guides
// to be used as border-image in css
void saveNinePatch(guidelines guides, std::vector<unsigned char> &image,
                   unsigned width, unsigned height, std::string filename,
                   bool addContent = false)
{
  unsigned minWidth = guides.getLeft() + guides.getRight() + 1;
  unsigned minHeight = guides.getTop() + guides.getBottom() + 1;

  std::vector<unsigned char> minimized;
  getResizedWithOffset(
   guides, image, minWidth, minHeight, width, height, minimized
  );

  unsigned nineWidth = minWidth + 2;
  unsigned nineHeight = minHeight + 2;
  std::vector<unsigned char> ninePatch;
  ninePatch.resize(nineWidth * nineHeight * 4);

  // Filling in transparent pixels in offset
  for(unsigned y = 0; y < nineHeight; ++y)
  {
    fillPixel(ninePatch, nineWidth, 0, y);
    fillPixel(ninePatch, nineWidth, nineWidth - 1, y);
  }

  for(unsigned x = 0; x < nineWidth; ++x)
  {
    fillPixel(ninePatch, nineWidth, x, 0);
    fillPixel(ninePatch, nineWidth, x, nineHeight - 1);
  }

  // Adding black dots
  fillPixel(ninePatch, nineWidth, guides.getLeft() + 1, 0, 0, 0, 0, 255);
  fillPixel(ninePatch, nineWidth, 0, guides.getTop() + 1, 0, 0, 0, 255);
  if(addContent)
  {
    fillPixel(ninePatch, nineWidth, guides.getLeft() + 1, nineHeight - 1, 0, 0, 0, 255);
    fillPixel(ninePatch, nineWidth, nineWidth - 1, guides.getTop() + 1, 0, 0, 0, 255);
  }

  copyImagePart(
    minimized, minWidth,
    ninePatch, nineWidth,
    minWidth, minHeight,
    0, 0, 1, 1
  );

  saveImage(ninePatch, nineWidth, nineHeight, filename);
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
