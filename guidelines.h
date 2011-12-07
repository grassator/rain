#include <vector>

namespace rain {

struct guidelines {
  unsigned top;
  unsigned right;
  unsigned bottom;
  unsigned left;
};

// Expects image to RGBA 8 bits per pixel std::vector
// of size width * height * 4
// If startX or startY are out of bounds they are forced to be within them
guidelines process(std::vector<unsigned char> &image,
                   unsigned width, unsigned height,
                   unsigned startX, unsigned startY, 
                   bool fluidWidth = false, bool fluidHeight = false)
{
  // Fixing bounds if necessary
  if(startX > width) startX = width;
  if(startY > height) startY = height;

  // Initializing output
  guidelines output;
  output.top = output.left = 0;
  output.bottom = height;
  output.right = width;

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
      for(unsigned x = startX - 1; x > output.left; --x)
      {
        i = (row + x) * 4;
        if(r != image[i] || g != image[++i] || b != image[++i] || a != image[++i])
        {
          output.left = x + 1;
          break;
        }
      }

      // Right guide
      for(unsigned x = startX ; x < output.right; ++x)
      {
        i = (row + x) * 4;
        if(r != image[i] || g != image[++i] || b != image[++i] || a != image[++i])
        {
          output.right = x;
          break;
        }
      }
    }
  }
  else
  {
    output.left = output.right = startX;
  }

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
      for(unsigned y = startY - 1; y > output.top; --y)
      {
        i = (width * y + x) * 4;
        if(r != image[i] || g != image[++i] || b != image[++i] || a != image[++i])
        {
          output.top = y + 1;
          break;
        }
      }

      // Bottom guideline
      for(unsigned y = startY; y < output.bottom; ++y)
      {
        i = (width * y + x) * 4;
        if(r != image[i] || g != image[++i] || b != image[++i] || a != image[++i])
        {
          output.bottom = y;
          break;
        }
      }
    }
  }
  else
  {
    output.top = output.bottom = startY;
  }

  // It's easier to consume when guidelines are defined as offsets from bounds
  output.right = width - output.right;
  output.bottom = height - output.bottom;

  return output;
}

// Convenience function to start processing from center
guidelines processFromCenter(std::vector<unsigned char> &image,
                             unsigned width, unsigned height,
                             bool fluidWidth = false,
                             bool fluidHeight = false)
{
  return process(image, width, height, width / 2, height / 2, 
                 fluidWidth, fluidHeight);
}


// Copies part of the source image to target starting from {fromX, fromY}
// with specified height and width.
// Doesn't check the size of source vector.
void copyImagePart(std::vector<unsigned char> &source, unsigned sourceWidth,
                   std::vector<unsigned char> &target,
                   unsigned width, unsigned height,
                   unsigned fromX = 0, unsigned fromY = 0)
{
  // Assume RGBA 8 bits per pixel
  target.resize(width * height * 4);

  // Index for target array
  unsigned i = -1, j = 0;

  // It's faster to have y on outer loop because of cache hits
  for(unsigned y = fromY; y < fromY + height; ++y)
  {
    for(unsigned x = fromX; x < fromX + width; ++x)
    {
      j = (y * sourceWidth + x) * 4;
      // I know that loops exist but it's faster that way
      target[++i] = source[j];
      target[++i] = source[++j];
      target[++i] = source[++j];
      target[++i] = source[++j];
    }
  }
}

} // end namespace rain