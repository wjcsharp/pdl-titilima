/**
 * \file pdl_base.h
 * \brief PDL ����ͷ�ļ�
 * \details ���� PDL �Ļ���ͷ�ļ���ÿ������ PDL ��Դ�����ļ���Ӧ������ͷ����������ļ���
 */

#pragma once

#ifndef STRICT
#define STRICT
#endif // STRICT

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

#ifdef _CRTIMP
#undef _CRTIMP
#endif // _CRTIMP
#define _CRTIMP

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <tchar.h>
#include <CommCtrl.h>

/**
 * \def PDLASSERT
 * �Ա���ʽ���ж��ԡ�
 */

/**
 * \def PDLTRACE
 * ���������Ϣ��
 */

/**
 * \def PDLLOG
 * �����־��Ϣ��
 */

#ifdef _DEBUG

#define _PDL_WSTR(s)    L ## s
#define PDL_WSTR(s)     _PDL_WSTR(s)
#define PDLASSERT(expr) (void)((!!(expr)) || (LAssertBox(L###expr, PDL_WSTR(__FILE__), __LINE__)))
#define PDLTRACE        LTrace
#define PDLLOG          LTrace

#else

#define PDLASSERT(expr) (expr)
#define PDLTRACE        (void)
#define PDLLOG          LAppModule::DebugPrint

#endif // _DEBUG

/**
 * \def PDLAPI
 * PDL ��������Լ����
 */
#ifndef PDLAPI
#define PDLAPI  __stdcall
#endif

/**
 * \def PDLINLINE
 * PDL ������־��
 */
#ifndef PDLINLINE
#define PDLINLINE   __inline
#endif // PDLINLINE

/**
 * \def GET_X_LPARAM
 * ����Ϣ�� lParam �����л�ȡ x ���ꡣ
 */
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
/**
 * \def GET_Y_LPARAM
 * ����Ϣ�� lParam �����л�ȡ y ���ꡣ
 */
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
/**
 * \def PDL_ARGUMENT_PRESENT
 * �ж�ָ���Ĳ����Ƿ���á�
 */
#ifndef PDL_ARGUMENT_PRESENT
#define PDL_ARGUMENT_PRESENT(ArgumentPointer)       \
    ((CHAR *)(ArgumentPointer) != (CHAR *)(NULL))
#endif // PDL_ARGUMENT_PRESENT

/**
 * \def PDL_NO_VTABLE
 * ָ��һ����Ϊ�������
 */
#ifndef PDL_NO_VTABLE
#define PDL_NO_VTABLE __declspec(novtable)
#endif // PDL_NO_VTABLE

/**
 * \def PDL_SYSBRUSH
 * ��ȡһ��ϵͳ��ˢ��
 */
#ifndef PDL_SYSBRUSH
#define PDL_SYSBRUSH(br)    ((HBRUSH)(br + 1))
#endif

/**
 * \def LEOF
 * �ļ�������־
 */
#ifndef LEOF
#define LEOF    (-1)
#endif

///////////////////////////////////////////////////////////////////////////////
// ȫ�ֱ�������

/**
 * \var WM_PDL_GETOBJECTA
 * \brief ��ȡ���ڶ�Ӧ�� PDL ������ָ�뼰�����ַ�����
 * @param [in] wParam ���ڽ��������Ļ�������С�����ַ��ơ�
 * @param [out] lParam ���ڽ��������Ļ�������ַ��
 * @return ��������Ӧ�� PDL ������ָ�롣
 */
extern UINT WM_PDL_GETOBJECTA;

/**
 * \var WM_PDL_GETOBJECTW
 * \brief ��ȡ���ڶ�Ӧ�� PDL ������ָ�뼰�����ַ�����
 * @param [in] wParam ���ڽ��������Ļ�������С���Կ��ַ��ơ�
 * @param [out] lParam ���ڽ��������Ļ�������ַ��
 * @return ��������Ӧ�� PDL ������ָ�롣
 */
extern UINT WM_PDL_GETOBJECTW;

/**
 * \def WM_PDL_GETOBJECT
 * WM_PDL_GETOBJECTA �� WM_PDL_GETOBJECTW �� UNICODE ���ݺꡣ
 */
#ifdef UNICODE
#define WM_PDL_GETOBJECT    WM_PDL_GETOBJECTW
#else
#define WM_PDL_GETOBJECT    WM_PDL_GETOBJECTA
#endif // UNICODE

#define PDL_NOTIFY              0
#define PDL_NOTIFY_DRAWITEM     1
#define PDL_NOTIFY_CUSTOMDRAW   2

