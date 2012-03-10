#include "guidelines.h"
#include <sstream>

using namespace rain;

guidelines::guidelines(
  std::vector<unsigned char> &image, unsigned width, unsigned height,
  unsigned startX, unsigned startY, bool fluidWidth, bool fluidHeight)
{
  this->init(image, width, height, startX, startY, fluidWidth, fluidHeight);
}

guidelines::guidelines(
  std::vector<unsigned char> &image, unsigned width,
  unsigned height, bool fluidWidth, bool fluidHeight)
{
  this->init(image, width, height, width/2, height/2, fluidWidth, fluidHeight);
}

std::string guidelines::toJSON()
{
  std::ostringstream json;
  json << '{' << "\"top\":"    << this->_top    << ',' <<
                 "\"right\":"  << this->_right  << ',' <<
                 "\"bottom\":" << this->_bottom << ',' <<
                 "\"left\":"   << this->_left   << '}';
  return json.str();
}

void guidelines::init(
  std::vector<unsigned char> &image, unsigned width, unsigned height,
  unsigned startX, unsigned startY, bool fluidWidth, bool fluidHeight)
{
  // Fixing bounds if necessary
  if(startX > width) startX = width - 1;
  if(startY > height) startY = height - 1;

  // Initializing output
  this->_top = this->_left = 0;
  this->_bottom = height;
  this->_right = width;

  // First we search for horizontal guidelines
  if(fluidWidth)
  {
    for(unsigned y = 0; y < height; ++y)
    {
      unsigned row = y * width;
      unsigned i = (row + startX) * 4;
      unsigned char r = image[i];
      unsigned char g = image[++i];
      unsigned char b = image[++i];
      unsigned char a = image[++i];

      // Left guide
      for(unsigned x = startX - 1; x > this->_left; --x)
      {
        i = (row + x) * 4;
        if(r != image[i] || g != image[++i] || b != image[++i] || a != image[++i])
        {
          this->_left = x;
          break;
        }
      }

      // Right guide
      for(unsigned x = startX ; x < this->_right; ++x)
      {
        i = (row + x) * 4;
        if(r != image[i] || g != image[++i] || b != image[++i] || a != image[++i])
        {
          this->_right = x;
          break;
        }
      }
    }
  }
  else
  {
    this->_left = startX - 1;
    this->_right = startX;
  }
  this->_left++;

  // Vertical guidelines
  if(fluidHeight)
  {
    for(unsigned x = 0; x < width; ++x)
    {
      unsigned i = (startY * width + x) * 4;
      unsigned char r = image[i];
      unsigned char g = image[++i];
      unsigned char b = image[++i];
      unsigned char a = image[++i];

      // Top guideline
      for(unsigned y = startY - 1; y > this->_top; --y)
      {
        i = (width * y + x) * 4;
        if(r != image[i] || g != image[++i] || b != image[++i] || a != image[++i])
        {
          this->_top = y;
          break;
        }
      }

      // Bottom guideline
      for(unsigned y = startY; y < this->_bottom; ++y)
      {
        i = (width * y + x) * 4;
        if(r != image[i] || g != image[++i] || b != image[++i] || a != image[++i])
        {
          this->_bottom = y;
          break;
        }
      }
    }
  }
  else
  {
    this->_top = startY - 1;
    this->_bottom = startY;
  }
  this->_top++;

  // It's easier to consume when guidelines are defined as offsets from bounds
  this->_right = width - this->_right;
  this->_bottom = height - this->_bottom;
}
