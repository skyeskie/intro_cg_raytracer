General Notes:
--------------------------------
The image scene_final.tif, created with scripts/ray_scene.cli is the required scene.
It features 4 lights of different directions and colors, 2 boxes (one reflective),
two triangles (one reflective), and four spheres (two reflective).

The image took approximately 2-3 seconds to generate at 1000x1000.
At 500x500, the image took just under one second.


Screen must be called before orthocamera or camera can be called. Changes to
screen resolution not realized until after a camera or orthocamera called.

For perspective screen, the vectors need not be unit vectors. Also, the up
vector entered doesn't need to be orthogonal to the gaze vector.

Triangles are visible from either side. The normal will always face back towards
the incoming ray.

Box can be entered using any two corners. The code will take care of swapping
to get the appropriate two corners it uses.

Images are writeable using tiffwrite.
As well, they can be resized per project 3 commands.


--------------------------------
(Extra Credit #2)
--------------------------------
Box and triangle also have reflections. The format is similar to sphere, with the
 3 specular colors after the reflect colors.

Both reflections are visible in the main scripts/ray_scene.cli
Box reflection also readily apparent in scripts/ray_test4.cli
The triangle is also reflective there, but it isn't as apparent, as it doesn't
  reflect a sphere like in ray_scene.cli.