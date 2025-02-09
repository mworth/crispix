// MMX assembler version of SDL_MixAudio for signed little endian 16 bit samples and signed 8 bit samples
// Copyright 2002 Stephane Marchesin (stephane.marchesin@wanadoo.fr)
// Converted to Intel ASM notation by Cth
// This code is licensed under the LGPL (see COPYING for details)
// 
// Assumes buffer size in bytes is a multiple of 16
// Assumes SDL_MIX_MAXVOLUME = 128


////////////////////////////////////////////////
// Mixing for 16 bit signed buffers
////////////////////////////////////////////////

#if defined(USE_ASM_MIXER_VC)
#include <windows.h>
#include <stdio.h>

void SDL_MixAudio_MMX_S16_VC(char* dst,char* src,unsigned int nSize,int volume)
{
	__asm
	{
		align	16

		push	edi
		push	esi
		push	ebx
		
		mov		edi, dst		// edi = dst
		mov		esi, src		// esi = src
		mov		eax, volume		// eax = volume
		mov		ebx, nSize		// ebx = size
		shr		ebx, 4			// process 16 bytes per iteration = 8 samples
		jz		endS16
		
		pxor	mm0, mm0
		movd	mm0, eax		//%%eax,%%mm0
		movq	mm1, mm0		//%%mm0,%%mm1
		psllq	mm0, 16			//$16,%%mm0
		por		mm0, mm1		//%%mm1,%%mm0
		psllq	mm0, 16			//$16,%%mm0
		por		mm0, mm1		//%%mm1,%%mm0
		psllq	mm0, 16			//$16,%%mm0
		por		mm0, mm1		//%%mm1,%%mm0			// mm0 = vol|vol|vol|vol

mixloopS16:
		movq	mm1, [esi]		//(%%esi),%%mm1\n" // mm1 = a|b|c|d
		movq	mm2, mm1		//%%mm1,%%mm2\n" // mm2 = a|b|c|d
		movq	mm4, [esi + 8]	//8(%%esi),%%mm4\n" // mm4 = e|f|g|h
		// pre charger le buffer dst dans mm7
		movq	mm7, [edi]		//(%%edi),%%mm7\n" // mm7 = dst[0]"
		// multiplier par le volume
		pmullw	mm1, mm0		//%%mm0,%%mm1\n" // mm1 = l(a*v)|l(b*v)|l(c*v)|l(d*v)
		pmulhw	mm2, mm0		//%%mm0,%%mm2\n" // mm2 = h(a*v)|h(b*v)|h(c*v)|h(d*v)
		movq	mm5, mm4		//%%mm4,%%mm5\n" // mm5 = e|f|g|h
		pmullw	mm4, mm0		//%%mm0,%%mm4\n" // mm4 = l(e*v)|l(f*v)|l(g*v)|l(h*v)
		pmulhw	mm5, mm0		//%%mm0,%%mm5\n" // mm5 = h(e*v)|h(f*v)|h(g*v)|h(h*v)
		movq	mm3, mm1		//%%mm1,%%mm3\n" // mm3 = l(a*v)|l(b*v)|l(c*v)|l(d*v)
		punpckhwd	mm1, mm2	//%%mm2,%%mm1\n" // mm1 = a*v|b*v
		movq		mm6, mm4	//%%mm4,%%mm6\n" // mm6 = l(e*v)|l(f*v)|l(g*v)|l(h*v)
		punpcklwd	mm3, mm2	//%%mm2,%%mm3\n" // mm3 = c*v|d*v
		punpckhwd	mm4, mm5	//%%mm5,%%mm4\n" // mm4 = e*f|f*v
		punpcklwd	mm6, mm5	//%%mm5,%%mm6\n" // mm6 = g*v|h*v
		// pre charger le buffer dst dans mm5
		movq	mm5, [edi + 8]	//8(%%edi),%%mm5\n" // mm5 = dst[1]
		// diviser par 128
		psrad	mm1, 7			//$7,%%mm1\n" // mm1 = a*v/128|b*v/128 , 128 = SDL_MIX_MAXVOLUME
		add		esi, 16			//$16,%%esi\n"
		psrad	mm3, 7			//$7,%%mm3\n" // mm3 = c*v/128|d*v/128
		psrad	mm4, 7			//$7,%%mm4\n" // mm4 = e*v/128|f*v/128
		// mm1 = le sample avec le volume modifie
		packssdw	mm3, mm1	//%%mm1,%%mm3\n" // mm3 = s(a*v|b*v|c*v|d*v)
		psrad	mm6, 7			//$7,%%mm6\n" // mm6= g*v/128|h*v/128
		paddsw	mm3, mm7		//%%mm7,%%mm3\n" // mm3 = adjust_volume(src)+dst
		// mm4 = le sample avec le volume modifie
		packssdw	mm6, mm4	//%%mm4,%%mm6\n" // mm6 = s(e*v|f*v|g*v|h*v)
		movq	[edi], mm3		//%%mm3,(%%edi)\n"
		paddsw	mm6, mm5		//%%mm5,%%mm6\n" // mm6 = adjust_volume(src)+dst
		movq	[edi + 8], mm6	//%%mm6,8(%%edi)\n"
		add		edi, 16			//$16,%%edi\n"
		dec		ebx				//%%ebx\n"
		jnz mixloopS16

ends16:
		emms
		
		pop		ebx
		pop		esi
		pop		edi
	}

}

