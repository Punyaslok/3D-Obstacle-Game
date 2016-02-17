#include <bits/stdc++.h>
#include <irrklang/irrKlang.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext.hpp"

#include <glad/glad.h>
#include <FTGL/ftgl.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include "line.cpp"
#include "cuboid.cpp"
#include "essentials.h"
#include "player.cpp"
#include "monsters.cpp"

using namespace std;
using namespace irrklang;

ISoundEngine *SoundEngine = createIrrKlangDevice();

glm::vec3 getRGBfromHue (int hue)
{
	float intp;
	float fracp = modff(hue/60.0, &intp);
	float x = 1.0 - abs((float)((int)intp%2)+fracp-1.0);

	if (hue < 60)
		return glm::vec3(1,x,0);
	else if (hue < 120)
		return glm::vec3(x,1,0);
	else if (hue < 180)
		return glm::vec3(0,1,x);
	else if (hue < 240)
		return glm::vec3(0,x,1);
	else if (hue < 300)
		return glm::vec3(x,0,1);
	else
		return glm::vec3(1,0,x);
}

GLuint programID, fontProgramID, textureProgramID;

/* Function to load Shaders - Use it as it is */
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	cout << "Compiling shader : " <<  vertex_file_path << endl;
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage( max(InfoLogLength, int(1)) );
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	cout << VertexShaderErrorMessage.data() << endl;

	// Compile Fragment Shader
	cout << "Compiling shader : " << fragment_file_path << endl;
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage( max(InfoLogLength, int(1)) );
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	cout << FragmentShaderErrorMessage.data() << endl;

	// Link the program
	cout << "Linking program" << endl;
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	cout << ProgramErrorMessage.data() << endl;

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

static void error_callback(int error, const char* description)
{
	cout << "Error: " << description << endl;
}

void quit(GLFWwindow *window)
{
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}


/**************************
 * Customizable functions *
 **************************/

Line x_axis, y_axis, z_axis;
Cuboid base[100][100], example;
Sphere monsters[10];
Player player;

long long int fact(long long int n)
{
	long long int ret=1;
	for (long long int i=2;i<=n;i++)ret*=i;
	return ret;
}

