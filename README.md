# pixmap-ops

Supported Image Manipulation Methods:
- Invert
- Swirl
- Rotate90
- Bitmap
- Sharpen
- Gaussian Blur
- Box Blur
- Ridge Detection
- Unsharp Masking
- Sobel Operator
- Add
- Subtraction
- Difference (not shown)
- Multiply (not shown)
- Darkest (not shown)
- Lightest (not shown)
- Grid Copy
- Color Jitter
- Glow

First the program grid copies the given image into a 5x4 grid of the same image, then the image effects from Top-Left to Bottom-Right:

- Original,        Gaussian Blur, Box Blur, Unsharp Masking 
- Sobel Operator,  Greyscale,     Invert,   Bitmap
- Ridge Detection, Sharpen,       Swirl,    Rotating 180
- Glow,            Redify,        Greenify, Blueify
- Redless,         Greenless,     Blueless, Color Jitter     



Earth Images

![earth](https://user-images.githubusercontent.com/72237791/218003167-59f76e32-78a3-4535-9867-d47db5330c4b.png)

Squirrel Images

![squirrel](https://user-images.githubusercontent.com/72237791/218003190-c8d5fc68-874c-445f-bc6f-7caacd30ad46.png)

Heimerdinger Images

![heimer](https://user-images.githubusercontent.com/72237791/218003222-89db528c-4c76-4b79-b563-e449981608af.png)

Jinx Images

![jinx-min](https://user-images.githubusercontent.com/72237791/218003921-d5f264c3-b92a-46a0-8645-d605dbd2d715.png)

Scenery Images

![scenery-min](https://user-images.githubusercontent.com/72237791/218003936-e6aef3a6-bf9c-4d86-988d-6afd79502fdb.png)

Ghost Psyduck Effect

![psyduck_ghost](https://user-images.githubusercontent.com/72237791/218003308-d9113faa-5256-4f2d-b878-75f788fd4fe1.png)






## How to build

*Windows*

Open git bash to the directory containing this repository.

```
pixmap-ops $ mkdir build
pixmap-ops $ cd build
pixmap-ops/build $ cmake -G "Visual Studio 17 2022" ..
pixmap-ops/build $ start pixmap-ops.sln
```

Your solution file should contain two projects: `pixmap_art` and `pixmap_test`.
To run from the git bash command shell, 

```
pixmap-ops/build $ ../bin/Debug/pixmap_test
pixmap-ops/build $ ../bin/Debug/pixmap_art
```

*macOS*

Open terminal to the directory containing this repository.

```
pixmap-ops $ mkdir build
pixmap-ops $ cd build
pixmap-ops/build $ cmake ..
pixmap-ops/build $ make
```

To run each program from build, you would type

```
pixmap-ops/build $ ../bin/pixmap_test
pixmap-ops/build $ ../bin/pixmap_art
```

## Image operators

TODO: Document the features of your PPM image class here. Include example images.

## Results

TODO: Show artworks using your class

