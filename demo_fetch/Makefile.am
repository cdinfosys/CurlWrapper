bin_PROGRAMS=curl_demo_fetch
CPP=g++
VPATH=$(srcdir) $(srcdir)/src/lib
AM_CXXFLAGS=-Wall -g -O0 -fPIC -fexceptions -std=gnu++11 -I$(top_srcdir)/src/lib -I$(top_srcdir)/src
curl_demo_fetch_SOURCES = main.cpp \
  $(srcdir)/../src/lib/CurlEasyWrapper.cpp \
  $(srcdir)/../src/lib/CurlException.cpp
