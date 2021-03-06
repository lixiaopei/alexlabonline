// FSGrid.cpp : implementation file
//

#include "stdafx.h"
#include "EMX.h"
#include "FSGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FullScreenGrid dialog


FullScreenGrid::FullScreenGrid(Matrix *pMatrix, CWnd* pParent /*=NULL*/)
	: CDialog(FullScreenGrid::IDD, pParent)
{
  m_pMatrix = pMatrix;

	//{{AFX_DATA_INIT(FullScreenGrid)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

  m_hWildCardBmp = NULL;
  m_hInsertRowBmp = NULL;
  m_hDeleteRowBmp = NULL;
}


FullScreenGrid::~FullScreenGrid()
{
  if (m_hWildCardBmp)
    ::DeleteObject(m_hWildCardBmp);

  if (m_hInsertRowBmp)
    ::DeleteObject(m_hInsertRowBmp);

  if (m_hDeleteRowBmp)
    ::DeleteObject(m_hDeleteRowBmp);
}



void FullScreenGrid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FullScreenGrid)
	DDX_Control(pDX, IDC_DATAVALUES_DELETEROW, m_oDeleteRowBtn);
	DDX_Control(pDX, IDC_DATAVALUES_INSERTROW, m_oInsertRowBtn);
	DDX_Control(pDX, IDC_DATAVALUES_WILDCARD, m_oWildCardBtn);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FullScreenGrid, CDialog)
	//{{AFX_MSG_MAP(FullScreenGrid)
	ON_BN_CLICKED(IDC_DATAVALUES_DELETEROW, OnDatavaluesDeleterow)
	ON_BN_CLICKED(IDC_DATAVALUES_INSERTROW, OnDatavaluesInsertrow)
	ON_BN_CLICKED(IDC_DATAVALUES_WILDCARD, OnDatavaluesWildcard)
    ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FullScreenGrid message handlers

BOOL FullScreenGrid::OnInitDialog()
{
  CDialog::OnInitDialog();

  CRect oR, oI;

  GetClientRect(&oR);
  GetDlgItem(IDC_GRID_PLACEHOLDER)->GetWindowRect(&oI);
  ScreenToClient(&oI);

  int iR = oI.left;
  int iB = oR.bottom-oI.bottom;

  SystemParametersInfo(SPI_GETWORKAREA,0,&oR,0);

  MoveWindow(&oR,FALSE);
  GetClientRect(&oR);

  oI.right = oR.right - iR;
  oI.bottom = oR.bottom - iB;

  GetDlgItem(IDC_GRID_PLACEHOLDER)->MoveWindow(&oI,FALSE);

  CenterWindow();

  EnableToolTips();

  m_hWildCardBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_WILDCARD));
  m_oWildCardBtn.SetBitmap(m_hWildCardBmp);

  m_hInsertRowBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_INSERTROW));
  m_oInsertRowBtn.SetBitmap(m_hInsertRowBmp);

  m_hDeleteRowBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_DELETEROW));
  m_oDeleteRowBtn.SetBitmap(m_hDeleteRowBmp);

  m_oGrid.m_pMatrix = m_pMatrix;
  m_oGrid.m_iParentType = 1;
  m_oGrid.AttachGrid(this,IDC_GRID_PLACEHOLDER);

  BuildGridStructure();

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}



