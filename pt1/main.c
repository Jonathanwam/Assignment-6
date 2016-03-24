/* 
 * File:   main.c
 * Author: Jonathan Wambach
 * Email:  jonathan.wambach@wsu.edu
 */

#include <stdio.h>
//#include <stdlib.h>

#define LINE_TOTAL 14

typedef struct _body {
	char name[20];
	char color[20];
	double size;
	double mass;
	double xPosition;
	double yPosition;
	double xVelocity;
	double yVelocity;
} body;

body bodies[LINE_TOTAL];


int main(int argc, char **argv) {
    //gnuplot -persist -e 'plot [-5:5] sinh(x)'
    //pipe to gnuplot
    FILE *pipe;
    pipe = popen("gnuplot -p", "w"); /* 'pipe' is the pipe descriptor, -p makes graph persist on screen after program closes. */
    if (pipe == NULL) {
        printf("Error opening pipe to GNU plot. Check if you have it! \n");
        return 0;
    }
   
    //1) Create an X11 plotter window on the display screen of some suitable size
    //(say 750 by 750 pixels) with a black background.    
    fprintf(pipe, " set object 11 rectangle from screen 0,0 to screen 1,1 fc rgb \"black\" front\n");  //black background, 11 as index reference
    fprintf(pipe, " set term x11 size 750, 750\n");

    //2) Reads the data from file 'planet.dat'.
    FILE* planets = fopen("planet.dat", "r");
    if (planets == NULL) {
        fprintf(stderr, "Error: Failed to open file: %p for read.\n",
                planets);
        return 0;
    }
    
    char str[200];

	for(int line = 0; line < LINE_TOTAL; ++line){
                //get all the characters on this line, should be no more than 200
		fgets (str, 200, planets);
                //skip to next line if either of these are found
		if ((str[0]== '#') || (str[0] == '\n') || (str[0] == '\0') ){
			//fgets (str, 200, datFilePointer);
			continue;
		}
                //read in the specified values for each line
		body planet;
		sscanf(str ,"%s %s %lf %lf %lf %lf %lf %lf ", 
			planet.name, 
			planet.color, 
			&planet.size, 
			&planet.mass, 
			&planet.xPosition, 
			&planet.yPosition, 
			&planet.xVelocity, 
			&planet.yVelocity );
		
            printf("%s %s %lf %lf %lf %lf %lf %lf \n",
			planet.name, 
			planet.color, 
			planet.size, 
			planet.mass, 
			planet.xPosition, 
			planet.yPosition, 
			planet.xVelocity, 
			planet.yVelocity );
		//store the values in an array to access later
                
                bodies[line] = planet;      
		if ( feof(planets) ){
			break;
		}		
   	}    
    
    //3) Plots the first 5 bodies listed in the file at their initial coordinates.  Each body should
    //be plotted as a filled circle of the color indicated in the file.
    //Inflate the diameter of the planets such that they are adequately visible on the screen.     
    for(int plot = 0; plot < 6; plot++){
        fprintf(pipe, "set object circle radius char %lf fillcolor rgb \"%s\" fillstyle solid noborder front\n", (bodies[plot].size * 100), bodies[plot].color);
        //fprintf(pipe, "set object circle at graph %lf, %lf radius char %lf fillcolor rgb \"%s\" fillstyle solid noborder front\n", bodies[plot].xPosition, bodies[plot].yPosition, (bodies[plot].size * 100), bodies[plot].color);
        fprintf(pipe, "plot %lf, %lf with filledcurves\n", bodies[plot].xPosition, bodies[plot].yPosition);        
        
        //4) After displaying the bodies on the screen for 10 seconds
        //remove the bodies from the screen (replot them in black), leaving the screen black.
        //fprintf(pipe, "set object 11 front\n");  //black background, 11 as index reference
        fprintf(pipe, "plot x with filledcurves\n");
        fflush(pipe); 
        sleep(10);
    }
    fclose(pipe);
    return 0;
}

