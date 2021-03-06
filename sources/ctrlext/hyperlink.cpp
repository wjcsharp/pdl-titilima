///////////////////////////////////////////////////////////////////////////////
// FileName:    hyperlink.cpp
// Created:     2009/04/21
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2009
//-----------------------------------------------------------------------------
// Information: 超级链接控件实现
///////////////////////////////////////////////////////////////////////////////

#include "..\..\include\pdl_ctrlext.h"
#include "..\..\include\pdl_registry.h"
#include "..\..\include\pdl_gdi.h"
#include "..\..\include\pdl_string.h"
#include "..\..\include\pdl_parser.h"

PDL_DEFINE_WINCLASS(LHyperLink)

LHyperLink::LHyperLink(void) : m_bHover(FALSE)
{
    m_hCursor = ::LoadCursor(NULL, IDC_HAND);

    LRegKey key;
    if (ERROR_SUCCESS == key.Open(HKCU,
        _T("Software\\Microsoft\\Internet Explorer\\Settings")))
    {
        LString strColor = _T("0,0,255");
        key.QueryStringValue(_T("Anchor Color"), &strColor);
        m_clrNormal = LParseColorString(strColor, COLOR_BLUE);
        strColor = _T("255,0,0");
        key.QueryStringValue(_T("Anchor Color Hover"), &strColor);
        m_clrHover = LParseColorString(strColor, COLOR_RED);
    }
    else
    {
        m_clrNormal = COLOR_BLUE;
        m_clrHover = COLOR_RED;
    }
}

LHyperLink& LHyperLink::operator=(__in HWND hWnd)
{
    Attach(hWnd);
    return *this;
}

BOOL LHyperLink::Attach(__in HWND hWnd)
{
    PDLASSERT(::IsWindow(hWnd) && NULL == m_hWnd);
    LONG lStyle = ::GetWindowLong(hWnd, GWL_STYLE);
    ::SetWindowLong(hWnd, GWL_STYLE, lStyle | SS_NOTIFY);
    LStatic::Attach(hWnd);
    return LSubclassWnd::SubclassWindow(hWnd);
}

BOOL LHyperLink::Create(
    __in PCSTR lpWindowName,
    __in DWORD dwStyle,
    __in LPCRECT lpRect,
    __in HWND hWndParent,
    __in UINT nID)
{
    BOOL bRet = LStatic::Create(lpWindowName, dwStyle | SS_NOTIFY,
        lpRect, hWndParent, nID);
    LSubclassWnd::SubclassWindow(m_hWnd);
    return bRet;
}

BOOL LHyperLink::Create(
    __in PCWSTR lpWindowName,
    __in DWORD dwStyle,
    __in LPCRECT lpRect,
    __in HWND hWndParent,
    __in UINT nID)
{
    BOOL bRet = LStatic::Create(lpWindowName, dwStyle | SS_NOTIFY,
        lpRect, hWndParent, nID);
    LSubclassWnd::SubclassWindow(m_hWnd);
    return bRet;
}

BOOL LHyperLink::CreateEx(
    __in DWORD dwExStyle,
    __in PCSTR lpWindowName,
    __in DWORD dwStyle,
    __in LPCRECT lpRect,
    __in HWND hWndParent,
    __in UINT nID)
{
    BOOL bRet = LStatic::CreateEx(dwExStyle, lpWindowName,
        dwStyle, lpRect, hWndParent, nID);
    LSubclassWnd::SubclassWindow(m_hWnd);
    return bRet;
}

BOOL LHyperLink::CreateEx(
    __in DWORD dwExStyle,
    __in PCWSTR lpWindowName,
    __in DWORD dwStyle,
    __in LPCRECT lpRect,
    __in HWND hWndParent,
    __in UINT nID)
{
    BOOL bRet = LStatic::CreateEx(dwExStyle, lpWindowName,
        dwStyle, lpRect, hWndParent, nID);
    LSubclassWnd::SubclassWindow(m_hWnd);
    return bRet;
}

COLORREF LHyperLink::SetHoverColor(
    __in COLORREF clrHover,
    __in BOOL bRedraw /* = TRUE */)
{
    PDLASSERT(IsWindow());
    COLORREF ret = m_clrHover;
    m_clrHover = clrHover;
    if (bRedraw)
        Invalidate(FALSE);
    return ret;
}

COLORREF LHyperLink::SetNormalColor(
    __in COLORREF clrNormal,
    __in BOOL bRedraw /* = TRUE */)
{
    PDLASSERT(IsWindow());
    COLORREF ret = m_clrNormal;
    m_clrNormal = clrNormal;
    if (bRedraw)
        Invalidate(FALSE);
    return ret;
}

