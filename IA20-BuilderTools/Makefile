##
## File: Makefile
##
## Copyright (C) 2021, Albert Krzymowski
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
##
## http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.
##
#########################################################################

notarget:
	@echo Specify one of the targets: all, clean.

#########################################################################

DEP_PROJECTS_DIRS=$(addprefix ../, $(DEP_PROJECTS))
DEP_PROJECTS_DIRS_INCLUDE=$(addsuffix /src, $(DEP_PROJECTS_DIRS))
DEP_PROJECTS_INC=$(addprefix -I, $(DEP_PROJECTS_DIRS_INCLUDE))

DEP_PROJECTS_DIRS_LIB=$(addsuffix /out/lib, $(DEP_PROJECTS_DIRS))
DEP_PROJECTS_LINK=$(addprefix -L, $(DEP_PROJECTS_DIRS_LIB))

#########################################################################
#
# Generate files.
#

ifdef IA20_DEST_DIR
DEST_DIR=$(IA20_DEST_DIR)
else
DEST_DIR=/usr/IA20
endif

CPPFLAGS+=-D__IA20_DEST_DIR=\"$(DEST_DIR)\"

CPPSRCS__=$(addprefix src/, $(FILES) )
CPPSRCS=$(addsuffix .cpp, $(CPPSRCS__))

CPPOBJS__=$(addprefix out/obj/, $(FILES) $(MAINFILES))
CPPOBJS=$(addsuffix .o, $(CPPOBJS__))

CPPOBJSNONEXE__=$(addprefix out/obj/, $(FILES))
CPPOBJSNONEXE=$(addsuffix .o, $(CPPOBJSNONEXE__))

CPPEXES=$(addprefix out/bin/, $(MAINFILES))

CPPDEPS__=$(addprefix out/obj/, $(FILES))
CPPDEPS=$(addsuffix .depend, $(CPPDEPS__))

CPPDIRS__=$(dir $(FILES) $(MAINFILES))
# sort will remove duplicates !! :-)
CPPDIRS=$(sort $(CPPDIRS__))

#########################################################################

OUTDIRS+=$(addprefix out/obj/, $(CPPDIRS))
OUTDIRS+=out/bin
OUTDIRS+=out/lib
#########################################################################
CPPFLAGS+=-Isrc $(DEP_PROJECTS_INC)
LNFLAGS+=$(DEP_PROJECTS_LINK)

DEP_PROJECT_TMP=$(subst IA20-,IA20, $(DEP_PROJECTS))
LNLIBS+=$(addprefix -l,$(DEP_PROJECT_TMP))

#########################################################################
include ../IA20-BuilderTools/Makefile.$(IA20_PLATFORM)
#########################################################################
LIB_LINKS=$(addprefix out/lib/, $(ARTIFACT_ALT_NAMES))

$(LIB_LINKS): out/lib/% : $(TARGET)
	@(cd out/lib; ln -fs $(TARGETFILE) $(@F))
	@echo Created link: $(@F) '->' $(TARGETFILE)
links: $(LIB_LINKS)
#########################################################################
ifndef INSTALL
INSTALL=install -Dm755
endif

INSTALL_EXES=$(addprefix $(DEST_DIR)/bin/, $(MAINFILES))
INSTALL_LIBS=$(addprefix $(DEST_DIR)/lib/, $(TARGETFILE))
INSTALL_LIBLNS=$(addprefix $(DEST_DIR)/lib/, $(ARTIFACT_ALT_NAMES))
INSTALL_LANGS=$(addprefix $(DEST_DIR)/lang/std/, $(LANG_FILES))


$(INSTALL_EXES) : $(DEST_DIR)/bin/% :  out/bin/%
	@mkdir -p $(DEST_DIR)/bin
	@$(INSTALL) $< $(DEST_DIR)/bin
	@echo Installing $(@F)

$(INSTALL_LIBS) : $(DEST_DIR)/lib/% :  out/lib/%
	@mkdir -p $(DEST_DIR)/lib
	@$(INSTALL)  $< $(DEST_DIR)/lib
	@echo Installing $(@F)

$(INSTALL_LIBLNS) : $(DEST_DIR)/lib/% : $(INSTALL_LIBS)
	@(cd $(DEST_DIR)/lib; ln -fs $(TARGETFILE) $(@F))
	@echo Installing link: $(@F) '->' $(TARGETFILE)

$(INSTALL_LANGS) : $(DEST_DIR)/lang/std/% :  lang/std/%
	@mkdir -p $(@D)
	@$(INSTALL)  $< $(@D)
	@echo Installing script $(@F) $<


install: $(INSTALL_EXES) $(INSTALL_LIBS) $(INSTALL_LIBLNS) $(INSTALL_LANGS)
	@tree  $(DEST_DIR)

uninstall:
	@echo Removing from: $(DEST_DIR) following files:
	@echo $(notdir $(INSTALL_EXES) $(INSTALL_LIBS) $(INSTALL_LIBLNS)) | tr ' ' \\n
	@rm -f $(INSTALL_EXES) $(INSTALL_LIBS) $(INSTALL_LIBLNS)
	@echo Done !

#########################################################################
all :
	@echo All done !!!
#########################################################################
dm :
#########################################################################
makedirs:
	@echo Ensuring output directories.
	@mkdir -p $(OUTDIRS)

clean:
	@echo Cleaning ...
	@rm -rf out

#########################################################################

basenames:
	@rm -f Makefile.FILES
	@echo
	@echo Creating source files list.
	@echo -n '#' >Makefile.FILES
	@date >>Makefile.FILES
	@bash -c 'cd src; (find -name \*.yy; find -name \*.cpp)|cut -c3-|grep / |cut -f1 -d\. | sort -u| while read a ; do echo "FILES+="$$a; done >> ../Makefile.FILES'
	@bash -c 'cd src; (find -name \*.cpp)|cut -c3-|grep -v / |cut -f1 -d\. | sort -u| while read a ; do echo "MAINFILES+="$$a; done >> ../Makefile.FILES'
	@echo `cat Makefile.FILES |grep FILES|wc -l` 'file(s) found.'
	@echo `cat Makefile.FILES |grep MAINFILES|wc -l` ' executable(s).'
	@echo

#########################################################################
# Testing only:
showfiles:
	@echo src:  $(CPPSRCS)
	@echo obj:  $(CPPOBJS)
	@echo deps: $(CPPDEPS)
	@echo dirs: $(CPPDIRS)
	@echo outd: $(OUTDIRS)
	@echo target:  $(TARGET)

	@echo install_exe: $(INSTALL_EXES)
	@echo install_lib: $(INSTALL_LIBS)
nothing:
	@date
