#ifndef RAIN_FUNCTIONS_H
#define RAIN_FUNCTIONS_H

namespace rain {

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

} // end namespace rain

#endif