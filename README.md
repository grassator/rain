# Overview

**Rain** is a command line utility designed for minimizing border images for use in CSS, Qt and Android (right now requires small modification after processing). In addition to that **rain** can split resulting minimized image in three (either vertical or horizontal) or nine parts for usage with CSS in older browsers or any other platform, like Cocoa (see NSDrawThreePartImage and NSDrawNinePartImage [reference](http://developer.apple.com/library/mac/#documentation/Cocoa/Reference/ApplicationKit/Miscellaneous/AppKit_Functions/Reference/reference.html)).

## How does rain work

**Rain** works by comparing pixels in every direction to the one specified as a starting point (center of the image by default). When it finds a pixel of different color that means that we can't minimize any more in this direction. Rain can print out offsets from each border in an easily-parsable and human-readable JSON format (`-j` or `--json` flag) to allow manual or automated usage in CSS/QSS files in border-image property. Sample output looks like this:

    {"top":50,"right":15,"bottom":15,"left":15}

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