#include "maze.h"

class MazeFactory
{
public:
	MazeFactory();

	virtual Maze * MakeMaze() const 
	{ 
		return new Maze(); 
	}

	virtual Wall * MakeWall() const
	{
		return New Wall();
	}

	virtual Room * MakeRoom( int aRoomNo ) const
	{
		return new Room( aRoomNo );
	}

	virtual Door * MakeDoor( Room * aR1, Room * aR2 ) const
	{
		return new Door( aR1, aR2 );
	}

private:

};

Maze * MazeGame::CreateMaze( MazeFactory & aFactory )
{
	Maze * sMaze = aFactory.MakeMaze();
	Room * sR1 = aFactory.MakeRoom( 1 );
	Room * sR2 = aFactory.MakeRoom( 2 );
	Door * sDoor = aFactory.MakeDoor( sR1, sR2 );

	sMaze->AddRoom( sR1 );
	sMaze->AddRoom( sR2 );

	sR1->SetSide( North, aFactory.Wall() );
	sR1->SetSide( East, sDoor );
	sR1->SetSide( South, aFactory.Wall() );
	sR1->SetSide( West, aFactory.Wall() );

	sR2->SetSide( North, aFactory.Wall() );
	sR2->SetSide( East, aFactory.Wall() );
	sR2->SetSide( South, aFactory.Wall() );
	sR2->SetSide( West, sDoor );

	return sMaze;
}


/* EnchantedMazeFactory 를 MazeFactory 에서 서브클래싱한 후, 멤버 함수를 재정의하여 
   Room, Wall 을 상속하는 다른 서브클래스의 인스턴스를 반환하게 만든다. */
class EnchanteMazeFactory : public MazeFactory
{
public:
	EnchantedMazeFactory();

	/* 이 때 Room 을 상속받은 EnchantedRoom 의 인스턴스를 생성하여 반환함 */
	virtual Room * MakeRoom( int n ) const
	{
		return new EnchantedRoom( n, CastSpell() );
	}

	/* 이 때 Door 을 상속받은 DoorNeedingSpell 의 인스턴스를 생성하여 반환함 */
	virtual Door * MakeDoor( Room * r1, Room * r2 ) const
	{
		return new DoorNeedingSpell( r1, r2 );
	}

protected:
	Spell * CastSpell() const;
}


int main( void )
{
	Maze *sMaze = NULL;
	MazeGame * sMazeGame = new MazeGame();
	
	sMaze = sMazeGame->CreateMaze();	
}
