// В Catch2 v3 функция main поставляется библиотекой, а в v2 её создаёт заголовок.
#if !__has_include(<catch2/catch_test_macros.hpp>)
#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_NO_POSIX_SIGNALS
#include <catch2/catch.hpp>
#endif
