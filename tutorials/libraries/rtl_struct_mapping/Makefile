DEVICE      := Arria10
TARGETS     := rtl_struct_packer_test
TARGET_SRCS := rtl_struct_packer_test.cpp
HLS_LIBS    := librtl.a
RTL_OBJS    := rtl_struct_packer.o
LIB_SRCS    := rtl_struct_packer.h rtl_struct_packer.xml rtl_struct_packer_emu_models.cpp rtl_struct_packer.sv
CXX         := i++
CXXFLAGS    := $(USERCXXFLAGS) -march=$(DEVICE) -ghdl
RM          := rm -rf

.PHONY: test
test: $(TARGETS)
	@$(foreach t,$(TARGETS),echo ./$(t); ./$(t); echo "";)

.PHONY: all
all: interface_structs.v librtl.a $(TARGETS)

.PHONY: clean
clean:
	-$(RM)  $(TARGETS) $(HLS_LIBS) $(RTL_OBJS) \
		$(foreach t,$(TARGETS),$(t).prj)

$(TARGETS) : $(TARGET_SRCS) $(HLS_LIBS) librtl.a
	$(CXX) $(CXXFLAGS) $(HLS_LIBS) $@.cpp -o $@

interface_structs.v: 
	$(CXX) $(CXX_FLAGS) -march="Arria10" --simulator none --component rtl_struct_a_packer rtl_struct_packer_emu_models.cpp
	mv a.prj/components/rtl_struct_a_packer/interface_structs.v interface_structs.sv
	$(RM) a.prj

librtl.a: $(LIB_SRCS)
	$(foreach rtl_obj,$(basename $(RTL_OBJS)), \
	   @fpga_crossgen --target hls $(rtl_obj).xml --emulation_model $(rtl_obj)_emu_models.cpp $(USERCXXFLAGS);)
	@fpga_libtool --target hls --create $@ $(RTL_OBJS)