void generate_layout()
{
	int arr[] = {0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1};	// 0 for up, 1 for right, First and last cells must exist
	srand (time(NULL));
	long long int tmp_randomize = rand()%(fact(9)/(fact(5)*fact(4)));
	//cout<<tmp_randomize<<endl;
	for (long long int i=0;i<tmp_randomize;i++){next_permutation(arr, arr+9);next_permutation(arr+10, arr+18);}
	//for (int i=0;i<18;i++)cout<<arr[i]<<" ";cout<<endl;
	int tr=0,tc=0,path_arr[10][10];
	memset(path_arr, 0, sizeof(path_arr));
	path_arr[0][0] = path_arr[9][9] = 1;
	for (int i=0;i<18;i++)
	{
		if (arr[i]==0)tr++;
		else if (arr[i]==1)tc++;
		path_arr[tr][tc]=1;
	}
	for (int i=0;i<10;i++)
		for (int j=0;j<10;j++)
		{
			if (path_arr[i][j]==1)
			{
				layout[i][j]=1;
				continue;
			}
			int tmp = rand()%10;
			if (tmp<2)
				layout[i][j]=0;
			else if (tmp==2)
				layout[i][j]=2;
			else
				layout[i][j]=1;
		}
	layout[0][1] = 2;
	return;
}
void initialize_monsters()
{
	monster_ct = 3 + (rand()%15);
	monsters_R = 255.0/255.0; monsters_G = 215.0/255.0;	monsters_B = 0.0/255.0;
	int monster_radius = 20;
	for (int i=0;i<monster_ct;i++)
	{
		int tr=(rand()%4)+5, tc=rand()%8+1;
		if (layout[tr][tc]==0)
			i--;
		else
		{
			monsters[i].initialize(		base[tr][tc].centre_x+(base[tr][tc].length()/2),	base[tr][tc].centre_y+(base[tr][tc].breadth()/2)	, 	base[tr][tc].centre_z+base[tr][tc].height()+monster_radius, monster_radius);
			//monsters[i].speed = (rand()%(15-10+1))+10;
			//monsters[i].move_direction = 0;//rand()%4;
			//monsters[i].curr_tile_ind = make_pair(tr, tc);
			/*if (monsters[i].move_direction==0)
				monsters[i].dest_tile_ind = make_pair(tr, tc-1);
			else if (monsters[i].move_direction==1)
				monsters[i].dest_tile_ind = make_pair(tr, tc+1);
			else if (monsters[i].move_direction==2)
				monsters[i].dest_tile_ind = make_pair(tr+1, tc);
			else if (monsters[i].move_direction==2)
				monsters[i].dest_tile_ind = make_pair(tr-1, tc);*/
			//cout<<tr<<" "<<tc<<"direction : "<<monsters[i].move_direction<<"  speed :  "<<monsters[i].speed<<endl;
		}
	}
}
void move_monsters()
{
	for (int i=0;i<monster_ct;i++)
	{
		Sphere tmp = monsters[i];
		pair <int, int> curr_tile = monsters[i].curr_tile_ind, dest_tile = monsters[i].dest_tile_ind;
		GLfloat tile_separation = base[0][0].length()+dx;
		if (tmp.move_direction == 0)
		{	cout<<dist(monsters[i].centre_x, monsters[i].centre_y, base[dest_tile.first][dest_tile.second].centre_x + (base[dest_tile.first][dest_tile.second].length()/2), base[dest_tile.first][dest_tile.second].centre_y + (base[dest_tile.first][dest_tile.second].breadth()/2))<<endl;
			if (dist(monsters[i].centre_x, monsters[i].centre_y, base[dest_tile.first][dest_tile.second].centre_x + (base[dest_tile.first][dest_tile.second].length()/2), base[dest_tile.first][dest_tile.second].centre_y + (base[dest_tile.first][dest_tile.second].breadth()/2))<5)
			{
				monsters[i].centre_x = base[dest_tile.first][dest_tile.second].centre_x + (base[dest_tile.first][dest_tile.second].length()/2);
				monsters[i].centre_y = base[dest_tile.first][dest_tile.second].centre_y + (base[dest_tile.first][dest_tile.second].breadth()/2);
				monsters[i].curr_tile_ind = dest_tile;
				monsters[i].move_direction = 1;
			}
			else
			{
				monsters[i].centre_x -= (tile_separation/monsters[i].speed);
			}
			/*if (curr_tile.first>0 && layout[curr_tile_ind.first-1][curr_tile_ind.second] ==1)
			{
				dest_tile = make_pair(curr_tile_ind.first-1, curr_tile_ind.second);
			}*/
		}
		else if (tmp.move_direction == 1)
		{

		}
	}
	return;
}
void initialize_all()
{
	level=1;
	lives = 5;	score = 100000;
	right_click_flag = 0;	gravity = 0.1;

	//x_axis.initialize(-10000, 0, 0, 	10000, 0, 0, 	1, 0, 0,	1, 0, 0);
	//y_axis.initialize(0, -10000, 0, 	0, 10000, 0, 	0, 1, 0,	0, 1, 0);
	//z_axis.initialize(0, 0, -10000, 	0, 0, 10000, 	0, 0, 1,	0, 0, 1);

	generate_layout();
	base_top_R = 72.0/255.0;		base_top_G = 72.0/255.0;		base_top_B = 72.0/255.0;
	base_bottom_R = 255.0/255.0;	base_bottom_G = 255.0/255.0;	base_bottom_B = 255.0/255.0;
	block_l = 50, block_b = 50, block_h =  100;
	dx = 10, dy = 10, dz = 10;
	for (int i=0;i<10*level;i++)
		for (int j=0;j<10*level;j++)
		{
			if (layout[i][j]==1)
				base[i][j].initialize(j*(block_l+dx), i*(block_b+dy), -block_h /*+ (rand()%(((int)block_h)/2))*/, block_l, block_b, block_h);
			else if (layout[i][j]==2)
			{
				int tmp_dir = rand()%2, tmp_speed = (rand()%(4-1+1))+1;		if(tmp_dir==0)tmp_dir=-1;
				//cout<<i<<" "<<j<<" "<<tmp_dir<<" "<<tmp_speed<<endl;
				base[i][j].initialize(j*(block_l+dx), i*(block_b+dy), -block_h, block_l, block_b, block_h, 1, tmp_speed, tmp_dir);
			}
		}

	//example.initialize(200,200,0, 60,60,60);


	//Player stuff
	speed=5;
	player_top_R = 0.0/255.0;	 	player_top_G = 255.0/255.0;	 player_top_B = 0.0/255.0;
	player_bottom_R = 0.0/255.0;	 player_bottom_G = 65.0/255.0;	player_bottom_B = 0.0/255.0;
	player.initialize(base[0][0].centre_x,base[0][0].centre_y,base[0][0].centre_z+base[0][0].height(), 20,20,20);


	//monsters stuff
	//initialize_monsters();


	//	Initializing camera
	current_view = 1;

	top_view_eye = glm::vec3(		(5*(block_l+dx)) - (dx/2),	(5*(block_b+dy)) - (dy/2),		200);
	top_view_tar = glm::vec3(		(5*(block_l+dx)) - (dx/2),	(5*(block_b+dy)) - (dy/2),		0);
	top_view_up = glm::vec3(0, 1, 0);

	tower_view_eye = glm::vec3(		-40, -40	,		300);
	tower_view_tar = glm::vec3(		(9*(block_l+dx)),	(9*(block_b+dy)),		0);
	tower_view_up = glm::vec3(0, 0, 1);

	follow_view_eye = glm::vec3(player.centre_x - 20, player.centre_y - 20, 50);
	follow_view_tar = glm::vec3(player.centre_x + (player.length()/2), player.centre_y + (player.breadth()/2), player.height()/2);
	follow_view_up = glm::vec3(0, 0, 1);

	adventurer_view_eye = glm::vec3((2*player.centre_x + player.length())/2, (2*player.centre_y + player.breadth())/2, 2*player.height());
	adventurer_view_tar = glm::vec3(player.centre_x + (player.length()/2), player.centre_y + (player.breadth()/2) +200, 0);
	adventurer_view_up = glm::vec3(0,0,1);

	helicopter_view_dist_max = 500;	helicopter_view_dist_min = 100;
	helicopter_view_dist = 200; helicopter_view_azimuthal = 45; helicopter_view_elevation = 30;
	vector <GLfloat> temp_coordinates = polar_to_cartesian(helicopter_view_dist, helicopter_view_azimuthal, helicopter_view_elevation);
	helicopter_view_eye = glm::vec3(player.centre_x + temp_coordinates[0], player.centre_y + temp_coordinates[1], temp_coordinates[2]);
	helicopter_view_tar = glm::vec3(player.centre_x + (player.length()/2), player.centre_y + (player.breadth()/2), player.height()/2);
	helicopter_view_up = glm::vec3(0, 0, 1);

	cam_eye = top_view_eye;
	cam_tar = top_view_tar;
	cam_up = top_view_up;
	cam_rot_ang = 0;

}

