#pragma once
#include"gameNode.h"
/*
	주어진 출발점에서 목표점 까지 가는 최단 경로를 찾아내는 그래프 알고리즘 중에 하나임.

	F =  G+H
	F = 비용(목적지 까지 가는 총 비용 현재까지 이동하는 걸린 비용과 예상비용을 합침)
	G =  시작점으로 부터 새로운 사각형 까지의 이동비용
	H =  사각형으로 부터 최종 목적지 까지의 예상 이동비용
	(대각선 이동이 아닌 가로세로 이동에 대한 비용으로 계산, 장애물에 대한 요소는 고려하지 않는다.)

	이등변 삼각형 길이비인 루트2,1,1에 의해 대각선은 14, 가로세로는 10을 부여한다. 루트2는 1.141..
	

	동작원리 : 시작점에서 이동할 수 있는 지점을 오픈 리스트에 추가함.
				위치를 다시 확인 할 필요가 없는 지점을 클로즈 리스트에 추가
				오픈 리스트에서 가장 비용이 적은 위치를 현재 위치로 지정(즉 F가 작은곳)
				위 과정을 반복한다.
				F값이 동일하다면 속도상의 목적으로 오픈리스트에 더 늦게 추가된것 을 선택하는 것이 빠르다.

*/
#define TILE_X 20	//가로 타일수
#define TILE_Y 20	//세로 타일 수
#define WIDTH 35		//타일 가로 길이
#define HEIGHT 35		//타일 세로길이
#define BIGNUM 5000		//초기값(F)

//타일 속성
enum TILE_TYPE2
{
	TILE_TYPE_EMPTY,		//빈타일(지나갈수 있는 곳)
	TILE_TYPE_START,		//시작지점
	TILE_TYPE_END,			//끝지점
	TILE_TYPE_WALL			//벽
};
enum ASTAR_STATE
{
	ASTAR_STATE_SEARCHING,	//검색중
	ASTAR_STATE_FOUND,		//발견!
	ASTAR_STATE_NOWAY,		//길없음
	ASTAR_STATE_END
};

struct aStarTile
{
	RECT rc;
	COLORREF color;
	bool walkable;			//지나갈수 있는지 여부
	bool listOn;			//openlist에 추가 되었는지 여부
	int i, j;
	int F, G, H;
	char str[128];
	aStarTile* parent;	//부모 타일 주소
	TILE_TYPE2 type;		//타일 속성
};
class aStar : public gameNode
{
private:
	
	aStarTile _tile[TILE_X][TILE_Y];
	vector<aStarTile*> _openList;
	vector<aStarTile*> _closeList;

	ASTAR_STATE _aStarState;

	TILE_TYPE2 _selectedType;		//타일 배치할때 사용할 속성
	COLORREF _selectedTypeColor;

	int _startX, _startY;			//시작지점의 i,j인덱스
	int _endX, _endY;				//도착지점의 i,j인덱스
	int _lastIndex;					//나중에 추가된 클로즈 리스트의 인덱스


	bool _startPointSet;			//시작지점이 배치 되었냐
	bool _endPointSet;				//도착지점이 배치 되었냐

		
	int Ci;							//마지막으로 추가된 클로즈 리스트의 i
	int Cj;
	int Cg;

	RECT rc[6];
	HBRUSH newBrush, oldBrush;
	HFONT newFont, oldFont;


public:
	aStar();
	~aStar();

	HRESULT init();
	void release();
	void update();
	void render();

	void tileComposition();		//배치
	void tileInitializing();	//배치 후 속성에 맞게 타일 초기화
	void addOpenList();			//오픈리스트 추가
	void calculateH();			//오픈 리스트 내 타일 H값계산
	void calculateF();			//오픈 리스트 내 타일 F값 계산
	void addCloseList();		//가장 작은 F선택후 클로즈 리스트에 추가
	void checkArrive();			//도착했는지 여부 체크
	void showWay(aStarTile* tile);	//도착 후 시작지점과 도착지점을 잇는 경로 보여주자.


};