void FullScreenGrid::BuildGridStructure()
{
  int iColCount = m_pMatrix->GetColumnCount();

  // erase current grid
  for (int iCol = m_oGrid.GetNumberCols()-1; iCol >= 0; iCol--)
    m_oGrid.DeleteCol(iCol);

  m_oGrid.SetNumberCols(0);
  m_oGrid.SetNumberRows(0);

  m_oGrid.SetNumberCols(iColCount+1);
  m_oGrid.SetNumberRows(m_pMatrix->GetRowCount() + 1);

  CUGCell oCell;
  short iType, iAlign;
  MatrixColDef *pColDef;
  LPCSTR cpMask;
  char caBuf[128];

  for (int iLup = 0; iLup < iColCount; iLup++)
    {
      pColDef = m_pMatrix->GetColumnDef(iLup);

      m_oGrid.GetColDefault(iLup, &oCell);
      if (pColDef->m_bUsePickList)
        {
	      oCell.SetCellType(UGCT_DROPLIST);
	      oCell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON);

          CString cChoices = "*\n";
          for (int iEntry = 0; iEntry < pColDef->m_oPickListChoices.GetSize(); iEntry++)
            cChoices += pColDef->m_oPickListChoices.GetAt(iEntry) + CString("\n");

	      oCell.SetLabelText(cChoices);
        }

      switch (pColDef->m_iColType)
        {
          case COLTYPE_STRING:
          case COLTYPE_FORMULA:
            iType = UGCELLDATA_STRING;
            iAlign = UG_ALIGNLEFT;
            cpMask = "";
            break;

          case COLTYPE_INTEGER:
            oCell.SetNumberDecimals(0);
            cpMask = "#########";
            iType = UGCELLDATA_NUMBER;
            iAlign = UG_ALIGNRIGHT;
            break;

          case COLTYPE_DOUBLE:
            oCell.SetNumberDecimals(-1);
            iType = UGCELLDATA_NUMBER;
            iAlign = UG_ALIGNRIGHT;
            cpMask = "666666666666666";
            break;

          case COLTYPE_DATE:
            iType = UGCELLDATA_TIME;
            iAlign = UG_ALIGNLEFT;

            caBuf[0] = 0;
            m_pMatrix->GetDateMask(caBuf);
            if (caBuf[0])
              cpMask = caBuf;
            else
              cpMask = "99/99/9999";
            break;

          default:
            iType = UGCELLDATA_STRING;
            iAlign = UG_ALIGNLEFT;
            cpMask = "";
            break;
        }

      oCell.SetDataType(iType);
      oCell.SetAlignment(iAlign);
      oCell.SetMask(cpMask);

      m_oGrid.SetColDefault(iLup, &oCell);

      m_oGrid.QuickSetText(iLup, -1, pColDef->m_cColName);
    }

  // now, add result column
  m_oGrid.GetColDefault(iColCount, &oCell);
  switch (m_pMatrix->GetReturnValueColumnType())
    {
      case COLTYPE_STRING:
      case COLTYPE_FORMULA:
        iType = UGCELLDATA_STRING;
        iAlign = UG_ALIGNLEFT;
        cpMask = "";
        break;

      case COLTYPE_INTEGER:
        oCell.SetNumberDecimals(0);
        cpMask = "#########";
        iType = UGCELLDATA_NUMBER;
        iAlign = UG_ALIGNRIGHT;
        break;

      case COLTYPE_DOUBLE:
        oCell.SetNumberDecimals(-1);
        iType = UGCELLDATA_NUMBER;
        iAlign = UG_ALIGNRIGHT;
        cpMask = "666666666666666";
        break;

      case COLTYPE_DATE:
        iType = UGCELLDATA_TIME;
        iAlign = UG_ALIGNLEFT;

        caBuf[0] = 0;
        m_pMatrix->GetDateMask(caBuf);
        if (caBuf[0])
          cpMask = caBuf;
        else
          cpMask = "99/99/9999";
        break;

      default:
        iType = UGCELLDATA_STRING;
        iAlign = UG_ALIGNLEFT;
        cpMask = "";
        break;
    }

  oCell.SetDataType(iType);
  oCell.SetAlignment(iAlign);
  oCell.SetMask(cpMask);

  m_oGrid.SetColDefault(iColCount, &oCell);

  m_oGrid.BestFit(0,iColCount,m_pMatrix->GetRowCount(),UG_BESTFIT_TOPHEADINGS);

  // expand columns evenly to fill the window...
  int iWidth = 0;
  int iTotalWidth = m_oGrid.GetSH_Width();
  iColCount++;
  for (iLup = 0; iLup < iColCount; iLup++)
    {
      m_oGrid.GetColWidth(iLup,&iWidth);
      iTotalWidth += iWidth;
    }

  CRect oR;
  m_oGrid.GetClientRect(&oR);
  int iWinWidth = oR.Width() - ::GetSystemMetrics(SM_CXVSCROLL);
  if (iTotalWidth < iWinWidth)
    {
      iWinWidth = ((iWinWidth - iTotalWidth) / iColCount) - 1;

      if (iWinWidth > 0)
        {
          for (iLup = 0; iLup < iColCount; iLup++)
            {
              m_oGrid.GetColWidth(iLup,&iWidth);
              iWidth += iWinWidth;
              m_oGrid.SetColWidth(iLup,iWidth);
            }
        }
    }
}



