CXX=clang++
FLAGSMSVC=-std=gnu++11 -DZELDA_GCC_11 -DBOOST_RESULT_OF_USE_TR1 -DBOOST_NO_SFINAE_EXPR -DZELDA_NO_STD_TUPLE -DZELDA_NO_VARIADIC_TEMPLATES -DZELDA_NO_RANGE_FOR
FLAGS03=-DZELDA_NO_STD_TUPLE -DZELDA_NO_EXPRESSION_SFINAE -DBOOST_NO_SFINAE_EXPR
FLAGS11=-std=gnu++11 -DZELDA_GCC_11
all:
	$(CXX) -std=c++11 -Wfatal-errors -DZELDA_NO_NOEXCEPT -DZELDA_TEST -I. test.cpp

clang:
	clang++ $(FLAGS03) -fmacro-backtrace-limit=4 -ftemplate-backtrace-limit=0 -DZELDA_TEST -I. -Wall -Wextra -Wfatal-errors test.cpp

clangmsvc:
	clang++ $(FLAGSMSVC) -Wfatal-errors -ftemplate-backtrace-limit=0 -fmacro-backtrace-limit=1 -DZELDA_TEST -I. -Wall -Wextra test.cpp

clang11:
	clang++ $(FLAGS11) -fmacro-backtrace-limit=0 -ftemplate-backtrace-limit=0 -DZELDA_TEST -I. -Wall -Wextra -Wdangling-field test.cpp

gcc:
	g++ $(FLAGS03) -Wfatal-errors -DZELDA_TEST -I. test.cpp

gcc42:
	g++-4.2 $(FLAGS03) -Wfatal-errors -DZELDA_TEST -I. test.cpp

gccmsvc:
	g++ $(FLAGSMSVC) -DZELDA_TEST -I. -Wall -Wextra test.cpp

gcc11:
	g++ $(FLAGS11) -DZELDA_TEST -I. test.cpp

gcc47:
	g++ $(FLAGS11) -DZELDA_TEST -I. test.cpp

gcc46:
	g++ -std=gnu++0x -DZELDA_GCC_11 -DZELDA_TEST -I. test.cpp

gcc45:
	g++ -std=gnu++0x -DZELDA_GCC_11 -DZELDA_TEST -I. test.cpp

pp:
	clang++ -E -C -DZELDA_NO_EXPRESSION_SFINAE -I. zelda/function/static.h

perfectpp:
	g++ -E -C -DZELDA_PARAMS_LIMIT=16 -I. zelda/function/static.h > pp.out

ppmsvc:
	clang++ -std=c++11 -E -C -fmacro-backtrace-limit=0 -Wfatal-errors -DZELDA_TEST -DZELDA_NO_STD_TUPLE -DZELDA_NO_VARIADIC_TEMPLATES -DZELDA_NO_RANGE_FOR -I. function/perfect.h
    
clean:
	rm a.out


