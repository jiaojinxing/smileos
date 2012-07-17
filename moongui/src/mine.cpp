#include "mine.h"
#include "colormap.h"
#include "imagemap.h"

CMine::CMine()
{
    this->InitBasic();
}
CMine::~CMine()
{
    this->Destroy();
}
int CMine::InitBasic()
{
	this->mine  = NULL;
	this->row = 0;
	this->col = 0;
	this->width = 10;
	this->draw_off_x = 1;
	this->draw_off_y = 20;
	this->win_id = 0;
	this->totol_mines = 0;
	this->game_over = 0;
	this->level = 1;

    return OK;
}
int CMine::Init(int row,int col)
{
	int i;

    this->InitBasic();
    
	MALLOC(this->mine,char,row * col)
	for(i = 0 ; i < row * col; i++)
	{
		this->mine[i] = 0;
	}

	this->row = row;
	this->col = col;

    return OK;
}
int CMine::Destroy()
{
    FREE(this->mine);
    this->InitBasic();
    return OK;
}
int CMine::Copy(CMine *p)
{
    if(this == p)
        return OK;
    return OK;
}
int CMine::Comp(CMine *p)
{
    return 0;
}
int CMine::Print()
{
    return TRUE;
}

int CMine::ToIndex(int r, int c)
{
	if(r < 0 || r >= this->row)
		return -1;
	if(c < 0 || c >= this->col)
		return -1;

	int i = r * this->col + c;

	if(i < 0 || i >= this->row * this->col)
		return -1;

	return i;
}

int CMine::Draw(int index,CDc *pdc)
{
	ASSERT(pdc && index >= 0  && index < this->row * this->col);
	
	int r,c;
	CRect rc,rt;

	this->IndexToRowCol(index,&r,&c);
	this->GetMineRect(r,c,&rc);
	
	if(!(this->mine[index] & MINE_IS_OPENED))
	{
		Draw3dThinUpFrame(pdc,&rc);
		if(this->mine[index] & MINE_FLAG)
		{
			ImageInfo *img = GetStockImage(IMAGE_MINE_FLAG);
			rc.CenterRect(img->width,img->height,&rt);
			pdc->DrawMonoImage(rt.left,rt.top,img,COLOR_RED);
		}
	}
	else
	{
		pdc->SetPenColor(COLOR_DARKGRAY);
		pdc->DrawRect(&rc);
		if(this->mine[index] & MINE_MINE)
		{
			ImageInfo *img = GetStockImage(IMAGE_MINE_MINE);
			rc.CenterRect(img->width,img->height,&rt);
			pdc->SetBrushColor(COLOR_RED);
			pdc->FillRect(&rc);
			pdc->DrawMonoImage(rt.left,rt.top,img,COLOR_BLACK);
		}
		else
		{
			char num[10];
			int t = this->GetMinesAround(index);
			if(t > 0)
			{
				sprintf(num,"%d",t);
				switch(t)
				{
					case 1: pdc->SetTextColor(COLOR_BLUE); break;
					case 2: pdc->SetTextColor(COLOR_LIGHTGREEN); break;
					case 3: pdc->SetTextColor(COLOR_RED); break;
					case 4: pdc->SetTextColor(COLOR_LIGHTBLUE); break;
					default: pdc->SetTextColor(COLOR_BLACK);
				}
				
				pdc->DrawText(num,&rc,DT_CENTER);
			}
		}
	}
	
	return OK;
}

int CMine::GetMineRect(int r, int c, CRect *rect)
{
	ASSERT(rect);

	rect->Set(c * width , r * width , (c + 1) * width - 1, (r + 1)*width -1);
	rect->Offset(this->draw_off_x,this->draw_off_y);

	return OK;
}


int CMine::IndexToRowCol(int i, int *r, int *c)
{
	ASSERT(r && c && col > 0);
	
	*r = i / col;
	*c = i % col;

	return OK;
}


int CMine::DrawAll(CDc *pdc)
{
	int i;

	for(i = 0; i < this->row * this->col; i++)
	{
		this->Draw(i,pdc);
	}

	return OK;
}

int CMine::PosToIndex(int x, int y)
{
	x -= this->draw_off_x;
	y -= this->draw_off_y;

	if(x < 0 || y < 0)
		return -1;

	int c = x / width;
	int r = y / width;

	return this->ToIndex(r,c);
}

