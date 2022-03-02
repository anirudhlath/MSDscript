SRCS = main.cpp Expr.cpp Parser.cpp Val.cpp
tSRCS = testMain.cpp exec.cpp

INCS = Expr.h catch.hpp Parser.h Val.h
tINCS = exec.h


msdscript: $(SRCS) $(INCS)
	$(CXX) --std=c++14 -O2 -o msdscript $(SRCS)

.PHONY: test
test: msdscript
	./msdscript --test

.PHONY: default
default: msdscript;

test_msdscript: $(tSRCS) $(tINCS)
	$(CXX) --std=c++14 -O2 -o test_msdscript $(tSRCS)