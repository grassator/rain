# Overview

**Rain** is a command line utility designed for minimizing border images for use
in CSS, Qt and Android (right now requires small modification after processing).
In addition to that **rain** can split resulting minimized image in three
(either vertical or horizontal) or nine parts for usage with CSS in older
browsers or any other platform, like Cocoa (see NSDrawThreePartImage and
NSDrawNinePartImage [reference][cocoa]).

## How does rain work

**Rain** works by comparing pixels in every direction to the one specified as
a starting point (center of the image by default). When it finds a pixel of
different color that means that we can't minimize any more in this direction.

After initial scanning is done **rain** can use retreived information in
several ways:

- Minimize image and save it as a regular PNG file to use with CSS3
  border-image property, or [9-patch image][android] for android UI development.

- Output a series of image - three or nine depending on your need to be able to
  use it for element with fluid height, width or both.

- Print out offsets from each border in an easily-parsable and
  human-readable JSON format to allow manual or automated usage in CSS/QSS files
  for border-image property.

Sample usage may look like this:

    rain -o minimized -a button.png

In this case **rain** will process `button.png`, minimize it and output into
file named `minimized.9.png` in nine-patch image format for android development.
A complete list of possible command line arguments presented in the next
chapter.

## Command line parameters

There are several groups of options. Some groups allow simultanious use of
flag and parameters while others don't. If you accidently violate these
restrictions you will be reminded of them by an error message printed to stderr.

### Common options

    <input_file>

The last unnamed command line parameter is considered to be input file name.
This is the only required parameter. If you specify it alone, **rain**
will only try to analyze provided file without giving any output except for the
case where input file can't be read or isn't proper PNG file.

    -o <output_base_name>,  --output <output_base_name>

This is basename for the **rain** output file(s). Don't specify an extension
here because depending on your output format it may be used to construct several
filenames (e.g. `basename-top.png`, `basename-bottom.png`, ...). By default
basename is constructed from input file name by removing extension.

    -j,  --json

Output offsets from each border that are used in CSS3 border-image porperty
in JSON format to stdout. Sample output look like this:
`{"top":50,"right":15,"bottom":15,"left":15}`

    -y <integer>,  --y-start <integer>

Specifies Y coordinate of starting point for the analysis. It is important when
our image is assymetrical and repeating part lies either below or above the
center. **Warning:** this parameter is always forced to be within image
boundaries.

    -x <integer>,  --x-start <integer>

Same as above but for X coordinate of starting point.

    --,  --ignore_rest

Standard *nix flag that ignores the rest of the labeled arguments
following this flag.

    --version

Displays version information and exits.

    -h,  --help

Displays usage information and exits.


### Minimization / resize

*Options in this group are mutually exclusive.*

    -a,  --android

Outputs minimized [Android 9-patch image][android]
with `.9.png` extension.

    -c,  --android-content

Same as above but with content area specified inside of repeating part of
the image.

    -m,  --minimize

Minimizes image to be used as a border-image in CSS 3 or Qt stylesheets.
**Warning:** with no basename specified this option overwrites input image.

    -r <WIDTHxHEIGHT>,  --resize <WIDTHxHEIGHT>

Resizes image to provided dimensions. Essentially this is a simplified version
of border-image technique where middle part is always repeated (CSS3 supports
stretch as well). **Warning:** image dimensions are forced to be at least the
size of minimized image which is `(left + right + 1)x(top + bottom + 1)`.

### Multiple images

*Options in this group are mutually exclusive.*

This set of option is useful when you target systems that don't have some sort
of border-image capability and require a set of image to provide support for
resizeable custom UI elements. Systems like that include but not limited to
Cocoa and older browsers like IE8 and below.

    -b,  --both

Creates 9 images by first minimizing input image and then splitting into
multiple images based on the result of analysis. Output images are suffixed
with cardinal directions (e.g. `basename-north-west.png`). Center image has
suffix `middle`.

    -t,  --vertical

Creates 3 images by only splitting input vertically. Output suffixes are:
`top`, `middle`, `bottom`.

    -l,  --horizontal

Creates 3 images by only splitting input horizontally. Output suffixes are:
`left`, `middle`, `right`.

## License

Copyright (c) 2011 Dmitriy Kubyshkin

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Aknowledgements

This software makes use of open source code for some of its functionality. A big thank you to everybody who created the code that helps me build a better software.

### [TCLAP (Templatized C++ Command Line Parser)](http://tclap.sourceforge.net/)

Copyright (c) 2003, Michael E. Smoot .
Copyright (c) 2004, Michael E. Smoot, Daniel Aarno .
All rights reverved.

See the file COPYING in the top directory of this distribution for
more information.

THE SOFTWARE IS PROVIDED _AS IS_, WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

### [LodePNG](http://lodev.org/lodepng/)

Copyright (c) 2005-2011 Lode Vandevenne

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.


[android]: http://developer.android.com/guide/developing/tools/draw9patch.html
[cocoa]: http://developer.apple.com/library/mac/#documentation/Cocoa/Reference/ApplicationKit/Miscellaneous/AppKit_Functions/Reference/reference.html