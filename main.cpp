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

    std::vector<unsigned char> topLeft;
    rain::copyImagePart(image, decoder.getWidth(), topLeft, result.left, result.top);
    LodePNG::Encoder encoder;
    encoder.getSettings().zlibsettings.windowSize = 2048;

    //encode and save
    std::vector<unsigned char> ob;
    encoder.encode(ob, topLeft.empty() ? 0 : &topLeft[0], result.left, result.top);
    LodePNG::saveFile(ob, "top-left.png");
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