////////////////////////////////////////////////
// Mixing for 8 bit signed buffers
////////////////////////////////////////////////

void SDL_MixAudio_MMX_S8_VC(char* dst,char* src,unsigned int nSize,int volume)
{
	_asm
	{
		align 16

		push	edi
		push	esi
		push	ebx
		
		mov		edi, dst	//movl	%0,%%edi	// edi = dst
		mov		esi, src	//%1,%%esi	// esi = src
		mov		eax, volume	//%3,%%eax	// eax = volume

		movd	mm0, ebx	//%%ebx,%%mm0
		movq	mm1, mm0	//%%mm0,%%mm1
		psllq	mm0, 16		//$16,%%mm0
		por		mm0, mm1	//%%mm1,%%mm0
		psllq	mm0, 16		//$16,%%mm0
		por		mm0, mm1	//%%mm1,%%mm0
		psllq	mm0, 16		//$16,%%mm0
		por		mm0, mm1	//%%mm1,%%mm0
		
		mov		ebx, nSize	//%2,%%ebx	// ebx = size
		shr		ebx, 3		//$3,%%ebx	// process 8 bytes per iteration = 8 samples
		cmp		ebx, 0		//$0,%%ebx
		je		endS8

mixloopS8:
		pxor	mm2, mm2	//%%mm2,%%mm2		// mm2 = 0
		movq	mm1, [esi]	//(%%esi),%%mm1	// mm1 = a|b|c|d|e|f|g|h
		movq	mm3, mm1	//%%mm1,%%mm3 	// mm3 = a|b|c|d|e|f|g|h
		// on va faire le "sign extension" en faisant un cmp avec 0 qui retourne 1 si <0, 0 si >0
		pcmpgtb		mm2, mm1	//%%mm1,%%mm2	// mm2 = 11111111|00000000|00000000....
		punpckhbw	mm1, mm2	//%%mm2,%%mm1	// mm1 = 0|a|0|b|0|c|0|d
		punpcklbw	mm3, mm2	//%%mm2,%%mm3	// mm3 = 0|e|0|f|0|g|0|h
		movq	mm2, [edi]	//(%%edi),%%mm2	// mm2 = destination
		pmullw	mm1, mm0	//%%mm0,%%mm1	// mm1 = v*a|v*b|v*c|v*d
		add		esi, 8		//$8,%%esi
		pmullw	mm3, mm0	//%%mm0,%%mm3	// mm3 = v*e|v*f|v*g|v*h
		psraw	mm1, 7		//$7,%%mm1		// mm1 = v*a/128|v*b/128|v*c/128|v*d/128 
		psraw	mm3, 7		//$7,%%mm3		// mm3 = v*e/128|v*f/128|v*g/128|v*h/128
		packsswb mm3, mm1	//%%mm1,%%mm3	// mm1 = v*a/128|v*b/128|v*c/128|v*d/128|v*e/128|v*f/128|v*g/128|v*h/128
		paddsb	mm3, mm2	//%%mm2,%%mm3	// add to destination buffer
		movq	[edi], mm3	//%%mm3,(%%edi)	// store back to ram
		add		edi, 8		//$8,%%edi
		dec		ebx			//%%ebx
		jnz		mixloopS8
		
endS8:
		emms
		
		pop		ebx
		pop		esi
		pop		edi
	}
}

int _SDL_IsMMX_VC()
{
	// This	bit	flag can get set on	calling	cpuid
	// with	register eax set to	1
	const int _MMX_FEATURE_BIT = 0x00800000;
	DWORD dwFeature	= 0;
	__try {
			_asm {
				mov	eax,1
				cpuid
				mov	dwFeature,edx
			}
	} __except ( EXCEPTION_EXECUTE_HANDLER)	{
			return 0;
	}
	if (dwFeature &	_MMX_FEATURE_BIT) {
		__try {
			__asm {
				pxor mm0, mm0
				emms
			}
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			return(0);
		}
		return 1;
	}

	return 0;
}

static int _notTwice = 2;

int SDL_IsMMX_VC()
{
	if (_notTwice > 1)
	{
		_notTwice = _SDL_IsMMX_VC();
/*
#ifdef _DEBUG
		if (_notTwice)
			MessageBox( NULL, "Using MMX!!!", "Error", MB_OK | MB_ICONINFORMATION );
		else
			MessageBox( NULL, "Not sing MMX!!!", "Error", MB_OK | MB_ICONINFORMATION );
#endif
*/
	}
	return _notTwice;
}

#endif

