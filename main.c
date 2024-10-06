#include "raylib.h"
#include <stdio.h>


#define SIZE 10
#define screenWidth 800
#define screenHeight 800 
#define cols screenWidth/SIZE
#define rows screenHeight/SIZE
#define FPS 30

struct cell {
	int row , col;
	bool active;
};

typedef struct cell cell;

struct cell cells[cols][rows];
struct cell next_cells[cols][rows];


int flag = -1;

int get_neighbours(cell a){
	
	int neighbhours = 0; 
	int i = a.col;
	int j = a.row;


	int directions[8][2] = {
		{-1,0},
		{-1,1},
		{0,1},
		{1,1},
		{1,0},
		{1,-1},
		{0,-1},
		{-1,-1}
	};
	for(int direction = 0; direction < 8; direction++){
		int a = directions[direction][0] + i;
		int b = directions[direction][1] + j;

		if(a > -1 && b > -1 && a < cols && b < rows){
			if(cells[a][b].active) neighbhours ++;
		}
	}

	return neighbhours;

}

void run_rules(int row, int col){

	int n = get_neighbours(cells[row][col]);
	
	//RULES
	// Any live cell with fewer  than two live neighbours dies, as if by UNDERPOPULATION
		
	if(cells[row][col].active  && n < 2) next_cells[row][col].active = 0;	

	// Any live cell with two or three live neighbours lives on to the next generation
		
	if(cells[row][col].active  && n == 2 || n == 3) next_cells[row][col].active = 1;	

	// Any live cell with more than three live neighbours dies, as if by OVERPOPULATION
		
	if(cells[row][col].active  && n > 3) next_cells[row][col].active = 0;

	// Any dead cell with exactly three live neighbhours becomes a live cell, as if by REPRODUCTION

	if(!cells[row][col].active  && n == 3) next_cells[row][col].active = 1;



}

int main () {
	
	SetRandomSeed((int) GetTime());
	
	int number_of_cells = cols * rows;
		

	char snum[5];
	// initiating the cells array
	
	for(int i = 0; i < cols; i++){
		for(int j =0; j < rows; j++){
			bool noise= GetRandomValue(1,10) > 5 ? 0 : 1;
			cell a = {.row = i,.col = j,.active = 0};
			cells[j][i] = a;

		}
	}

	InitWindow(screenWidth, screenHeight,"convays game of life");
	SetTargetFPS(FPS);

	while(!WindowShouldClose()){
	
		
		for(int t=0; t < cols; t++)for(int tt = 0 ; tt < rows; tt++)next_cells[t][tt] = cells[t][tt];

		int mouse_col = GetMouseX() / SIZE;

		int mouse_row = GetMouseY() / SIZE;
			
		if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){

			next_cells[mouse_col][mouse_row].active = 1;
		}	

		if(IsKeyPressed(KEY_SPACE)){
			flag *= -1;
			printf("flag : %d \n ",flag);
		}


		for (int col = 0; col < cols; col++){

			for (int row= 0; row< rows; row++){
				
				if (IsKeyPressed(KEY_ENTER)){
					run_rules(row,col);
				}	
					
				if (flag > 0) run_rules(row,col); 

			}
		}	
		for(int t=0; t < cols; t++)for(int tt = 0 ; tt < rows; tt++)cells[t][tt] = next_cells[t][tt];




			BeginDrawing();
			for (int col = 0; col < cols; col++){
				for (int row= 0; row< rows; row++){
						
			
					// drawing part
					DrawRectangle(SIZE * row , SIZE * col, SIZE, SIZE, cells[row][col].active ? GRAY: BLACK);
		//			DrawRectangleLines(SIZE * row, SIZE * col, SIZE, SIZE, GRAY );
					
					int n = get_neighbours(cells[row][col]);
		//			DrawText(TextFormat("%d",n),SIZE * row + SIZE / 3, SIZE * col + SIZE / 4, SIZE, WHITE);
			
				}
			}
		
			ClearBackground(BLACK);
			EndDrawing();

	}	
}

