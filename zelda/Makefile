all:
	clang++ -std=c++11 -Wfatal-errors -DZELDA_TEST -I/home/paul/github/Zelda test.cpp

test:
	clang++ -fmacro-backtrace-limit=1 -ftemplate-backtrace-limit=0 -DZELDA_TEST -DZELDA_NO_STD_TUPLE -DZELDA_NO_EXPRESSION_SFINAE -I/home/paul/github/Zelda -Wall -Wextra -Wfatal-errors test.cpp

testmsvc:
	clang++ -std=c++11 -ftemplate-backtrace-limit=0 -fmacro-backtrace-limit=1 -DZELDA_TEST -DZELDA_NO_STD_TUPLE -DZELDA_NO_VARIADIC_TEMPLATES -DZELDA_NO_RANGE_FOR -I/home/paul/github/Zelda -Wall -Wextra test.cpp

test11:
	clang++ -std=c++11 -fmacro-backtrace-limit=0 -ftemplate-backtrace-limit=0 -DZELDA_TEST -I/home/paul/github/Zelda -Wall -Wextra -Wdangling-field test.cpp

gcc:
	g++ -DZELDA_TEST -I/home/paul/github/Zelda test.cpp

gcc47:
	g++-4.7 -DZELDA_TEST -I/home/paul/github/Zelda -Wfatal-errors test.cpp

gcc11:
	g++ -std=gnu++11 -DZELDA_TEST -DZELDA_GCC_11 -DZELDA_NO_EXPRESSION_SFINAE -I/home/paul/github/Zelda test.cpp

pp:
	clang++ -E -C -fmacro-backtrace-limit=0 -Wfatal-errors -DZELDA_TEST -I/home/paul/github/Zelda function/variadic.h

ppmsvc:
	clang++ -std=c++11 -E -C -fmacro-backtrace-limit=0 -Wfatal-errors -DZELDA_TEST -DZELDA_NO_STD_TUPLE -DZELDA_NO_VARIADIC_TEMPLATES -DZELDA_NO_RANGE_FOR -I/home/paul/github/Zelda function/perfect.h

tuple:
	clang++ -fmacro-backtrace-limit=0 -Wfatal-errors -DZELDA_NO_STD_TUPLE -DZELDA_TEST -I/home/paul/github/Zelda test_tuple.cpp

tuple11:
	clang++ -fmacro-backtrace-limit=0 -std=c++11 -DZELDA_TEST -I/home/paul/github/Zelda test_tuple.cpp

tuplemsvc:
	clang++ -fmacro-backtrace-limit=0 -std=c++11 -DZELDA_TEST -DZELDA_NO_STD_TUPLE -DZELDA_NO_VARIADIC_TEMPLATES -DZELDA_NO_RANGE_FOR -I/home/paul/github/Zelda test_tuple.cpp
    
clean:
	rm a.out