void toggle_camera( int val )
{
	current_view = val;
	if (val==2)
	{
		cam_eye = tower_view_eye;
		cam_tar = tower_view_tar;
		cam_up = tower_view_up;
	}
	else if (val==1)
	{
		cam_eye = top_view_eye;
		cam_tar = top_view_tar;
		cam_up = top_view_up;
	}
	else if (val==3)
	{
		cam_eye = adventurer_view_eye;
		cam_tar = adventurer_view_tar;
		cam_up = adventurer_view_up;
	}
	else if (val==4)
	{
		cam_eye = follow_view_eye;
		cam_tar = follow_view_tar;
		cam_up = follow_view_up;
	}
	else if (val==5)
	{
		cam_eye = helicopter_view_eye;
		cam_tar = helicopter_view_tar;
		cam_up = helicopter_view_up;
	}
	return;
}
void update_follow_view()
{
	follow_view_eye = glm::vec3(player.centre_x - 20, player.centre_y - 20, 50);
	follow_view_tar = glm::vec3(player.centre_x + (player.length()/2), player.centre_y + (player.breadth()/2), player.height()/2);
	if (current_view == 4)
	{
		cam_eye = follow_view_eye;
		cam_tar = follow_view_tar;
		cam_up = follow_view_up;
	}
	return;
}

void update_helicopter_view()
{
	//helicopter_view_eye = glm::vec3(player.centre_x - 20, player.centre_y - 20, 100);
	vector <GLfloat> temp_coordinates = polar_to_cartesian(helicopter_view_dist, helicopter_view_azimuthal, helicopter_view_elevation);
	//cout<<temp_coordinates[0]<<" "<<temp_coordinates[1]<<" "<<temp_coordinates[2]<<"  -----  "<<sqrt(temp_coordinates[0]*temp_coordinates[0] + temp_coordinates[1]*temp_coordinates[1])<<endl;
	helicopter_view_eye = glm::vec3(player.centre_x + temp_coordinates[0], player.centre_y + temp_coordinates[1], temp_coordinates[2]);
	helicopter_view_tar = glm::vec3(player.centre_x + (player.length()/2), player.centre_y + (player.breadth()/2), player.height()/2);
	if (current_view == 5)
	{
		cam_eye = helicopter_view_eye;
		cam_tar = helicopter_view_tar;
		cam_up = helicopter_view_up;
	}
	return;
}

void update_adventurer_view()
{
	//cout<<player.centre_x<<" "<<player.centre_y<<endl;
	vector <GLfloat> tmp_cam = player.camera_centres();
	//cout<<"camera eye : "<<tmp_cam[0]<<" "<<tmp_cam[1]<<" "<<tmp_cam[2]<<endl;
	adventurer_view_eye = glm::vec3(tmp_cam[0], tmp_cam[1], tmp_cam[2]);
	vector <GLfloat> tmpc = polar_to_cartesian(300, player.azimuthal_angle, player.elevation_angle);
	//cout<<"camera tar : "<<tmp_cam[0] + tmpc[0] <<" "<< tmp_cam[1] + tmpc[1] <<" "<< tmp_cam[2]+tmpc[2]<<endl;
	adventurer_view_tar = glm::vec3(tmp_cam[0] + tmpc[0], tmp_cam[1] + tmpc[1], tmp_cam[2]+tmpc[2]);
	if (current_view==3)
	{
		cam_eye = adventurer_view_eye;
		cam_tar = adventurer_view_tar;
		cam_up = adventurer_view_up;
	}
	return;
}
void reset_player()
{
	player.centre_x = player.centre_y = player.vertical_speed = 0;
	player.azimuthal_angle = 90;	player.elevation_angle=0;
	update_adventurer_view();
	update_follow_view();
	lives--;
	score-=1000;
	if (lives==0)
	{
		cout<<"Final Score : "<<score<<endl;
	}
	return;
}

