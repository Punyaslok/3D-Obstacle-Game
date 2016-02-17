all: sample2D

sample2D: Sample_GL3_2D.cpp glad.c
	g++ -o sample2D Sample_GL3_2D.cpp glad.c -lGL -lglfw -lftgl -lSOIL -ldl -I/usr/local/include -I/usr/local/include/freetype2 -L/usr/local/lib -I/usr/include /home/punya/graphics/3D_game/GL3_Fonts_Textures/ikpMP3.so /home/punya/graphics/3D_game/GL3_Fonts_Textures/libIrrKlang.so -pthread 

clean:
	rm sample2D
