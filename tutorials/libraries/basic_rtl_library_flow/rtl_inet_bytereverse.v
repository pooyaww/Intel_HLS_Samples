`timescale 1 ps / 1 ps

// This module provides RTL byte-order reversal modules that can be used to
// convert between little- and big-endian word formats. 
// This is just a re-wiring of the input data which does not incur any 
// propagation delay; therefore the modules do not need to be clocked. 
//
// Despite this the modules provide clock, reset, and handshake signals
// in their interface so that the HLS or OpenCL compiler can integrate
// these modules into a computational pipeline. 
//
// The module names incorporate the library name to prevent clashes with
// the names of other RTL modules that may be present in the user's design.

module rtl_inet_bytereverse_uint16 (
  input   clock,
  input   resetn,
  input   ivalid, 
  input   iready,
  output  ovalid,
  output  oready,
  input   [15:0]  datain,
  output  [15:0]  dataout);

  // handshake outputs are tied high
  assign  ovalid = 1'b1;
  assign  oready = 1'b1;
  // clk, ivalid, iready, resetn are ignored
  assign dataout = {datain[7:0], datain[15:8]};
endmodule

module rtl_inet_bytereverse_uint32 (
  input   clock,
  input   resetn,
  input   ivalid, 
  input   iready,
  output  ovalid,
  output  oready,
  input   [31:0]  datain,
  output  [31:0]  dataout);

  // handshake outputs are tied high
  assign  ovalid = 1'b1;
  assign  oready = 1'b1;
  // clk, ivalid, iready, resetn are ignored
  assign dataout = {datain[7:0], datain[15:8], datain[23:16], datain[31:24]};

endmodule


