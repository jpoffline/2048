
// game.h

#ifndef GAME_H
#define GAME_H

struct GAME{

#include "print.h"

	int nx, ny;
	int moveID;
	int *cells;
	int up,down,left,right;
	int emptyCell;
	int TurnNumber, ThisTurn, NextTurn;
	int score;
	int tilespacing;
	double RandomCellBias;
	bool play;
	bool terminate, GameOver, MoveDidSomething;

	void startup(struct GAME *game){
		
		// initiate the empty cell value
		game->emptyCell = 0;
		game->tilespacing = 5;
		game->MoveDidSomething = true;
		
		// Identifiers for each type of move
		game->up = 1;
		game->down = 2;
		game->left = 3;
		game->right = 4;
		
		// Whats the threshold for the random cell to be a "4"?
		game->RandomCellBias = 0.8;
		
		// Start game at turn #0
		game->TurnNumber = 0;
		
		// zero the score
		game->score = 0;
		
		// Start off game
		game->play = true;
		
		// Dont terminate yet!
		game->terminate = false;
		
		
		// allocate memory to the grid
		game->cells = new int[ 2 * game->nx * game->ny ];
		
		
		// start off whole grid with empty cells
		for(int i=0; i < game->nx; i ++){
			for(int j = 0; j < game->ny; j++){
				game->cells[game->ind(0,i,j,game)] = game->emptyCell;			
				//game->cells[game->ind(0,i,j,game)] = 4;
			} // END j-loop
		} // END i-loop
		
		
	} // END startup()
	
	void PutRandomTile(struct GAME *game){
		
		// Routine to put down a random tile at the start of each turn
		
		// Count the number of empty cells
		int numEmptyCells = 0;

		// Arrays to store indices of empty cells
		int eCellsX[ game->nx * game->ny ];
		int eCellsY[ game->nx * game->ny ];	
		
		// Find out how many cells are empty,
		// and record their coordinates	
		for(int i = 0; i < game->nx; i++){
			for(int j = 0; j < game->ny; j++){
				
				if( game->cells[ game->ind(game->ThisTurn,i,j,game) ] == game->emptyCell ) {	
				
					eCellsX[numEmptyCells] = i;
					eCellsY[numEmptyCells] = j;
					numEmptyCells++;
					
				}
				
			}
		}
		
		//cout << "Number of empty cells = " << numEmptyCells << endl;
		
		// If there are some empty cells, put a "2" or "4" randomly in one of them
		if( numEmptyCells != 0 ){
			
			// seed random number generator
			srand ( time(NULL) );
			// get (random) cell identifier which is empty
			int RandEmptyCell = rand() % numEmptyCells;
			// Get random number between 0 & 1
			double ran = rand() / (double)RAND_MAX;
		
			int RandCellVal = 2;
			// Bias the number generator so that "4" is sometimes chosene
			if(ran > game->RandomCellBias) RandCellVal = 4;
		
			if(	game->cells[ game->ind(game->ThisTurn, eCellsX[RandEmptyCell], eCellsY[RandEmptyCell], game) ] == game->emptyCell )
				game->cells[ game->ind(game->ThisTurn, eCellsX[RandEmptyCell], eCellsY[RandEmptyCell], game) ] = RandCellVal;
			
			else
				cout << "something bad happend to the filling in of the random tile!" << endl;
		
		//	cout << "Cell picked had coordinates " ;
		//	cout << " ("<< eCellsX[RandEmptyCell] << ", " << eCellsY[RandEmptyCell] << ") and had value = " << RandCellVal << endl;
			
		}
		
	} // END PutRandomTile()
	
	void SetTurnNumber(struct GAME *game){
		
		// Turn identifiers
		game->ThisTurn = 1 , 
		game->NextTurn = 0;
		
		if( game->TurnNumber % 2 == 0 ) {
			game->ThisTurn = 0; 
			game->NextTurn = 1;
		}
		
	} // END SetTurnNumber()
	
