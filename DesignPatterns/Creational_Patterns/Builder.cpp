#include "maze.h"

/* 미로를 복합하는데 필요한 인터페이스를 정의 
   어떤 구현도 정의하지 않는다. 이는 Builder 의 서브클래스에서
   관심 있을 때만 이들 연산을 재정의할 수 있도록 하기 위해서 */
class MazeBuilder
{
public:
	virtual void BuildMaze() {}
	virtual void BuildRoom( int aRoomNo ) {}
	virtual void BuildDoor( int aRoomFrom, int aRoomTo ) {}

	virtual Maze * GetMaze() { return 0; }

protected:
	MazeBuilder();

};

Maze * MazeGame::CreateMaze( MazeBuilder & aBuilder )
{
	aBuilder.BuildMaze();

	aBuilder.BuildRoom( 1 );
	aBuilder.BuildRoom( 2 );
	aBuilder.BuildDoor( 1, 2 );

	return aBuilder.GetMaze();
};

Maze * MazeGame::CreateComplexMaze( MazeBuilder & aBuilder )
{
	aBuilder.BuildRoom( 1 );
	aBuilder.BuildRoom( 2 );
	aBuilder.BuildRoom( 3 );

	aBuilder.BuildDoor( 1, 2 );
	aBuilder.BuildDoor( 1, 3 );
	
	return aBuilder.GetMaze();
};
