/* ScoreDisplayNormal - Shows point score during gameplay and some menus. */

#ifndef SCORE_DISPLAY_NORMAL_H
#define SCORE_DISPLAY_NORMAL_H

#include "ScoreDisplay.h"
#include "BitmapText.h"



class ScoreDisplayNormal : public ScoreDisplay
{
public:
	ScoreDisplayNormal();

	virtual void Init( PlayerNumber pn );

	virtual void Update( float fDeltaTime );

	virtual void SetScore( int iNewScore );
	virtual void SetText( CString s );

protected:
	Sprite		m_sprFrame;
	BitmapText	m_text;

	int   m_iScore;			// the actual score
	int   m_iTrailingScore;	// what is displayed temporarily
	int   m_iScoreVelocity;	// how fast trailing approaches real score
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
