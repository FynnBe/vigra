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


    #if defined(HAVE_ASM_XGETBV)

	VIGRA_EXPORT inline xgetbv_t xgetbv();

    #elif defined(HAVE_INTRIN_XGETBV)

	VIGRA_EXPORT inline xgetbv_t xgetbv();

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

    // static bool _supports_avx2()
    // {
        // cpuid_t cpuid;

        // // CPUID.(EAX=07H, ECX=0H):EBX.AVX2[bit 5]==1
        // int res = get_cpuid(7, &cpuid);

        // if (!res)
            // return false;

        // if ((cpuid.ebx & cpuid7_bit_AVX2) != cpuid7_bit_AVX2)
            // return false;

        // xgetbv_t xcr0;
        // xcr0 = xgetbv();

        // // check for OS support: XCR0[2] (AVX state) and XCR0[1] (SSE state)
        // if ((xcr0 & xcr0_bit_XMM) != xcr0_bit_XMM)
            // return false;
        // if ((xcr0 & xcr0_bit_YMM) != xcr0_bit_YMM)
            // return false;

        // return true;
    // }

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

    // static bool _supports_avx()
    // {
        // cpuid_t cpuid;

        // // CPUID.(EAX=01H, ECX=0H):ECX.AVX[bit 28]==1
        // int res = get_cpuid(1, &cpuid);

        // if (!res)
            // return false;

        // if ((cpuid.ecx & cpuid_bit_AVX) != cpuid_bit_AVX)
            // return false;

        // xgetbv_t xcr0;
        // xcr0 = xgetbv();

        // // check for OS support: XCR0[2] (AVX state) and XCR0[1] (SSE state)
        // if (((xcr0 & 6) != 6))
            // return false;
        // return true;
    // }

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

    // static bool _supports_fma()
    // {
        // cpuid_t cpuid;

        // if (!_supports_avx())
            // return false;

        // // check for CPU FMA support: CPUID.(EAX=01H, ECX=0H):ECX.FMA[bit 12]==1
        // int res = get_cpuid(1, &cpuid);

        // if (!res)
            // return false;

        // if ((cpuid.ecx & cpuid_bit_FMA) != cpuid_bit_FMA)
            // return false;

        // return true;
    // }

    // #endif

    // int main()
    // {
        // cpuid_t test_cpuid;
        // get_cpuid(0, &test_cpuid);

        // std::cout << "xgetbv(): " << xgetbv() << std::endl;
        // std::cout << "get_cpuid.eax, .ebx, ecx, edx: " << test_cpuid.eax << ", " << test_cpuid.ebx << ", " << test_cpuid.ecx << ", " << test_cpuid.edx << std::endl;
        // std::cout << "_supports_avx2(): " << _supports_avx2() << std::endl;
        // std::cout << "_supports_avx(): " << _supports_avx() << std::endl;
        // std::cout << "_supports_fma(): " << _supports_fma() << std::endl;

        // return 0;
    // }

    //static bool g_supports_avx = false;
    //static bool g_supports_fma = false;
    //static bool g_supports_avx2 = false;
    //
    //void fastfilters_cpu_init(void)
    //{
    //    g_supports_avx = _supports_avx();
    //    g_supports_fma = _supports_fma();
    //    g_supports_avx2 = _supports_avx2();
    //}
    //
    ////bool DLL_PUBLIC fastfilters_cpu_enable(fastfilters_cpu_feature_t feature, bool enable)
    //bool fastfilters_cpu_enable(fastfilters_cpu_feature_t feature, bool enable)
    //{
    //    switch (feature) {
    //    case FASTFILTERS_CPU_AVX:
    //        if (enable)
    //            g_supports_avx = _supports_avx();
    //        else
    //            g_supports_avx = false;
    //        break;
    //    case FASTFILTERS_CPU_FMA:
    //        if (enable)
    //            g_supports_fma = _supports_fma();
    //        else
    //            g_supports_fma = false;
    //        break;
    //    case FASTFILTERS_CPU_AVX2:
    //        if (enable)
    //            g_supports_avx2 = _supports_avx2();
    //        else
    //            g_supports_avx2 = false;
    //        break;
    //    default:
    //        return false;
    //    }
    //
    //    return fastfilters_cpu_check(feature);
    //}
    //
    //bool DLL_PUBLIC fastfilters_cpu_check(fastfilters_cpu_feature_t feature)
    ////bool fastfilters_cpu_check(fastfilters_cpu_feature_t feature)
    //{
    //    switch (feature) {
    //    case FASTFILTERS_CPU_AVX:
    //        return g_supports_avx;
    //    case FASTFILTERS_CPU_FMA:
    //        return g_supports_fma;
    //    case FASTFILTERS_CPU_AVX2:
    //        return g_supports_avx2;
    //    default:
    //        return false;
    //    }
    //}
}
}

#endif // VIGRA_IMPEX_SIMDCHECK_HXX
