all:
	clang++ -std=c++11 -DZELDA_TEST -I/home/paul/github/Zelda test.cpp

test:
	clang++ -fmacro-backtrace-limit=0 -DZELDA_TEST -I/home/paul/github/Zelda -Wfatal-errors test.cpp

test11:
	clang++ -std=c++11 -fmacro-backtrace-limit=0 -DZELDA_TEST -I/home/paul/github/Zelda -Wfatal-errors test.cpp

gcc11:
	g++-4.7 -std=gnu++11 -DZELDA_TEST -I/home/paul/github/Zelda -Wfatal-errors test.cpp

gcc:
	g++ -std=gnu++0x -DZELDA_TEST -I/home/paul/github/Zelda -Wfatal-errors test.cpp

pp:
	clang++ -E -C -fmacro-backtrace-limit=0 -DZELDA_TEST -I/home/paul/github/Zelda is_callable.h
    
clean:
	rm a.out


