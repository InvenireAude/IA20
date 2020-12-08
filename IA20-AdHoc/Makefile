#########################################################################
ARTIFACTS=
#########################################################################
DEP_PROJECTS+=IA20-CommonLib
#########################################################################
include Makefile.FILES
#########################################################################
include ../IA20-BuilderTools/Makefile
#########################################################################
-include $(CPPDEPS)
#########################################################################
-include Makefile.$(IA20_PLATFORM)

all : makedirs $(CPPEXES)

FBOPTS=--reflect-names


FB_NAMES+=src/ia20/net/engine/raft/fb/rpc

FB_SRCS=$(addsuffix _generated.h, $(FB_NAMES))

#FLATC_FLAGS+=--raw-binary --cpp-str-type IA20::String  --cpp-str-flex-ctor
#FLATC_FLAGS+=--cpp-ptr-type XXX --gen-object-api --gen-mutable
FLATC_FLAGS+=--force-defaults --reflect-names


gen_flatbuffers: $(FB_SRCS)
	@echo Done for gen_flatbuffers

$(FB_SRCS) : %_generated.h : %.fbs
	@echo Creating flatbuffer artifact: $@
	flatc $(FLATC_FLAGS) --cpp -I $(dir $<) -o $(dir $<) $<


#########################################################################
-include Makefile.depend


