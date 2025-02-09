/* FadingBanner Fades between two banners. */

#ifndef FADING_BANNER_H
#define FADING_BANNER_H

#include "Banner.h"
#include "ActorFrame.h"
#include "RageTimer.h"

class FadingBanner : public ActorFrame
{
public:
	FadingBanner();

	void ScaleToClipped( float fWidth, float fHeight );

	bool Load( RageTextureID ID );
	void LoadFromSong( const Song* pSong );		// NULL means no song
	void LoadAllMusic();
	void LoadSort();
	void LoadMode();
	void LoadFromGroup( CString sGroupName );
	void LoadFromCourse( const Course* pCourse );
	void LoadRoulette();
	void LoadRandom();
	void LoadFallback();

	bool LoadFromCachedBanner( const CString &path );

	void SetMovingFast( bool fast ) { m_bMovingFast=fast; }
	virtual void Update( float fDeltaTime );
	virtual void DrawPrimitives();

protected:
	void BeforeChange();


	Banner		m_Banner[2];
	int			m_iIndexFront;
	int			GetBackIndex() { return m_iIndexFront==0 ? 1 : 0; }

	bool		m_bMovingFast;
	bool		m_bSkipNextBannerUpdate;
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
