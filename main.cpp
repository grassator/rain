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

    TCLAP::SwitchArg argJSON("j", "json", "Output guidelines in JSON to stdout");
    cmd.add(argJSON);

    TCLAP::SwitchArg argMinimize("m", "minimize", "Minimizes image");
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
    if((argHorizontal.getValue() + argVertical.getValue() + argBoth.getValue()) > 1)
    {
      std::cerr << "error: you can't specify more than one "
      << "of the following flags: -l, -t, -b" << std::endl;
      return -1;
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

    // Convinience variables
    unsigned width = decoder.getWidth();
    unsigned height = decoder.getHeight();

    // Stopping here if something is wrong
    if(decoder.hasError())
    {
      std::cerr << "error: " <<
      LodePNG_error_text(decoder.getError()) << std::endl;
      return decoder.getError();
    }

    // Getting guidelines from the image (for now always all)
    guidelines guides(image, width, height, true, true);

    // Outputting JSON if requested
    if(argJSON.getValue())
    {
      std::cout << guides.toJSON() << "\n";
    }

    // Saving minimized image if requested
    if(argMinimize.getValue())
    {
      saveMinimized(guides, image, width, height, outputBasename + ".png");
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
