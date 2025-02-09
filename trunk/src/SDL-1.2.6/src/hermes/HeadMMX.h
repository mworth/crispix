/*
   Header definitions for the MMX routines for the HERMES library
   Copyright (c) 1998 Christian Nentwich (c.nentwich@cs.ucl.ac.uk)
   This source code is licensed under the GNU LGPL
  
   Please refer to the file COPYING.LIB contained in the distribution for
   licensing conditions
*/

#ifndef __HERMES_HEAD_MMX__
#define __HERMES_HEAD_MMX__


/* If you cannot stand ifdefs, then please do not look into this file, it's
   going to end your life :) */

#ifdef X86_ASSEMBLER


#ifdef __cplusplus
extern "C" {
#endif

void STACKCALL ConvertMMX(HermesConverterInterface *);

void STACKCALL ClearMMX_32(HermesClearInterface *);
void STACKCALL ClearMMX_24(HermesClearInterface *);
void STACKCALL ClearMMX_16(HermesClearInterface *);
void STACKCALL ClearMMX_8(HermesClearInterface *);

void ConvertMMXpII32_24RGB888();
void ConvertMMXpII32_16RGB565();
void ConvertMMXpII32_16BGR565();
void ConvertMMXpII32_16RGB555();
void ConvertMMXpII32_16BGR565();
void ConvertMMXpII32_16BGR555();

void ConvertMMXp32_16RGB555();

#ifdef __cplusplus
}
#endif



/* Fix the underscore business with ELF compilers */

#if defined(__ELF__) && defined(__GNUC__)
  #ifdef __cplusplus 
  extern "C" {   
  #endif

  void ConvertMMX(HermesConverterInterface *) __attribute__ ((alias ("_ConvertMMX")));
#if 0
  void ClearMMX_32(HermesClearInterface *) __attribute__ ((alias ("_ClearMMX_32")));
  void ClearMMX_24(HermesClearInterface *)  __attribute__ ((alias ("_ClearMMX_24")));
  void ClearMMX_16(HermesClearInterface *)  __attribute__ ((alias ("_ClearMMX_16")));
  void ClearMMX_8(HermesClearInterface *)  __attribute__ ((alias ("_ClearMMX_8")));

  void ConvertMMXp32_16RGB555() __attribute__ ((alias ("_ConvertMMXp32_16RGB555")));
#endif

  void ConvertMMXpII32_24RGB888() __attribute__ ((alias ("_ConvertMMXpII32_24RGB888")));
  void ConvertMMXpII32_16RGB565() __attribute__ ((alias ("_ConvertMMXpII32_16RGB565")));
  void ConvertMMXpII32_16BGR565() __attribute__ ((alias ("_ConvertMMXpII32_16BGR565")));
  void ConvertMMXpII32_16RGB555() __attribute__ ((alias ("_ConvertMMXpII32_16RGB555")));
  void ConvertMMXpII32_16BGR555() __attribute__ ((alias ("_ConvertMMXpII32_16BGR555")));

  #ifdef __cplusplus
  }
  #endif

#endif /* ELF and GNUC */




/* Make it work with Watcom */
#ifdef __WATCOMC__
#pragma warning 601 9

#pragma aux ConvertMMX "_*" modify [EAX EBX ECX EDX ESI EDI]

#pragma aux ClearMMX_32 "_*" modify [EAX EBX ECX EDX ESI EDI]
#pragma aux ClearMMX_24 "_*" modify [EAX EBX ECX EDX ESI EDI]
#pragma aux ClearMMX_16 "_*" modify [EAX EBX ECX EDX ESI EDI]
#pragma aux ClearMMX_8 "_*" modify [EAX EBX ECX EDX ESI EDI]

#pragma aux ConvertMMXpII32_24RGB888 "_*"
#pragma aux ConvertMMXpII32_16RGB565 "_*"
#pragma aux ConvertMMXpII32_16BGR565 "_*"
#pragma aux ConvertMMXpII32_16RGB555 "_*"
#pragma aux ConvertMMXpII32_16BGR555 "_*"
#pragma aux ConvertMMXp32_16RGB555 "_*"

#endif /* WATCOM */

#endif /* X86_ASSEMBLER */


#endif
