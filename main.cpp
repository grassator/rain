//
//  main.cpp
//
//  Created by Dmitriy Kubyshkin on 12/7/11.
//  Copyright (c) 2011 Dmitriy Kubyshkin. All rights reserved.
//

#include <iostream>
#include <libgen.h>
#include "lodepng.h"
#include "tclap/CmdLine.h"
#include "functions.h"
#include "guidelines.h"

using namespace rain;

int main (int argc, const char * argv[])
{
  try
  {
    // Defining command line args
    TCLAP::CmdLine cmd("Auto-minimization tool for fluid images.", ' ', "1.0b");

    TCLAP::ValueArg<unsigned> argX(
      "x", "x-start", "X coordinate of starting point", false, 0, "integer"
    );
    cmd.add(argX);

    TCLAP::ValueArg<unsigned> argY(
      "y", "y-start", "Y coordinate of starting point", false, 0, "integer"
    );
    cmd.add(argY);

    TCLAP::SwitchArg argJSON("j", "json", "Output guidelines in JSON to stdout");
    cmd.add(argJSON);

    TCLAP::ValueArg<std::string> argResize(
      "r", "resize", "Resizes to provided dimensions. Forced to be \
at least the size of minimized image which is (left+right+1)x(top+bottom+1)",
      false, "", "WIDTHxHEIGHT"
    );
    cmd.add(argResize);

    TCLAP::SwitchArg argMinimize("m", "minimize", "Minimizes image\n\n  OR");
    cmd.add(argMinimize);

    // These modes are mutually exclusive
    TCLAP::SwitchArg argHorizontal("l", "horizontal", "Creates 3 images horizontally");
    cmd.add(argHorizontal);

    TCLAP::SwitchArg argVertical("t", "vertical", "Creates 3 images vertically\n\n  OR");
    cmd.add(argVertical);

    TCLAP::SwitchArg argBoth("b", "both", "Creates 9 images (both directions)\n\n  OR");
    cmd.add(argBoth);

    TCLAP::ValueArg<std::string> argBasename(
      "o", "output", "Basename for output (without .png)", false, "", "output_base_name"
    );
    cmd.add(argBasename);

    TCLAP::UnlabeledValueArg<std::string> argInput(
      "filename", "Input filename (with extension)", true, "", "input_file"
    );
    cmd.add(argInput);

    // Get thing going
    cmd.parse(argc, argv);

    // First we validate separate image arguments
    if((argHorizontal.isSet() + argVertical.isSet() + argBoth.isSet()) > 1)
    {
      std::cerr << "ERROR: you can't specify more than one "
      << "of the following flags: -l, -t, -b" << std::endl;
      return -1;
    }

    // Can't minimize and resize at the same
    if((argMinimize.isSet() + argResize.isSet()) > 1)
    {
      std::cerr << "ERROR: you can't specify more than one "
      << "of the following flags: -m, -r" << std::endl;
      return -1;
    }

    unsigned resizeWidth = 0, resizeHeight = 0;
    // Trying to parse new dimensions if provided
    if(argResize.isSet())
    {
      std::stringstream size(argResize.getValue());
      // Trying to get width
      bool ok = !!(size >> resizeWidth);
      if(ok)
      {
        // Now reading up to delimiter (x)
        std::string tail;
        std::getline(size, tail, 'x');
        // If there's no garbage after width tail should be empty
        ok = tail.empty() && !!(size >> resizeHeight);
      }
      if(!ok)
      {
        std::cerr << "ERROR: You have to specify dimensions in format WIDTHxHEIGHT"
        << std::endl;
        return -1;
      }
    }

    // If we didn't get explicit basename assume it is the same as input one
    std::string filename = argInput.getValue();
    std::string outputBasename = argBasename.getValue();
    if(outputBasename.empty())
    {
      outputBasename = basenameWithoutExtension(filename);
    }

    // Loading image from file
    std::vector<unsigned char> buffer, image;
    LodePNG::loadFile(buffer, filename);
    LodePNG::Decoder decoder;
    decoder.decode(image, buffer.empty() ? 0 : &buffer[0], (unsigned)buffer.size());

    // Stopping here if something is wrong
    if(decoder.hasError())
    {
      std::cerr << "error: " <<
      LodePNG_error_text(decoder.getError()) << std::endl;
      return decoder.getError();
    }

    // Convinience variables
    unsigned width = decoder.getWidth();
    unsigned height = decoder.getHeight();
    unsigned startX = argX.isSet() ? argX.getValue() : (width / 2);
    unsigned startY = argY.isSet() ? argY.getValue() : (height / 2);

    // Getting guidelines from the image (for now always all)
    guidelines guides(image, width, height, startX, startY, true, true);

    // Outputting JSON if requested
    if(argJSON.getValue())
    {
      std::cout << guides.toJSON() << "\n";
    }

    // Saving minimized image if requested
    if(argMinimize.isSet())
    {
      saveMinimized(guides, image, width, height, outputBasename + ".png");
    }

    // Saving resized image if requested
    if(argResize.isSet())
    {
      saveResized(guides, image, resizeWidth, resizeHeight, width,
                  height, outputBasename + ".png");
    }

    // Saving nine-part image if requested
    if(argBoth.getValue())
    {
      saveNinePart(guides, image, width, height, outputBasename);
    }

    // Saving nine-part image if requested
    if(argHorizontal.getValue())
    {
      saveThreePart(guides, image, width, height, outputBasename, HORIZONTAL);
    }

    // Saving nine-part image if requested
    if(argVertical.getValue())
    {
      saveThreePart(guides, image, width, height, outputBasename, VERTICAL);
    }
  }
  catch(TCLAP::ArgException &e)
  {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
  }

}
