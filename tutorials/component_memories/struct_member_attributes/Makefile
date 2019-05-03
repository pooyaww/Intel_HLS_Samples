DEVICE   := Arria10
TARGETS  := part_1_attr_ok \
	    part_2_array_prevails \
	    part_3_outer_prevails

CXX      := i++
CXXFLAGS := $(USERCXXFLAGS) -march=$(DEVICE) -ghdl
RM       := rm -rf
.PHONY: test
test: $(TARGETS)
	@$(foreach t,$(TARGETS),echo ./$(t); ./$(t); echo "";)
.PHONY: all
all: $(TARGETS)
.PHONY: clean
clean:
	-$(RM) $(TARGETS) $(foreach t,$(TARGETS),$(t).prj)
$(TARGETS) :
	$(CXX) $(CXXFLAGS) $@.cpp -o $@
