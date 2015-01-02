
// main.cpp

#include "main.h"

int main(){
	
	USER user;	
	GAME game;

	
	// Get the users name, grid size, etc
	user.GetPlayersInfo(&user);
	
	
	string UserInfoLog = "userdata/data.log";
	user.ReadInUserLog(&user, UserInfoLog);
	
	
	// Get the game board size
	// from users input
	game.nx = user.usersnx;
	game.ny = user.usersny;
	
	// Initialise all values to zero, and other stuff.	
	game.startup(&game);
	
	// Write the users info to screen
	user.WriteUsersInfo(&user);
	
	// play the game
	
	while(game.play){

		// We had to check if the users move did anything;
		// if it did, play on
		if(game.MoveDidSomething){
			
			// Set the turn number identifiers
			game.SetTurnNumber(&game);

			// Put down a random tile
			game.PutRandomTile(&game);	
		
			game.GameOver = game.IsGameOver(&game);
			if(game.GameOver) game.terminate = true;
		
			// Print the state of the board to the screen
			game.WriteState(cout, &game);
		}
		// If the game needs to terminate, set it here
		if(game.terminate) game.play = false;
		
		if(game.play){

			// Get the move to perform from the user (which direction to swipe in)
			user.GetMove(&game, &user);
			
			if(game.play){
				
				// Perform users move
				game.DoMove(&game);	
				// Find out if users move did anything to the state of the board
				game.MoveDidSomething = game.DidMoveDoAnything(&game);
				// If it did, increment the turn number
				if(game.MoveDidSomething){
					// Increment the turn number
					game.TurnNumber++;
				}
				
			}
			
		}
		
	}

	user.WriteScoreInfo(&user, UserInfoLog, game.score);	
	
	// deallocate memory
	delete game.cells;
	
	if(game.GameOver) cout << "Game Over" << endl;	
	if(!game.GameOver) cout << "DONE" << endl;
	
} // END main()

