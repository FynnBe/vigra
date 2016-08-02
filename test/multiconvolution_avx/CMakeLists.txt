VIGRA_ADD_TEST(test_multiconvolution_avx test.cxx fastfilters.cxx LIBRARIES vigraimpex)

VIGRA_ADD_TEST(test_multiconvolution_avx_speed speedtest.cxx)

VIGRA_COPY_TEST_DATA(oi_single.gif lenna128.xv lenna_simple_sharpening_orig.xv lenna_gaussian_sharpening_orig.xv lenna128sepgrad.xv lennahessxx.xv lennastxx.xv lenna128recgrad.xv lenna128nonlinear.xv resampling.xv lennahessyy.xv lennastyy.xv lennahessxy.xv lennastxy.xv lenna128rgb.xv lenna128rgbsepgrad.xv lenna_level-2.xv lenna_level-1.xv lenna_level1.xv lenna_level2.xv lenna_levellap0.xv lenna_levellap1.xv lenna_levellap2.xv lennargbst.xv)
