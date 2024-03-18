####################################################
###################### MAKE ########################
####################################################

EXECUTAVEL = ALNS100
PATHEXEC = ./bin
PATHSRC= ./src
PATHTEMP = ./.temp

all:
	mkdir -p $(PATHEXEC)
	mkdir -p $(PATHTEMP)
	make $(EXECUTAVEL)

#Juntando todos os objetos e gerando o programa
$(EXECUTAVEL):  $(PATHTEMP)/ExactTSCFL.o $(PATHTEMP)/CSALNS.o  $(PATHTEMP)/graphcost.o $(PATHTEMP)/localsearch.o $(PATHTEMP)/heuristic.o $(PATHTEMP)/checkfeas.o $(PATHTEMP)/instance.o $(PATHTEMP)/main.o
	$(CPP) $(CCFLAGS) $(PATHTEMP)/ExactTSCFL.o $(PATHTEMP)/CSALNS.o $(PATHTEMP)/graphcost.o $(PATHTEMP)/localsearch.o $(PATHTEMP)/heuristic.o $(PATHTEMP)/checkfeas.o $(PATHTEMP)/instance.o $(PATHTEMP)/main.o $(CCLNFLAGS) -o $(PATHEXEC)/$(EXECUTAVEL)


$(PATHTEMP)/main.o: $(PATHSRC)/main.cpp
	$(CPP) $(CCFLAGS) -c $(PATHSRC)/main.cpp -o $(PATHTEMP)/main.o

$(PATHTEMP)/CSALNS.o: $(PATHSRC)/CSALNS.cpp
	$(CPP) $(CCFLAGS) -c $(PATHSRC)/CSALNS.cpp -o $(PATHTEMP)/CSALNS.o


$(PATHTEMP)/instance.o: $(PATHSRC)/instance.cpp
	$(CPP) $(CCFLAGS) -c $(PATHSRC)/instance.cpp -o $(PATHTEMP)/instance.o

$(PATHTEMP)/heuristic.o: $(PATHSRC)/heuristic.cpp
	$(CPP) $(CCFLAGS) -c $(PATHSRC)/heuristic.cpp -o $(PATHTEMP)/heuristic.o

$(PATHTEMP)/graphcost.o: $(PATHSRC)/graphcost.cpp
	$(CPP) $(CCFLAGS) -c $(PATHSRC)/graphcost.cpp -o $(PATHTEMP)/graphcost.o

$(PATHTEMP)/localsearch.o: $(PATHSRC)/localsearch.cpp
	$(CPP) $(CCFLAGS) -c $(PATHSRC)/localsearch.cpp -o $(PATHTEMP)/localsearch.o

$(PATHTEMP)/checkfeas.o: $(PATHSRC)/checkfeas.cpp
	$(CPP) $(CCFLAGS) -c $(PATHSRC)/checkfeas.cpp -o $(PATHTEMP)/checkfeas.o

$(PATHTEMP)/ExactTSCFL.o: $(PATHSRC)/ExactTSCFL.cpp
	$(CPP) $(CCFLAGS) -c $(PATHSRC)/ExactTSCFL.cpp -o $(PATHTEMP)/ExactTSCFL.o

####################################################
###################### CLEAN #######################
####################################################
clean:
	#rm -rf $(PATHEXEC)
	rm -rf $(PATHTEMP)
	rm *.tmp
	rm *.lp
	rm solution.txt
	rm *.jpg

####################################################
####################### CPLEX ######################
####################################################

SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic

#------------------------------------------------------------
#
# When you adapt this makefile to compile your CPLEX programs
# please copy this makefile and set CPLEXDIR and CONCERTDIR to
# the directories where CPLEX and CONCERT are installed.
#
#------------------------------------------------------------

CPLEXDIR      = /opt/ibm/ILOG/CPLEX_Studio1210/cplex

##### CPLEX CONFIGURATION's
# System architecture
SYSTEM = x86-64_linux
# Static library format for Cplex
LIBFORMAT = static_pic
# Cplex directory
ILCPLEXDIR = /opt/ibm/ILOG/CPLEX_Studio1210/cplex
# Concert directory
ILCONCERTDIR = /opt/ibm/ILOG/CPLEX_Studio1210/concert

##### CPLEX DIRECTIVE's
# Cplex static libraries directory
ILCPLEXLIBDIR = $(ILCPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
# Concert static libraries directory
ILCONCERTLIBDIR = $(ILCONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
# Cplex header's directory
ILCPLEXINCDIR = $(ILCPLEXDIR)/include
# Concert header's directory
ILCONCERTINCDIR = $(ILCONCERTDIR)/include

# ---------------------------------------------------------------------
# Compiler selection 
# ---------------------------------------------------------------------
 
CPP = g++ 
# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------


COPT  = -O3 -m64 -fPIC -fno-strict-aliasing -DIL_STD


# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

CPLEXLIB  = cplex

CLNDIRS   = -L$(CPLEXLIBDIR)
CCLNFLAGS  =  -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(ILCONCERTLIBDIR) -lconcert -lm -lpthread -ldl

CPLEXINCDIR   = $(CPLEXDIR)/include/ilcplex/

CCFLAGS  = $(COPT) -I$(ILCPLEXINCDIR) -I$(ILCONCERTINCDIR)  -I$(CPLEXINCDIR) 


