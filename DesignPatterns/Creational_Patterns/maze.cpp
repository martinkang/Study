#include <iostream>
#include "maze.h"

Maze * MazeGame::CreateMaze()
{
	Maze * sMaze = new Maze();
	Room * sR1 = new Room( 1 );
	Room * sR2 = new Room( 2 );
	Door * sDoor = new Door( sR1, sR2 );

	sMaze->AddRoom( sR1 );
	sMaze->AddRoom( sR2 );

	sR1->SetSide( North, new Wall() );
	sR1->SetSide( East, sDoor );
	sR1->SetSide( South, new Wall() );
	sR1->SetSide( West, new Wall() );

	sR2->SetSide( North, new Wall() );
	sR2->SetSide( East, new Wall() );
	sR2->SetSide( South, new Wall() );
	sR2->SetSide( West, sDoor );

	return sMaze;
}

int main( void )
{
	Maze *sMaze = NULL;
	MazeGame * sMazeGame = new MazeGame();
	
	sMaze = sMazeGame->CreateMaze();	
}