pair <int, int> clear_for_gravity()
{
	int ret = 1, val=INT_MIN, no_contact=1;
	for (int i=0;i<10;i++)
		for (int j=0;j<10;j++)
		{
			if (layout[i][j]>0)
			{				
				int f1 = base[i][j].is_on_cuboid(player.centre_x, player.centre_y);
				int f2 = base[i][j].is_on_cuboid(player.centre_x+player.length(), player.centre_y);
				int f3 = base[i][j].is_on_cuboid(player.centre_x, player.centre_y+player.breadth());
				int f4 = base[i][j].is_on_cuboid(player.centre_x+player.length(), player.centre_y+player.breadth());
				if (f1 || f2 || f2 || f4)
				{
					if (layout[i][j]==2)
					{
						cout<<abs(player.vertical_speed-base[i][j].vertical_move_speed)<<endl;
						if (abs(player.vertical_speed-base[i][j].vertical_move_speed) > 7)
							reset_player();
					}
					no_contact=0;
					if (player.centre_z + player.vertical_speed - gravity< base[i][j].centre_z + base[i][j].height())
					{
						ret = 0;
						player.vertical_speed = 0;
						val = max(val, (int)(base[i][j].centre_z + base[i][j].height()));
					}
					else if (ret==1)
					{
						player.vertical_speed -= gravity;
						player.centre_z += player.vertical_speed;
					}
				}
			}
		}
	if (!ret)
		player.centre_z = val;
	if (no_contact==1)
	{
		player.vertical_speed -= gravity;
		player.centre_z += player.vertical_speed;
		if (player.centre_z<-500)
			reset_player();
	}

	//cout<<player.centre_z<<" "<<player.vertical_speed<<" "<<ret<<endl;
	return make_pair(ret, val);
}
void upadte_player_wrt_tiles()
{
	if (clear_for_gravity().first)
		return;

	int not_in_contact_stationary=0, total_stationary_checked=0, moving_tile_flag=0;
	Cuboid tmp_moving_tile;
	for (int i=0;i<10;i++)
		for (int j=0;j<10;j++)
		{
			if (layout[i][j]>0)
			{

				int f1 = base[i][j].is_on_cuboid(player.centre_x, player.centre_y);
				int f2 = base[i][j].is_on_cuboid(player.centre_x+player.length(), player.centre_y);
				int f3 = base[i][j].is_on_cuboid(player.centre_x, player.centre_y+player.breadth());
				int f4 = base[i][j].is_on_cuboid(player.centre_x+player.length(), player.centre_y+player.breadth());
				if (layout[i][j]==1)
				{
					total_stationary_checked++;
					if ( (player.centre_z > base[0][0].centre_z + base[i][j].height()) || ( (!f1) && (!f2) && (!f3) && (!f4) ))
						not_in_contact_stationary++;
					/*if ( (player.centre_z <= base[0][0].centre_z + base[i][j].height()) && !( (!f1) && (!f2) && (!f3) && (!f4) ))
						cout<<i<<" "<<j<<" ---- "<<endl;
					*///cout<<base[0][1].centre_x<<" "<<player.centre_x + player.length()<<endl;
				}
				else if (layout[i][j]==2)
				{
					if (f1||f2||f3||f4)
					{
						tmp_moving_tile = base[i][j];	
						moving_tile_flag = 1;
					}
				}
				//	cout<<"i = "<<i<<" j = "<<j<<endl;
			}
		}
	//cout<<totct<<" "<<ct<<endl;
	//cout<<not_in_contact_stationary<<" "<<total_stationary_checked<<endl;
	if (moving_tile_flag==1)
	{
		if (not_in_contact_stationary==total_stationary_checked)
			player.centre_z = tmp_moving_tile.centre_z + tmp_moving_tile.height();
		else
			player.centre_z = max(base[0][0].centre_z + base[0][0].height(), tmp_moving_tile.centre_z + tmp_moving_tile.height());
		return;
	}
	if (not_in_contact_stationary==total_stationary_checked)		// Will fall down
		return;
	return;
}
int can_move(GLfloat xa, GLfloat ya)
{

}
void move_up(GLfloat val)
{
		player.centre_x+=(val*cos(deg2rad(player.azimuthal_angle)));
		player.centre_y+=(val*sin(deg2rad(player.azimuthal_angle)));
		/*if (can_move()==0)
		{
			player.centre_x-=(val*cos(deg2rad(player.azimuthal_angle)));
			player.centre_y-=(val*sin(deg2rad(player.azimuthal_angle)));
		}*/
}
void move_down(GLfloat val)
{
	player.centre_x-=(val*cos(deg2rad(player.azimuthal_angle)));
	player.centre_y-=(val*sin(deg2rad(player.azimuthal_angle)));
}
void move_left(GLfloat val)
{
	player.centre_x+=(val*cos(deg2rad(player.azimuthal_angle+90)));
	player.centre_y+=(val*sin(deg2rad(player.azimuthal_angle+90)));
}
void move_right(GLfloat val)
{
	player.centre_x+=(val*cos(deg2rad(player.azimuthal_angle-90)));
	player.centre_y+=(val*sin(deg2rad(player.azimuthal_angle-90)));
}

/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */

