DEVICE  := Stratix10
TARGETS := hyper-optimized-handshaking-default \
           hyper-optimized-handshaking-off
SRC     := tutorial.cpp
CXX     := i++
RM      := rm -rf

.PHONY: test
test: $(TARGETS)
	@$(foreach t,$(TARGETS),echo ./$(t); ./$(t); echo "";)

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) $(foreach t,$(TARGETS),$(t).prj)

$(TARGETS) :
	$(CXX) $(CXXFLAGS) $(SRC) -o $@

hyper-optimized-handshaking-default: CXXFLAGS := $(USERCXXFLAGS) -march=$(DEVICE)
hyper-optimized-handshaking-off:     CXXFLAGS := $(USERCXXFLAGS) -march=$(DEVICE) --hyper-optimized-handshaking=off
