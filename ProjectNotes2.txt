CS 258 Spring 2011
Project 2: TIFF read/write
Author: Scott Yeskie

Comments:
    If using existing directory, use "make clean" before building with make
    Changes will not propogate through if old .o files exist.
    
    Hard limit of 1024x1024 for reading (and writing).

    TiffStat will NOT kill the read stack if trying to read an invalid TIFF.
    However, it will not print out tags at all if any of the required tags
    are missing for that type.

    Included scripts exercise functions over given files.
    See documentation at the header for each file for info on what they do.

    Included own image in three formats: sky_rgb.tif, sky_g.tif, and sky_bw.tif
    
    sky_bw.tif is an image the program can stat but not read
        It is a bilevel (1 BitPerSample)

    Also make pooh_comp.tif which has LZW compression, so program cannot read.

    I tested an image that was too large to make sure it quit, but that image
    at 20MB was a little large to include with the submission.