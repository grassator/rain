#ifndef RAIN_GUIDELINES_H
#define RAIN_GUIDELINES_H

#include <vector>

namespace rain {

class guidelines {
  private:
    unsigned _top;
    unsigned _right;
    unsigned _bottom;
    unsigned _left;

    void init(std::vector<unsigned char> &image, unsigned width, unsigned height,
              unsigned startX, unsigned startY, bool fluidWidth, bool fluidHeight);
  
  public:
    // Main function that creates guidelines from the image
    // Expects image to bt RGBA 8 bits per pixel std::vector
    // of size width * height * 4
    // If startX or startY are out of bounds they are forced to be within them
    guidelines(std::vector<unsigned char> &image, unsigned width,
               unsigned height, unsigned startX, unsigned startY, 
               bool fluidWidth = false, bool fluidHeight = false);
    
    // Overloaded constructors that starts processing from the center of the image
    guidelines(std::vector<unsigned char> &image, unsigned width,
               unsigned height, bool fluidWidth = false, bool fluidHeight = false);
    
    std::string toJSON();

    // Getters
    inline unsigned getTop()    { return _top;    }
    inline unsigned getRight()  { return _right;  }
    inline unsigned getBottom() { return _bottom; }
    inline unsigned getLeft()   { return _left;   }

};

} // end namespace rain

#endif