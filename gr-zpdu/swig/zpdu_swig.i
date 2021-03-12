/* -*- c++ -*- */

#define ZPDU_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "zpdu_swig_doc.i"

%{
#include "zpdu/newmq.h"
#include "zpdu/newzmqtoo.h"
#include "zpdu/sub_pull_test.h"
#include "zpdu/pub_push_test.h"
%}

%include "zpdu/newmq.h"
GR_SWIG_BLOCK_MAGIC2(zpdu, newmq);
%include "zpdu/newzmqtoo.h"
GR_SWIG_BLOCK_MAGIC2(zpdu, newzmqtoo);
%include "zpdu/sub_pull_test.h"
GR_SWIG_BLOCK_MAGIC2(zpdu, sub_pull_test);
%include "zpdu/pub_push_test.h"
GR_SWIG_BLOCK_MAGIC2(zpdu, pub_push_test);
