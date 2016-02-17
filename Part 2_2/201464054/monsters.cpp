#include "essentials.h"
#include <iostream>

class Sphere
{
public:
	VAO *sphere;
	GLfloat centre_x, centre_y, centre_z, radius;
	pair<int, int> curr_tile_ind, dest_tile_ind;	//sphere is on which tile currently, and heading towards which tile
	int move_direction, speed;		// monster will move as separation between 2 tiles divided by speed. So less the value of speed, the faster it moves
	// Move direction : 0 for left, 1 for right, 2 for up, 3 for down

	void initialize (GLfloat cx, GLfloat cy, GLfloat cz, GLfloat rad,int moving=0, int ver_move_speed=0, int ver_move_direction=1)
	{
		// centre_x and centre_y are bottom corner coordinates
		// l is associated with x, b with y, and h with z
		centre_x = cx;	centre_y = cy;	centre_z = cz;	radius = rad;

		GLfloat vertex_buffer_data[180*360*3];
		int ct=0;

		for (int i=0;i<180;i++)		// i is azimuthal, j is elevation
			for (int j=0;j<360;j++)
			{
				vertex_buffer_data[ct++] = radius * cos(deg2rad(i)) * cos(deg2rad(j));
				vertex_buffer_data[ct++] = radius * sin(deg2rad(i)) * cos(deg2rad(j));
				vertex_buffer_data[ct++] = radius * sin(deg2rad(j));
			}
					//std::cout<<"sdhfgsdjfgdsf"<<endl;
	ct=0;
    GLfloat color_buffer_data[180*360*3];
    for (int i=0;i<180;i++)		// i is azimuthal, j is elevation
			for (int j=0;j<360;j++)
			{
				color_buffer_data[ct++] = monsters_R;
				color_buffer_data[ct++] = monsters_G;
				color_buffer_data[ct++] = monsters_B;
			}
     
 	sphere = create3DObject(GL_POINTS, 180*360, vertex_buffer_data, color_buffer_data, GL_FILL);
	}

	void drawSphere(glm::mat4 VP)
	{
		glm::mat4 MVP2;
		 
		Matrices.model = glm::mat4(1.0f);

      	glm::mat4 translateSphere = glm::translate (glm::vec3(centre_x, centre_y, centre_z)); // glTranslatef

      	Matrices.model *= (translateSphere);
      	MVP2 = VP * Matrices.model; // MVP = p * V * M
      	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP2[0][0]);
		draw3DObject(sphere);
	}
};