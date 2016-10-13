#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/CBA_CAN.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/CBA_CAN.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../spi_open.c ../spi_read.c ../spi_writ.c ../main.c ../queue.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/spi_open.o ${OBJECTDIR}/_ext/1472/spi_read.o ${OBJECTDIR}/_ext/1472/spi_writ.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/queue.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/spi_open.o.d ${OBJECTDIR}/_ext/1472/spi_read.o.d ${OBJECTDIR}/_ext/1472/spi_writ.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1472/queue.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/spi_open.o ${OBJECTDIR}/_ext/1472/spi_read.o ${OBJECTDIR}/_ext/1472/spi_writ.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/queue.o

# Source Files
SOURCEFILES=../spi_open.c ../spi_read.c ../spi_writ.c ../main.c ../queue.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/CBA_CAN.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F2680
MP_PROCESSOR_OPTION_LD=18f2680
MP_LINKER_DEBUG_OPTION= -u_DEBUGCODESTART=0xfd30 -u_DEBUGCODELEN=0x2d0 -u_DEBUGDATASTART=0xcf4 -u_DEBUGDATALEN=0xb
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/spi_open.o: ../spi_open.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_open.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_open.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I".." -I"../C:/MCC18/3_35/h" -I"../X:" -I"." -ms -oa- -o-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/spi_open.o   ../spi_open.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/spi_open.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/spi_open.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/spi_read.o: ../spi_read.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_read.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_read.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I".." -I"../C:/MCC18/3_35/h" -I"../X:" -I"." -ms -oa- -o-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/spi_read.o   ../spi_read.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/spi_read.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/spi_read.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/spi_writ.o: ../spi_writ.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_writ.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_writ.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I".." -I"../C:/MCC18/3_35/h" -I"../X:" -I"." -ms -oa- -o-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/spi_writ.o   ../spi_writ.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/spi_writ.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/spi_writ.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I".." -I"../C:/MCC18/3_35/h" -I"../X:" -I"." -ms -oa- -o-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/main.o   ../main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/queue.o: ../queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I".." -I"../C:/MCC18/3_35/h" -I"../X:" -I"." -ms -oa- -o-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/queue.o   ../queue.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/queue.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
else
${OBJECTDIR}/_ext/1472/spi_open.o: ../spi_open.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_open.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_open.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I".." -I"../C:/MCC18/3_35/h" -I"../X:" -I"." -ms -oa- -o-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/spi_open.o   ../spi_open.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/spi_open.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/spi_open.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/spi_read.o: ../spi_read.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_read.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_read.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I".." -I"../C:/MCC18/3_35/h" -I"../X:" -I"." -ms -oa- -o-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/spi_read.o   ../spi_read.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/spi_read.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/spi_read.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/spi_writ.o: ../spi_writ.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_writ.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_writ.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I".." -I"../C:/MCC18/3_35/h" -I"../X:" -I"." -ms -oa- -o-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/spi_writ.o   ../spi_writ.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/spi_writ.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/spi_writ.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I".." -I"../C:/MCC18/3_35/h" -I"../X:" -I"." -ms -oa- -o-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/main.o   ../main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/queue.o: ../queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I".." -I"../C:/MCC18/3_35/h" -I"../X:" -I"." -ms -oa- -o-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/queue.o   ../queue.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/queue.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/CBA_CAN.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../18f2680.lkr
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) "../18f2680.lkr"  -p$(MP_PROCESSOR_OPTION_LD)  -w -x -u_DEBUG -m"$(BINDIR_)$(TARGETBASE).map" -w -l"../C:/MCC18/3_35/lib" -l"../X:/compiler" -l"."  -z__MPLAB_BUILD=1  -u_CRUNTIME -z__MPLAB_DEBUG=1 -z__MPLAB_DEBUGGER_PK3=1 $(MP_LINKER_DEBUG_OPTION) -l ${MP_CC_DIR}/../lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/CBA_CAN.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
else
dist/${CND_CONF}/${IMAGE_TYPE}/CBA_CAN.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../18f2680.lkr
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) "../18f2680.lkr"  -p$(MP_PROCESSOR_OPTION_LD)  -w  -m"$(BINDIR_)$(TARGETBASE).map" -w -l"../C:/MCC18/3_35/lib" -l"../X:/compiler" -l"."  -z__MPLAB_BUILD=1  -u_CRUNTIME -l ${MP_CC_DIR}/../lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/CBA_CAN.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
