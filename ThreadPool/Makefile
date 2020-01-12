executbale := ThreadPool

sources := $(wildcard *.cpp)

deps := $(sources:.cpp=.d)

C++ := g++ -std=c++11 

RM :=rm -rf

$(executbale) : $(sources)
	$(C++) -o $@ -cpp $< -lpthread

sinclude $(deps)
$(deps):%.d:%.cpp
	$(C++) -MM $< > $@

.PHONY: clean
clean:
	$(RM) $(executbale) $(wildcard *.d)


