DEVICE      := Arria10
TARGETS     := part_1_unbalanced \
               part_2_balanced

CXX      := i++
CXXFLAGS := $(USERCXXFLAGS) -march=$(DEVICE) --fp-relaxed -ghdl
RM       := rm -rf

.PHONY: test
test: $(TARGETS)
	@$(foreach t,$(TARGETS),echo ./$(t); ./$(t); echo "";)
	@echo "Compilation complete.  Open the README and the report files to learn how to properly balance pipeline latency when using system of tasks."

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) $(foreach t,$(TARGETS),$(t).prj)

$(TARGETS) :
	$(CXX) $(CXXFLAGS) $@.cpp -o $@

