DEVICE      := Arria10
TARGETS     := part_1_unbalanced \
               part_2_balanced
RUN_TARGETS := part_2_balanced

CXX      := i++
CXXFLAGS := $(USERCXXFLAGS) -march=$(DEVICE) -ghdl
RM       := rm -rf

.PHONY: test
test: $(TARGETS)
	@$(foreach t,$(RUN_TARGETS),echo ./$(t); ./$(t); echo "";)
	@echo "Compilation complete.  Open the README and the report files to learn how to properly balance loop delay when using system of tasks."

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) $(foreach t,$(TARGETS),$(t).prj)

$(TARGETS) :
	$(CXX) $(CXXFLAGS) $@.cpp -o $@