	void DoMove(struct GAME *game){
		
		// Get the cells into a grid for easy reading of the code
		int grid[ game->nx ][ game->ny ];
		
		for(int i = 0; i < game->nx; i++){
			for(int j = 0; j < game->ny; j++){
				grid[i][j] = game->cells[ game->ind(game->ThisTurn, i, j, game) ];
			}
		}
		
		int NumberOfNonZeroCells;
		bool changed;
		
		// Do the specific moves required
		// (1) Combine like tiles in the given direction
		// (2) Push all tiles in the given direction
		
		if(game->moveID == game->right) {
			
			// Shuffle all tiles across in the given direction
			// Works by starting at the end; if a cell is non-empty,
			// the array "nonzerocells" is populate with the cells value.
			// Once the whole line is gone over,
			// the main grid is repopulated starting at the end.

			// Create array to hold all non-zero cell values
			int nonzerocells[ game->nx ];
			
			int inc, ip;
			
			
			// Loop over all j-directed columns
			for(int j = 0 ; j < game->ny; j++){
				
			// (1) 1st Shuffle
				
				// initialise all nonzerocells to be empty								
				for(int i = 0; i < game->nx; i++) nonzerocells[i] = game->emptyCell;
				
				// index to count the number of non-zero cells
				NumberOfNonZeroCells = game->nx-1;
				
				// Now loop over the i-direction,
				for(int i = game->nx - 1; i > -1; i--){
					// If a cell is non-empty, dump value into array
					// and increment the number of non-zero-cells
					if(grid[i][j] != game->emptyCell){					
						nonzerocells[NumberOfNonZeroCells] = grid[i][j];
						NumberOfNonZeroCells--;
					}
				} // END for(i)	
				
				// Repopulate the grid with these shuffled non-zero cell values
				for(int i = game->nx - 1; i > -1; i--) grid[i][j] = nonzerocells[i];

			// (2) Check if same & combine

				// After shuffle, change all tiles that are adjacent 
				// and identically valued.
			
				// Initialise the incrementor size,
				inc = 1;
				// and the changed flag
				changed = false;
				
				// Begin loop over i, to check if tiles need changing
				for(int i = 0; i < game->nx; i = i + inc){
					
					// location of "next" tile
					ip = i + 1;
					// We definitely didnt change anything yet
					changed = false;
					
					// Make sure we arent going to look off-grid,
					if( ip < game->nx ){
						// If two tiles are the same,
						if( grid[ip][j] == grid[i][j] && !changed ) {
							// double "this" tile,
							grid[i][j] = 2 * grid[i][j];
							// make the next tile empty
							grid[ip][j] = game->emptyCell;
							// and change the "changed" flag.
							changed = true;
							// If we changed a tile, add on the new
							// combined value to the score
							if(changed) game->score += grid[i][j];
						}
					}
					
					// If we changed a tile, skip the next tile, else 
					// we would change it again.
					if(changed) inc = 2;
					if(!changed) inc = 1;
					
				} // END for(i)

			
			// (3) 2nd Shuffle
			
				// Redo the shuffling, incase the change yielded empty tiles
				// Loop over all j-directed columns

				// initialise all nonzerocells to be empty								
				for(int i = 0; i < game->nx; i++) nonzerocells[i] = game->emptyCell;				
				
				// index to count the number of non-zero cells
				 NumberOfNonZeroCells = game->nx-1;
				
				// Now loop backwards over the i-direction,
				for(int i = game->nx - 1; i > -1; i--){
					// If a cell is non-empty, dump value into array
					// and increment the number of non-zero-cells
					if(grid[i][j] != game->emptyCell){					
						nonzerocells[NumberOfNonZeroCells] = grid[i][j];
						NumberOfNonZeroCells--;
					}
				} // END for(i)	
				
				// Repopulate the grid with these shuffled non-zero cell values
				for(int i = game->nx - 1; i > -1; i--) grid[i][j] = nonzerocells[i];

			} // END j-loop
			
			
		} // END right-moves
		
		if(game->moveID == game->up) {
			// Shuffle all tiles across in the given direction
			// Works by starting at the end; if a cell is non-empty,
			// the array "nonzerocells" is populate with the cells value.
			// Once the whole line is gone over,
			// the main grid is repopulated starting at the end.

			// Create array to hold all non-zero cell values
			int nonzerocells[ game->ny ];
			int inc, jp;

			
			// Loop over all j-directed columns
			for(int i = 0 ; i < game->nx; i++){
				
			// (1) 1st Shuffle
				
				// initialise all nonzerocells to be empty								
				for(int j = 0; j < game->ny; j++) nonzerocells[j] = game->emptyCell;
				
				// index to count the number of non-zero cells
				NumberOfNonZeroCells = game->ny-1;
				
				// Now loop over the i-direction,
				for(int j = game->ny - 1; j > -1; j--){
					// If a cell is non-empty, dump value into array
					// and increment the number of non-zero-cells
					if(grid[i][j] != game->emptyCell){					
						nonzerocells[NumberOfNonZeroCells] = grid[i][j];
						NumberOfNonZeroCells--;
					}
				} // END for(i)	
				
				// Repopulate the grid with these shuffled non-zero cell values
				for(int j = game->ny - 1; j > -1; j--) grid[i][j] = nonzerocells[j];

				// (2) Check if same & combine

				// After shuffle, change all tiles that are adjacent 
				// and identically valued.
			
				// Initialise the incrementor size,
				inc = 1;
				// and the changed flag
				changed = false;
				
				// Begin loop over i, to check if tiles need changing
				for(int j = 0; j < game->ny; j = j + inc){
					
					// location of "next" tile
					jp = j + 1;
					// We definitely didnt change anything yet
					changed = false;
					
					// Make sure we arent going to look off-grid,
					if( jp < game->ny ){
						// If two tiles are the same,
						if( grid[i][jp] == grid[i][j] && !changed ) {
							// double "this" tile,
							grid[i][j] = 2 * grid[i][j];
							// make the next tile empty
							grid[i][jp] = game->emptyCell;
							// and change the "changed" flag.
							changed = true;
							// If we changed a tile, add on the new
							// combined value to the score
							if(changed) game->score += grid[i][j];
						}
					}
					
					// If we changed a tile, skip the next tile, else 
					// we would change it again.
					if(changed) inc = 2;
					if(!changed) inc = 1;
					
				} // END for(i)

			
				// (3) 2nd Shuffle
			
				// Redo the shuffling, incase the change yielded empty tiles
				// Loop over all j-directed columns

				// initialise all nonzerocells to be empty								
				for(int j = 0; j < game->ny; j++) nonzerocells[j] = game->emptyCell;				
				
				// index to count the number of non-zero cells
				 NumberOfNonZeroCells = game->ny-1;
				
				// Now loop backwards over the i-direction,
				for(int j = game->ny - 1; j > -1; j--){
					// If a cell is non-empty, dump value into array
					// and increment the number of non-zero-cells
					if(grid[i][j] != game->emptyCell){					
						nonzerocells[NumberOfNonZeroCells] = grid[i][j];
						NumberOfNonZeroCells--;
					}
				} // END for(i)	
				
				// Repopulate the grid with these shuffled non-zero cell values
				for(int j = game->ny - 1; j > -1; j--) grid[i][j] = nonzerocells[j];

			} // END j-loop
			
		} // END up-moves

		if(game->moveID == game->down) {
			// Shuffle all tiles across in the given direction
			// Works by starting at the end; if a cell is non-empty,
			// the array "nonzerocells" is populate with the cells value.
			// Once the whole line is gone over,
			// the main grid is repopulated starting at the end.

			// Create array to hold all non-zero cell values
			int nonzerocells[ game->ny ];
			int inc, jp;

			
			// Loop over all j-directed columns
			for(int i = 0 ; i < game->nx; i++){
				
			// (1) 1st Shuffle
				
				// initialise all nonzerocells to be empty								
				for(int j = 0; j < game->ny; j++) nonzerocells[j] = game->emptyCell;
				
				// index to count the number of non-zero cells
				NumberOfNonZeroCells = 0;
				
				// Now loop over the i-direction,
				for(int j = 0; j < game->ny; j++){
					// If a cell is non-empty, dump value into array
					// and increment the number of non-zero-cells
					if(grid[i][j] != game->emptyCell){					
						nonzerocells[NumberOfNonZeroCells] = grid[i][j];
						NumberOfNonZeroCells++;
					}
				} // END for(i)	
				
				// Repopulate the grid with these shuffled non-zero cell values
				for(int j = 0; j < game->ny - 1; j++) grid[i][j] = nonzerocells[j];

				// (2) Check if same & combine

				// After shuffle, change all tiles that are adjacent 
				// and identically valued.
			
				// Initialise the incrementor size,
				inc = 1;
				// and the changed flag
				changed = false;
				
				// Begin loop over i, to check if tiles need changing
				for(int j = 0; j < game->ny; j = j + inc){
					
					// location of "next" tile
					jp = j + 1;
					// We definitely didnt change anything yet
					changed = false;
					
					// Make sure we arent going to look off-grid,
					if( jp < game->ny ){
						// If two tiles are the same,
						if( grid[i][jp] == grid[i][j] && !changed ) {
							// double "this" tile,
							grid[i][j] = 2 * grid[i][j];
							// make the next tile empty
							grid[i][jp] = game->emptyCell;
							// and change the "changed" flag.
							changed = true;
							// If we changed a tile, add on the new
							// combined value to the score
							if(changed) game->score += grid[i][j];
						}
					}
					
					// If we changed a tile, skip the next tile, else 
					// we would change it again.
					if(changed) inc = 2;
					if(!changed) inc = 1;
					
				} // END for(i)

			
			// (1) 1st Shuffle
				
				// initialise all nonzerocells to be empty								
				for(int j = 0; j < game->ny; j++) nonzerocells[j] = game->emptyCell;
				
				// index to count the number of non-zero cells
				NumberOfNonZeroCells = 0;
				
				// Now loop over the i-direction,
				for(int j = 0; j < game->ny; j++){
					// If a cell is non-empty, dump value into array
					// and increment the number of non-zero-cells
					if(grid[i][j] != game->emptyCell){					
						nonzerocells[NumberOfNonZeroCells] = grid[i][j];
						NumberOfNonZeroCells++;
					}
				} // END for(i)	
				
				// Repopulate the grid with these shuffled non-zero cell values
				for(int j = 0; j < game->ny - 1; j++) grid[i][j] = nonzerocells[j];
				
			} // END j-loop
			
		} // END down-moves
		
		if(game->moveID == game->left) {
			
			// Shuffle all tiles across in the given direction
			// Works by starting at the end; if a cell is non-empty,
			// the array "nonzerocells" is populate with the cells value.
			// Once the whole line is gone over,
			// the main grid is repopulated starting at the end.

			// Create array to hold all non-zero cell values
			int nonzerocells[ game->nx ];
			
			int inc, ip;
			
			
			// Loop over all j-directed columns
			for(int j = 0 ; j < game->ny; j++){
				
			// (1) 1st Shuffle
				
				// initialise all nonzerocells to be empty								
				for(int i = 0; i < game->nx; i++) nonzerocells[i] = game->emptyCell;
				
				// index to count the number of non-zero cells
				NumberOfNonZeroCells = 0;
				
				// Now loop over the i-direction,
				for(int i = 0; i < game->nx; i++){
					// If a cell is non-empty, dump value into array
					// and increment the number of non-zero-cells
					if(grid[i][j] != game->emptyCell){					
						nonzerocells[NumberOfNonZeroCells] = grid[i][j];
						NumberOfNonZeroCells++;
					}
				} // END for(i)	
				
				// Repopulate the grid with these shuffled non-zero cell values
				for(int i = 0; i < game->nx; i++) grid[i][j] = nonzerocells[i];
			// (2) Check if same & combine

				// After shuffle, change all tiles that are adjacent 
				// and identically valued.
			
				// Initialise the incrementor size,
				inc = 1;
				// and the changed flag
				changed = false;
				
				// Begin loop over i, to check if tiles need changing
				for(int i = 0; i < game->nx; i = i + inc){
					
					// location of "next" tile
					ip = i + 1;
					// We definitely didnt change anything yet
					changed = false;
					
					// Make sure we arent going to look off-grid,
					if( ip < game->nx ){
						// If two tiles are the same,
						if( grid[ip][j] == grid[i][j] && !changed ) {
							// double "this" tile,
							grid[i][j] = 2 * grid[i][j];
							// make the next tile empty
							grid[ip][j] = game->emptyCell;
							// and change the "changed" flag.
							changed = true;
							// If we changed a tile, add on the new
							// combined value to the score
							if(changed) game->score += grid[i][j];
						}
					}
					
					// If we changed a tile, skip the next tile, else 
					// we would change it again.
					if(changed) inc = 2;
					if(!changed) inc = 1;
					
				} // END for(i)
			// (3) 2nd Shuffle
			
				// Redo the shuffling, incase the change yielded empty tiles
				// Loop over all j-directed columns

				// initialise all nonzerocells to be empty								
				for(int i = 0; i < game->nx; i++) nonzerocells[i] = game->emptyCell;
				
				// index to count the number of non-zero cells
				NumberOfNonZeroCells = 0;
				
				// Now loop over the i-direction,
				for(int i = 0; i < game->nx; i++){
					// If a cell is non-empty, dump value into array
					// and increment the number of non-zero-cells
					if(grid[i][j] != game->emptyCell){					
						nonzerocells[NumberOfNonZeroCells] = grid[i][j];
						NumberOfNonZeroCells++;
					}
				} // END for(i)	
				
				// Repopulate the grid with these shuffled non-zero cell values
				for(int i = 0; i < game->nx; i++) grid[i][j] = nonzerocells[i];

			} // END j-loop
			
			
			
			
		} // END left-moves
		
		
		// Stopped messing about now, so put tiles back into main array
		for(int i = 0; i < game->nx; i++){
			for(int j = 0; j < game->ny; j++){
				game->cells[ game->ind(game->NextTurn, i, j, game) ]  = grid[i][j];
			}
		}
		
		
			
	} // END DoMove()
	
