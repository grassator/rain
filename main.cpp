//
//  main.cpp
//
//  Created by Dmitriy Kubyshkin on 12/7/11.
//  Copyright (c) 2011 Dmitriy Kubyshkin. All rights reserved.
//

#include <iostream>
#include <libgen.h>
#include "lodepng.h"
#include "functions.h"
#include "guidelines.h"

using namespace rain;

int main (int argc, const char * argv[])
{
  std::string filename = argc > 1 ? argv[1] : "test.png";
  
  std::vector<unsigned char> buffer, image;
  LodePNG::loadFile(buffer, filename); //load the image file with given filename
  LodePNG::Decoder decoder;
  decoder.decode(image, buffer.empty() ? 0 : &buffer[0], (unsigned)buffer.size());
  
  //if there's an error, display it, otherwise display information about the image
  if(decoder.hasError())
  {
    std::cout << "error " << decoder.getError() << ": "
    << LodePNG_error_text(decoder.getError()) << std::endl;
  }
  else
  {
    guidelines guides(
      image, decoder.getWidth(), decoder.getHeight(), true, true
    );
    std::cout << guides.toJSON() << "\n";
  }
}


