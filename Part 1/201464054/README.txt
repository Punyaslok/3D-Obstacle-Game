OpenGL 3D Game

Punyaslok Pattnaik
201464054

Compiles executable name : sample2D
execute "./sample2D" in terminal to run game.

Source Code : Sample_GL3_2D.cpp

Makefile name : "Makefile"

To compile, run "g++ -o sample2D Sample_GL3_2D.cpp glad.c -lGL -lglfw -lftgl -lSOIL -ldl -I/usr/local/include -I/usr/local/include/freetype2 -L/usr/local/lib"

Gameplay :
	Reach the last tile of the maze. Running into an empty spot will mkae you fall down, and game will restart.

Controls :
	UP, DOWN, LEFT, RIGHT : Move player up, down, left, right respectively.
	Number keys 1 to 4 : Toggle camera views.
		1 : Top view
		2 : Tower view
		3 : Adventurer view (not working)
		4 : Follow me view
