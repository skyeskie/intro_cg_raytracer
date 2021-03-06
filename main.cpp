
/*  This program is a modified version of the program "image.c" 
 *  appearing in _The OpenGL Programming Guide_ (red book).
 *  It demonstrates drawing pixels and shows the effect
 *  of glDrawPixels() and glCopyPixels().
 *  Interaction: moving the mouse while pressing the mouse button
 *  will copy the image in the lower-left corner of the window
 *  to the mouse position.
 *  There is no attempt to prevent you from drawing over the original
 *  image.
 *
 *  IMPORTANT: The window with the image must be the active window, i.e.,
 *  it must be clicked or the mouse must be over it, for input to occur.
 *  The result will be shown in the window you started the program from, 
 *  but don't make the mistake of typing in this window: it won't work.
 *
 */

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <stdexcept>

#include "mod_base/FunctionCommand.h"
#include "mod_base/CommandHandle.h"
#include "mod_dummy/DummyHandle.h"
#include "mod_tiff/TiffIO.h"
#include "mod_filter/ImageResize.h"
#include "mod_3d/DrawTransformed.h"
#include "mod_ray/RayTracer.h"

using namespace std;

/*	Create checkerboard image	*/
#ifndef MAIN_WINDOW_SET
#define MAIN_WINDOW_SET
#define	checkImageWidth 1024
#define	checkImageHeight 1024
GLubyte checkImage[checkImageHeight][checkImageWidth][3];
#endif

//For now, make command handler global
CommandHandle main_handler;


static GLint height;

void 
makeCheckImage(void)
{
   int i, j, c;
    
   for (i = 0; i < checkImageHeight; i++) {
      for (j = 0; j < checkImageWidth; j++) {
        c = ((((i&0x8)==0)^((j&0x8)==0)))*255;
        main_handler.main_window[i][j][0] = (GLubyte) c;
        main_handler.main_window[i][j][1] = (GLubyte) c;
        main_handler.main_window[i][j][2] = (GLubyte) c;
      }
   }
}

/*
 * Function name: init
 * Description:   Operations that only need be performed once for rendering
 *                are done here.
 * Arguments:	  none
 * Globals:       none
 * Returns:	  void
 */
void 
init(void)
{    
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   makeCheckImage();
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

/*
 * Function name: display
 * Description:   The glut display callback, operations to render and 
 *                re-render are here.
 * Arguments:	  none
 * Globals:       none
 * Returns:	  void
 */
void 
display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glRasterPos2i(0, 0);
   glDrawPixels(checkImageWidth, checkImageHeight, GL_RGB, 
                GL_UNSIGNED_BYTE, main_handler.main_window);
   glFlush();
}

/*
 * Function name: reshape
 * Description:   glut reshape callback, what actions to take when the 
 *                window is reshaped.
 * Arguments:	  w - the new width
 *                h - the new height
 * Globals:       none
 * Returns:	  void
 */
void 
reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   height = (GLint) h;
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

/*
 * Function name: motion
 * Description:   glut mouse motion callback, copies the image.
 * Arguments:	  x - x position of mouse
 *                y - y positions of mouse
 * Globals:	  none
 * Returns:	  void
 */
void 
motion(int x, int y)
{
   static GLint screeny;
   
   screeny = height - (GLint) y;
   glRasterPos2i (x, screeny);
   glCopyPixels (0, 0, checkImageWidth, checkImageHeight, GL_COLOR);
   glFlush ();
}

/*
 * Function name: mouse
 * Description:   glut mouse callback, draws lines on the image.
                  First left mouse click puts red dot at mouse location
                  Subsequent right mouse clicks draw a red line from that
                  location to current mouse location. Second left mouse
                  click resets routine and redraws the screen.
 * Arguments:	  button - which button (left, right, middle) is being pressed
                  press  - is it down?
                  x - x position of mouse
 *                y - y position of mouse (note this is from upper left corner)
 * Globals:	  none
 * Returns:	  void
 */
void
mouse(int button, int press, int x, int y)
{
  static int state, start_x, start_y;

  GLint viewport[4];
  int height;

  printf("Mouse: %d %d\n", x, y);
  switch(button)
  {
      case GLUT_LEFT_BUTTON:
	if(press == GLUT_DOWN)
        {
	   if(!state)    
           {
	     glGetIntegerv(GL_VIEWPORT, viewport);
	     height = abs(viewport[3]-viewport[1]);
             glColor3f(1.0, 0.0, 0.0);
             glBegin(GL_POINTS);
             glVertex2i(x, height - y);
	     glEnd();
	     glFlush();
             state = 1;
             start_x = x;
	     start_y = y;
	   }
	   else
           {
	     state = 0;
	     display();
	   }
	}
	break;
     case GLUT_RIGHT_BUTTON:
	if (press == GLUT_DOWN)
        {
	  glGetIntegerv(GL_VIEWPORT, viewport);
	  height = abs(viewport[3]-viewport[1]);
	  glColor3f(1.0, 0.0, 0.0);
	  glBegin(GL_LINES);
	  glVertex2i(start_x, height - start_y);
	  glVertex2i(x, height - y);
	  glEnd();
	  glFlush();
	}
	break;
  }
}

