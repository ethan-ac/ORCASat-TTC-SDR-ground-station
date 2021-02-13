/* -*- c++ -*- */

#define PDUENCODE_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "pduencode_swig_doc.i"

%{
#include "pduencode/length.h"
#include "pduencode/crc16.h"
#include "pduencode/usrp_pad.h"
#include "pduencode/bytes.h"
%}

%include "pduencode/length.h"
GR_SWIG_BLOCK_MAGIC2(pduencode, length);
%include "pduencode/crc16.h"
GR_SWIG_BLOCK_MAGIC2(pduencode, crc16);
%include "pduencode/usrp_pad.h"
GR_SWIG_BLOCK_MAGIC2(pduencode, usrp_pad);
%include "pduencode/bytes.h"
GR_SWIG_BLOCK_MAGIC2(pduencode, bytes);