BOOL LHyperLink::SizeToContent(
    __in BOOL bRedraw /* = TRUE */,
    __in BOOL bForce /* = FALSE */)
{
    if (!IsWindow())
        return FALSE;

    // 无文本直接返回
    int nLen = GetWindowTextLengthA();
    if (0 == nLen)
        return FALSE;

    LString strText;
    GetWindowText(&strText);

    RECT rc = { 0 };
    GetClientRect(&rc);

    RECT rcTxt = { 0 };
    LClientDC dc(m_hWnd);
    HFONT hFontParent = (HFONT)::SendMessage(GetParent(), WM_GETFONT, 0, 0);
    HFONT hFontOld = dc.SelectFont(hFontParent);
    dc.DrawText(strText, -1, &rcTxt, DT_CALCRECT);
    dc.SelectFont(hFontOld);

    if (rc.right < rcTxt.right || bForce)
        rc.right = rcTxt.right;
    if (rc.bottom < rcTxt.bottom || bForce)
        rc.bottom = rcTxt.bottom;

    UINT uFlags = SWP_NOZORDER | SWP_NOMOVE;
#ifndef _WIN32_WCE
    if (!bRedraw)
        uFlags |= SWP_NOREDRAW;
#endif // _WIN32_WCE
    return SetWindowPos(NULL, 0, 0, rc.right, rc.bottom, uFlags);
}

///////////////////////////////////////////////////////////////////////////////

PDL_BEGIN_MSGMAP(LHyperLink)
    PROCESS_ERASEBKGND(OnEraseBkgnd)
    PROCESS_MOUSELEAVE(OnMouseLeave)
    PROCESS_MOUSEMOVE(OnMouseMove)
    PROCESS_PAINT(OnPaint)
    PROCESS_SETCURSOR(OnSetCursor)
PDL_END_MSGMAP(LSubclassWnd)

BOOL LHyperLink::OnEraseBkgnd(HDC hdc, BOOL& bHandled)
{
    return TRUE;
}

void LHyperLink::OnMouseLeave(BOOL& bHandled)
{
    m_bHover = FALSE;
#ifndef _WIN32_WCE
    Invalidate(WS_EX_TRANSPARENT & GetWindowLong(GWL_EXSTYLE));
#endif // _WIN32_WCE
}

void LHyperLink::OnMouseMove(UINT uFlags, int x, int y, BOOL& bHandled)
{
#ifdef _WIN32_WCE
    bHandled = FALSE;
#else
    if (!m_bHover)
    {
        m_bHover = TRUE;
        TrackMouseEvent(TME_LEAVE);
        Invalidate(FALSE);
    }
#endif // _WIN32_WCE
}

void LHyperLink::OnPaint(BOOL& bHandled)
{
    LPaintDC dc(m_hWnd);

    RECT rect;
    GetClientRect(&rect);

    HBRUSH hBrush = NULL;
    DWORD dwExStyle = GetWindowLong(GWL_EXSTYLE);

#ifndef _WIN32_WCE
    if (!(WS_EX_TRANSPARENT & dwExStyle))
    {
        hBrush = (HBRUSH)::SendMessage(GetParent(), WM_CTLCOLORSTATIC,
            (WPARAM)dc.GetSafeHDC(), (LPARAM)m_hWnd);
        dc.FillRect(&rect, hBrush);
    }
#endif // _WIN32_WCE

    // 画文字
    HFONT hFontParent = (HFONT)::SendMessage(GetParent(), WM_GETFONT, 0, 0);
    LOGFONT lf;
    GetObject(hFontParent, sizeof(LOGFONT), &lf);
    if (m_bHover)
        lf.lfUnderline = 1;

    LFont font = CreateFontIndirect(&lf);

    HFONT hFontOld = dc.SelectFont(font);
    COLORREF clr = dc.SetTextColor(m_bHover ? m_clrHover : m_clrNormal);
    int mode = dc.SetBkMode(TRANSPARENT);

    LString str;
    GetWindowText(&str);

    DWORD dwStyle = GetWindowLong(GWL_STYLE);
    UINT uFormat = 0;
    if (SS_CENTER & dwStyle)
        uFormat |= DT_SINGLELINE | DT_CENTER | DT_VCENTER;
    if (SS_RIGHT & dwStyle)
        uFormat |= DT_RIGHT;
    dc.DrawText(str, -1, &rect, uFormat);

    dc.SetBkMode(mode);
    dc.SetTextColor(clr);
    dc.SelectFont(hFontOld);
}

BOOL LHyperLink::OnSetCursor(
    HWND hWnd,
    UINT nHitTest,
    UINT message,
    BOOL& bHandled)
{
    SetCursor(m_hCursor);
    return FALSE;
}
