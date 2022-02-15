SRCS = main.cpp Expr.cpp Parser.cpp
INCS = Expr.h catch.hpp Parser.h

expr: $(SRCS) $(INCS)
	$(CXX) --std=c++14 -O2 -o expr $(SRCS)

.PHONY: test
test: expr
	./expr --test