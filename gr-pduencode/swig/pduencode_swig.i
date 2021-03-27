/* -*- c++ -*- */

#define PDUENCODE_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "pduencode_swig_doc.i"

%{
#include "pduencode/crc16.h"
#include "pduencode/usrp_pad.h"
#include "pduencode/selector_test.h"
#include "pduencode/preamble.h"
#include "pduencode/autoswitch.h"
#include "pduencode/add_length.h"
#include "pduencode/custom_hex.h"
#include "pduencode/syncword.h"
#include "pduencode/hwid_mover.h"
#include "pduencode/byte_flip.h"
%}

%include "pduencode/crc16.h"
GR_SWIG_BLOCK_MAGIC2(pduencode, crc16);
%include "pduencode/usrp_pad.h"
GR_SWIG_BLOCK_MAGIC2(pduencode, usrp_pad);


%include "pduencode/selector_test.h"
GR_SWIG_BLOCK_MAGIC2(pduencode, selector_test);

%include "pduencode/preamble.h"
GR_SWIG_BLOCK_MAGIC2(pduencode, preamble);


%include "pduencode/autoswitch.h"
GR_SWIG_BLOCK_MAGIC2(pduencode, autoswitch);
%include "pduencode/add_length.h"
GR_SWIG_BLOCK_MAGIC2(pduencode, add_length);

%include "pduencode/custom_hex.h"
GR_SWIG_BLOCK_MAGIC2(pduencode, custom_hex);


%include "pduencode/syncword.h"
GR_SWIG_BLOCK_MAGIC2(pduencode, syncword);


%include "pduencode/hwid_mover.h"
GR_SWIG_BLOCK_MAGIC2(pduencode, hwid_mover);
%include "pduencode/byte_flip.h"
GR_SWIG_BLOCK_MAGIC2(pduencode, byte_flip);
