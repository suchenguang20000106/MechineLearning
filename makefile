###########################################################################
#
# 5/Aug/04: The /usr/lib/gcc-lib/i386-redhat-linux/2.96/libg2c.a library
#           is needed for the Fortran Interface. If this is used the 
#           mainAnalysis.o during linking must be BEFORE the -lg2c    --SP
#
# 3/Apr/05: Defined some static rules to simplify handling and output --LF
#
###########################################################################

# Root variables
ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
#LDLIBSOPTIONS := xgboost/lib/libxgboost.a xgboost/rabit/lib/librabit.a xgboost/dmlc-core/libdmlc.a 
#LDLIBSOPTIONS := /cvmfs/sft.cern.ch/lcg/releases/LCG_96b/xgboost/0.90/x86_64-centos7-gcc8-opt/xgboost/libxgboost.so /cvmfs/sft.cern.ch/lcg/releases/LCG_96b/xgboost/0.90/x86_64-centos7-gcc8-opt/dmlc-core/libdmlc.a 
#LDLIBSOPTIONS := /cvmfs/sft.cern.ch/lcg/releases/LCG_96b/xgboost/0.90/x86_64-centos7-gcc8-opt/lib/libxgboost.so
#LDLIBSOPTIONS := /mnt/d/xgboost/xgboost/lib/libxgboost.so

# Programs
CXX          = g++
CXXFLAGS     = -g -Wall -fPIC -Wno-deprecated
LDFLAGS      = -g -fopenmp
SOFLAGS      = -shared 

# Local Includes
INCDIR       = -IMain
INCDIR      += -Isrc
#INCDIR      += -Ixgboost/include 
#INCDIR      += -Ixgboost/rabit/include
#INCDIR      += -Ixgboost/dmlc-core/include

# Direct to make the directories with the sources:
VPATH  = ./lib / ./bin ./src ./Tools 
VPATH += ./Main 

# Assign or Add variables
CXXFLAGS    += $(ROOTCFLAGS) 
CXXFLAGS    += $(INCDIR)
LIBS        += $(ROOTLIBS)
LIBS        += $(LDLIBSOPTIONS)

CXXOBJS      = $(CXXSRCS:.cc=.o)
GARBAGE      = $(CXXOBJS) libana.so *~ lib/*.so lib/*.o ./*/*.o


############################################################################
# Build main.c compiling only the first prereq: ($<) means main.c
############################################################################
Draw : Draw.o

############################################################################
# General rules. The first two need not be specified due to implicit rules, 
# but redefined for the "echo"
############################################################################
%.o : %.cxx %.h
	@echo "compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< 

%.o : %.cxx
	@echo "compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< 

%	: %.o
	@echo "compiling and linking $@"
	@$(CXX) $(LDFLAGS) $^ $(LIBS)  -o $@
	@if [ `ls | grep "\.so"$ | wc -l` != 0 ]; then mv *.so lib/; fi
	@if [ `ls | grep "\.o"$  | wc -l` != 0 ]; then mv *.o  lib/; fi
	@mv $@ bin/

############################################################################
# Build libana.so compiling all prerequisites: ($^) means CXXOBJS 
# ($@) means the target libana.so
############################################################################
libana.so : $(CXXOBJS) 
	@echo "linking libana.so"
	@$(CXX) $(SOFLAGS) $(LDFLAGS) $^ -o $@


############################################################################
# Phony rules (no prerequisites)
############################################################################

.PHONY: clean cln print xemacs backup clnPM

clean :
	@rm -f $(GARBAGE)

cln :
	@rm -f *~ *.o */*~

clnPM :
	@rm -f usePM pairManager.o SingleElectron.o */*~

print :
	@echo compiler  : $(CXX)
	@echo c++ srcs  : $(CXXSRCS)
	@echo c++ objs  : $(CXXOBJS)
	@echo c++ flags : $(CXXFLAGS)
	@echo libs      : $(LIBS)
	@echo so flags  : $(SOFLAGS)
	@echo rootlibs  : $(ROOTLIBS)