/*
 * Function name: main_loop
 * Description:   Process the line of input
 * Arguments:	  line - the line of input
 * Globals:       none
 * Returns:	  void
 */
void
main_loop(char line[])
{
    /**
    * Pass line to main_handler
    * return "exit" indicates end the program
    * return "null" indicates suppress output
    */
    std::string result;
    try {
        result = main_handler.execute(line);
    }
    catch(std::invalid_argument &excpt) {
        cerr << "Error: " << excpt.what() << endl;
        main_handler.help(); //Help for last call
    }
    catch(std::runtime_error &excpt) {
        cerr << "Error: " << excpt.what() << endl;
        result = "null";
    }

    bool newprompt = true;
    if(result=="pause") {
        printf("\nPress any key to continue...");
        result = "null";
        main_handler.wait_ack = true;
        newprompt = false;
    }

//Suppress close and exitFS commands from cmd prompt
   if(result=="close" || result=="exitFS") result = "null";
   
   if(result=="redraw") {
       glutReshapeWindow(main_handler.window_width, main_handler.window_height);
       glutPostRedisplay();
       //reshape(main_handler.window_width, main_handler.window_height);
       //display();
   } else if (line == NULL || result == "exit" || result == "quit") {
      printf("Exiting...\n");
      exit(0);
   }
   else if( result != "null" )
      cout << "RESULT: " << result << endl;

   if(newprompt)
      printf("CLI> ");
   fflush(stdout);

   return;
}

#define MAXLINE 1024

/*  ASCII Values for return, backspace, delete, escape and ctrl-d */
#define CR   13
#define BS    8
#define DEL 127
#define ESC  27
#define EOT   4

/*
 * Function name: read_next_command
 * Description:   The glut callback function which is called when a key
 *                is pressed. ESC redraws the screen, ctrl-d exits.
 * Arguments:	  key - The ASCII value of the key pressed.
 *                x   - x position of the mouse (window-relative)
 *                y   - y position of the mouse (window-relative)
 * Globals:       none
 * Returns:	  void
 */
void
read_next_command(unsigned char key, int x, int y)
{
    //Hack for pause
    if(main_handler.wait_ack) {
        printf("\n");
        main_handler.wait_ack = false;
        main_handler.read_loop();        
        if(main_handler.wait_ack) //Another pause...
            printf("\nPress any key to continue...");
        else printf("\nCLI> ");
        fflush(stdout);
        return;
    }
    //End pause
   static char line[MAXLINE];
   static int count;
  
   if(count >= MAXLINE - 1)
   {
      printf("Error: Maximum line length exceeded. Discarded input.\n");
      count = 0;
      return;
   }

   putchar(key);
   fflush(stdout);  /* Immediate output (gratification) */
   
   if (key != CR && key != BS && key != DEL && key != ESC && key != EOT)
   {
      line[count]=key;
      count++;
   }
   else if (key == BS || key == DEL)
   {
      if(count == 0)
         return;
      
      fflush(stdout);
      count--;
   }
   else if (key == CR)
   {
      printf("\n");
      line[count] = '\0';
      count = 0;
      main_loop(line);
   }
   else if (key == ESC)
   {
      glutPostRedisplay();
   }
   else
   {
      printf("Exiting...\n");
      exit(0);
   }
}

/*
 * Function name: main
 * Description:   Set up GL window and render
 * Arguments:	  command line arguments.
 * Globals:       none
 * Returns:	  void
 */
int 
main(int argc, char** argv)
{
    bool ds = (argc>1);
    if(ds) ds =(string(argv[1]) == "-ds");
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(250, 250);

   glutInitWindowPosition((ds)?3000:100, 100);
   glutCreateWindow(argv[0]);
   init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(read_next_command);
   glutMouseFunc(mouse);
   //   glutMotionFunc(motion);

   /**
	* Load modules into main handler here
	* general syntax is ModuleClass varname( handler )
	* 
	* main_handler is declared as global at beginning
	*/
   
   //main_handler.main_window = &checkImage;
   
   DummyHandle handler_mod_dummy(main_handler);
   TiffIO handler_tiff_io(main_handler);
   ImageResize handler_resample(main_handler);
   DrawTransformed handler_sgp(main_handler);
   RayTracer handler_raytrace(main_handler);

   
   printf("CLI> ");
   fflush(stdout);

   glutMainLoop();
   return 0; 
}
