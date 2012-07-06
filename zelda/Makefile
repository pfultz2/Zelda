all:
	clang++ -std=c++11 -DZELDA_TEST -I/home/paul/github/Zelda test.cpp

test:
	clang++ -fmacro-backtrace-limit=2 -DZELDA_TEST -I/home/paul/github/Zelda -Wfatal-errors test.cpp

pp:
	clang++ -E -C -fmacro-backtrace-limit=0 -DZELDA_TEST -I/home/paul/github/Zelda is_callable.h
    
clean:
	rm a.out


