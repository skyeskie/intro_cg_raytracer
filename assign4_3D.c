/*-- NOTE: You will have to manipulate this code to coorespond with the
            structures and stack implementation you chose to use. 
	    Don't forget to put declarations in your .h files. --*/




/*  Implement the following routines into your file */


#define ROW 4
#define COL 4


/* Definitions of the types of structures and variables used in the following 
   code.  Use these or implement your own. */

        typedef struct                  /* structure definitions */
        {
        float  mat[ROW][COL];
        }  matrix_unit;

        typedef struct
        {
        float i;
        float j;
        float k;
        } Vector;

        typedef struct
        {
        float x;
        float y;
        float z;
        float r;
        float g;
        float b;
        } Vertex_unit;

        typedef struct
        {
        float mat41[ROW];
        } matrix41;

        static float Near, Far;

        float w;

        int perspflag=0;

        static matrix_unit I = {
        { 1., 0., 0., 0.,
            0., 1., 0., 0.,
            0., 0., 1., 0.,
            0., 0., 0., 1  },
        };

        matrix_unit *stack[50];    /* array of pointers to act as a stack */

        int top;                   /* points to top of the stack */

        int width, height;         /* height and width of frame buffer */

        static matrix_unit orth;       /* global ortho and perspective matrices */
                                    /* to be used in Vertex3f */
        static matrix_unit perspect;


/* These go in your .h file */
void gtLookAt( float fx, float fy, float fz, float atx, float aty, 
	       float atz, float upx, float upy, float upz);
void gtVertex3f(float x, float y, float z);

/*========================================================================*/
/* Finds a cross product of two vectors                                   */
/*========================================================================*/
void Cross(Vector *x, Vector *y, Vector *z) {
    
    z->i = x->j * y->k - x->k * y->j;
    z->j = x->k * y->i - x->i * y->k;
    z->k = x->i * y->j - x->j * y->i;
}


/*========================================================================*/
/* Finds a unit vector in the direction of a given vector                 */
/*========================================================================*/
void Unitvec(float x, float y, float z, Vector *vec){
    float vec_length;
    
    vec_length = sqrt(x*x + y*y + z*z);
    vec->i = x/vec_length;
    vec->j = y/vec_length;
    vec->k = z/vec_length;
}

/*========================================================================*/
/* Multiplies a 4x4 matrix by a 4x1 vector                                */
/* Returns 0 upon success and 1 if pointers are not pointing to matrices  */
/*========================================================================*/
int Mult_end(matrix_unit *M, matrix41 *V, matrix41 *result) {
    int i, j;

    if (M == NULL || V == NULL || result == NULL) {
        printf("error in matrix multiplication \n");
        return 1;
    }
    for (i=0; i<4; i++) {
            (result->mat41)[i] = 0;
        for (j=0; j<4; j++) 
                (result->mat41)[i] += (M->mat)[i][j]*(V->mat41)[j];
    }

    return 0;
}

/*========================================================================*/
/* Multiplies two 4x4 matrices                                            */
/* Returns 0 upon success and 1 if pointers are not pointing to matrices  */
/*========================================================================*/
int Mult_mat(matrix_unit *left, matrix_unit *right, matrix_unit *result) {
    int i, j, k;

    if (left == NULL || right == NULL || result == NULL) {
        printf("error in matrix multiplication -- matrix not allocated\n");
        return 1;
    }
    for (i=0; i<4; i++) {
        for (j=0; j<4; j++) {
            (result->mat)[i][j] = 0;
            for (k=0; k<4; k++)
                (result->mat)[i][j] += (left->mat)[i][k]*(right->mat)[k][j];
        }
    }
    return 0;
}

int Copy_mat(matrix_unit *from, matrix_unit *to) {
    int i, j;
    if (from != NULL && to != NULL) {
        for (i=0; i<ROW; i++) 
            for (j=0; j<ROW; j++) 
                (to->mat)[i][j] = (from->mat)[i][j];
        return 0;
    }
    else {
        printf(" Cannot copy matrix\n");
        return 1;
    }
}


/*========================================================================*/
/*gtLookAt allows a change of the point of view from which the scene is
  is looked at.  (like a virtual camera)*/
/*========================================================================*/

