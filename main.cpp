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

void saveMinimized(rain::guidelines guides, std::vector<unsigned char> &image,
                   unsigned width, unsigned height)
{
  unsigned outputWidth = guides.left + guides.right + 1;
  unsigned outputHeight = guides.top + guides.bottom + 1;

  // Preparing image output
  std::vector<unsigned char> minimized;
  minimized.resize(outputWidth * outputHeight * 4);

  // North-west
  rain::copyImagePart(
    image, width,
    minimized, outputWidth,
    guides.left, guides.top,
    0, 0, 0, 0
  );

  // North
  rain::copyImagePart(
    image, width,
    minimized, outputWidth,
    1, guides.top,
    guides.left, 0,
    guides.left, 0
  );

  // North-east
  rain::copyImagePart(
    image, width,
    minimized, outputWidth,
    guides.right, guides.top,
    width - guides.right, 0,
    guides.left + 1, 0
  );

  // West
  rain::copyImagePart(
    image, width,
    minimized, outputWidth,
    guides.left, 1,
    0, guides.top,
    0, guides.top
  );

  // Center
  rain::copyImagePart(
    image, width,
    minimized, outputWidth,
    1, 1,
    guides.left, guides.top,
    guides.left, guides.top
  );

  // East
  rain::copyImagePart(
    image, width,
    minimized, outputWidth,
    guides.right, 1,
    width - guides.right, guides.top,
    guides.left + 1, guides.top
  );

  // South-west
  rain::copyImagePart(
    image, width,
    minimized, outputWidth,
    guides.left, guides.bottom,
    0, height - guides.bottom,
    0, guides.top + 1
  );

  // South
  rain::copyImagePart(
    image, width,
    minimized, outputWidth,
    1, guides.bottom,
    guides.left, height - guides.bottom,
    guides.left, guides.top + 1
  );

  // South-east
  rain::copyImagePart(
    image, width,
    minimized, outputWidth,
    guides.right, guides.bottom,
    width - guides.right, height - guides.bottom,
    guides.left + 1, guides.top + 1
  );




  // Output file
  LodePNG::Encoder encoder;
  encoder.getSettings().zlibsettings.windowSize = 2048;

  //encode and save
  std::vector<unsigned char> ob;
  encoder.encode(ob, &minimized[0], outputWidth, outputHeight);
  LodePNG::saveFile(ob, "minimized.png");
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

    saveMinimized(result, image, decoder.getWidth(), decoder.getHeight());
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

