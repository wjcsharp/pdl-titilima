///////////////////////////////////////////////////////////////////////////////
// FileName:    ptrvector.cpp
// Created:     2009/06/06
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2009
//-----------------------------------------------------------------------------
// Information: ָ��������ʵ��
///////////////////////////////////////////////////////////////////////////////

#include <pdl_container.h>
#include "..\common\lock.h"

#define VECTOR_ITERATING    0x00000001

LPtrVector::LPtrVector(void)
{
    m_dwStatus = 0;
    m_pvData = NULL;
    m_dwUnitSize = 0;
    m_dwUnitCnt = 0;
    m_dwMaxCnt = 0;
    m_nGrowCnt = 0;
    m_pfnCopy = NULL;
    m_pfnDestroy = NULL;
    m_lock = NULL;
}

LPtrVector::~LPtrVector(void)
{
    Destroy();
}

BOOL LPtrVector::Clear(void)
{
    if (NULL == m_pvData || (VECTOR_ITERATING & m_dwStatus))
        return FALSE;

    LAutoLock lock(m_lock);
    if (NULL != m_pfnDestroy)
    {
        PBYTE dst = (PBYTE)m_pvData;
        for (DWORD i = 0; i < m_dwUnitCnt; ++i)
        {
            m_pfnDestroy(dst);
            dst += m_dwUnitSize;
        }
    }
    m_dwUnitCnt = 0;
    return TRUE;
}

BOOL LPtrVector::Create(
    __in DWORD dwUnitSize,
    __in DWORD dwMaxCnt,
    __in int nGrowCnt /* = -1 */,
    __in CopyPtr pfnCopy /* = NULL */,
    __in DestructPtr pfnDestroy /* = NULL */,
    __in ILock* lock /* = NULL */)
{
    if (NULL != m_pvData)
        Destroy();

    m_dwUnitSize = dwUnitSize;
    m_dwUnitCnt = 0;
    m_dwMaxCnt = dwMaxCnt;
    m_nGrowCnt = nGrowCnt;
    m_pfnCopy = pfnCopy;
    m_pfnDestroy = pfnDestroy;
    if (NULL != lock)
        m_lock = lock;
    else
        m_lock = &g_lock;

    m_pvData = new BYTE[dwUnitSize * dwMaxCnt];
    return NULL != m_pvData;
}

PDLINLINE PVOID LPtrVector::DataFromPos(__in int idx)
{
    return (PBYTE)m_pvData + m_dwUnitSize * idx;
}

void LPtrVector::Destroy(void)
{
    if (NULL == m_pvData || (VECTOR_ITERATING & m_dwStatus))
        return;

    LAutoLock lock(m_lock);
    if (NULL != m_pfnDestroy)
    {
        PBYTE dst = (PBYTE)m_pvData;
        for (DWORD i = 0; i < m_dwUnitCnt; ++i)
        {
            m_pfnDestroy(dst);
            dst += m_dwUnitSize;
        }
    }

    delete [] m_pvData;
    m_pvData = NULL;
    m_dwUnitSize = 0;
    m_dwUnitCnt = 0;
    m_dwMaxCnt = 0;
    m_nGrowCnt = 0;
    m_pfnCopy = NULL;
    m_pfnDestroy = NULL;

    m_lock = NULL;
}

int LPtrVector::ForEach(__in IteratePtr pfnCallBack, __in PVOID param)
{
    LAutoLock lock(m_lock);
    for (int i = 0; i < (int)m_dwUnitCnt; ++i)
    {
        if (!pfnCallBack(this, i, param))
            return i;
    }
    return -1;
}

BOOL LPtrVector::GetAt(__in int idx, __out PVOID buf)
{
    if (NULL == m_pvData || m_dwUnitCnt <= (DWORD)idx)
        return FALSE;

    CopyMemory(buf, DataFromPos(idx), m_dwUnitSize);
    return TRUE;
}

DWORD LPtrVector::GetCount(void)
{
    return m_dwUnitCnt;
}

PDLINLINE ILock* LPtrVector::GetSafeLock(void) const
{
    return (VECTOR_ITERATING & m_dwStatus) ? &g_lock : m_lock;
}

void LPtrVector::Grow(void)
{
    DWORD dwMaxCnt = m_dwMaxCnt;
    if (m_nGrowCnt > 0)
        dwMaxCnt += m_nGrowCnt;
    else
        dwMaxCnt *= 2;

    PVOID newmem = new BYTE[dwMaxCnt * m_dwUnitSize];
    CopyMemory(newmem, m_pvData, m_dwUnitSize * m_dwUnitCnt);
    delete [] m_pvData;
    m_pvData = newmem;
    m_dwMaxCnt = dwMaxCnt;
}

int LPtrVector::InsertAfter(__in int idx, __in LPCVOID pvData)
{
    if (VECTOR_ITERATING & m_dwStatus)
        return -1;

    if (NULL == m_pvData || (DWORD)idx >= m_dwUnitCnt)
        return -1;

    if (idx < 0)
        idx = m_dwUnitCnt - 1;

    LAutoLock lock(m_lock);
    if (m_dwUnitCnt == m_dwMaxCnt)
        Grow();

    PBYTE dst = (PBYTE)DataFromPos(idx + 1);
    DWORD dwSize = (m_dwUnitCnt - idx - 1) * m_dwUnitSize;
    MoveMemory(dst + m_dwUnitSize, dst, dwSize);
    CopyMemory(dst, pvData, m_dwUnitSize);
    if (NULL != m_pfnCopy)
        m_pfnCopy(dst, pvData);
    return idx + 1;
}

int LPtrVector::InsertBefore(__in int idx, __in LPCVOID pvData)
{
    if (VECTOR_ITERATING & m_dwStatus)
        return -1;

    if (NULL == m_pvData || idx < 0 || (DWORD)idx >= m_dwUnitCnt)
        return -1;

    LAutoLock lock(m_lock);
    if (m_dwUnitCnt == m_dwMaxCnt)
        Grow();

    PBYTE dst = (PBYTE)DataFromPos(idx);
    DWORD dwSize = (m_dwUnitCnt - idx) * m_dwUnitSize;
    MoveMemory(dst + m_dwUnitSize, dst, dwSize);
    CopyMemory(dst, pvData, m_dwUnitSize);
    if (NULL != m_pfnCopy)
        m_pfnCopy(dst, pvData);
    return idx;
}

BOOL LPtrVector::SetAt(__in int idx, __in LPCVOID pvData)
{
    if (NULL == m_pvData || idx < 0 || (DWORD)idx >= m_dwUnitCnt)
        return -1;

    LAutoLock lock(GetSafeLock());
    PVOID p = DataFromPos(idx);
    CopyMemory(p, pvData, m_dwUnitSize);
    if (NULL != m_pfnCopy)
        m_pfnCopy(p, pvData);
    return TRUE;
}