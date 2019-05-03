DEVICE   := Arria10
SRC      := tutorial.cpp
TARGETS  := tutorial_original \
            tutorial_coalesce
CXX      := i++
CXXFLAGS := $(USERCXXFLAGS) -march=$(DEVICE)

RM       := rm -rf

.PHONY: test
test: $(TARGETS)
	@$(foreach t,$(TARGETS),echo ./$(t); ./$(t); echo "";)
	@echo "Compilation complete.  Open the README and the report files to examine the impact of using #pragma loop_coalesce."

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) $(foreach t,$(TARGETS),$(t).prj)

$(TARGETS) : % : $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

tutorial_original: CXXFLAGS := $(CXXFLAGS)
tutorial_coalesce: CXXFLAGS := $(CXXFLAGS) -DUSE_COALESCE
