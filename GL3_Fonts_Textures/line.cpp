#include "essentials.h"

class Line
{
	VAO *line, *xAxis;
	GLfloat x1, y1, z1, x2, y2, z2, R1, G1, B1, R2, G2, B2;
	public:
	void initialize(GLfloat xa, GLfloat ya, GLfloat za, GLfloat xb, GLfloat yb, GLfloat zb, GLfloat col_Ra, GLfloat col_Ga, GLfloat col_Ba, GLfloat col_Rb, GLfloat col_Gb, GLfloat col_Bb)
	{
		x1 = xa;	y1=ya;	z1=za;	x2=xb;	y2=yb;	z2=zb;
		R1=col_Ra;	G1=col_Ga;	B1=col_Ba;
		R2=col_Rb;	G2=col_Gb;	B2=col_Bb;

	    GLfloat vertex_buffer_data[] = { x1, y1, z1,	x2, y2, z2 };

	    GLfloat color_buffer_data[] = {R1, G1, B1,		R2, G2, B2};

	    line = create3DObject(GL_LINES, 2, vertex_buffer_data, color_buffer_data, GL_FILL);
	}

	void drawLine ()
	{
		    draw3DObject(line);
	}
};