/**
 * \var WM_PDL_GETNOTIFY
 * \brief ��ȡ���ڶ�Ӧ��֪ͨ����ָ�롣
 * @param [in] wParam ����ָ��֪ͨ��������͡�
 * @return ���ڶ�Ӧ��֪ͨ����ָ�롣
 */
extern UINT WM_PDL_GETNOTIFY;

/**
 * ָ������ת��������ͻ�����Ա���������ơ�
 */
template <typename R, typename T>
PDLINLINE R ptr_cast(T t)
{
    PVOID r = *reinterpret_cast<PVOID*>(&t);
    return static_cast<R>(r);
}

/**
 * ƫ��������ת��
 */
template <typename T>
PDLINLINE T offset_cast(PVOID base, int offset)
{
    PBYTE pb = reinterpret_cast<PBYTE>(base);
    return reinterpret_cast<T>(pb + offset);
}

/**
 * �������ԶԻ���
 * @param [in] expr ���Ա���ʽ��
 * @param [in] srcfile �������Ե�Դ�ļ���
 * @param [in] nLine Դ�ļ���������
 */
int PDLAPI LAssertBox(__in PCWSTR expr, __in PCWSTR srcfile, __in int nLine);

/**
 * ���������Ϣ��
 */
void PDLAPI LTrace(__in PCSTR lpszFormat, ...);
/**
 * ���������Ϣ��
 */
void PDLAPI LTrace(__in PCWSTR lpszFormat, ...);

/**
 * Thunk ָ�붨��
 */
typedef struct _tagThisThunk *PTHISTHUNK;

/**
 * ����һ�� This Thunk��
 * @param [in] This Ҫ�󶨵������ָ�롣
 * @param [in] pfn Ҫ�󶨵����Ա����ָ�롣
 * @return ����ɹ��򷵻� Thunk ��ָ�룬���򷵻� NULL��
 * \sa LDestroyThisThunk
 */
PTHISTHUNK PDLAPI LCreateThisThunk(PVOID This, PVOID pfn);

/**
 * ����һ�� This Thunk��
 * @param [in] thunk һ����Ч�� Thunk ָ�롣
 * \sa LCreateThisThunk
 */
void PDLAPI LDestroyThisThunk(PTHISTHUNK thunk);

/**
 * \class LThisThunk
 * \brief This Thunk �����ࡣ
 */

template <typename T>
class LThisThunk
{
    /**
     * Thunk ָ��
     */
    PTHISTHUNK thunk;
public:
    /**
     * ���캯��
     */
    LThisThunk(void)
    {
        thunk = NULL;
    }
    /**
     * ��������
     */
    ~LThisThunk(void)
    {
        DestroyThunk();
    }
    /**
     * ����ת��������
     */
    operator T (void)
    {
        return (T)thunk;
    }
    /**
     * ����һ�� Thunk��
     */
    template<typename F>
    BOOL CreateThunk(PVOID This, F fn)
    {
        if (NULL != thunk)
            return FALSE;

        thunk = LCreateThisThunk(This, ptr_cast<PVOID>(fn));
        return NULL != thunk;
    }
    /**
     * ���� Thunk��
     */
    void DestroyThunk(void)
    {
        if (NULL != thunk)
            LDestroyThisThunk(thunk);
        thunk = NULL;
    }
};

/**
 * \class ILock
 * \brief PDL ������
 */
class ILock
{
public:
    /**
     * ����һ���������ӿڡ�
     * @return �������ӿڡ�
     */
    static ILock* Create(void);
    /**
     * ����һ��ȫ�ֲ������ӿڡ�
     * @param [in] lpName ���������ơ�
     * @return �������ӿڡ�
     */
    static ILock* Create(__in PCSTR lpName);
    /**
     * ����һ��ȫ�ֲ������ӿڡ�
     * @param [in] lpName ���������ơ�
     * @return �������ӿڡ�
     */
    static ILock* Create(__in PCWSTR lpName);
    /**
     * ���ٲ�������
     */
    virtual void Destroy(void) = 0;
    /**
     * ������������
     */
    virtual void Lock(void) = 0;
    /**
     * ������
     */
    virtual void Unlock(void) = 0;
};

/**
 * \class LAutoLock
 * \brief �Զ���
 */
class LAutoLock
{
public:
    /**
     * ���캯��
     */
    LAutoLock(__in ILock* lock) : m_lock(lock)
    {
        m_lock->Lock();
    }
    /**
     * ��������
     */
    ~LAutoLock(void)
    {
        m_lock->Unlock();
    }
private:
    /**
     * ������
     */
    ILock* m_lock;
};