void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Function is called first on GLFW_PRESS.

	if (action == GLFW_RELEASE) {
		switch (key) {
			case GLFW_KEY_X:
				// do something ..
				break;
			default:
				break;
		}
	}
	else if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_ESCAPE:
				quit(window);
				break;
			case GLFW_KEY_1:
				toggle_camera(1);
				break;
			case GLFW_KEY_2:
				toggle_camera(2);
				break;
			case GLFW_KEY_3:
				toggle_camera(3);
				break;
			case GLFW_KEY_4:
				toggle_camera(4);
				break;
			case GLFW_KEY_5:
				toggle_camera(5);
				break;
			case GLFW_KEY_W:
				move_up(speed);
				update_follow_view();
				update_helicopter_view();
				
				break;
			case GLFW_KEY_S:
				move_down(speed);
				update_follow_view();
				update_helicopter_view();
				
				break;
			case GLFW_KEY_A:
				move_left(speed);
				update_follow_view();
				update_helicopter_view();
				
				break;
			case GLFW_KEY_D:
				move_right(speed);
				update_follow_view();
				update_helicopter_view();
				
				break;
			case GLFW_KEY_SPACE:
				player.jump();
				break;
			default:
				break;
		}
	}
	else if (action == GLFW_REPEAT) {
		switch (key) {
			case GLFW_KEY_ESCAPE:
				quit(window);
				break;
			case GLFW_KEY_W:
				move_up(speed);
				update_follow_view();
				update_helicopter_view();
				
				break;
			case GLFW_KEY_S:
				move_down(speed);
				update_follow_view();
				update_helicopter_view();
				
				break;
			case GLFW_KEY_A:
				move_left(speed);
				update_follow_view();
				update_helicopter_view();
				
				break;
			case GLFW_KEY_D:
				move_right(speed);
				update_follow_view();
				update_helicopter_view();
				
				break;
			default:
				break;
		}
	}
}

/* Executed for character input (like in text boxes) */
void keyboardChar (GLFWwindow* window, unsigned int key)
{
	switch (key) {
		case 'Q':
		case 'q':
			quit(window);
			break;
		default:
			break;
	}
}

/* Executed when a mouse button is pressed/released */
void mouseButton (GLFWwindow* window, int button, int action, int mods)
{
	switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			if (action == GLFW_RELEASE) {
				right_click_flag = 0;
			}
			else if (action == GLFW_PRESS){
				right_click_flag = 1;
			}
			break;
		default:
			break;
	}
}


/* Executed when window is resized to 'width' and 'height' */
/* Modify the bounds of the screen here in glm::ortho or Field of View in glm::Perspective */
void reshapeWindow (GLFWwindow* window, int width, int height)
{
	int fbwidth=width, fbheight=height;
	/* With Retina display on Mac OS X, GLFW's FramebufferSize
	 is different from WindowSize */
	glfwGetFramebufferSize(window, &fbwidth, &fbheight);

	GLfloat fov = 90.0f;

	// sets the viewport of openGL renderer
	glViewport (0, 0, (GLsizei) fbwidth, (GLsizei) fbheight);

	// set the projection matrix as perspective
	 /*glMatrixMode (GL_PROJECTION);
	 glLoadIdentity ();
	 gluPerspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1, 500.0);*/
	// Store the projection matrix in a variable for future use
	// Perspective projection for 3D views
	Matrices.projection = glm::perspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1f, 10000.0f);

	// Ortho projection for 2D views
	//Matrices.projection = glm::ortho(-scr_width/2, scr_width/2, -scr_height/2, scr_height/2, 0.1f, 10000.0f);
	//Matrices.projection = glm::ortho(-scr_width/2, scr_width/2, -scr_height/2, scr_height/2, 0.1f, 10000.0f);
}

VAO *triangle, *rectangle;

// Creates the triangle object used in this sample code
void createTriangle ()
{
	/* ONLY vertices between the bounds specified in glm::ortho will be visible on screen */

	/* Define vertex array as used in glBegin (GL_TRIANGLES) */
	static const GLfloat vertex_buffer_data [] = {
		-300, 100,0, // vertex 0
		-400,-100,0, // vertex 1
		-200,-100,0, // vertex 2
	};

	static const GLfloat color_buffer_data [] = {
		1,0,0, // color 0
		0,1,0, // color 1
		0,0,1, // color 2
	};

	// create3DObject creates and returns a handle to a VAO that can be used later
	triangle = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, color_buffer_data, GL_FILL);
}

// Creates the rectangle object used in this sample code
void createRectangle (GLuint textureID)
{
	// GL3 accepts only Triangles. Quads are not supported
	static const GLfloat vertex_buffer_data [] = {
		-120,-100,0, // vertex 1
		120,-100,0, // vertex 2
		120, 100,0, // vertex 3

		120, 100,0, // vertex 3
		-120, 100,0, // vertex 4
		-120,-100,0  // vertex 1
	};

	static const GLfloat color_buffer_data [] = {
		1,0,0, // color 1
		0,0,1, // color 2
		0,1,0, // color 3

		0,1,0, // color 3
		0.3,0.3,0.3, // color 4
		1,0,0  // color 1
	};

	// Texture coordinates start with (0,0) at top left of the image to (1,1) at bot right
	static const GLfloat texture_buffer_data [] = {
		0,1, // TexCoord 1 - bot left
		1,1, // TexCoord 2 - bot right
		1,0, // TexCoord 3 - top right

		1,0, // TexCoord 3 - top right
		0,0, // TexCoord 4 - top left
		0,1  // TexCoord 1 - bot left
	};

	// create3DTexturedObject creates and returns a handle to a VAO that can be used later
	rectangle = create3DTexturedObject(GL_TRIANGLES, 6, vertex_buffer_data, texture_buffer_data, textureID, GL_FILL);
}

