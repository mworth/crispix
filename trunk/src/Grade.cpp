#include "global.h"
#include "Grade.h"
#include "RageUtil.h"
#include "ThemeManager.h"
#include "RageLog.h"

CString GradeToThemedString( Grade g )
{
	CString s = GradeToString(g);
	if( !THEME->HasMetric("Grade",s) )
		return "???";
	return THEME->GetMetric( "Grade",s );
}

CString GradeToOldString( Grade g )
{
	// string is meant to be human readable
	switch( g )
	{
	case GRADE_TIER_1:	return "AAAA";
	case GRADE_TIER_2:	return "AAA";
	case GRADE_TIER_3:	return "AA";
	case GRADE_TIER_4:	return "A";
	case GRADE_TIER_5:	return "B";
	case GRADE_TIER_6:	return "C";
	case GRADE_TIER_7:	return "D";
	case GRADE_FAILED:	return "E";
	case GRADE_NO_DATA:	return "N";
	default:			return "N";
	}
};

Grade StringToGrade( const CString &sGrade )
{
	CString s = sGrade;
	s.MakeUpper();

	// for backward compatibility
	if	   ( s == "AAAA" )		return GRADE_TIER_1;
	else if( s == "AAA" )		return GRADE_TIER_2;
	else if( s == "AA" )		return GRADE_TIER_3;
	else if( s == "A" )			return GRADE_TIER_4;
	else if( s == "B" )			return GRADE_TIER_5;
	else if( s == "C" )			return GRADE_TIER_6;
	else if( s == "D" )			return GRADE_TIER_7;
	else if( s == "E" )			return GRADE_FAILED;
	else if( s == "N" )			return GRADE_NO_DATA;


	// new style
	if	   ( s == "FAILED" )	return GRADE_FAILED;
	else if( s == "NODATA" )	return GRADE_NO_DATA;

	int iTier;
	if( sscanf(sGrade.c_str(),"Tier%02d",&iTier) == 1 )
		return (Grade)(iTier-1);

	LOG->Warn( "Invalid grade: %s", sGrade.c_str() );
	return GRADE_NO_DATA;
};

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
