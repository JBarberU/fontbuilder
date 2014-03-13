# Df-generator (Distance fields generator)

## About

df-generator is a commandline tool used for generating compact textures for 
use with alpha testing and alpha thresholding. This is done to achieve crisp 
looking textures with minimal video memory footprint.

The technique used is based on Valves [paper](http://www.valvesoftware.com/publications/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf) 
from Siggraph 2007

## Installation

```bash
git clone git@github.com:DreamlerAB/df-generator.git
mkdir df-generator/build
cd df-generator/build
cmake ..
make
```

## Usage

```bash
# some-file.png - inputfile
# out-file.png	- outfile
# 512			- the output maximum dimension
# 10			- the number of cores to use

./df-generator -i some-file.png -o out-file.png -m 512 -c 10
```

## Author

John Barbero Unenge - john@dreamler.com

## License

    The MIT License (MIT)

    Copyright (c) 2014 John Barbero Unenge

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