float camera_rotation_angle = 90;
float rectangle_rotation = 0;
float triangle_rotation = 0;


void update_camera()
{
	//GLfloat temp_radius = sqrt(		(cam_eye_x*cam_eye_x) + (cam_eye_y*cam_eye_y)	+ (cam_eye_z*cam_eye_z)		);
	/*GLfloat temp_radius = 100;
	cam_rot_ang++;	if (cam_rot_ang>=360)cam_rot_ang=0;
	cam_eye_z = temp_radius*cos(deg2rad(cam_rot_ang));
	cam_eye_y = temp_radius*sin(deg2rad(cam_rot_ang));*/
	//cout<<temp_radius<<endl;
}

void update_player_direction(float mouse_x, float mouse_y)
{
	if (current_view == 3)
	{
		GLfloat angle_increment = 1;
		if (mouse_x < last_mouse_x)
		    {
		    	player.increase_azimuthal(angle_increment);
		    	last_mouse_x = mouse_x;
		    }
		    else if (mouse_x > last_mouse_x)
		    {
		    	player.decrease_azimuthal(angle_increment);
		    	last_mouse_x = mouse_x;
		    }

		    if (mouse_y < last_mouse_y)
		    {
		    	player.increase_elevation(angle_increment);
		    	last_mouse_y = mouse_y;
		    }
		    else if (mouse_y > last_mouse_y)
		    {
		    	player.decrease_elevation(angle_increment);
		    	last_mouse_y = mouse_y;
		    }
	}
	return;
}

void update_player_position()		// after jump
{
	player.vertical_speed -= gravity;
	player.centre_z += player.vertical_speed;
	if (player.centre_z <=0)
	{
		player.centre_z = 0;
		player.vertical_speed = 0;
	}
	return;
}

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw ()
{
	//cout<<player.azimuthal_angle<<" "<<player.elevation_angle<<endl;
	// clear the color and depth in the frame buffer
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use the loaded shader program
	// Don't change unless you know what you are doing
	glUseProgram (programID);

	update_adventurer_view();

	update_camera();
	// Eye - Location of camera. Don't change unless you are sure!!
	glm::vec3 eye = cam_eye;

	// Target - Where is the camera looking at.  Don't change unless you are sure!!
	glm::vec3 target = cam_tar;
	// Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
	glm::vec3 up = cam_up;
	//cout<<"eye : "<<glm::to_string(eye)<<endl;
	//cout<<"tar : "<<glm::to_string(target)<<endl;
	//cout<<"up : "<<glm::to_string(up)<<endl<<endl<<endl;

	// Compute Camera matrix (view)
	// Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
	//  Don't change unless you are sure!!
	static float c = 0;
	//	c++;						// CHECK
	//Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(sinf(c*M_PI/180.0),3*cosf(c*M_PI/180.0),0)); // Fixed camera for 2D (ortho) in XY plane
	Matrices.view = glm::lookAt(eye, target, up); // Fixed camera for 2D (ortho) in XY plane
	// Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
	//  Don't change unless you are sure!!
	glm::mat4 VP = Matrices.projection * Matrices.view;

	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// For each model you render, since the MVP will be different (at least the M part)
	//  Don't change unless you are sure!!
	glm::mat4 MVP;	// MVP = Projection * View * Model

	// Load identity to model matrix
	Matrices.model = glm::mat4(1.0f);

	/* Render your scene */
	//	glm::mat4 translateTriangle = glm::translate (glm::vec3(-2.0f, 0.0f, 0.0f)); // glTranslatef
	//	glm::mat4 rotateTriangle = glm::rotate((float)(triangle_rotation*M_PI/180.0f), glm::vec3(0,0,1));  // rotate about vector (1,0,0)
	//	glm::mat4 triangleTransform = translateTriangle * rotateTriangle;
	//	Matrices.model *= triangleTransform;
	MVP = VP * Matrices.model; // MVP = p * V * M

	//  Don't change unless you are sure!!
	// Copy MVP to normal shaders
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

	// draw3DObject draws the VAO given to it using current MVP matrix
	//draw3DObject(triangle);

	Matrices.model = glm::mat4(1.0f);
	MVP = VP * Matrices.model; // MVP = p * V * M
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	for (int i=0;i<10;i++)
		for (int j=0;j<10;j++)
		{
			if (layout[i][j]==1)
				base[i][j].drawCuboid(VP);
			else if (layout[i][j]==2)
			{
				if (base[i][j].vertical_move_direction==1)
				{
					base[i][j].centre_z+=base[i][j].vertical_move_speed;
					//cout<<base[i][j].centre_z<<endl;
					if (base[i][j].centre_z>=base[0][0].centre_z+base[0][0].height())
						base[i][j].vertical_move_direction=-1, base[i][j].centre_z = base[0][0].centre_z+base[0][0].height();
				}
				else if (base[i][j].vertical_move_direction==-1)
				{
					base[i][j].centre_z-=base[i][j].vertical_move_speed;
					if (base[i][j].centre_z<=-2*block_h)
						base[i][j].vertical_move_direction=1, base[i][j].centre_z = -2*block_h;
				}
				base[i][j].drawCuboid(VP);
			}
		}

	upadte_player_wrt_tiles();
	//update_player_position();
	Matrices.model = glm::mat4(1.0f);
	MVP = VP * Matrices.model; // MVP = p * V * M
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	player.drawCuboid(VP);


	//move_monsters();
	/*Matrices.model = glm::mat4(1.0f);
	MVP = VP * Matrices.model; // MVP = p * V * M
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	for (int i=0;i<monster_ct;i++)
	{
		monsters[i].centre_z = base[monsters[i].curr_tile_ind.first][monsters[i].curr_tile_ind.second].centre_z + base[monsters[i].curr_tile_ind.first][monsters[i].curr_tile_ind.second].height() + monsters[i].radius;
		cout<<player.centre_x<<" "<<player.centre_y<<" "<<player.centre_z<<endl;
		monsters[i].drawSphere(VP);
	}*/


	//Matrices.model = glm::mat4(1.0f);
	//MVP = VP * Matrices.model; // MVP = p * V * M
	//glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	//x_axis.drawLine();	y_axis.drawLine();	z_axis.drawLine();


	// Render with texture shaders now
	glUseProgram(textureProgramID);

	// Pop matrix to undo transformations till last push matrix instead of recomputing model matrix
	// glPopMatrix ();
	Matrices.model = glm::mat4(1.0f);

	//	glm::mat4 translateRectangle = glm::translate (glm::vec3(2, 0, 0));        // glTranslatef
	//	glm::mat4 rotateRectangle = glm::rotate((float)(rectangle_rotation*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
	//	Matrices.model *= (translateRectangle * rotateRectangle);
	MVP = VP * Matrices.model;

	// Copy MVP to texture shaders
	glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);

	// Set the texture sampler to access Texture0 memory
	glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);

	// draw3DObject draws the VAO given to it using current MVP matrix
	//draw3DTexturedObject(rectangle);

	// Increment angles
	float increments = 1;

	// Render font on screen
	/*	static int fontScale = 0;
	float fontScaleValue = 0.75 + 0.25*sinf(fontScale*M_PI/180.0f);
	glm::vec3 fontColor = getRGBfromHue (fontScale);



	// Use font Shaders for next part of code
	glUseProgram(fontProgramID);
	Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane

	// Transform the text
	Matrices.model = glm::mat4(1.0f);
	glm::mat4 translateText = glm::translate(glm::vec3(-3,2,0));
	glm::mat4 scaleText = glm::scale(glm::vec3(fontScaleValue,fontScaleValue,fontScaleValue));
	Matrices.model *= (translateText * scaleText);
	MVP = Matrices.projection * Matrices.view * Matrices.model;
	// send font's MVP and font color to fond shaders
	glUniformMatrix4fv(GL3Font.fontMatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniform3fv(GL3Font.fontColorID, 1, &fontColor[0]);

	// Render font
	GL3Font.font->Render("Round n Round we go !!");
	*/

	//camera_rotation_angle++; // Simulating camera rotation


	/*	// font size and color changes
	fontScale = (fontScale + 1) % 360;*/


	cout<<"Score : "<<score<<" Lives : "<<lives<<endl;
	//cout<<dist(base[9][9].centre_x+base[9][9].length()/2, base[9][9].centre_y+base[9][9].breadth()/2, player.centre_x+player.length(), player.centre_y+player.breadth())<<endl;
	if (dist(base[9][9].centre_x+base[9][9].length()/2, base[9][9].centre_y+base[9][9].breadth()/2, player.centre_x+player.length(), player.centre_y+player.breadth()) > 30)
	score--;	
	score = max(score, 0);

}

