#if defined _VCRT_BUILD && !defined _ASSERT_OK
#error assert.h not for CRT internal use
#endif

#include <corecrt.h>

_CRT_BEGIN_C_HEADER

#undef ASSERT

#ifdef NDEBUG

#define ASSERT(expression) ((void)0)

#else

/*_ACRTIMP void __cdecl _wassert(
	_In_z_ wchar_t const* _Message,
	_In_z_ wchar_t const* _File,
	_In_   unsigned       _Line
);

#define ASSERT(expression) (void)(                                                  \
            (!!(expression)) ||                                                              \
            (_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) \
        )
*/
#define ASSERT(expression) ((void)0)

#endif

_CRT_END_C_HEADER