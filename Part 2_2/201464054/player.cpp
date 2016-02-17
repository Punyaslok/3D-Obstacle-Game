#include "essentials.h"
#include <iostream>

class Player
{
public:
	VAO *cuboid;
	GLfloat centre_x, centre_y, centre_z, l, b, h;
	GLfloat after_rot_centre_x, after_rot_centre_y;
	GLfloat vertical_speed;
	GLfloat azimuthal_angle, elevation_angle;
	void initialize (GLfloat cx, GLfloat cy, GLfloat cz, GLfloat len, GLfloat br, GLfloat ht)
	{
		azimuthal_angle = 90;	elevation_angle = 0;
		vertical_speed = 0;
		// centre_x and centre_y are bottom corner coordinates
		// l is associated with x, b with y, and h with z
		centre_x = after_rot_centre_x = cx;	centre_y = after_rot_centre_y = cy;	centre_z = cz;	l = ht;		b = len;		h = br;

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
		    0,h,l,

		    //Front triangles begin
		    0,h,0,
		    0,h,l,
		    b/2,h+(h/2),0,

		    b,h,0,
		    b,h,l,
		    b/2,h+(h/2),0,


		    0,h,l,
		    b,h,l,
		    b/2,h+(h/2),0,

		    0,h,0,
		    b,h,0,
		    b/2,h+(h/2),0

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
	     player_top_R, player_top_G, player_top_B,

	     // Triangles color
	     0,0,1,
	     0,0,1,
	     0,0,1,

	     0,0,1,
	     0,0,1,
	     0,0,1,

	     0,0,1,
	     0,0,1,
	     0,0,1,

	     0,0,1,
	     0,0,1,
	     0,0,1

	 	};
	 	cuboid = create3DObject(GL_TRIANGLES, 48, vertex_buffer_data, color_buffer_data, GL_FILL);
	}

	void jump()
	{
		if (vertical_speed==0)
			vertical_speed=5;
		//cout<<"jump"<<endl;
		return;
	}
	vector <GLfloat> camera_centres()
	{
		GLfloat tmp_len = length(), tmp_bre = breadth(), tmp_r = sqrt(  (tmp_len*tmp_len) + (tmp_bre*tmp_bre)  )/2;
		GLfloat tmp_x = ((2*centre_x)+tmp_len)/2, tmp_y = ((2*centre_y)+tmp_bre)/2;
		vector <GLfloat> ret;	ret.resize(3);
		ret[0] = tmp_x;	ret[1] = tmp_y;	ret[2] = centre_z+height()+50;
		return ret;
	}
	void increase_azimuthal(GLfloat val)
	{
		azimuthal_angle++;
    	if (azimuthal_angle>360)
    		azimuthal_angle-=360;
	}
	void decrease_azimuthal(GLfloat val)
	{
		azimuthal_angle--;
    	if (azimuthal_angle<0)
    		azimuthal_angle+=360;
	}


	void increase_elevation(GLfloat val)
	{
		elevation_angle++;
		if (elevation_angle>85)
			elevation_angle=85;
	}
	void decrease_elevation(GLfloat val)
	{
		elevation_angle--;
		if (elevation_angle<-40)
			elevation_angle=-40;
	}


	GLfloat length(){return b;}
	GLfloat breadth(){return h;}
	GLfloat height(){return l;}


	void drawCuboid(glm::mat4 VP)
	{
		glm::mat4 MVP2;
		 
		Matrices.model = glm::mat4(1.0f);
      	//printf("%f\n", ang);
      	glm::mat4 translateCuboid = glm::translate (glm::vec3(-(length()/2), -(breadth()/2), 0)); // glTranslatef
      	//glm::mat4 translateCuboid = glm::translate (glm::vec3(-centre_x, -centre_y, -centre_z));
      	//std::cout<<centre_x<<" "<<centre_y<<" "<<centre_z<<"\n";
      	glm::mat4 rotateCuboid = glm::rotate(deg2rad(azimuthal_angle-90), glm::vec3(0,0,1));  // rotate about vector (0,0,1) Rotating about z-axis
      	glm::mat4 translateCuboid2 = glm::translate (glm::vec3(centre_x+(length()/2), centre_y+(breadth()/2), centre_z)); // glTranslatef

      	Matrices.model *= (translateCuboid2*rotateCuboid*translateCuboid);
      	MVP2 = VP * Matrices.model; // MVP = p * V * M
      	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP2[0][0]);
		draw3DObject(cuboid);
	}
};