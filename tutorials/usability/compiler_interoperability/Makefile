DEVICE   := Arria10
TARGETS  := x86-64_testbench \
            fpga_testbench

CXX      := i++
CXXFLAGS := $(USERCXXFLAGS)
RM       := rm -rf

#Get Path to our include files. Our compiler knows where they are but gcc/g++ doesn't
HLS_INCLUDE := $(shell dirname `which i++`)/../include

.PHONY: test
test: $(TARGETS)
	@$(foreach t,$(TARGETS),echo ./$(t); ./$(t); echo "";)


.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) *.o fpga_accelerate.prj

testbench.o : testbench.cpp accelerate.h
	g++ -std=c++14 $(EXTRACXXFLAGS) -c -I$(HLS_INCLUDE) $< -o $@

x86-64_accelerate.o: accelerate.cpp accelerate.h
	$(CXX) $(CXXFLAGS) $(EXTRACXXFLAGS) -c -march=x86-64 $< -o $@

x86-64_testbench : testbench.o x86-64_accelerate.o
	$(CXX) $(CXXFLAGS) $(EXTRACXXFLAGS) $^ -o $@

fpga_accelerate.o: accelerate.cpp accelerate.h
	$(CXX) $(CXXFLAGS) $(EXTRACXXFLAGS) -c -march=$(DEVICE) $< -o $@

fpga_testbench : testbench.o fpga_accelerate.o
	$(CXX) $(CXXFLAGS) $(EXTRACXXFLAGS) -march=$(DEVICE) $^ -o $@

