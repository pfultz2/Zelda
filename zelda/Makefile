all:
	clang++ -std=c++11 -DZELDA_TEST -I/home/paul/github/Zelda test.cpp

test:
	clang++ -fmacro-backtrace-limit=0 -DZELDA_TEST -DZELDA_NO_EXPRESSION_SFINAE -I/home/paul/github/Zelda -Wfatal-errors test.cpp

test11:
	clang++ -std=c++11 -fmacro-backtrace-limit=0 -DZELDA_TEST -I/home/paul/github/Zelda -Wfatal-errors test.cpp

gcc:
	g++ -DZELDA_TEST -I/home/paul/github/Zelda -Wfatal-errors test.cpp

gcc11:
	g++-4.7 -std=gnu++11 -DZELDA_TEST -I/home/paul/github/Zelda -Wfatal-errors test.cpp

pp:
	clang++ -E -C -fmacro-backtrace-limit=0 -Wfatal-errors -DZELDA_TEST -I/home/paul/github/Zelda tuple.h

tuple:
	clang++ -fmacro-backtrace-limit=0 -Wfatal-errors -DZELDA_NO_STD_TUPLE -DZELDA_TEST -I/home/paul/github/Zelda test_tuple.cpp

tuple11:
	clang++ -fmacro-backtrace-limit=0 -std=c++11 -DZELDA_TEST -I/home/paul/github/Zelda test_tuple.cpp
    
clean:
	rm a.out


