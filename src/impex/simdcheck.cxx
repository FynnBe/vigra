/************************************************************************/
/*                                                                      */
/*          Copyright 2016 by Sven Peter, Fynn Beutten�ller             */
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

#include <vigra/simdcheck.hxx>

namespace vigra {

namespace detail {

	inline int get_cpuid(unsigned int level , cpuid_t *id)
	{
		 #if defined(HAVE_CPUID_H) || defined(HAVE_ASM_CPUID)

			 if ((unsigned int)__get_cpuid_max(0, NULL) < level)
				 return 0;

			 __cpuid_count(level, 0, id->eax, id->ebx, id->ecx, id->edx);
			 return 1;

		 #elif defined(HAVE_CPUIDEX)
			 int cpuid[4];
			 __cpuidex(cpuid, level, 0);

			 id->eax = cpuid[0];
			 id->ebx = cpuid[1];
			 id->ecx = cpuid[2];
			 id->edx = cpuid[3];

			 return 1;

		 #else
		 #error "No known way to query for CPUID."
		 #endif
	}

	#if defined(HAVE_ASM_XGETBV)

	inline xgetbv_t xgetbv()
	{
		unsigned int index = 0;
		unsigned int eax, edx;

		// CPUID.(EAX=01H, ECX=0H):ECX.OSXSAVE[bit 27]==1
		cpuid_t cpuid;
		int res = get_cpuid(1, &cpuid);

		if (!res)
			return 0;

		if ((cpuid.ecx & cpuid_bit_XSAVE) != cpuid_bit_XSAVE)
			return 0;
		if ((cpuid.ecx & cpuid_bit_OSXSAVE) != cpuid_bit_OSXSAVE)
			return 0;

		__asm__ __volatile__("xgetbv" : "=a"(eax), "=d"(edx) : "c"(index));

		return ((unsigned long long)edx << 32) | eax;
	}

	#elif defined(HAVE_INTRIN_XGETBV)

	inline xgetbv_t xgetbv()
	{
		return _xgetbv(_XCR_XFEATURE_ENABLED_MASK);
	}

	#else
	#error "no known way to use xgetbv."
	#endif

	// #if defined(HAVE_GNU_CPU_SUPPORTS_AVX2)

	// static bool _supports_avx2()
	// {
		// if (__builtin_cpu_supports("avx2"))
			// return true;
		// else
			// return false;
	// }

	// #else

	bool _supports_avx2()
	{
		cpuid_t cpuid;

		// CPUID.(EAX=07H, ECX=0H):EBX.AVX2[bit 5]==1
		int res = get_cpuid(7, &cpuid);

		if (!res)
			return false;

		if ((cpuid.ebx & cpuid7_bit_AVX2) != cpuid7_bit_AVX2)
			return false;

		xgetbv_t xcr0;
		xcr0 = xgetbv();

		// check for OS support: XCR0[2] (AVX state) and XCR0[1] (SSE state)
		if ((xcr0 & xcr0_bit_XMM) != xcr0_bit_XMM)
			return false;
		if ((xcr0 & xcr0_bit_YMM) != xcr0_bit_YMM)
			return false;

		return true;
	}

	// #endif

	// #if defined(HAVE_GNU_CPU_SUPPORTS_AVX)

	// static bool _supports_avx()
	// {
		// if (__builtin_cpu_supports("avx"))
			// return true;
		// else
			// return false;
	// }

	// #else

	bool _supports_avx()
	{
		cpuid_t cpuid;

		// CPUID.(EAX=01H, ECX=0H):ECX.AVX[bit 28]==1
		int res = get_cpuid(1, &cpuid);

		if (!res)
			return false;

		if ((cpuid.ecx & cpuid_bit_AVX) != cpuid_bit_AVX)
			return false;

		xgetbv_t xcr0;
		xcr0 = xgetbv();

		// check for OS support: XCR0[2] (AVX state) and XCR0[1] (SSE state)
		if (((xcr0 & 6) != 6))
			return false;
		return true;
	}

	// #endif

	// #if defined(HAVE_GNU_CPU_SUPPORTS_FMA)

	// static bool _supports_fma()
	// {
		// if (__builtin_cpu_supports("fma") && __builtin_cpu_supports("avx"))
			// return true;
		// else
			// return false;
	// }

	// #else

	bool _supports_fma()
	{
		cpuid_t cpuid;

		if (!_supports_avx())
			return false;

		// check for CPU FMA support: CPUID.(EAX=01H, ECX=0H):ECX.FMA[bit 12]==1
		int res = get_cpuid(1, &cpuid);

		if (!res)
			return false;

		if ((cpuid.ecx & cpuid_bit_FMA) != cpuid_bit_FMA)
			return false;

		return true;
	}

	// #endif

}} // namespace vigra::detail