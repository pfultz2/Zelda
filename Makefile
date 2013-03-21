CXX=clang++

all:
	$(CXX) -std=c++11 -Wfatal-errors -DZELDA_NO_NOEXCEPT -DZELDA_TEST -I. test.cpp

clang:
	clang++ -fmacro-backtrace-limit=1 -ftemplate-backtrace-limit=0 -DZELDA_TEST -DZELDA_NO_STD_TUPLE -DZELDA_NO_EXPRESSION_SFINAE -I. -Wall -Wextra -Wfatal-errors test.cpp

clangmsvc:
	clang++ -std=c++11 -ftemplate-backtrace-limit=0 -fmacro-backtrace-limit=1 -DZELDA_TEST -DZELDA_NO_STD_TUPLE -DZELDA_NO_VARIADIC_TEMPLATES -DZELDA_NO_RANGE_FOR -I. -Wall -Wextra test.cpp

clang11:
	clang++ -std=c++11 -fmacro-backtrace-limit=0 -ftemplate-backtrace-limit=0 -DZELDA_TEST -I. -Wall -Wextra -Wdangling-field test.cpp

gcc:
	g++ -DZELDA_TEST -I. test.cpp

gccmsvc:
	g++ -std=c++11 -DZELDA_GCC_11 -DZELDA_TEST -DZELDA_NO_STD_TUPLE -DZELDA_NO_VARIADIC_TEMPLATES -DZELDA_NO_RANGE_FOR -I. -Wall -Wextra test.cpp

gcc11:
	g++ -std=gnu++11 -DZELDA_NO_NOEXCEPT -DZELDA_TEST -DZELDA_GCC_11 -DZELDA_NO_EXPRESSION_SFINAE -I. test.cpp

pp:
	g++ -E -C -DZELDA_PARAMS_LIMIT=16 -I. zelda/function/static.h > pp.out

ppmsvc:
	clang++ -std=c++11 -E -C -fmacro-backtrace-limit=0 -Wfatal-errors -DZELDA_TEST -DZELDA_NO_STD_TUPLE -DZELDA_NO_VARIADIC_TEMPLATES -DZELDA_NO_RANGE_FOR -I. function/perfect.h
    
clean:
	rm a.out


