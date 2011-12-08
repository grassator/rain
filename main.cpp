//
//  main.cpp
//  rain
//
//  Created by Dmitriy Kubyshkin on 12/7/11.
//  Copyright (c) 2011 Dmitriy Kubyshkin. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "lodepng.h"
#include "guidelines.h"

void saveImage(std::vector<unsigned char> &image,
               unsigned width, unsigned height,
               std::string filename)
{
  // Creating encoder
  LodePNG::Encoder encoder;
  encoder.getSettings().zlibsettings.windowSize = 2048;

  // Encode and save
  std::vector<unsigned char> ob;
  encoder.encode(ob, image.empty() ? 0 : &image[0], width, height);
  LodePNG::saveFile(ob, filename);
}

void saveMinimized(rain::guidelines guides, std::vector<unsigned char> &image,
                   unsigned width, unsigned height, std::string filename)
{
  unsigned outputWidth = guides.left + guides.right + 1;
  unsigned outputHeight = guides.top + guides.bottom + 1;

  // Preparing image output
  std::vector<unsigned char> minimized;
  minimized.resize(outputWidth * outputHeight * 4);

  unsigned sizeX[3] = { guides.left,  1,            guides.right           };
  unsigned sizeY[3] = { guides.top,   1,            guides.bottom          };
  unsigned fromX[3] = { 0,            guides.left,  width - guides.right   };
  unsigned fromY[3] = { 0,            guides.top,   height - guides.bottom };
  unsigned toX[3]   = { 0,            guides.left,  guides.left + 1        };
  unsigned toY[3]   = { 0,            guides.top,   guides.top + 1         };

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



int main (int argc, const char * argv[])
{
  const char* filename = argc > 1 ? argv[1] : "test.png";
  
  std::vector<unsigned char> buffer, image;
  LodePNG::loadFile(buffer, filename); //load the image file with given filename
  LodePNG::Decoder decoder;
  decoder.decode(image, buffer.empty() ? 0 : &buffer[0], (unsigned)buffer.size()); //decode the png
  
  //if there's an error, display it, otherwise display information about the image
  if(decoder.hasError())
  {
    std::cout << "error " << decoder.getError() << ": "
    << LodePNG_error_text(decoder.getError()) << std::endl;
  }
  else
  {
    rain::guidelines result = rain::processFromCenter(
      image, decoder.getWidth(), decoder.getHeight(), true, true
    );
    std::cout << 
    "top: " << result.top << "\n" <<
    "right: " << result.right << "\n" <<
    "left: " << result.left << "\n" <<
    "bottom: " << result.bottom << "\n";

    saveMinimized(result, image, decoder.getWidth(), decoder.getHeight(), "min.png");
  }
}
    // std::cout << "\n" <<
    // "w: " << decoder.getWidth() << "\n" <<
    // "h: " << decoder.getHeight() << "\n" <<
    // "bitDepth: " << decoder.getInfoPng().color.bitDepth << "\n" <<
    // "bpp: " << decoder.getBpp() << "\n" <<
    // "colorChannels: " << decoder.getChannels() << "\n" <<
    // "paletteSize: " << decoder.getInfoPng().color.palettesize / 4 << "\n" <<
    // "colorType: " << decoder.getInfoPng().color.colorType << "\n";

