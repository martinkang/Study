#include <iostream>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define MAX_ROOM 256
#define START_ROOM 0

using namespace std;

class MazeFactory;
class MazeBilder;

enum Direction
{
	North = 0,
	South = 1,
	East  = 2,
	West  = 3
};

enum MapSiteType
{
	Wall_t = 0,
	Door_t = 1
};

/* 미로의 구성요소들에 필요한 모든 연산을 정의한 공통 추상 클래스 */
class MapSite
{
	public:
		/* 무엇에 Enter 를 하느냐에 따라 그 의미가 달라진다. */
		virtual void Enter() = 0; /* 멤버함수 선언 = 0  =>  순수 가상 함수로 정의. 구현부를 갖지 않는다. */

	private:
};


class Room : public MapSite
{
	public:
		Room( int aRoomNo ) 
		{ 
			_roomNumber = aRoomNo; 
		}

		MapSite * GetSide( Direction aDirection ) const 
		{ 
			return _sides[aDirection]; 
		}

		void SetSide( Direction aDirection, MapSite * aMapSite )
		{
			_sides[aDirection] = aMapSite;
		}

		int GetRoomNo()
		{
			return _roomNumber;
		}

		virtual void Enter()
		{
			cout << "Enter Room" << endl;
		}

	private:
		MapSite * _sides[4]; /* 방은 네 개의 방향을 갖고 있고 각 방향에는
								MapSite 의 서브클래스 인스턴스가 올 수 있다. */
		int _roomNumber;
};


class Wall : public MapSite
{
	public:
		Wall()
		{
			_type = Wall_t;
		}

		virtual void Enter()
		{
			cout << "is Wall do not enter" << endl;
		}

	private:
		MapSiteType _type;
};


class Door : public MapSite
{
	public:
		/* 문을 초기화하기 위해서는 문이 어느 방 사이에 있는지 알아야 한다 */
		Door( Room * aR1, Room * aR2 )
		{
			_room1 = aR1;
			_room2 = aR2;
			_isOpen = FALSE;

			_type = Door_t;
		}

		virtual void Enter()
		{
			cout << "is Door you can enter" << endl;
		}

		Room * OtherSideFrom( Room* );

	private:	
		MapSiteType _type;

		Room* _room1;
		Room* _room2;
		bool _isOpen;
};


class Maze 
{
	public:
		Maze()
		{
			memset( (void*)_room, 0, sizeof( Room * ) * MAX_ROOM );

			_numOfRooms = 0;
			AddRoom( new Room( START_ROOM ) );
		}

		void AddRoom( Room* aR )
		{
			_room[_numOfRooms] = aR;
			_numOfRooms++;
		}

	private:
		Room * _room[MAX_ROOM];
		int _numOfRooms;

		Room * getRoomByNo ( int aRoomNo ) const
		{
			int i = 0;
			int sRoomNo = 0;

			for ( i = 0; i < _numOfRooms; i++ )
			{
				sRoomNo =  _room[i]->GetRoomNo();
				if ( sRoomNo == aRoomNo )
					return _room[i];
			}

			return NULL;
		}
};


class MazeGame
{
	public:
		MazeGame()
		{
			cout << "New MazeGame!" << endl;
		}

		Maze * CreateMaze();
		Maze * CreateMaze( MazeFactory & factory );
	private:
};

