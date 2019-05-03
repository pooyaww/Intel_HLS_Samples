DEVICE   := Arria10
SRC      := rng.cpp
TARGETS  := tutorial-rng-x86 \
            tutorial-rng-fpga
CXX      := i++
CXXFLAGS := $(USERCXXFLAGS) -march=$(DEVICE)
RM       := rm -rf

.PHONY: test
test: $(TARGETS)
	@$(foreach t,$(TARGETS),echo ./$(t); ./$(t); echo "";)
	

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) $(foreach t,$(TARGETS),$(t).prj)

$(TARGETS) : % : $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

tutorial-rng-x86:            CXX = i++
tutorial-rng-x86:            CXXFLAGS := $(CXXFLAGS) -march=x86-64

tutorial-rng-fpga:           CXX = i++
tutorial-rng-fpga:           CXXFLAGS := $(CXXFLAGS) -march=$(DEVICE)