void zoom_out()
{
	helicopter_view_dist = min(helicopter_view_dist+5, helicopter_view_dist_max);
	update_helicopter_view();
	return;
}

void zoom_in()
{
	helicopter_view_dist = max(helicopter_view_dist-5, helicopter_view_dist_min);
	update_helicopter_view();
	return;
}



void CursorPosition(GLFWwindow *window, double x_positiona,double y_positiona)
{
    float x_position = (float)x_positiona;
    float y_position = (float)y_positiona;

    if (right_click_flag == 1)
    {
	    if (x_position < last_mouse_x)
	    {
	    	helicopter_view_azimuthal--;
	    	if (helicopter_view_azimuthal<0)
	    		helicopter_view_azimuthal+=360;
	    	last_mouse_x = x_position;
	    }
	    else if (x_position > last_mouse_x)
	    {
	    	helicopter_view_azimuthal++;
	    	if (helicopter_view_azimuthal>360)
	    		helicopter_view_azimuthal-=360;
	    	last_mouse_x = x_position;
	    }

	    if (y_position < last_mouse_y)
	    {
	    	helicopter_view_elevation--;
	    	if (helicopter_view_elevation<10)
	    		helicopter_view_elevation=10;
	    	last_mouse_y = y_position;
	    }
	    else if (y_position > last_mouse_y)
	    {
	    	helicopter_view_elevation++;
	    	if (helicopter_view_elevation>85)
	    		helicopter_view_elevation=85;
	    	last_mouse_y = y_position;
	    }

	    update_helicopter_view();
	}
	else
	{
		update_player_direction(x_position, y_position);
	}
    //cout<<"moved"<<endl;

    return;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  //printf("%lf\t%lf\n", xoffset, yoffset);

  if (yoffset<0)
    zoom_out();

  else if (yoffset>0)
    zoom_in();
  return;
}

