bin_PROGRAMS = lc_web
lib_LTLIBRARIES = liblcweb.la
AM_CXXFLAGS = -Wall -I$(top_srcdir)/include -lpthread -lmagic

lc_web_SOURCES = main.cpp
lc_web_LDADD = liblcweb.la

liblcweb_la_SOURCES = launch_code_webserver.cpp

