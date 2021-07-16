CS28 Spring
Scott Yeskie
Project 4: 3D Transformations
-----------------------------------

To run through all the included p4 drawing scripts, run
  read p4all.cli


Additional commands implemented:
  pause -- pauses execution of the file until a key is pressed.
      exits and reenters file stack, so usable with any operations

  resizewin w h -- forces window resize to w x h. (If one param, w x w)


Notes on defaults:
  For scale, if only one argument is given, it scales all three directions by that amount.
  Default values for translate are zero.
  Rotate fills any values omitted for the point as zero. Also, if (0,0,0) is given as the vecotor around which to rotate, (0, 0, 1) is used instead. Thus a rotate with one argument rotates around the z-axis.

  The reset command sets the image array to black as well. All drawing changes are posted to the image array (clipped to max size).



Images from the screen are writeable with tiffwrite.
What is written is the image array, so a max size of 1024x1024 can be written.



The orient command was implemented. Most of the commands are essentially combinations of scale and rotate. The one item that wasn't a composition of these two was shearing. For example, (1 .5 0; 0 1 0; 0 0 1) is an example of a single shearing in the y component affected by x. It's characterized by keeping the diagonal ones (unless scaling and/or rotating is wanted as well) and adding a nonzero value in one of the other cells of the matrix. Since shearing is different depending on which component is being affected by which other component, an orient is actually the best way to describe the transform.