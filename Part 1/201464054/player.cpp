#include "essentials.h"
#include <iostream>

class Player
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
			0,0,0,
		    b,0,0,
		    b,0,l,

		    0,0,0,
		    b,0,l,
		    0,0,l,

		    0,h,0,
		    b,h,0,
		    b,h,l,

		    0,h,0,
		    b,h,l,
		    0,h,l,

		    0,0,0,
		    0,h,0,
		    0,h,l,

		    0,h,l,
		    0,0,0,
		    0,0,l,

		    b,0,0,
		    b,h,0,
		    b,h,l,

		    b,h,l,
		    b,0,0,
		    b,0,l,

		    0,0,0,
		    b,0,0,
		    b,h,0,

		    0,0,0,
		    b,h,0,
		    0,h,0,

		    0,0,l,
		    b,0,l,
		    b,h,l,

		    0,0,l,
		    b,h,l,
		    0,h,l
		};

    GLfloat color_buffer_data[] = {

	//Face IV
     player_bottom_R, player_bottom_G, player_bottom_B,
     player_bottom_R, player_bottom_G, player_bottom_B,
     player_top_R, player_top_G, player_top_B,

     player_bottom_R, player_bottom_G, player_bottom_B,
     player_top_R, player_top_G, player_top_B,
     player_top_R, player_top_G, player_top_B,
	
	//Face II
     player_bottom_R, player_bottom_G, player_bottom_B,
     player_bottom_R, player_bottom_G, player_bottom_B,
     player_top_R, player_top_G, player_top_B,

     player_bottom_R, player_bottom_G, player_bottom_B,
     player_top_R, player_top_G, player_top_B,
     player_top_R, player_top_G, player_top_B,
	
	//Face I
     player_bottom_R, player_bottom_G, player_bottom_B,
     player_bottom_R, player_bottom_G, player_bottom_B,
     player_top_R, player_top_G, player_top_B,

     player_top_R, player_top_G, player_top_B,
     player_bottom_R, player_bottom_G, player_bottom_B,
     player_top_R, player_top_G, player_top_B,

     //Face III

     player_bottom_R, player_bottom_G, player_bottom_B,
     player_bottom_R, player_bottom_G, player_bottom_B,
     player_top_R, player_top_G, player_top_B,

     player_top_R, player_top_G, player_top_B,
     player_bottom_R, player_bottom_G, player_bottom_B,
     player_top_R, player_top_G, player_top_B,

     //Face VI

     player_bottom_R, player_bottom_G, player_bottom_B,
     player_bottom_R, player_bottom_G, player_bottom_B,
     player_bottom_R, player_bottom_G, player_bottom_B,

     player_bottom_R, player_bottom_G, player_bottom_B,
	 player_bottom_R, player_bottom_G, player_bottom_B,
     player_bottom_R, player_bottom_G, player_bottom_B,

     //Face V

     player_top_R, player_top_G, player_top_B,
     player_top_R, player_top_G, player_top_B,
     player_top_R, player_top_G, player_top_B,

     player_top_R, player_top_G, player_top_B,
     player_top_R, player_top_G, player_top_B,
     player_top_R, player_top_G, player_top_B

 };
 	cuboid = create3DObject(GL_TRIANGLES, 36, vertex_buffer_data, color_buffer_data, GL_FILL);
	}

	void move_up(GLfloat val)
	{
		centre_y+=val;
	}
	void move_down(GLfloat val)
	{
		centre_y-=val;
	}
	void move_left(GLfloat val)
	{
		centre_x-=val;
	}
	void move_right(GLfloat val)
	{
		centre_x+=val;
	}
	GLfloat length(){return b;}
	GLfloat breadth(){return h;}
	GLfloat height(){return l;}


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