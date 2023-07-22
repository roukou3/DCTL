# Fisheye correction in DCTL

## Description

This is DCTL which can make correction of the fisheye lens.
You can convert fisheye video to ultra wide angle video.

DCTL is the DaVinci Color Transform Language (DCTL) that runs on Blackmagic Design's DaVinci Resolve Studio.

Supported Lenses:
- Meike 6.5 mm F/2.0
- Laowa 4.0 mm F/2.8 - not enough precision
- Any - [PTGui 11](https://wiki.panotools.org/Fisheye_Projection) - The fisheye factor k can model any possible fisheye. 

## Demonstration

[Corrected video samples](https://www.youtube.com/playlist?list=PLsqEeiE8zVJUeSfI9roCVS3D1I04Xmk4k)

[Usage explanation](https://youtu.be/w6r0EYwTiZo)

## Install

1. Open the DaVinci Resolve LUT directory.
    * File -> Project Setting -> Color Management -> Lookup Tables -> Open LUT Folder
2. Copy files(.dctl, .h) to the directory.
3. Reboot DaVinci Resolve.

* On macOS, there is a problem with GPU compatibility (metal). Please try below if the DCTL doesn't work.
    * Preferences-> Configuration -> GPU processing mode-> Changed from "Auto" to "OpenCL"

## Usage

### Basic

1. On the Resolve Color page, drag and drop DCTL filter to color node.
    * DCTL is in OpenFX -> ResolveFX Color -> DCTL.
2. Choose the added DCTL file in DCTL List.

### Parameters

* adjustment mode - Transit to the mode to adjust the position and size of the image circle.
* adjustment:center x, centery y, radius, aspect - Adjust the position, size and aspect of the image circle with each parameter.
* focal length log2 - Specify the logarithmic value of the full-size equivalent focal length. For example, 5 mm is 2.3219(2^2.322=5). The reason for logarithm is to zoom in smoothly.
* pan, tilt, rotate - Adjust the camera's 3 axes.
* defish rate - Deform continuously before and after fisheye correction.
* pan 2, tilt 2, rotate 2 - Additional camera's 3 axes for camera move.
* use bicubic - Apply bi-cubic interpolation.
* equirectangular map - Convert into an equirectangular projection. By setting the timeline resolution to 2:1, you can easily create 360 format video.
* hide outside of circle - Mask the corresponding image on the outside of the image circle.
* hide outside in degree - Adjust angle for masking.

## Requirement

Blackmagic Design DaVinci Resolve Studio 16, 17

DCTL does not work with free version of DaVinci Resolve.

## TIPS

- If the aspect of the timeline is different from the clip, [see here](https://github.com/roukou3/DCTL/issues/3#issuecomment-933454757).

- About shooting settings

   For movie shooting, IBIS(In Body Image Stabilization) should be turned off.
   And for GH 5, anamorphic(4:3) 6K is preferable.

## Licence

MIT License

## Related

- [DeFisheye (Qt)](https://github.com/Franticware/DCTL-Qt)

    [Franticware](https://github.com/Franticware) have created a standalone qt app which correct fisheye photos (not videos) without DaVinci resolve.
