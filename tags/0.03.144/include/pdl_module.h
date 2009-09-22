/**
 * \file pdl_module.h
 * \brief PDL Ӧ�ó���ģ����
 */

#pragma once

#include "pdl_base.h"

/**
 * \class LAppModule
 * \brief PDL Ӧ�ó���ģ����
 */

class LStringA;
class LStringW;
class LAppModule
{
public:

    /**
     * ��� LAppModule ����ָ�롣
     * @return ��ǰȫ�ֵ� LAppModule ����ָ�롣
     */
    static LAppModule* GetApp(void);

    /**
     * ���Ӧ�ó���ʵ�������
     * @return Ӧ�ó����ʵ�������Ҳ���� WinMain ����ڲ�����
     */
    HINSTANCE GetInstance(void) const;

    /**
     * ��ʼ�� LAppModule ȫ�ֶ���
     * @param [in] hInstance Ӧ�ó����ʵ�������Ҳ���� WinMain ����ڲ�����
     * @return ����ɹ��򷵻����ɵ� LAppModule ����ָ�룬���򷵻� NULL��
     * \note ���������Ҫ�� WinMain ��ڴ����á�
     */
    static LAppModule* Initialize(__in HINSTANCE hInstance);

    /**
     * ���� LAppModule ȫ�ֶ���
     * @return ������ٳɹ��򷵻� TRUE�����򷵻� FALSE��
     */
    static BOOL Destroy(void);

    /**
     * ��һ����������ָ������ݴ档
     * @param [in] lpWndData һ���������ݵ�ָ�롣
     */
    void AddWndData(__in PVOID lpWndData);

    /**
     * ��ȡ��һ���ݴ�Ĵ�������ָ�롣
     * @return ��һ���ݴ�Ĵ�������ָ�롣
     */
    PVOID ExtractWndData(void);

public:

    /**
     * ���������־��
     * @param [in] lpszFormat Ҫ�������־�ַ�����ʽ��
     * \note ���Ӧ�ó���Ŀ¼�´��� debuglog.yes �ļ�������־������ļ��У������Ե�����Ϣ�����
     */
    static void DebugPrint(__in PCSTR lpszFormat, ...);

    /**
     * ���������־��
     * @param [in] lpszFormat Ҫ�������־�ַ�����ʽ��
     * \note ���Ӧ�ó���Ŀ¼�´��� debuglog.yes �ļ�������־������ļ��У������Ե�����Ϣ�����
     */
    static void DebugPrint(__in PCWSTR lpszFormat, ...);

    HRSRC FindResourceA(__in PCSTR lpName, __in PCSTR lpType);
    HRSRC FindResourceW(__in PCWSTR lpName, __in PCWSTR lpType);

    /**
     * ��ȡӦ�ó�������·����
     * @param [out] path ���ڽ���Ӧ�ó���·���� LStringA ����ָ�롣
     * @return ����ɹ��򷵻� TRUE�����򷵻� FALSE��
     */
    static BOOL GetAppPath(__out LStringA* path);

    /**
     * @param [out] path ���ڽ���Ӧ�ó���·���� LStringW ����ָ�롣
     * @return ����ɹ��򷵻� TRUE�����򷵻� FALSE��
     */
    static BOOL GetAppPath(__out LStringW* path);

    HACCEL LoadAcceleratorsA(__in PCSTR lpTableName);
    HACCEL LoadAcceleratorsW(__in PCWSTR lpTableName);
    HBITMAP LoadBitmapA(__in PCSTR lpBitmapName);
    HBITMAP LoadBitmapW(__in PCWSTR lpBitmapName);
    HCURSOR LoadCursorA(__in PCSTR lpCursorName);
    HCURSOR LoadCursorW(__in PCWSTR lpCursorName);
    HICON LoadIconA(__in PCSTR lpIconName);
    HICON LoadIconW(__in PCWSTR lpIconName);
    HANDLE LoadImageA(__in PCSTR name, __in UINT type, __in int cx,
        __in int cy, __in UINT fuLoad);
    HANDLE LoadImageW(__in PCWSTR name, __in UINT type, __in int cx,
        __in int cy, __in UINT fuLoad);
    HMENU LoadMenuA(__in PCSTR lpMenuName);
    HMENU LoadMenuW(__in PCWSTR lpMenuName);
    HGLOBAL LoadResource(__in HRSRC hResInfo);
    int LoadStringA(__in UINT uID, __out PSTR lpBuffer,
        __in int nBufferMax);
    int LoadStringW(__in UINT uID, __out PWSTR lpBuffer,
        __in int nBufferMax);
    int LoadStringA(__in UINT id, __out LStringA* str);
    int LoadStringW(__in UINT id, __out LStringW* str);
    DWORD SizeofResource(__in HRSRC hResInfo);
protected:
    LAppModule(__in HINSTANCE hInstance);
    virtual ~LAppModule(void);
protected:
    /**
     * ȫ�ֵ� LAppModule ����ָ��
     */
    static LAppModule *m_pApp;
    /**
     * Ӧ�ó���ʵ�����
     */
    HINSTANCE m_hInstance;
    /**
     * �ݴ洰������
     */
    PVOID* m_pvWndData;
    /**
     * �������ݸ���
     */
    int m_cntWndData;
    /**
     * ��������������
     */
    int m_maxWndData;
};