SRCS = main.cpp Expr.cpp
INCS = Expr.h catch.hpp

expr: $(SRCS) $(INCS)
	$(CXX) --std=c++14 -O2 -o expr $(SRCS)

.PHONY: test
test: expr
	./expr --test