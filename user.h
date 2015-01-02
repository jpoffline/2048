
// user.h

#ifndef USER_H
#define USER_H

struct USERSINFO{
	string UserName;
	int TopScore;
};

struct USER{
	
	int move;
	string UserName;
	int usersnx;
	int usersny;
	int TopScore;
	
	void GetPlayersInfo(struct USER *U){
		
		cout << endl;
		cout << "Tiles game (rip off & souped up 2048)" << endl;
		
		cout << "Enter your name (no spaces)" << endl;
		cin >> U->UserName;
		cout << "What x-size? (integer)" << endl;
		cin >> U->usersnx;
		cout << "what y-size? (integer)"<< endl;
		cin >> U->usersny;
		
	} // END GetGameInfo()
	
	void WriteUsersInfo(struct USER *U){
		cout << "Ta very much for inputting your info, ";
		cout <<  U->UserName << endl;
		cout << "Incase you forgot, your board will be ";
		cout <<  U->usersnx << "x";
		cout <<  U->usersny << endl;
		cout << "The game will now begin" << endl;
		
	} // END GetGameInfo()
	
	void ReadInUserLog(struct USER *U, string UserInfoLog){
		
		ifstream UsersLog_in;
		UsersLog_in.open(UserInfoLog);
		int TempTop=0;
		// Read in all users info
		int ID = 0;
		int ID_user = -1;
		if(UsersLog_in){			
			while(!UsersLog_in.eof()){
				USERSINFO tempInfo;
				UsersLog_in >> tempInfo.UserName >> tempInfo.TopScore;
				if(tempInfo.UserName == U->UserName) {
					ID_user = ID;
					TempTop = tempInfo.TopScore;
				}
			}	
		}
		UsersLog_in.close();

		// If the user has played before, tell them their previous high-score
		U->TopScore=0;
		if(ID_user!=-1){
			cout << endl;
			cout << "The current top score for user " << U->UserName;
			cout << " is " << TempTop << endl;
			cout << endl;
			// Remember the users previous top score
			U->TopScore = TempTop;
		}
		
		
		/*
		// Now write all user info back to file
		// except the current users info.
		ofstream UsersLog_out;
		UsersLog_out.open(UserInfoLog);
		for(int i = 0; i < ID; i++) {
			if(i!=ID_user){
				UsersLog_out << users[i].UserName << " " << users[i].TopScore << endl;
			}
		}
		UsersLog_out.close();
		*/
		
		
	} // END ReadInUserLog()
	
	void WriteScoreInfo(struct USER *U, string UserInfoLog, int Score){
		
		ifstream UsersLog_in;
		UsersLog_in.open(UserInfoLog);
		vector<USERSINFO> users;
		
		// Read in all users info
		if(UsersLog_in){			
			while(UsersLog_in){
				USERSINFO tempInfo;
				UsersLog_in >> tempInfo.UserName >> tempInfo.TopScore;
				cout << tempInfo.UserName << " " << tempInfo.TopScore << endl;
			//	if(tempInfo.UserName!=U->UserName) users.push_back(tempInfo);
			}	
		}
		UsersLog_in.close();
		
		USERSINFO tempInfo;		
		tempInfo.UserName = U->UserName;
		tempInfo.TopScore = U->TopScore;
		if(Score > U->TopScore) {
			tempInfo.TopScore = Score;
			cout << endl;
			cout << "New top score: " << tempInfo.TopScore << endl;
			cout << endl;
		}

		//users.push_back(tempInfo);
		int NumberOfUsers = users.size();

		ofstream UsersLog_out;
		UsersLog_out.open("DUMMY.dat");
		for(int i = 0; i < NumberOfUsers; i++) {
		//	UsersLog_out << users[i].UserName << " " << users[i].TopScore << endl;
			cout << users[i].UserName << " " << users[i].TopScore << endl;
		}
		UsersLog_out.close();
		
	} // END WriteScoreInfo()
	
	// Routine to get the users move.
	void GetMove(struct GAME *game, struct USER *U){
		string input;
		if(!game->MoveDidSomething)cout << "Your move didnt do anything." << endl;
		bool ValidInput = false;
		cout << U->UserName << ", whats your move? :: Swipe (u, d, l, r). e will stop"<< endl;
		
		while(!ValidInput){
			
			cin >> input;
			
			if( input == "u" && !ValidInput ) {
				game->moveID = game->up; 
				ValidInput = true;
			}
			
			if( input == "d" && !ValidInput ) {
				game->moveID = game->down; 
				ValidInput = true;
			}
			
			if( input == "l" && !ValidInput ) {
				game->moveID = game->left; 
				ValidInput = true;
			}
			
			if( input == "r" && !ValidInput ) {
				game->moveID = game->right; 
				ValidInput = true;
			}
			
			if( input == "e" && !ValidInput ) {
				game->play = false;
				ValidInput = true;
			}
				
			if(!ValidInput) cout << "Please re-enter" << endl;
			
		}
		if( input != "e" )
			 cout << "Board will now swipe " << input << endl;

	} // END GetMove()

};


#endif