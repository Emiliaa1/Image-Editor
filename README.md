# Image Editor

## Overview
This project is an image editor implemented in C as part of the PLCLP1 course. It processes NetPBM images (PGM & PPM formats) and supports several basic image manipulation commands.

## Features
- Load and save images in ASCII or binary format.
- Select and crop image regions.
- Apply transformations such as rotation, equalization, and histogram calculations.
- Use image processing kernels like edge detection, sharpening, blur, and Gaussian blur.

## Supported Commands
| Command | Description |
|---------|-------------|
| `LOAD <file>` | Loads an image from a file. |
| `SAVE <file> [ascii]` | Saves the current image to a file in binary or ASCII format. |
| `SELECT <x1> <y1> <x2> <y2>` | Selects a rectangular region of the image. |
| `SELECT ALL` | Selects the entire image. |
| `CROP` | Crops the image to the selected region. |
| `ROTATE <angle>` | Rotates the selected region by 90, 180, 270, or 360 degrees. |
| `HISTOGRAM <x> <y>` | Displays an ASCII histogram of the image. |
| `EQUALIZE` | Applies histogram equalization to grayscale images. |
| `APPLY <FILTER>` | Applies a filter (EDGE, SHARPEN, BLUR, GAUSSIAN_BLUR) to the image. |
| `EXIT` | Exits the program. |

## Installation & Usage
### Compilation
Ensure you have `gcc` installed, then compile using:
```bash
make
```
This will generate the `image_editor` executable.

### Running the Program
Run the editor with:
```bash
./image_editor
```
Then, enter commands interactively.

### Example Usage
```bash
LOAD example.pgm
SELECT 0 0 100 100
CROP
EQUALIZE
SAVE output.pgm
EXIT
```

## File Formats Supported
The program supports:
- **PGM (Grayscale)**: ASCII (P2) and binary (P5)
- **PPM (RGB Color)**: ASCII (P3) and binary (P6)

## Development Guidelines
- Uses dynamic memory allocation for efficient image processing.
- Adheres to coding best practices with modular design.
- Avoids global variables and ensures proper memory management.