int CMine::OnLButtonDown(int x, int y,int flag)
{
	if(this->game_over)
		return ERROR;

	int i = this->PosToIndex(x,y);
	if(i < 0)
		return ERROR;
	
	if( !flag)
	{
		if( ! (this->mine[i] & MINE_FLAG) )
		{
			this->OpenMine(i);
			if(this->game_over)
			{
				CWndMsgBox::Create(this->win_id,"游戏结束","   你完蛋了.",MB_OK);
			}
			else if(this->IsWin())
			{
				this->game_over = 1;
				CWndMsgBox::Create(this->win_id,"完成","恭喜你过关.",MB_OK);
			}
		}
	}
	else
	{
		if(this->mine[i] & MINE_FLAG)
			this->mine[i] &= (~MINE_FLAG);
		else
			this->mine[i] |= MINE_FLAG;
		this->RedrawMine(i);
	}
	

	return OK;
}

int CMine::RedrawMine(int index)
{
	int r,c;
	CRect rc;

	this->IndexToRowCol(index,&r,&c);

	this->GetMineRect(r,c,&rc);
	afx.wndmgr->InvalidateRect(afx.wndmgr->IdToIndex(this->win_id),&rc,1);

	return OK;
}

int CMine::OpenMine(int index)
{
	if(index < 0  || index >= this->row * this->col)
		return ERROR;

	if(this->mine[index] & MINE_IS_OPENED)
		return ERROR;

	this->mine[index] |= MINE_IS_OPENED;
	this->RedrawMine(index);

	if(this->mine[index] & MINE_MINE)
	{
		this->game_over = 1;
		return ERROR;
	}

	int i,r,c;

	this->IndexToRowCol(index,&r,&c);

	if(this->GetMinesAround(index) == 0)
	{
		i = this->ToIndex(r , c - 1);
		this->OpenMine(i);

		i = this->ToIndex(r - 1, c );
		this->OpenMine(i);

		i = this->ToIndex(r , c + 1);
		this->OpenMine(i);
		
		i = this->ToIndex(r + 1, c );
		this->OpenMine(i);
	}

	return OK;
}

int CMine::PutMines(int probility)
{
	int i,r;

	this->totol_mines = 0;
	srand(get_sys_time());

	for(i = 0; i < row * col; i++)
	{
		r = rand() % 100;
		this->mine[i] = 0;
		
		if(r >= 0 && r < probility)
		{		
			this->mine[i] |= MINE_MINE;
			this->totol_mines ++;
		}
	}

	return OK;
}

int CMine::GetMinesAround(int index)
{
	int i,r,c,t = 0;

	this->IndexToRowCol(index,&r,&c);
	
	i = this->ToIndex(r - 1,c - 1);
	if(i >= 0)
	{
		if(this->mine[i] & MINE_MINE)
			t++;
	}

	i = this->ToIndex(r - 1,c);
	if(i >= 0)
	{
		if(this->mine[i] & MINE_MINE)
			t++;
	}

	i = this->ToIndex(r - 1,c + 1);
	if(i >= 0)
	{
		if(this->mine[i] & MINE_MINE)
			t++;
	}
	
	i = this->ToIndex(r ,c + 1);
	if(i >= 0)
	{
		if(this->mine[i] & MINE_MINE)
			t++;
	}

	i = this->ToIndex(r + 1,c + 1);
	if(i >= 0)
	{
		if(this->mine[i] & MINE_MINE)
			t++;
	}
	
	i = this->ToIndex(r + 1,c);
	if(i >= 0)
	{
		if(this->mine[i] & MINE_MINE)
			t++;
	}
	
	i = this->ToIndex(r + 1,c - 1);
	if(i >= 0)
	{
		if(this->mine[i] & MINE_MINE)
			t++;
	}
	
	i = this->ToIndex(r ,c - 1);
	if(i >= 0)
	{
		if(this->mine[i] & MINE_MINE)
			t++;
	}

	return t;
}

int CMine::Init(int level)
{

	if(level == 1)
	{
		this->Init(9,9);
		this->PutMines(10);
	}

	if(level == 2)
	{
		this->Init(11,15);
		this->PutMines(12);
	}

	if(level == 3)
	{
		this->Init(11,20);
		this->PutMines(15);
	}
	this->level = level;
	return OK;
}

int CMine::IsWin()
{
	int i;

	for(i = 0; i < row * col; i++)
	{
		if(! (this->mine[i] & MINE_IS_OPENED) && !(this->mine[i] & MINE_MINE))
			return FALSE;
	}

	return TRUE;

}
