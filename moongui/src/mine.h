#ifndef __MINE_H
#define __MINE_H

#include "moongui.h"

#define MINE_MINE		0x01
#define MINE_FLAG		0x02
#define MINE_IS_OPENED	0x04

class CMine{
public:
	char *mine;
	int row,col;
	int width;
	int draw_off_x,draw_off_y;
	int win_id;
	int totol_mines;
	int game_over;
	int level;
public:
	int IsWin();
	int Init(int level);
	int GetMinesAround(int index);
	int PutMines(int probility);
	int OpenMine(int index);
	int RedrawMine(int index);
	int OnLButtonDown(int x,int y,int flag);
	int PosToIndex(int x,int y);
	int DrawAll(CDc *pdc);
	int IndexToRowCol(int i,int *r,int *c);
	int GetMineRect(int r,int c,CRect *rect);
	int Draw(int index,CDc *pdc);
	int ToIndex(int r,int c);
    CMine();
    virtual ~CMine();
    int Init(int row,int col);
    int Destroy();
    int Copy(CMine *p);
    int Comp(CMine *p);
    int Print();
    int InitBasic();
};

#endif
