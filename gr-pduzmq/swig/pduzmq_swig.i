/* -*- c++ -*- */

#define PDUZMQ_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "pduzmq_swig_doc.i"

%{
#include "pduzmq/newzmq.h"
%}

%include "pduzmq/newzmq.h"
GR_SWIG_BLOCK_MAGIC2(pduzmq, newzmq);
