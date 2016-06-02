VIGRA_ADD_TEST(test_multiconvolution_avx test.cxx LIBRARIES vigraimpex)

VIGRA_ADD_TEST(test_multiconvolution_avx_speed speedtest.cxx)

VIGRA_COPY_TEST_DATA(oi_single.gif)
