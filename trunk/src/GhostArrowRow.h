/* GhostArrowRow - Row of GhostArrow actors. */

#ifndef GHOSTARROWROW_H
#define GHOSTARROWROW_H

#include "ActorFrame.h"
#include "GhostArrow.h"
#include "HoldGhostArrow.h"
#include "GameConstantsAndTypes.h"
#include "Style.h"



class GhostArrowRow : public ActorFrame
{
public:
	GhostArrowRow();
	virtual ~GhostArrowRow() { Unload(); }
	virtual void Update( float fDeltaTime );
	virtual void DrawPrimitives();
	virtual void CopyTweening( const GhostArrowRow &from );

	void Load( PlayerNumber pn, CString NoteSkin, float fYReverseOffset );
	void Unload();
	
	void DidTapNote( int iCol, TapNoteScore score, bool bBright );
	void SetHoldIsActive( int iCol );
	
protected:
	int m_iNumCols;
	float m_fYReverseOffsetPixels;
	PlayerNumber m_PlayerNumber;

	vector<GhostArrow *> 	m_GhostDim;
	vector<GhostArrow *>	m_GhostBright;
	vector<HoldGhostArrow *> m_HoldGhost;
};


#endif

/*
 * (c) 2001-2004 Chris Danford
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
