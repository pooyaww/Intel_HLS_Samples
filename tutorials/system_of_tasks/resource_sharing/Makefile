DEVICE   := Arria10
TARGETS  := part_1_direct_computing \
            part_2_resource_sharing
CXX      := i++
CXXFLAGS := $(USERCXXFLAGS) -march=$(DEVICE)
RM       := rm -rf

.PHONY: test
test: $(TARGETS) 
	@$(foreach t,$(TARGETS),echo ./$(t); ./$(t); echo "";)
	@echo "Compilation complete. Open the README and the report files to learn how to use system of task for resource sharing."

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) $(foreach t,$(TARGETS),$(t).prj)


$(TARGETS) :
	$(CXX) $(CXXFLAGS) $@.cpp -o $@
