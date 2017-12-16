output: main.o InitShader.o
	g++ main.o InitShader.o -o output -lGL -lGLU -lglut -lGLEW
main.o: main.cpp mat-yjc-new.h
	g++ -c -lGL -lGLU -lglut -lGLEW main.cpp 
InitShader.o: InitShader.cpp mat-yjc-new.h
	g++ -c -lGL -lGLU -lglut -lGLEW InitShader.cpp
clean:
	rm *.o output
