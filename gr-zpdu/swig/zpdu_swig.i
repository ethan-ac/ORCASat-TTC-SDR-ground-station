/* -*- c++ -*- */

#define ZPDU_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "zpdu_swig_doc.i"

%{
#include "zpdu/sub_pull_socket.h"
#include "zpdu/pub_push_socket.h"
%}

%include "zpdu/sub_pull_socket.h"
GR_SWIG_BLOCK_MAGIC2(zpdu, sub_pull_socket);
%include "zpdu/pub_push_socket.h"
GR_SWIG_BLOCK_MAGIC2(zpdu, pub_push_socket);
