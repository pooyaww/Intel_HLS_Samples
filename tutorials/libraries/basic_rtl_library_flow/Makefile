DEVICE   := Arria10
TARGETS  := basic_rtl_library_tutorial
HLS_LIBS := librtl.a
RTL_OBJS := rtl_inet.o
LIB_SRCS := rtl_inet.h rtl_inet.xml rtl_inet_emu_models.cpp
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
	-$(RM)  $(TARGETS) $(HLS_LIBS) $(RTL_OBJS) \
		$(foreach t,$(TARGETS),$(t).prj)

$(TARGETS) : $(HLS_LIBS)
	$(CXX) $(CXXFLAGS) $(HLS_LIBS) $@.cpp -o $@

librtl.a: $(LIB_SRCS)
	@$(foreach rtl_obj,$(basename $(RTL_OBJS)), \
	   fpga_crossgen --target hls $(rtl_obj).xml --emulation_model $(rtl_obj)_emu_models.cpp;)
	@fpga_libtool --target hls --create $@ $(RTL_OBJS)