	bool DidMoveDoAnything(struct GAME *game){
		
		// Routine to check if the users move did anything to the state
		// of the board.
		
		bool SomethingMoved = false;
		
		for(int i = 0; i < game->nx; i++){
			for(int j = 0; j < game->ny; j++){
				if(game->cells[game->ind(game->NextTurn,i,j,game)] != game->cells[game->ind(game->ThisTurn,i,j,game)]){
					SomethingMoved = true;
				}
			}
		}
	
		return SomethingMoved;
	
	} // END DidMoveDoAnything()
	
	void WriteState(ostream& whereto, struct GAME *game){
		
		whereto << endl;
		whereto << "Current score :: " << game->score << endl;
		whereto << "Turn number :: " << game->TurnNumber << endl;		
		
		// Print out the grid state
		
		WriteGrid(whereto, game);

		whereto << endl;

	} // END WriteState()
	
	void WriteGrid(ostream& whereto, struct GAME *game){
		
		PRINT print;
		
		for(int j = game->ny-1; j >-1 ; j--){		
			for(int i = 0; i < game->nx; i++){
				int value = game->cells[ game->ind(game->ThisTurn, i, j , game) ];
				print.printtile(value, whereto, game->tilespacing);
			}	
			whereto << endl;
		}
		
	} // END WriteGrid()
	
