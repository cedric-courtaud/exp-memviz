include $(top_srcdir)/Makefile.tool.am

#----------------------------------------------------------------------------
# Headers, etc
#----------------------------------------------------------------------------

noinst_HEADERS = \
	mv_arch.h \
	mv_profiler.h \
	bitfield.h
	mv_sim.h

#----------------------------------------------------------------------------
# cachegrind-<platform>
#----------------------------------------------------------------------------

noinst_PROGRAMS  = exp-memviz-@VGCONF_ARCH_PRI@-@VGCONF_OS@
if VGCONF_HAVE_PLATFORM_SEC
noinst_PROGRAMS += exp-memviz-@VGCONF_ARCH_SEC@-@VGCONF_OS@
endif

CACHEGRIND_SOURCES_COMMON = \
	mv_main.c \
	mv_arch.c \
	bitfield.c \
	mv_profiler.c

exp_memviz_@VGCONF_ARCH_PRI@_@VGCONF_OS@_SOURCES      = \
	$(CACHEGRIND_SOURCES_COMMON)
exp_memviz_@VGCONF_ARCH_PRI@_@VGCONF_OS@_CPPFLAGS     = \
	$(AM_CPPFLAGS_@VGCONF_PLATFORM_PRI_CAPS@)
exp_memviz_@VGCONF_ARCH_PRI@_@VGCONF_OS@_CFLAGS       = $(LTO_CFLAGS) \
	$(AM_CFLAGS_@VGCONF_PLATFORM_PRI_CAPS@)
exp_memviz_@VGCONF_ARCH_PRI@_@VGCONF_OS@_DEPENDENCIES = \
	$(TOOL_DEPENDENCIES_@VGCONF_PLATFORM_PRI_CAPS@)
exp_memviz_@VGCONF_ARCH_PRI@_@VGCONF_OS@_LDADD        = \
	$(TOOL_LDADD_@VGCONF_PLATFORM_PRI_CAPS@)
exp_memviz_@VGCONF_ARCH_PRI@_@VGCONF_OS@_LDFLAGS      = \
	$(TOOL_LDFLAGS_@VGCONF_PLATFORM_PRI_CAPS@)
exp_memviz_@VGCONF_ARCH_PRI@_@VGCONF_OS@_LINK = \
	$(top_builddir)/coregrind/link_tool_exe_@VGCONF_OS@ \
	@VALT_LOAD_ADDRESS_PRI@ \
	$(LINK) \
	$(exp_memviz_@VGCONF_ARCH_PRI@_@VGCONF_OS@_CFLAGS) \
	$(exp_memviz_@VGCONF_ARCH_PRI@_@VGCONF_OS@_LDFLAGS)

if VGCONF_HAVE_PLATFORM_SEC
exp_memviz_@VGCONF_ARCH_SEC@_@VGCONF_OS@_SOURCES      = \
	$(CACHEGRIND_SOURCES_COMMON)
exp_memviz_@VGCONF_ARCH_SEC@_@VGCONF_OS@_CPPFLAGS     = \
	$(AM_CPPFLAGS_@VGCONF_PLATFORM_SEC_CAPS@)
exp_memviz_@VGCONF_ARCH_SEC@_@VGCONF_OS@_CFLAGS       = $(LTO_CFLAGS)\
	$(AM_CFLAGS_@VGCONF_PLATFORM_SEC_CAPS@)
exp_memviz_@VGCONF_ARCH_SEC@_@VGCONF_OS@_DEPENDENCIES = \
	$(TOOL_DEPENDENCIES_@VGCONF_PLATFORM_SEC_CAPS@)
exp_memviz_@VGCONF_ARCH_SEC@_@VGCONF_OS@_LDADD        = \
	$(TOOL_LDADD_@VGCONF_PLATFORM_SEC_CAPS@)
exp_memviz_@VGCONF_ARCH_SEC@_@VGCONF_OS@_LDFLAGS      = \
	$(TOOL_LDFLAGS_@VGCONF_PLATFORM_SEC_CAPS@)
exp_memviz_@VGCONF_ARCH_SEC@_@VGCONF_OS@_LINK = \
	$(top_builddir)/coregrind/link_tool_exe_@VGCONF_OS@ \
	@VALT_LOAD_ADDRESS_SEC@ \
	$(LINK) \
	$(exp_memviz_@VGCONF_ARCH_SEC@_@VGCONF_OS@_CFLAGS) \
	$(exp_memviz_@VGCONF_ARCH_SEC@_@VGCONF_OS@_LDFLAGS)
endif
