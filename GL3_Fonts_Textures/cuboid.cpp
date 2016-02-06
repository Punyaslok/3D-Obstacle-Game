#include "essentials.h"
#include <iostream>

class Cuboid
{
public:
	VAO *cuboid;
	GLfloat centre_x, centre_y, centre_z, l, b, h;
	void initialize (GLfloat cx, GLfloat cy, GLfloat cz, GLfloat len, GLfloat br, GLfloat ht)
	{
		// centre_x and centre_y are bottom corner coordinates
		// l is associated with x, b with y, and h with z
		centre_x = cx;	centre_y = cy;	centre_z = cz;	l = ht;		b = len;		h = br;

		GLfloat vertex_buffer_data[]={
			// Face IV
			0,0,0,
		    b,0,0,
		    b,0,l,

		    0,0,0,
		    b,0,l,
		    0,0,l,

		    // Face II
		    0,h,0,
		    b,h,0,
		    b,h,l,

		    0,h,0,
		    b,h,l,
		    0,h,l,

		    // Face I

		    0,0,0,
		    0,h,0,
		    0,h,l,

		    0,h,l,
		    0,0,0,
		    0,0,l,

		    //Face III

		    b,0,0,
		    b,h,0,
		    b,h,l,

		    b,h,l,
		    b,0,0,
		    b,0,l,

		    //Face VI

		    0,0,0,
		    b,0,0,
		    b,h,0,

		    0,0,0,
		    b,h,0,
		    0,h,0,

		    //Face V

		    0,0,l,
		    b,0,l,
		    b,h,l,

		    0,0,l,
		    b,h,l,
		    0,h,l
		};

    GLfloat color_buffer_data[] = {

	//Face IV
     base_bottom_R, base_bottom_G, base_bottom_B,
     base_bottom_R, base_bottom_G, base_bottom_B,
     base_top_R, base_top_G, base_top_B,

     base_bottom_R, base_bottom_G, base_bottom_B,
     base_top_R, base_top_G, base_top_B,
     base_top_R, base_top_G, base_top_B,
	
	//Face II
     base_bottom_R, base_bottom_G, base_bottom_B,
     base_bottom_R, base_bottom_G, base_bottom_B,
     base_top_R, base_top_G, base_top_B,

     base_bottom_R, base_bottom_G, base_bottom_B,
     base_top_R, base_top_G, base_top_B,
     base_top_R, base_top_G, base_top_B,
	
	//Face I
     base_bottom_R, base_bottom_G, base_bottom_B,
     base_bottom_R, base_bottom_G, base_bottom_B,
     base_top_R, base_top_G, base_top_B,

     base_top_R, base_top_G, base_top_B,
     base_bottom_R, base_bottom_G, base_bottom_B,
     base_top_R, base_top_G, base_top_B,

     //Face III

     base_bottom_R, base_bottom_G, base_bottom_B,
     base_bottom_R, base_bottom_G, base_bottom_B,
     base_top_R, base_top_G, base_top_B,

     base_top_R, base_top_G, base_top_B,
     base_bottom_R, base_bottom_G, base_bottom_B,
     base_top_R, base_top_G, base_top_B,

     //Face VI

     base_bottom_R, base_bottom_G, base_bottom_B,
     base_bottom_R, base_bottom_G, base_bottom_B,
     base_bottom_R, base_bottom_G, base_bottom_B,

     base_bottom_R, base_bottom_G, base_bottom_B,
	 base_bottom_R, base_bottom_G, base_bottom_B,
     base_bottom_R, base_bottom_G, base_bottom_B,

     //Face V

     base_top_R, base_top_G, base_top_B,
     base_top_R, base_top_G, base_top_B,
     base_top_R, base_top_G, base_top_B,

     base_top_R, base_top_G, base_top_B,
     base_top_R, base_top_G, base_top_B,
     base_top_R, base_top_G, base_top_B

 };
     
 	cuboid = create3DObject(GL_TRIANGLES, 36, vertex_buffer_data, color_buffer_data, GL_FILL);
	}

	GLfloat length(){return b;}
	GLfloat breadth(){return h;}
	GLfloat height(){return l;}
	int is_on_cuboid ( GLfloat ax, GLfloat ay )
	{
		if (ax>=centre_x && ax<=centre_x+length() && ay>=centre_y && ay<=centre_y+breadth())
			return 1;
		return 0;
	}


	void drawCuboid(glm::mat4 VP)
	{
		glm::mat4 MVP2;
		 
		Matrices.model = glm::mat4(1.0f);
      	GLfloat ang = 0;
      	//printf("%f\n", ang);
      	glm::mat4 translateCuboid = glm::translate (glm::vec3(centre_x, centre_y, centre_z)); // glTranslatef
      	//std::cout<<centre_x<<" "<<centre_y<<" "<<centre_z<<"\n";
      	//glm::mat4 rotateTriangle = glm::rotate(ang, glm::vec3(0,0,1));  // rotate about vector (0,0,1) Rotating about z-axis
      	//glm::mat4 translateTriangle2 = glm::translate (glm::vec3(centre_x, centre_y, centre_z)); // glTranslatef

      	Matrices.model *= (translateCuboid);
      	MVP2 = VP * Matrices.model; // MVP = p * V * M
      	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP2[0][0]);
		draw3DObject(cuboid);
	}
};