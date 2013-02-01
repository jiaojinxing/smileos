/*********************************************************************************************************
**
** Copyright (c) 2011 - 2012  Jiao JinXing <jiaojinxing1987@gmail.com>
**
** Licensed under the Academic Free License version 2.1
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
**--------------------------------------------------------------------------------------------------------
** File name:               GUI_Alloc.c
** Last modified Date:      2013-1-31
** Last Version:            1.0.0
** Descriptions:
**
** ���һ���ܱ��ߵ��ļ�, ��Ҳ����ȥʵ������ļ�, ���Ҵ� NXP ����վ
**
** http://www.lpcware.com/content/project/emwin-graphics-library
**
** �������� emWin �Ŀ�, ���� GUI_ALLOC_AllocInit ���ڴ������صĺ�������������,
**
** ���Ѿ�ȷ���ҵ���ֲ�Ѿ�û��������, ���ν⿪�� emWin �Ŀ�, �Ѽ��� *alloc*.o ɾ����,
**
** �����´��һ���µ� emWin ��, ����Ҫ����ʵ������ĺ�����, ����û�ĵ��ɲο�,
**
** Ҳû����࿴���ľ���ʵ��(�Ǻ�, ����Ȩ����), �Ҳ�ȷ����󼸸�������ʵ���Ƿ�������
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-1-31
** Version:                 1.0.0
** Descriptions:            �����ļ�
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#include "GUI.h"
#include "kern/kern.h"
#include "kern/heap.h"
#include "kern/ipc.h"
#include <string.h>

void              *GUI_ALLOC__Context = NULL;

static heap_t      gui_heap;
static mutex_t     gui_heap_lock;

void               GUI_ALLOC_Init            (void)
{
    static int init = FALSE;
    if (!init) {
        init = TRUE;
        mutex_new(&gui_heap_lock);
    }
}

void               GUI_ALLOC_Lock            (void)
{
    mutex_lock(&gui_heap_lock, 0);
}

void               GUI_ALLOC_Unlock          (void)
{
    mutex_unlock(&gui_heap_lock);
}

void               GUI_ALLOC_AssignMemory    (void * p, U32 NumBytes)
{
    GUI_ALLOC_Init();

    GUI_ALLOC_Lock();

    heap_init(&gui_heap, "emWin", p, NumBytes);

    GUI_ALLOC_Unlock();
}

GUI_HMEM           GUI_ALLOC_AllocInit       (const void * pInitData, GUI_ALLOC_DATATYPE Size)
{
    GUI_HMEM ptr;

    GUI_ALLOC_Lock();

    ptr = (GUI_HMEM)heap_alloc(&gui_heap, __func__, __LINE__, Size);
    if (ptr) {
        memcpy((void *)ptr, pInitData, Size);
    }

    GUI_ALLOC_Unlock();

    return ptr;
}

GUI_HMEM           GUI_ALLOC_AllocNoInit     (GUI_ALLOC_DATATYPE size)
{
    GUI_HMEM ptr;

    GUI_ALLOC_Lock();

    ptr = (GUI_HMEM)heap_alloc(&gui_heap, __func__, __LINE__, size);

    GUI_ALLOC_Unlock();

    return ptr;
}

GUI_HMEM           GUI_ALLOC_AllocZero       (GUI_ALLOC_DATATYPE size)
{
    GUI_HMEM ptr;

    GUI_ALLOC_Lock();

    ptr = (GUI_HMEM)heap_alloc(&gui_heap, __func__, __LINE__, size);
    if (ptr) {
        memset((void *)ptr, 0, size);
    }

    GUI_ALLOC_Unlock();

    return ptr;
}

void               GUI_ALLOC_Free            (GUI_HMEM  hMem)
{
    GUI_ALLOC_Lock();

    heap_free(&gui_heap, __func__, __LINE__, (void *)hMem);

    GUI_ALLOC_Unlock();
}

GUI_HMEM           GUI_ALLOC_Realloc         (GUI_HMEM hOld, int NewSize)
{
    GUI_HMEM hNew;

    if (hOld) {
        hNew = GUI_ALLOC_AllocInit((const void *)hOld, NewSize);
        GUI_ALLOC_Free(hOld);
    } else {
        hNew = GUI_ALLOC_AllocNoInit(NewSize);
    }
    return hNew;
}

void               GUI_ALLOC_FreePtrArray    (GUI_HMEM * pArray, int NumElems)
{
    int i;

    for (i = 0; i < NumElems; i++) {
        GUI_ALLOC_Free(pArray[i]);
    }
}

void               GUI_ALLOC_FreePtr         (GUI_HMEM * phMem)
{
    if (phMem) {
        GUI_ALLOC_Free(*phMem);
        *phMem =0;
    }
}

void *             GUI_ALLOC_GetFixedBlock   (GUI_ALLOC_DATATYPE Size)
{
    return (void *)GUI_ALLOC_AllocNoInit(Size);
}

void               GUI_ALLOC_FreeFixedBlock  (void * p)
{
    GUI_ALLOC_Free((GUI_HMEM)p);
}

GUI_ALLOC_DATATYPE GUI_ALLOC_GetNumFreeBytes (void)
{
    GUI_ALLOC_DATATYPE ret;

    GUI_ALLOC_Lock();

    ret = gui_heap.size - gui_heap.used_size;

    GUI_ALLOC_Unlock();

    return ret;
}

GUI_ALLOC_DATATYPE GUI_ALLOC_GetNumUsedBytes (void)
{
    GUI_ALLOC_DATATYPE ret;

    GUI_ALLOC_Lock();

    ret = gui_heap.used_size;

    GUI_ALLOC_Unlock();

    return ret;
}

GUI_ALLOC_DATATYPE GUI_ALLOC_GetSize         (GUI_HMEM  hMem)
{
    GUI_ALLOC_DATATYPE ret;

    if (hMem == 0) {
        return 0;
    }

    GUI_ALLOC_Lock();

    ret = mem_size(&gui_heap, __func__, __LINE__, (void *)hMem);

    GUI_ALLOC_Unlock();

    return ret;
}

void *             GUI_ALLOC_h2p             (GUI_HMEM  hMem)
{
    return (void *)hMem;
}

GUI_HMEM           GUI_ALLOC_p2h             (void * p)
{
    return (GUI_HMEM)p;
}

static U32 AvBlockSize;

void               GUI_ALLOC_SetAvBlockSize  (U32 BlockSize)
{
    GUI_ALLOC_Lock();

    AvBlockSize = BlockSize;

    GUI_ALLOC_Unlock();
}

GUI_ALLOC_DATATYPE GUI_ALLOC_GetNumUsedBlocks(void)
{
    return 100;
}

GUI_ALLOC_DATATYPE GUI_ALLOC_GetNumFreeBlocks(void)
{
    return 100;
}

/*********************************************************************************************************
** ������ĺ���:
*********************************************************************************************************/

GUI_ALLOC_DATATYPE GUI_ALLOC_RequestSize     (void)
{
    GUI_ALLOC_DATATYPE ret;

    GUI_ALLOC_Lock();

    ret = AvBlockSize;

    GUI_ALLOC_Unlock();

    return ret;
}

GUI_ALLOC_DATATYPE GUI_ALLOC_GetMaxSize      (void)
{
    GUI_ALLOC_DATATYPE ret;

    GUI_ALLOC_Lock();

    ret = gui_heap.size;

    GUI_ALLOC_Unlock();

    return ret;
}

int                GUI_ALLOC_SetMaxPercentage(int MaxPercentage)
{
    return 0;
}

void *             GUI_ALLOC_LockH           (GUI_HMEM  hMem)
{
    return (void *)hMem;
}

void *             GUI_ALLOC_UnlockH         (void ** pp)
{
    return 0;
}

/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
