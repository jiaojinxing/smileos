#include "init.h"
#include "upscreen.h"

int init_all()
{
/************************************** jiaojinxing1987@gmail.com ***************************************/
#if _NDS_
	irqInit();
	irqSet(IRQ_VBLANK, 0);

	fatInitDefault();
#endif
/************************************************* end **************************************************/

	GAL_InitGraph();
	
	CRect::InitRectPool();
	afx.Init();
	
/************************************** jiaojinxing1987@gmail.com ***************************************/
#if _NDS_
	set_current_timer(3);
	timer_int_enable();
	install_timer3_int_handler();
	start_timer();
#endif
/************************************************* end **************************************************/

	SetStockImgBuf(afx.mem_img->p);

/************************************** jiaojinxing1987@gmail.com ***************************************/
#if _NDS_
	fill_rect(0,0,256,192,RGB(31,31,31));
#endif
/************************************************* end **************************************************/
	
	afx.wndmgr->AddWindow(CDesktop::CreateDesktop());
	
	afx.mouse->SetPos(100,100);
	afx.mouse->SetPos(100,100);
	afx.mouse->Show();
	
	return OK;
}
