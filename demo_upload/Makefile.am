bin_PROGRAMS=curl_demo_upload
CPP=g++
AM_CXXFLAGS=-Wall -fPIC -fexceptions -std=gnu++11 -I$(top_srcdir)/src/lib -I$(top_srcdir)/src
curl_demo_upload_SOURCES = main.cpp \
  $(srcdir)/../src/lib/CurlEasyWrapper.cpp \
  $(srcdir)/../src/lib/CurlException.cpp
