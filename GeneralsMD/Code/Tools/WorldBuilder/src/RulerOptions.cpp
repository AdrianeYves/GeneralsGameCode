/*
**	Command & Conquer Generals Zero Hour(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Mike Lytle
// 01/07/03
// RulerOptions.cpp

#include "StdAfx.h"
#include "resource.h"
#include "Lib/BaseType.h"
#include "RulerOptions.h"
#include "WorldBuilderView.h"
#include "WorldBuilderDoc.h"
#include "RulerTool.h"

RulerOptions*	RulerOptions::m_staticThis = NULL;

/////////////////////////////////////////////////////////////////////////////
RulerOptions::RulerOptions(CWnd* pParent /*=NULL*/)
{
	//{{AFX_DATA_INIT(RulerOptions) 
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

/// Windows default stuff.
void RulerOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RulerOptions)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


/** Update the value in the edit control. */
void RulerOptions::setWidth(Real width)
{
	CString buf;
	// Multiply by 2 because we are changing from radius to diameter, then convert
	// to the selected display unit (feet or meters).
	buf.Format("%f", RulerTool::toDisplayUnits(width * 2.0f));
	if (m_staticThis && !m_staticThis->m_updating) {
		CWnd *pEdit = m_staticThis->GetDlgItem(IDC_RULER_WIDTH);
		if (pEdit && pEdit->IsWindowEnabled()) {
			pEdit->SetWindowText(buf);
		}
	}
}


/// Dialog UI initialization.
BOOL RulerOptions::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_staticThis = this;
	m_updating = false;

	// Reflect the current feet/meters preference in the checkbox.
	CButton *pMeters = (CButton*)GetDlgItem(IDC_RULER_USE_METERS);
	if (pMeters) {
		pMeters->SetCheck(RulerTool::getUseMeters() ? 1 : 0);
	}

	// Reflect whether activating the ruler forces the grid overlay on.
	CButton *pGrid = (CButton*)GetDlgItem(IDC_RULER_SHOW_GRID);
	if (pGrid) {
		pGrid->SetCheck(RulerTool::getShowGridOnActivate() ? 1 : 0);
	}

	if (RulerTool::getType() != RULER_CIRCLE) {
		CWnd *pEdit = GetDlgItem(IDC_RULER_WIDTH);
		if (pEdit) {
			// Disable the edit box since the ruler isn't a circle.
			pEdit->EnableWindow(false);
		}
	} else {
		// Only the circle rulers use the edit box to change the size.
		setWidth(RulerTool::getLength());
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void RulerOptions::OnChangeWidthEdit() 
{
	if (m_updating) return;
	CWnd *pEdit = GetDlgItem(IDC_RULER_WIDTH);
	char buffer[_MAX_PATH];
	if (pEdit) {
		pEdit->GetWindowText(buffer, sizeof(buffer));
		float width;
		m_updating = true;
		// Pull out the length from the text.
		if (1 == sscanf(buffer, "%f", &width)) {
			// The typed value is in the selected display unit; convert back to feet
			// (= world units) before storing, then change from diameter to radius.
			if (RulerTool::getUseMeters()) {
				width /= 0.3048f;
			}
			RulerTool::setLength(width / 2.0f);
		}
		m_updating = false;
	}
}

void RulerOptions::OnChangeCheckRuler() 
{
	if (m_updating) return;
	CWnd *pCheck = GetDlgItem(IDC_CHECK_RULER);
	if (pCheck) {
		if (RulerTool::switchType()) {
			CWnd *pEdit = GetDlgItem(IDC_RULER_WIDTH);
			if (pEdit) {
				// The edit box is disabled when the ruler is a line.
				pEdit->EnableWindow(!pEdit->IsWindowEnabled());
				// Make sure that the length is updated.
				if (pEdit->IsWindowEnabled()) {
					setWidth(RulerTool::getLength());
				}
			}
		}
	}
}




/** The "Use meters" checkbox toggles the display unit for all ruler readouts. The
 ** stored length stays in feet; only the displayed numbers change. Refresh the edit
 ** box (for circle mode) and the 3D view so the new unit shows immediately. */
void RulerOptions::OnChangeUseMeters()
{
	if (m_updating) return;
	CWnd *pCheck = GetDlgItem(IDC_RULER_USE_METERS);
	if (pCheck) {
		RulerTool::setUseMeters(((CButton*)pCheck)->GetCheck() != 0);
	}

	// Re-display the current length in the new unit (only meaningful for circles,
	// where the edit box is enabled).
	if (RulerTool::getType() == RULER_CIRCLE) {
		setWidth(RulerTool::getLength());
	}

	// Repaint the 3D view so the in-view length label updates right away.
	CWorldBuilderDoc *pDoc = CWorldBuilderDoc::GetActiveDoc();
	if (pDoc) {
		pDoc->updateAllViews();
	}
}




/** The "Show Ruler Grid" checkbox controls whether activating the ruler tool forces
 ** the grid overlay on. RulerTool applies the change live if the ruler is active. */
void RulerOptions::OnChangeShowGrid()
{
	if (m_updating) return;
	CWnd *pCheck = GetDlgItem(IDC_RULER_SHOW_GRID);
	if (pCheck) {
		RulerTool::setShowGridOnActivate(((CButton*)pCheck)->GetCheck() != 0);
	}
}




BEGIN_MESSAGE_MAP(RulerOptions, COptionsPanel)
	//{{AFX_MSG_MAP(RulerOptions)
	ON_EN_CHANGE(IDC_RULER_WIDTH, OnChangeWidthEdit)
	ON_BN_CLICKED(IDC_CHECK_RULER, OnChangeCheckRuler)
	ON_BN_CLICKED(IDC_RULER_USE_METERS, OnChangeUseMeters)
	ON_BN_CLICKED(IDC_RULER_SHOW_GRID, OnChangeShowGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

