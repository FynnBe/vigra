##################################################
#
#     check for SIMD instructions
#
##################################################

macro(CHECK_FOR_AVX AVX_FLAGS_NAME HAVE_FMA HAVE_AVX_NAME HAVE_AVX2_NAME)
    include(CheckCXXSourceRuns)
    set(CMAKE_REQUIRED_FLAGS)

    # Check FMA
    check_cxx_source_runs("
        #include <immintrin.h>
        #include <stdlib.h>
        #include <stdio.h>
        int main()
        {
            __m256 a = _mm256_set1_ps(rand());
            __m256 b = _mm256_set1_ps(rand());
            b = _mm256_fmadd_ps(a, a, b);
            float result = _mm_cvtss_f32(_mm256_extractf128_ps(b, 0));
            printf(\"%f\", result);
            return 0;
        }"
       HAVE_FMA)

    message("FMA in marco = ${HAVE_FMA}")
    message("FMA in macro = ${${HAVE_FMA}}")

    # Check AVX
    if(MSVC AND NOT MSVC_VERSION LESS 1600)
        set(CMAKE_REQUIRED_FLAGS "/arch:AVX")
    endif()

    check_cxx_source_runs("
        #include <immintrin.h>
        int main()
        {
          __m256 a, b, c;
          const float src[8] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };
          float dst[8];
          a = _mm256_loadu_ps( src );
          b = _mm256_loadu_ps( src );
          c = _mm256_add_ps( a, b );
          _mm256_storeu_ps( dst, c );
          for( int i = 0; i < 8; i++ ){
            if( ( src[i] + src[i] ) != dst[i] ){
              return -1;
            }
          }
          return 0;
        }"
        ${HAVE_AVX_NAME})

    # Check AVX2
    if(MSVC AND NOT MSVC_VERSION LESS 1800)
        set(CMAKE_REQUIRED_FLAGS "/arch:AVX2")
    endif()

    check_cxx_source_runs("
        #include <immintrin.h>
        int main()
        {
          __m256i a, b, c;
          const int src[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
          int dst[8];
          a =  _mm256_loadu_si256( (__m256i*)src );
          b =  _mm256_loadu_si256( (__m256i*)src );
          c = _mm256_add_epi32( a, b );
          _mm256_storeu_si256( (__m256i*)dst, c );
          for( int i = 0; i < 8; i++ ){
            if( ( src[i] + src[i] ) != dst[i] ){
              return -1;
            }
          }
          return 0;
        }"
        ${HAVE_AVX2_NAME})

    # Set Flags
    message( STATUS "HEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEERE: " ${HAVE_AVX_NAME})
    message( STATUS "HEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEERE TOO: " ${HAVE_AVX2_NAME})
    if(MSVC)
        if(${HAVE_AVX2_NAME} AND NOT MSVC_VERSION LESS 1800)
            set(${AVX_FLAGS_NAME} "/arch:AVX2")
        elseif(${HAVE_AVX_NAME}  AND NOT MSVC_VERSION LESS 1600)
            set(${AVX_FLAGS_NAME} "/arch:AVX")
        endif()
    endif()
endmacro()


macro(CHECK_CPUID HAVE_CPUID_H_NAME HAVE_CPUIDEX_NAME HAVE_ASM_CPUID_NAME)
    include(CheckCXXSourceRuns)
    set(CMAKE_REQUIRED_FLAGS)

    # message(STATUS "Checking if the compiler supports the \"__get_cpuid\" intrinsic")
    check_cxx_source_runs("
        #include <cpuid.h>
        int main(int argc, char *argv[]) {
            (void)argc; (void)argv;
            unsigned int tmp;
            __get_cpuid(0, &tmp, &tmp, &tmp, &tmp);
            return 0;
        }"
        ${HAVE_CPUID_H_NAME})

    # message(STATUS "Checking if the compiler supports the \"__cpuidex\" intrinsic")
    check_cxx_source_runs("
        #include <stdio.h>
        #include <intrin.h>
        int main()
        {
            int cpuid[4];
            __cpuidex(cpuid, 7, 0);
            return 0; //# todo: talk about the behavior of CheckCXXSourceRuns/CheckCXXSourceCompiles
            return cpuid[0];
        }"
        ${HAVE_CPUIDEX_NAME})

    # message(STATUS "Checking if the compiler supports the 'cpuid' instruction")
    check_cxx_source_runs("
        #include <stdio.h>
        int main()
        {
            unsigned int a, b, c, d;

            a = 1;
            c = 0;
            __asm__ __volatile__ (\"cpuid\" : \"+a\"(a), \"+b\"(b), \"+c\"(c), \"=d\"(d));
            return 0; //# todo: talk about the behavior of CheckCXXSourceRuns/CheckCXXSourceCompiles
            return a;
        }"
        ${HAVE_ASM_CPUID_NAME})

    if(NOT ${HAVE_CPUID_H_NAME} AND
       NOT ${HAVE_CPUIDEX_NAME} AND
       NOT ${HAVE_ASM_CPUID_NAME})
        message(FATAL_ERROR "No known compiler intrinsic to read cpuid.")
    endif()
endmacro()

macro(CHECK_XGETBV HAVE_ASM_XGETBV HAVE_INTRIN_XGETBV)
    include(CheckCXXSourceRuns)
    #include(CheckCXXSourceCompiles)
    set(CMAKE_REQUIRED_FLAGS)

    # message(STATUS "Checking if the compiler supports the \"xgetbv\" instruction")
    check_cxx_source_runs("
        #include <stdio.h>
        int main()
        {
            unsigned int a, b, c;

            c = 0;
            __asm__ __volatile__(\"xgetbv\" : \"=a\"(a), \"=d\"(b) : \"c\"(c));
            return 0; //# todo: talk about the behavior of CheckCXXSourceRuns/CheckCXXSourceCompiles
            return a;
        }"
        ${HAVE_ASM_XGETBV})

    # if(${HAVE_ASM_XGETBV})
        # message(STATUS "- Compiler supports the 'xgetbv' instruction")
    # endif()

    # message(STATUS "Checking if the compiler supports the '_xgetbv' intrinsic")
    check_cxx_source_runs("
        #include <stdio.h>
        #include <intrin.h>
        int main()
        {
            unsigned int xcr0 = _xgetbv(_XCR_XFEATURE_ENABLED_MASK);
            return 0; //# todo: talk about the behavior of CheckCXXSourceRuns/CheckCXXSourceCompiles
            return xcr0;
        }"
        ${HAVE_INTRIN_XGETBV})
        # todo: ask Sven why "return xcr0" was in this code

    # message(${HAVE_INTRIN_XGETBV}: ${${HAVE_INTRIN_XGETBV}})
    # if(${HAVE_INTRIN_XGETBV})
        # message(STATUS "- Compiler supports the '_xgetbv' intrinsic")
    # endif()

    if(NOT ${HAVE_ASM_XGETBV} AND NOT ${HAVE_INTRIN_XGETBV})
        message(FATAL_ERROR "No known compiler intrinsic to read xcr0.")
    endif()
endmacro()