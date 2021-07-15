******************************************
* CS 258 Spring 2011
* Project 2: TIFF read/write
* Author: Scott Yeskie
******************************************

Sample scripts scale0.cli through scale2.cli are included. They read in an
image and scale and/or resize it, for easy comparison of kernels and border
methods.

I didn't get my comments on Project 2 until today, so no corrections were made there.
The problems affected the writing of string tags and greyscale images, but do not
affect the functionality of the program for this assignment.

******************************************
    Extra Credit: border command
******************************************
Implemented border command to change edge handling. Possible options:
    zero (default) -- treats pixels outside as black (0,0,0)
    freeze -- will use edge pixel for pixels outside
    circular -- reflects back in pixels that go across edge
    truncate -- Renormalized filter at edges to exclude pixels outside the range
Type "border" with no arguments to see the current method.

The zero (default) is by far the worst of these filters as it produces a dark
border around the edge. This isn't noticable on eeyore.tif (as it already has
a border) or any of the p4_.tif images (as they're black at the border). Any
image with content will display a dark band along the edge. It is very hard to
distinguish the other filters, however, as all perform well for the small
default radius values.

To notice a difference, you really had to try to abuse it. The scale_rpt.cli
script does multiple magnify operations with a filter width 7 to compound
problems. Truncate and circular produce much smoother transitions at the edge
than freeze. I prefer the truncate implementation to circular (and it's simpler
to code). It just throws away the weight for that pixel, leaving it at zero.
For freeze and circular, the correct pixel needs to be found.

******************************************
Extra Credit: select command/filters
******************************************
All listed filter kernels are implemented. The valid options for "select" are:
    "lanczos", "box", "tent", "hann", "hamming", "mitchell" and "gaussian"
    "triangle" is included as an alias for the tent kernel

A select with no arguments will print the info on the current kernel.
A select with one argument will change to the selected kernel with the default
    width. All filters have a default width of 2 except for the
    raised cosine filters, (Hann and Hamming) which default to 3.

A nonzero third argument can be added to any filter to change the width.
The width is coded as an int throughout, so only integer inputs are acccepted.
If the width is zero, the default filter width is used.
Negatives are converted to the positive equivalent.

Comparisons: done using scale0.cli (magnify 1.23x realistic)
On the default widths, the kernels I liked best were the Hann, Hamming, and
Mitchell filters. The Lanczos and Gaussian filters added a significant amount
of blur, and produced some aliasing in the fur along the cat's back. The tent
was pretty good on sharpness, but had moderate aliasing to go along with it. The
box was bad with tons of aliasing.

For scale1.cli (magnify 2x cartoon - pooh.tif)
The box filter performed even worse here. The tent performs much better. It
has moderate aliasing, but is good for it's speed/simplicity. The Gaussian and
Lanczos filters perform about the same and both have very little aliasing and
a moderate amount of blurring. The Hamming and Hann filters are in between those
two and the tent. Slight aliasing and slight blurring. The Mitchell kernel was
actually very close to the tent kernel. I prefer the Hamming and Hann filters
for these settings.

scale2.cli compares photographic minification
Here, the blurriness helps the image quality greatly. The best filters are the
Gaussian and Lanczos filters.

******************************************
Extra Credit: negative scaling
******************************************
A negative value in the scale or resize function will flip the image in those
dimension(s). The image is flipped while copying from the window, then the
factors are converted to positive for the rest of the resampling.

******************************************
Extra Credit: efficiency notes
******************************************
The algorithms used follow the book for the most part. As such it follows the
same order. It is proportional to the new image size and the filter radius.
Resampling the border areas is separated from resampling the majority of the
image to reduce the number of checks. Also, since the filters can have variable
widths, they need to be normalized to prevent loss of brightness. The values
are normalized with one divide at the end of sampling all the points in the
radius.

Several O(1) improvements do remain that could be implemented, but would require
less code reuse and a more complicated approach than is currently implemented.
The number of memory writes could be cut in half. Currently the window is read
into a local variable, resampled on local variables, and then written back out
to the window array. Combining resample(x) with read window and resample(y)
with write to window would cut the major memory allocations from 3 to 1.

Similarly, the program could check which dimension to resample first to minimize
the number of computations. (Eg if minimizing, work on the long edge first).