/* Initialise glfw window, I/O callbacks and the renderer to use */
/* Nothing to Edit here */
GLFWwindow* initGLFW (int width, int height)
{
	GLFWwindow* window; // window desciptor/handle

	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "Sample OpenGL 3.3 Application", NULL, NULL);

	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	glfwSwapInterval( 1 );

	/* --- register callbacks with GLFW --- */

	/* Register function to handle window resizes */
	/* With Retina display on Mac OS X GLFW's FramebufferSize
	 is different from WindowSize */
	glfwSetFramebufferSizeCallback(window, reshapeWindow);
	glfwSetWindowSizeCallback(window, reshapeWindow);

	/* Register function to handle window close */
	glfwSetWindowCloseCallback(window, quit);

	/* Register function to handle keyboard input */
	glfwSetKeyCallback(window, keyboard);      // general keyboard input
	glfwSetCharCallback(window, keyboardChar);  // simpler specific character handling

	/* Register function to handle mouse click */
	glfwSetMouseButtonCallback(window, mouseButton);  // mouse button clicks

	return window;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL (GLFWwindow* window, int width, int height)
{
	// Load Textures
	// Enable Texture0 as current texture memory
	glActiveTexture(GL_TEXTURE0);
	// load an image file directly as a new OpenGL texture
	// GLuint texID = SOIL_load_OGL_texture ("beach.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_TEXTURE_REPEATS); // Buggy for OpenGL3
	GLuint textureID = createTexture("beach2.png");
	// check for an error during the load process
	if(textureID == 0 )
		cout << "SOIL loading error: '" << SOIL_last_result() << "'" << endl;

	// Create and compile our GLSL program from the texture shaders
	textureProgramID = LoadShaders( "TextureRender.vert", "TextureRender.frag" );
	// Get a handle for our "MVP" uniform
	Matrices.TexMatrixID = glGetUniformLocation(textureProgramID, "MVP");


	/* Objects should be created before any other gl function and shaders */
	// Create the models
	//createTriangle (); // Generate the VAO, VBOs, vertices data & copy into the array buffer
	//createRectangle (textureID);


	// Create and compile our GLSL program from the shaders
	programID = LoadShaders( "Sample_GL3.vert", "Sample_GL3.frag" );
	// Get a handle for our "MVP" uniform
	Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


	reshapeWindow (window, width, height);

	// Background color of the scene
	glClearColor (1.0f, 1.0f, 1.0f, 0.0f); // R, G, B, A
	glClearDepth (1.0f);




	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialise FTGL stuff
	/*	const char* fontfile = "arial.ttf";
	GL3Font.font = new FTExtrudeFont(fontfile); // 3D extrude style rendering

	if(GL3Font.font->Error())
	{
		cout << "Error: Could not load font `" << fontfile << "'" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Create and compile our GLSL program from the font shaders
	fontProgramID = LoadShaders( "fontrender.vert", "fontrender.frag" );
	GLint fontVertexCoordAttrib, fontVertexNormalAttrib, fontVertexOffsetUniform;
	fontVertexCoordAttrib = glGetAttribLocation(fontProgramID, "vertexPosition");
	fontVertexNormalAttrib = glGetAttribLocation(fontProgramID, "vertexNormal");
	fontVertexOffsetUniform = glGetUniformLocation(fontProgramID, "pen");
	GL3Font.fontMatrixID = glGetUniformLocation(fontProgramID, "MVP");
	GL3Font.fontColorID = glGetUniformLocation(fontProgramID, "fontColor");

	GL3Font.font->ShaderLocations(fontVertexCoordAttrib, fontVertexNormalAttrib, fontVertexOffsetUniform);
	GL3Font.font->FaceSize(1);
	GL3Font.font->Depth(0);
	GL3Font.font->Outset(0, 0);
	GL3Font.font->CharMap(ft_encoding_unicode);
	*/

	glfwSetCursorPosCallback(window, CursorPosition);
    glfwSetScrollCallback(window, scroll_callback);

	cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
	cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
	cout << "VERSION: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

}


int main (int argc, char** argv)
{
	int width = 1280;
	int height = 720;

	scr_width = width;
	scr_height = height;

	GLFWwindow* window = initGLFW(width, height);

	initGL (window, width, height);

	double last_update_time = glfwGetTime(), current_time;
		//initializing all
		initialize_all();
		SoundEngine->play2D("game.mp3", GL_TRUE);
	/* Draw in loop */
	while (!glfwWindowShouldClose(window)) {
		//FPS counter



		
		// OpenGL Draw commands
		draw();

		// Swap Frame Buffer in double buffering
		glfwSwapBuffers(window);

		// Poll for Keyboard and mouse events
		glfwPollEvents();

		// Control based on time (Time based transformation like 5 degrees rotation every 0.5s)
		current_time = glfwGetTime(); // Time in seconds
		if ((current_time - last_update_time) >= 0.5) { // atleast 0.5s elapsed since last frame
			// do something every 0.5 seconds ..
			last_update_time = current_time;
		}
	}


	glfwTerminate();
	exit(EXIT_SUCCESS);
}
