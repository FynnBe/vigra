
#ifndef FASTFILTERS_HXX
#define FASTFILTERS_HXX

class dummy
{	
	public:
		dummy();
};


template <class SrcIterator, class SrcShape, class SrcAccessor,
class DestIterator, class DestAccessor, class KernelIterator>
void
separableConvolveMultiArrayFastFilters( SrcIterator s, SrcShape const & shape, SrcAccessor src,
                                        DestIterator d, DestAccessor dest,
                                        KernelIterator kernels,
                                        SrcShape start = SrcShape(),
                                        SrcShape stop = SrcShape())
{


    //typedef struct _fastfilters_array2d_t {
    //	float *ptr;
    //	size_t n_x;
    //	size_t n_y;
    //	size_t stride_x;
    //	size_t stride_y;
    //	size_t n_channels;
    //} fastfilters_array2d_t;


    internalSeparableConvolveMultiArrayFastFilters();





    //typedef typename NumericTraits<typename DestAccessor::value_type>::RealPromote TmpType;


    //if (stop != SrcShape())
    //{

    //    enum { N = 1 + SrcIterator::level };
    //    detail::RelativeToAbsoluteCoordinate<N - 1>::exec(shape, start);
    //    detail::RelativeToAbsoluteCoordinate<N - 1>::exec(shape, stop);

    //    for (int k = 0; k < N; ++k)
    //        vigra_precondition(0 <= start[k] && start[k] < stop[k] && stop[k] <= shape[k],
    //            "separableConvolveMultiArray(): invalid subarray shape.");

    //    detail::internalSeparableConvolveSubarray(s, shape, src, d, dest, kernels, start, stop);
    //}
    //else if (!IsSameType<TmpType, typename DestAccessor::value_type>::boolResult)
    //{
    //    // need a temporary array to avoid rounding errors
    //    MultiArray<SrcShape::static_size, TmpType> tmpArray(shape);
    //    internalSeparableConvolveMultiArrayFastFilters(s, shape, src,
    //        tmpArray.traverser_begin(), typename AccessorTraits<TmpType>::default_accessor(), kernels);
    //    copyMultiArray(srcMultiArrayRange(tmpArray), destIter(d, dest));
    //}
    //else
    //{
    //    // work directly on the destination array
    //    internalSeparableConvolveMultiArrayFastFilters(s, shape, src, d, dest, kernels);
    //}


}


void internalSeparableConvolveMultiArrayFastFilters();

#endif // FASTFILTERS_HXX