	bool IsGameOver(struct GAME *game){
		// Routine to check if game is over
		
		// The logical value "GameOver" changes to "true"
		// if the game is over:
		// (a) no empty places are left
		// (b) no swipes can be made to combine tiles.
		bool GameOver = false;
		
		// First, check if there are any empty cells		
		int numEmptyCells =  0;
		for(int i = 0; i < game->nx; i++){
			for(int j = 0; j < game->ny; j++){
				
				if( game->cells[ game->ind(game->ThisTurn,i,j,game) ] == game->emptyCell ) 
					numEmptyCells++;
				
			}
		}
		
		// If there are no empty cells, then check if any adjacent tiles are identical
		if( numEmptyCells == 0 ){
			
			bool AnyAdjacent = false;
			
			int ip, jp;
			for(int i = 0; i < game->nx-1; i++ ){
				ip = i + 1;
				// Only both to do the next loop if no adjacent already found
				if(!AnyAdjacent){
					for(int j = 0; j < game->ny-1; j++ ){
						jp = j + 1;
						
						// Do the check across
						if(!AnyAdjacent){
							if( game->cells[ game->ind(game->ThisTurn,i,j,game) ]
								 == game->cells[ game->ind(game->ThisTurn,ip,j,game) ])
									 AnyAdjacent = true;
						}
						// Do the check up
						if(!AnyAdjacent){
							if( game->cells[ game->ind(game->ThisTurn,i,j,game) ]
								 == game->cells[ game->ind(game->ThisTurn,i,jp,game) ])
									 AnyAdjacent = true;
						}
						
					} // END j-loop
					
				} // END if(!AnyAdjacent)
				
			} // END i-loop
			
			if(AnyAdjacent) GameOver = false;
		
		} // END if( numEmptyCells == 0 )

		return GameOver;
		
	} // END IsGameOver()
	
	int ind(int t, int i, int j, struct GAME *game){
		
		return 	 i * game->ny * game->nx + j * game->nx + t;
			
	} // END loc()
	
};

#endif