void gtLookAt( float fx, float fy, float fz,
               float atx, float aty, float atz,
               float upx, float upy, float upz)
{
   float dx, dy, dz ;
   Vector slnv, rx, ry, rz, up;
   matrix_unit ltrans, tmpsln, *t, rfin, lookat;
   Vertex_unit v, sv;
   int i, j;

   /*   translation */
   ltrans = I;
   t = &ltrans;
   t->mat[0][3] = -fx;
   t->mat[1][3] = -fy;
   t->mat[2][3] = -fz; 

   up.i = upx; up.j = upy; up.k = upz;
   Unitvec(up.i, up.j, up.k, &slnv);
   up = slnv;

   /*  make P1P2  (rz) vector */
   dx = atx - fx;   dy = aty - fy;  dz = atz - fz;
   Unitvec( dx, dy, dz, &rz);  

   rfin = I;
      /* make rx */
   Cross(&rz, &up, &rx);  
   Unitvec( rx.i, rx.j, rx.k, &slnv);
   rx = slnv;

      /* make ry */
   Cross(&rx, &rz, &ry);  

   rfin.mat[0][0] = rx.i; rfin.mat[0][1] = rx.j; rfin.mat[0][2] = rx.k;
   rfin.mat[1][0] = ry.i; rfin.mat[1][1] = ry.j; rfin.mat[1][2] = ry.k;
   rfin.mat[2][0] = -rz.i; rfin.mat[2][1] = -rz.j; rfin.mat[2][2] = -rz.k;

   Mult_mat(&rfin, &ltrans, &lookat);
  
   /* Multiply the lookat matrix by the matrix currently on the top 
      of the stack.  This then becomes the new top of the stack. */

   /*  This will depend on how you implemented your stack */
  Mult_mat(stack[top], &lookat, &tmpsln);  
  Copy_mat(&tmpsln, stack[top]);
   
}
/*------------------------------End of gtLookAt -------------------------*/

/*=====================================================================*/
/*gtVertex3f specifies a 3D vertex that is a line endpoint.
  White lines are drawn between successive odd/even pairs of 
  these vertices.*/
/*=====================================================================*/

void gtVertex3f(float x, float y, float z)
 
{
  matrix41 printvec, tmp, vertex1, vertex2, pvert1, pvert2, permat1, permat2;
  float x0, y0, z0, x1, y1, z1;
  static int printflag=0;
  static matrix41 savemat;
  float xpixel1, ypixel1, xpixel2, ypixel2;

  printvec.mat41[0]=x;    /* set up 4x1 matrix of the vertex */
  printvec.mat41[1]=y;
  printvec.mat41[2]=z;
  printvec.mat41[3]=1;

/*Mult_end is a function to multiply matrix a * matrix b and put the
  solution in sln.  This function multiplies [4][4]*[4][1].  This will
  vary depending on your implementation. */

  Mult_end(stack[top],&printvec, &tmp);

  printflag++;       /* increase counter */
  
  if(printflag==1) { /* if the first vertex, save the points */
    savemat.mat41[0]=tmp.mat41[0];
    savemat.mat41[1]=tmp.mat41[1];
    savemat.mat41[2]=tmp.mat41[2];
    savemat.mat41[3]=1;
  }
  if(printflag==2) { /* if the second vertex, test clipping */

    x1=tmp.mat41[0];
    y1=tmp.mat41[1];
    z1=tmp.mat41[2];

    x0=savemat.mat41[0];
    y0=savemat.mat41[1];
    z0=savemat.mat41[2];
  
    /* if clipping occurs and points are within view volume, draw line */
    /* from v1 to v2 */
    if(near_far_clip(Near, Far, &x0, &y0, &z0, &x1, &y1, &z1)==1) {
	
	pvert1.mat41[0]=x0;
	pvert1.mat41[1]=y0; 
	pvert1.mat41[2]=z0;
	pvert1.mat41[3]=1;
	pvert2.mat41[0]=x1;
	pvert2.mat41[1]=y1;
	pvert2.mat41[2]=z1;
	pvert2.mat41[3]=1;

	if(perspflag==0) {  /* if not a perpective projection, use ortho */

	  Mult_end(&orth, &pvert1, &vertex1); /* calculate 2d coordinates */
	  Mult_end(&orth, &pvert2, &vertex2);

	  draw_line(vertex1.mat41[0], vertex1.mat41[1], 
		    vertex2.mat41[0], vertex2.mat41[1]);
	  
	  printflag=0;
	}
	else {   /* if a perspective projection, use persp */
	  
	  permat1.mat41[0]=x0/abs(z0);   /* divide by abs(z) to account for */
	  permat1.mat41[1]=y0/abs(z0);  /* z=1 assumption in gtPerspective */
	  permat1.mat41[2]=1.0;
	  permat1.mat41[3]=(float)(1/abs(z0));
	  permat2.mat41[0]=x1/abs(z1);
	  permat2.mat41[1]=y1/abs(z1);
	  permat2.mat41[2]=1.0;
	  permat2.mat41[3]=(float)(1/abs(z1));
	  
	  Mult_end(&perspect, &permat1, &vertex1);
	  Mult_end(&perspect, &permat2, &vertex2);
	  
	  draw_line(vertex1.mat41[0], 
		    vertex1.mat41[1], 
		    vertex2.mat41[0], 
		    vertex2.mat41[1]);
	   

	  printflag=0; /* set counter=0 to look for another set of vertices */
	}
    }

    else
      printflag=0;  /* if vertices beyond clipping plane, reset counter */

  }
}
  
/*--------------------End of gtVertex3f----------------------------------*/





