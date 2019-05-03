DEVICE   := Arria10
TARGETS  := part_1_default_memory \
            part_2_optimized_memory
CXX      := i++
CXXFLAGS := $(USERCXXFLAGS) -v -march=$(DEVICE)
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
