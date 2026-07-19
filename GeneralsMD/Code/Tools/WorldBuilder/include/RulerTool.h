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

// RulerTool.h
// Author: Mike Lytle, January 2003

#pragma once

#ifndef RULER_TOOL_H
#define RULER_TOOL_H

#include "Tool.h"

class RulerTool : public Tool
{
protected:
	Coord3D		m_downPt3d;
	int				m_rulerType; 
	WbView*		m_View;
	Real			m_savedLength;

	static RulerTool*	m_staticThis;
	static Bool				m_useMeters;	///< If true, length readouts are shown in meters instead of feet.
	static Bool				m_showGridOnActivate;	///< If true, activating the ruler forces the Show Ruler Grid overlay on.

public:
	RulerTool(void);
	~RulerTool(void);

public:
	/// Clear the selection on activate or deactivate.
	virtual void activate();
	virtual void deactivate();

	virtual void setCursor(void);
	virtual void mouseDown(TTrackingMode m, CPoint viewPt, WbView* pView, CWorldBuilderDoc *pDoc);
	virtual void mouseMoved(TTrackingMode m, CPoint viewPt, WbView* pView, CWorldBuilderDoc *pDoc);
	virtual Bool followsTerrain(void) {return false;};	
	
	static void setLength(Real length);
	static Bool switchType();
	static int	getType();
	static Real getLength(void);

	/// Feet<->meters readout toggle. Conversion is applied at display time only;
	/// the stored length (m_savedLength) is always in feet (= world units).
	static void setUseMeters(Bool useMeters) { m_useMeters = useMeters; }
	static Bool getUseMeters(void) { return m_useMeters; }
	/// Convert a stored (feet) length to the currently-selected display unit.
	static Real toDisplayUnits(Real feet) { return m_useMeters ? (feet * 0.3048f) : feet; }

	/// Whether activating the ruler tool forces the Show Ruler Grid overlay on.
	static void setShowGridOnActivate(Bool val);
	static Bool getShowGridOnActivate(void) { return m_showGridOnActivate; }

};

#endif //RULER_TOOL_H

