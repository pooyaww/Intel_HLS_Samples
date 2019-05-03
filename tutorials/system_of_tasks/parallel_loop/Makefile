DEVICE   := Arria10
TARGETS  := part_1_sequential_loop \
            part_2_merged_loop \
            part_3_parallel_loop            
CXX      := i++
CXXFLAGS := $(USERCXXFLAGS) -march=$(DEVICE)
RM       := rm -rf

.PHONY: test
test: $(TARGETS) 
	@$(foreach t,$(TARGETS),echo ./$(t); ./$(t); echo "";)
	@echo "Compilation complete. Open the README and the report files to learn how to seperate a loop with high ii into an asynchronous task."

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) $(foreach t,$(TARGETS),$(t).prj)

$(TARGETS) :
	$(CXX) $(CXXFLAGS) $@.cpp -o $@
