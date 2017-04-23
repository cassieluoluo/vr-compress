# Project Goals

There are two parts of this project _encoder_ and _decoder_.

## Encoder

### Existing Libraries
The following functions are implemented in OpenCV. There is no need to manually implemented them.
* DCT
* RGB to YUV
* etc.

### Multi-threading
The encoder should be able to utiize multi-CPU compuation ability by implementing multi-threading. With C++ 11 and later, threading can be done natively using the standard library.

### File Format
The compressed file should have a well-defined structure that contains metadata in addition to the actual video data.

## Decoder

### GUI
For cross-platform GUI, OpenCV highgui module is used. All user interaction will be implemented with that library

## Miscellaneous

### Testing
Each module must have unit test.

