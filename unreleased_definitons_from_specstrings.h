/* version specific fixes to bring sal.h upto date */
#if __SAL_H_FULL_VER <= 140050728 // [

#if !defined(__midl) && defined(_PREFAST_) && _MSC_VER >= 1000 // [

/* Missing from RTM sal.h */
#define __inner_bound                     _SAL_L_Source_(__inner_bound, (), _SA_annotes0(SAL_bound))
#define __inner_range(lb,ub)              _SAL_L_Source_(__inner_range, (lb,ub), _SA_annotes2(SAL_range,lb,ub))
#define __inner_assume_bound_dec          __inline __nothrow void __AssumeBoundInt(_Post_ __inner_bound int i) {i;}
#define __inner_assume_bound(i)           __AssumeBoundInt(i);
#define __inner_allocator                 _SAL_L_Source_(__inner_allocater, (), _SA_annotes0(SAL_allocator))

#define __static_context(ctx, annotes) \
    _SAL_L_Source_(__static_context, (ctx, annotes), _SA_annotes1(SAL_context,ctx) _Group_(__nop_impl(annotes)))

#define __failure(x) __static_context(SAL_return_convention, \
    _SAL_L_Source_(__failure, (x), _SA_annotes1(SAL_failure,x)))

__ANNOTATION(SAL_valueUndefined());
#define __valueUndefined _SAL_L_Source_(__valueUndefined, (), _SA_annotes0(SAL_valueUndefined))

enum __SAL_failureKind{__failureUnspecified = 0, __failureUndefined = 1};

__ANNOTATION(SAL_failureDefault(enum __SAL_failureKind));
#define __failureDefault(kind) _SAL_L_Source_(__failureDefault, (kind), __static_context(SAL_return_convention,  \
        _SA_annotes1(SAL_failureDefault,kind)))

#else // ][

#define __inner_bound
#define __inner_range(lb,ub)
#define __inner_assume_bound_dec
#define __inner_assume_bound(i)
#define __inner_allocator

#define __static_context(ctx, annotes)
#define __failure(x)
#define __valueUndefined
#define __failureDefault(x)

#endif // ]
