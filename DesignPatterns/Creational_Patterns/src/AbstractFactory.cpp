#include "maze.h"

enum Spell
{
	None = -1,
	Magic = 0,
};

enum MazeType
{
	Normal = 0,
	Enchanted = 1,
	Bomb = 2
};

/* Abstract Factory *
   개념적 제품에 대한 객체를 생성하는 연산으로 인터페이스를 정의 */
class MazeFactory
{
public:
	MazeFactory()
	{ 
		cout << "MazeFactory" << endl;
	}

	virtual Maze * MakeMaze() const 
	{ 
		return new Maze(); 
	}

	virtual Wall * MakeWall() const
	{
		return new Wall();
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

	sR1->SetSide( North, aFactory.MakeWall() );
	sR1->SetSide( East, sDoor );
	sR1->SetSide( South, aFactory.MakeWall() );
	sR1->SetSide( West, aFactory.MakeWall() );

	sR2->SetSide( North, aFactory.MakeWall() );
	sR2->SetSide( East, aFactory.MakeWall() );
	sR2->SetSide( South, aFactory.MakeWall() );
	sR2->SetSide( West, sDoor );

	return sMaze;
};

/* Room 은 Abstract Product 이고 
   EnchantedRoom 은 Concrete Product 이다. */
class EnchantedRoom : public Room
{
public:
	EnchantedRoom( int aRoomNo, Spell aSpell ) : Room( aRoomNo )
	{
		_spell = aSpell;
	}

	virtual void Enter()
	{
		cout << "Magic Room!!!!" << endl;
	}

private:
	Spell _spell;
};

class DoorNeedingSpell : public Door
{
public:
	DoorNeedingSpell( Room * aR1, Room * aR2 ) : Door( aR1, aR2 )
	{
		_spell = Magic;
	}
	virtual void Enter()
	{
		cout << " Magic Door!!!!" << endl;
	}

private:
	Spell _spell;
};

/* EnchantedMazeFactory 를 MazeFactory 에서 서브클래싱한 후, 멤버 함수를 재정의하여 
   Room, Wall 을 상속하는 다른 서브클래스의 인스턴스를 반환하게 만든다. */
class EnchantedMazeFactory : public MazeFactory
{
public:
	EnchantedMazeFactory()
	{
		cout << "EnchantedMazeFactory" << endl;
	};
	/* 이 때 Room 을 상속받은 EnchantedRoom 의 인스턴스를 생성하여 반환함 */
	virtual Room * MakeRoom( int aRoomNo ) const
	{
		return new EnchantedRoom( aRoomNo, CastSpell() );
	}
	/* 이 때 Door 을 상속받은 DoorNeedingSpell 의 인스턴스를 생성하여 반환함 */
	virtual Door * MakeDoor( Room * aR1, Room * aR2 ) const
	{
		return new DoorNeedingSpell( aR1, aR2 );
	}

protected:
	Spell CastSpell() const
	{
		return Magic;
	}
};

class RoomWithABomb : public Room
{
public:
	RoomWithABomb( int aRoomNo ) : Room( aRoomNo ) { }

	virtual void Enter()
	{
		cout << "Bomb!!!!" << endl;
	}
private:
};

class BombedWall : public Wall
{
public:
	BombedWall() : Wall() {}
	
	virtual void Enter()
	{
		cout << "Bomb!!!!" << endl;
	}
};

/* Concrete Factory */
class BombedMazeFactory : public MazeFactory
{
public:
	BombedMazeFactory()
	{
		cout << "BombedMazeFactory" << endl;
	}

	virtual Room * MakeRoom( int aRoomNo ) const
	{
		return new RoomWithABomb( aRoomNo );
	}

	virtual Wall * MakeWall() const
	{
		return new BombedWall();
	}

private:
};


int main( void )
{
	Maze * sMaze1 = NULL;
	Maze * sMaze2 = NULL;
	Maze * sMaze3 = NULL;

	MazeGame sGame;

	MazeFactory sN_Factory;
	EnchantedMazeFactory sE_Factory;
	BombedMazeFactory sB_Factory;

	sMaze1 = sGame.CreateMaze( sN_Factory );
	sMaze2 = sGame.CreateMaze( sE_Factory );
	sMaze3 = sGame.CreateMaze( sB_Factory );

	return 0;
}
