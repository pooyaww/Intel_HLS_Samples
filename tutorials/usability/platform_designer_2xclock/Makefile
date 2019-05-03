DEVICE   := Arria10
SRC      := simple-component.cpp
TARGETS  := tutorial-fpga
RM       := rm -rf

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) $(foreach t,$(TARGETS),$(t).prj) top.qsys top.qsf top.qpf db ip qdb top *.ipx

$(TARGETS): $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@
	ip-make-ipx --thorough-descent > ip-make-ipx.log 2>&1
	qsys-script --script=top.tcl --quartus-project=none --cmd="set device_family {$(DEVICE)}" > qsys-script.log 2>&1

tutorial-fpga:           CXX = i++
tutorial-fpga:           CXXFLAGS := $(USERCXXFLAGS) -march=$(DEVICE) --simulator none
