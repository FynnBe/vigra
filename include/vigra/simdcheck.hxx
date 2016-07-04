/************************************************************************/
/*                                                                      */
/*          Copyright 2016 by Sven Peter, Fynn Beuttenüller             */
/*                                                                      */
/*    This file is part of the VIGRA computer vision library.           */
/*    The VIGRA Website is                                              */
/*        http://hci.iwr.uni-heidelberg.de/vigra/                       */
/*    Please direct questions, bug reports, and contributions to        */
/*        ullrich.koethe@iwr.uni-heidelberg.de    or                    */
/*        vigra@informatik.uni-hamburg.de                               */
/*                                                                      */
/*    Permission is hereby granted, free of charge, to any person       */
/*    obtaining a copy of this software and associated documentation    */
/*    files (the "Software"), to deal in the Software without           */
/*    restriction, including without limitation the rights to use,      */
/*    copy, modify, merge, publish, distribute, sublicense, and/or      */
/*    sell copies of the Software, and to permit persons to whom the    */
/*    Software is furnished to do so, subject to the following          */
/*    conditions:                                                       */
/*                                                                      */
/*    The above copyright notice and this permission notice shall be    */
/*    included in all copies or substantial portions of the             */
/*    Software.                                                         */
/*                                                                      */
/*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND    */
/*    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES   */
/*    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND          */
/*    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT       */
/*    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,      */
/*    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      */
/*    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR     */
/*    OTHER DEALINGS IN THE SOFTWARE.                                   */
/*                                                                      */
/************************************************************************/

// based on fastfilters github.com/svenpeter42/fastfilters

#ifndef VIGRA_IMPEX_SIMDCHECK_HXX
#define VIGRA_IMPEX_SIMDCHECK_HXX

#include <iostream>
//#define HAVE_INTRIN_XGETBV // todo define in cmake

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef HAVE_CPUID_H
#include <cpuid.h>
#elif defined(HAVE_ASM_CPUID)
#include "clang_cpuid.h"
#endif

#ifdef HAVE_CPUIDEX
#include <intrin.h>
#endif

#include "config.hxx"

#define cpuid_bit_XSAVE 0x04000000
#define cpuid_bit_OSXSAVE 0x08000000
#define cpuid_bit_AVX 0x10000000
#define cpuid_bit_FMA 0x00001000
#define cpuid7_bit_AVX2 0x00000020

#define xcr0_bit_XMM 0x00000002
#define xcr0_bit_YMM 0x00000004

namespace vigra {

namespace detail{
	
	struct cpuid_t {
		unsigned int eax;
		unsigned int ebx;
		unsigned int ecx;
		unsigned int edx;
	};

    typedef unsigned long long xgetbv_t;

	VIGRA_EXPORT inline int get_cpuid(unsigned int, cpuid_t*);

	VIGRA_EXPORT inline xgetbv_t xgetbv();

	VIGRA_EXPORT bool _supports_avx2();

	VIGRA_EXPORT bool _supports_avx();

	VIGRA_EXPORT bool _supports_fma();
	
}} // namespace vigra::detail
#endif // VIGRA_IMPEX_SIMDCHECK_HXX
