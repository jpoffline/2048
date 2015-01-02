#ifndef PRINT_H
#define PRINT_H

#include <iostream>

struct PRINT{

	void printtile(int value, ostream& whereto, int spacing){
		whereto.width(spacing);
		whereto << value << " ";
	}
	
};

#endif