DEVICE   := Arria10
TARGETS  := tutorial
CXX      := i++
CXXFLAGS := $(USERCXXFLAGS) -march=$(DEVICE)

RM       := rm -rf

.PHONY: test
test: $(TARGETS)
	@$(foreach t,$(TARGETS),echo ./$(t); ./$(t); echo "";)
	@echo "Compilation complete.  Open the README and example the report files to examine the impact of sin/cos vs sinpi/cospi."

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) $(foreach t,$(TARGETS),$(t).prj)

$(TARGETS) : % : %.cpp
	$(CXX) $(CXXFLAGS) $@.cpp -o $@