void FullScreenGrid::OnDatavaluesWildcard()
{
  int  iStartCol, iEndCol;
  long lStartRow, lEndRow;

  iStartCol = iEndCol = m_oGrid.GetCurrentCol();
  lStartRow = lEndRow = m_oGrid.GetCurrentRow();

  m_oGrid.m_GI->m_multiSelect->GetCurrentBlock(&iStartCol, &lStartRow, &iEndCol, &lEndRow);
  if (iStartCol == -1 && iEndCol == -1)
    {
      iStartCol = 0;
      iEndCol = m_oGrid.GetNumberCols() - 1;
    }

  if (lStartRow == -1 && lEndRow == -1)
    {
      lStartRow = 0;
      lEndRow = m_oGrid.GetNumberRows();
    }

  for (int iCol = iStartCol; iCol <= iEndCol; iCol++)
    {
      if (iCol < 0 || iCol == m_pMatrix->GetColumnCount())
        continue;  // skip headings and return columns

      for (int iRow = lStartRow; iRow <= lEndRow; iRow++)
        {
          if (iRow < 0)
            continue;  // skip heading row

          m_oGrid.QuickSetText(iCol,iRow,"*");
        }
    }

  m_oGrid.RedrawAll();

  GotoDlgCtrl(&m_oGrid);
}



void FullScreenGrid::OnDatavaluesDeleterow()
{
  int  iStartCol, iEndCol;
  long lStartRow, lEndRow;

  iStartCol = iEndCol = m_oGrid.GetCurrentCol();
  lStartRow = lEndRow = m_oGrid.GetCurrentRow();

  // m_oGrid.m_GI->m_multiSelect->GetCurrentBlock(&iStartCol, &lStartRow, &iEndCol, &lEndRow);
  m_oGrid.m_GI->m_multiSelect->GetTotalRange(&iStartCol, &lStartRow, &iEndCol, &lEndRow);

  if (lStartRow != lEndRow && (iStartCol == iEndCol || iStartCol != 0))
    return;

  CString cMsg, cTitle;

  if (!cTitle.LoadString(IDS_CONFIRMDELETETITLE))
    cTitle = "Confirm Delete";

  if (!cMsg.LoadString(IDS_CONFIRMDELETEROWS))
    cMsg = "Delete These Rows.\x0D\x0A Are You Sure?";

  if (::MessageBox(GetParent()->m_hWnd,(LPCSTR)cMsg,(LPCSTR)cTitle,MB_ICONQUESTION|MB_YESNO) == IDYES)
    {
      for (int iLup = lEndRow; iLup >= lStartRow; iLup--)
        {
          m_pMatrix->DeleteRow(iLup);
          m_oGrid.DeleteRow(iLup);
        }

      // make sure that we always have at least one row...
      if (m_oGrid.GetNumberRows() == 0)
        m_oGrid.AppendRow();
    }

  GotoDlgCtrl(&m_oGrid);
}



void FullScreenGrid::OnDatavaluesInsertrow()
{
  long lRow = m_oGrid.GetCurrentRow();
  if (lRow < 0)
    lRow = 0;

  int iCol = m_oGrid.GetCurrentCol();
  if (iCol < 0)
    iCol = 0;

  int  iStartCol, iEndCol;
  long lStartRow, lEndRow;

  iStartCol = iEndCol = iCol;
  lStartRow = lEndRow = lRow;

  m_oGrid.m_GI->m_multiSelect->GetCurrentBlock(&iStartCol, &lStartRow, &iEndCol, &lEndRow);
  if (lStartRow == lEndRow && lStartRow != lRow)
    lRow = lStartRow;

  m_pMatrix->InsertRow(lRow);
  m_oGrid.AppendRow();

  m_oGrid.RedrawAll();

  m_oGrid.GotoCell(iCol,lRow);

  GotoDlgCtrl(&m_oGrid);
}



BOOL FullScreenGrid::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
  TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
  UINT nID = pNMHDR->idFrom;
  if (pTTT->uFlags & TTF_IDISHWND)
    {
      // idFrom is actually the HWND of the tool
      nID = ::GetDlgCtrlID((HWND)nID);
      if (nID)
        {
          switch (nID)
            {
              case IDC_DATAVALUES_DELETEROW:
                // pTTT->lpszText = "Delete Selected Rows";
                pTTT->lpszText = MAKEINTRESOURCE(IDS_DELETEROWSTOOLTIP);
                break;
              case IDC_DATAVALUES_INSERTROW:
                // pTTT->lpszText = "Insert A New Row";
                pTTT->lpszText = MAKEINTRESOURCE(IDS_INSERTROWTOOLTIP);
                break;
              case IDC_DATAVALUES_WILDCARD:
                // pTTT->lpszText = "Wildcard The Current Selection";
                pTTT->lpszText = MAKEINTRESOURCE(IDS_WILDCARDTOOLTIP);
                break;
              default:
                return FALSE;
                break;
            }

          pTTT->hinst = AfxGetResourceHandle();

          return(TRUE);
        }
    }

  return(FALSE);
}


