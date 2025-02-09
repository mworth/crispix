#include "global.h"

/*
 * Styles define a set of columns for each player, and information about those
 * columns, like what Instruments are used play those columns and what track 
 * to use to populate the column's notes.
 * A "track" is the term used to descibe a particular vertical sting of note 
 * in NoteData.
 * A "column" is the term used to describe the vertical string of notes that 
 * a player sees on the screen while they're playing.  Column notes are 
 * picked from a track, but columns and tracks don't have a 1-to-1 
 * correspondance.  For example, dance-versus has 8 columns but only 4 tracks
 * because two players place from the same set of 4 tracks.
 */

#include "Style.h"
#include "RageLog.h"
#include "RageUtil.h"
#include "Game.h"
#include "IniFile.h"
#include "GameState.h"
#include "NoteData.h"


void Style::GetTransformedNoteDataForStyle( PlayerNumber pn, const NoteData* pOriginal, NoteData* pNoteDataOut ) const
{
	int iNewToOriginalTrack[MAX_COLS_PER_PLAYER];
	for( int col=0; col<m_iColsPerPlayer; col++ )
	{
		ColumnInfo colInfo = m_ColumnInfo[pn][col];
		int originalTrack = colInfo.track;
		
		iNewToOriginalTrack[col] = originalTrack;
	}
	
	pNoteDataOut->LoadTransformed( pOriginal, m_iColsPerPlayer, iNewToOriginalTrack );
}


GameInput Style::StyleInputToGameInput( const StyleInput& StyleI ) const
{
	ASSERT_M( StyleI.player < NUM_PLAYERS, ssprintf("P%i", StyleI.player) );
	ASSERT_M( StyleI.col < MAX_COLS_PER_PLAYER, ssprintf("C%i", StyleI.col) );
	GameController c = m_ColumnInfo[StyleI.player][StyleI.col].controller;
	GameButton b = m_ColumnInfo[StyleI.player][StyleI.col].button;
	return GameInput( c, b );
};

StyleInput Style::GameInputToStyleInput( const GameInput &GameI ) const
{
	StyleInput SI;

	FOREACH_PlayerNumber( p )
	{
		for( int t=0; t<m_iColsPerPlayer; t++ )
		{
			if( m_ColumnInfo[p][t].controller == GameI.controller  &&
				m_ColumnInfo[p][t].button == GameI.button )
			{
				SI = StyleInput( (PlayerNumber)p, t );

				// HACK:  Looking up the player number using m_ColumnInfo 
				// returns the wrong answer for ONE_PLAYER_TWO_CREDITS styles
				if( m_StyleType == ONE_PLAYER_TWO_CREDITS )
					SI.player = GAMESTATE->m_MasterPlayerNumber;
				
				return SI;
			}
		}
	}
	return SI;	// Didn't find a match.  Return invalid.
}


PlayerNumber Style::ControllerToPlayerNumber( GameController controller ) const
{
	switch( m_StyleType )
	{
	case ONE_PLAYER_ONE_CREDIT:
	case TWO_PLAYERS_TWO_CREDITS:
		return (PlayerNumber)controller;
	case ONE_PLAYER_TWO_CREDITS:
		return GAMESTATE->m_MasterPlayerNumber;
	default:
		ASSERT(0);
		return PLAYER_INVALID;
	}
}

bool Style::MatchesStepsType( StepsType type ) const
{
	if(type == m_StepsType) return true;

	return false;
}

void Style::GetMinAndMaxColX( PlayerNumber pn, float& fMixXOut, float& fMaxXOut ) const
{
	fMixXOut = +100000;
	fMaxXOut = -100000;
	for( int i=0; i<m_iColsPerPlayer; i++ )
	{
		fMixXOut = min( fMixXOut, m_ColumnInfo[pn][i].fXOffset );
		fMaxXOut = max( fMaxXOut, m_ColumnInfo[pn][i].fXOffset );
	}
}

/*
 * (c) 2001-2002 Chris Danford
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
