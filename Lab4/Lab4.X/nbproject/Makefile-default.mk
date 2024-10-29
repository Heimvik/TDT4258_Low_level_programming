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
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=null
FINAL_IMAGE=${DISTDIR}/Lab4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=null
FINAL_IMAGE=${DISTDIR}/Lab4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=main.c usart.c ac.c led.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.${OUTPUT_EXTENSION_C} ${OBJECTDIR}/usart.${OUTPUT_EXTENSION_C} ${OBJECTDIR}/ac.${OUTPUT_EXTENSION_C} ${OBJECTDIR}/led.${OUTPUT_EXTENSION_C}
POSSIBLE_DEPFILES=${OBJECTDIR}/main.${OUTPUT_EXTENSION_C}.d ${OBJECTDIR}/usart.${OUTPUT_EXTENSION_C}.d ${OBJECTDIR}/ac.${OUTPUT_EXTENSION_C}.d ${OBJECTDIR}/led.${OUTPUT_EXTENSION_C}.d

# Object Files
OBJECTFILES=${OBJECTDIR}/main.${OUTPUT_EXTENSION_C} ${OBJECTDIR}/usart.${OUTPUT_EXTENSION_C} ${OBJECTDIR}/ac.${OUTPUT_EXTENSION_C} ${OBJECTDIR}/led.${OUTPUT_EXTENSION_C}

# Source Files
SOURCEFILES=main.c usart.c ac.c led.c



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Lab4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/main.${OUTPUT_EXTENSION_C}: main.c  .generated_files/flags/default/b0e93bd4cd902135109bf6130ddb56da5a5a0101 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.${OUTPUT_EXTENSION_C}.d 
	@${RM} ${OBJECTDIR}/main.${OUTPUT_EXTENSION_C} 
	 -D__DEBUG=1    -fshort-double -fshort-float ${code-model-external.prefix}wordwrite -O1 ${optimization-options.prefix}+asm${optimization-options.separator}${optimization-assembler-files.true}${optimization-options.separator}-speed,+space${optimization-options.separator}${optimization-debug.false}${optimization-options.separator}${local-generation.false} ${what-to-do.prefix}ignore --mode=free ${preprocess-assembler.trueemission} -N255 ${warn-level.prefix}-3 ${asmlist.true} -D=$(CND_CONF)  ${linker-report-options.prefix}-psect${linker-report-options.separator}-class${linker-report-options.separator}+mem${linker-report-options.separator}-hex${linker-report-options.separator}-file ${ld-extra.prefix}${ld-extra.false} ${linker-runtime-options.prefix}+clear${linker-runtime-options.separator}${initialize-data.true}${linker-runtime-options.separator}${keep-generated-startup.as.false}${linker-runtime-options.separator}${opt-xc8-linker-link_startup.false}${linker-runtime-options.separator}${calibrate-oscillator.false}${linker-runtime-options.separator}${backup-reset-condition-flags.false}${linker-runtime-options.separator}${format-hex-file-for-download.false}${linker-runtime-options.separator}${managed-stack.false}${linker-runtime-options.separator}${program-the-device-with-default-config-words.false}${linker-runtime-options.separator}-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs ${stack-options.prefix}compiled    main.c 
	
${OBJECTDIR}/usart.${OUTPUT_EXTENSION_C}: usart.c  .generated_files/flags/default/790a5eb4525ea72f3b102e1d940c6ebc141802b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usart.${OUTPUT_EXTENSION_C}.d 
	@${RM} ${OBJECTDIR}/usart.${OUTPUT_EXTENSION_C} 
	 -D__DEBUG=1    -fshort-double -fshort-float ${code-model-external.prefix}wordwrite -O1 ${optimization-options.prefix}+asm${optimization-options.separator}${optimization-assembler-files.true}${optimization-options.separator}-speed,+space${optimization-options.separator}${optimization-debug.false}${optimization-options.separator}${local-generation.false} ${what-to-do.prefix}ignore --mode=free ${preprocess-assembler.trueemission} -N255 ${warn-level.prefix}-3 ${asmlist.true} -D=$(CND_CONF)  ${linker-report-options.prefix}-psect${linker-report-options.separator}-class${linker-report-options.separator}+mem${linker-report-options.separator}-hex${linker-report-options.separator}-file ${ld-extra.prefix}${ld-extra.false} ${linker-runtime-options.prefix}+clear${linker-runtime-options.separator}${initialize-data.true}${linker-runtime-options.separator}${keep-generated-startup.as.false}${linker-runtime-options.separator}${opt-xc8-linker-link_startup.false}${linker-runtime-options.separator}${calibrate-oscillator.false}${linker-runtime-options.separator}${backup-reset-condition-flags.false}${linker-runtime-options.separator}${format-hex-file-for-download.false}${linker-runtime-options.separator}${managed-stack.false}${linker-runtime-options.separator}${program-the-device-with-default-config-words.false}${linker-runtime-options.separator}-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs ${stack-options.prefix}compiled    usart.c 
	
${OBJECTDIR}/ac.${OUTPUT_EXTENSION_C}: ac.c  .generated_files/flags/default/f431a32afb86c282699be1cdb34fa999b26dcdd7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ac.${OUTPUT_EXTENSION_C}.d 
	@${RM} ${OBJECTDIR}/ac.${OUTPUT_EXTENSION_C} 
	 -D__DEBUG=1    -fshort-double -fshort-float ${code-model-external.prefix}wordwrite -O1 ${optimization-options.prefix}+asm${optimization-options.separator}${optimization-assembler-files.true}${optimization-options.separator}-speed,+space${optimization-options.separator}${optimization-debug.false}${optimization-options.separator}${local-generation.false} ${what-to-do.prefix}ignore --mode=free ${preprocess-assembler.trueemission} -N255 ${warn-level.prefix}-3 ${asmlist.true} -D=$(CND_CONF)  ${linker-report-options.prefix}-psect${linker-report-options.separator}-class${linker-report-options.separator}+mem${linker-report-options.separator}-hex${linker-report-options.separator}-file ${ld-extra.prefix}${ld-extra.false} ${linker-runtime-options.prefix}+clear${linker-runtime-options.separator}${initialize-data.true}${linker-runtime-options.separator}${keep-generated-startup.as.false}${linker-runtime-options.separator}${opt-xc8-linker-link_startup.false}${linker-runtime-options.separator}${calibrate-oscillator.false}${linker-runtime-options.separator}${backup-reset-condition-flags.false}${linker-runtime-options.separator}${format-hex-file-for-download.false}${linker-runtime-options.separator}${managed-stack.false}${linker-runtime-options.separator}${program-the-device-with-default-config-words.false}${linker-runtime-options.separator}-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs ${stack-options.prefix}compiled    ac.c 
	
${OBJECTDIR}/led.${OUTPUT_EXTENSION_C}: led.c  .generated_files/flags/default/5c241b082efcd523d7b295596ec3ae5c9da9dba .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/led.${OUTPUT_EXTENSION_C}.d 
	@${RM} ${OBJECTDIR}/led.${OUTPUT_EXTENSION_C} 
	 -D__DEBUG=1    -fshort-double -fshort-float ${code-model-external.prefix}wordwrite -O1 ${optimization-options.prefix}+asm${optimization-options.separator}${optimization-assembler-files.true}${optimization-options.separator}-speed,+space${optimization-options.separator}${optimization-debug.false}${optimization-options.separator}${local-generation.false} ${what-to-do.prefix}ignore --mode=free ${preprocess-assembler.trueemission} -N255 ${warn-level.prefix}-3 ${asmlist.true} -D=$(CND_CONF)  ${linker-report-options.prefix}-psect${linker-report-options.separator}-class${linker-report-options.separator}+mem${linker-report-options.separator}-hex${linker-report-options.separator}-file ${ld-extra.prefix}${ld-extra.false} ${linker-runtime-options.prefix}+clear${linker-runtime-options.separator}${initialize-data.true}${linker-runtime-options.separator}${keep-generated-startup.as.false}${linker-runtime-options.separator}${opt-xc8-linker-link_startup.false}${linker-runtime-options.separator}${calibrate-oscillator.false}${linker-runtime-options.separator}${backup-reset-condition-flags.false}${linker-runtime-options.separator}${format-hex-file-for-download.false}${linker-runtime-options.separator}${managed-stack.false}${linker-runtime-options.separator}${program-the-device-with-default-config-words.false}${linker-runtime-options.separator}-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs ${stack-options.prefix}compiled    led.c 
	
else
${OBJECTDIR}/main.${OUTPUT_EXTENSION_C}: main.c  .generated_files/flags/default/6bbdd1eecdc1e45539842fa34b1c91803efbb145 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.${OUTPUT_EXTENSION_C}.d 
	@${RM} ${OBJECTDIR}/main.${OUTPUT_EXTENSION_C} 
	   -fshort-double -fshort-float ${code-model-external.prefix}wordwrite -O1 ${optimization-options.prefix}+asm${optimization-options.separator}${optimization-assembler-files.true}${optimization-options.separator}-speed,+space${optimization-options.separator}${optimization-debug.false}${optimization-options.separator}${local-generation.false} ${what-to-do.prefix}ignore --mode=free ${preprocess-assembler.trueemission} -N255 ${warn-level.prefix}-3 ${asmlist.true} -D=$(CND_CONF)  ${linker-report-options.prefix}-psect${linker-report-options.separator}-class${linker-report-options.separator}+mem${linker-report-options.separator}-hex${linker-report-options.separator}-file ${ld-extra.prefix}${ld-extra.false} ${linker-runtime-options.prefix}+clear${linker-runtime-options.separator}${initialize-data.true}${linker-runtime-options.separator}${keep-generated-startup.as.false}${linker-runtime-options.separator}${opt-xc8-linker-link_startup.false}${linker-runtime-options.separator}${calibrate-oscillator.false}${linker-runtime-options.separator}${backup-reset-condition-flags.false}${linker-runtime-options.separator}${format-hex-file-for-download.false}${linker-runtime-options.separator}${managed-stack.false}${linker-runtime-options.separator}${program-the-device-with-default-config-words.false}${linker-runtime-options.separator}-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs ${stack-options.prefix}compiled    main.c 
	
${OBJECTDIR}/usart.${OUTPUT_EXTENSION_C}: usart.c  .generated_files/flags/default/4883857d26c65d9ea0c1f2c315c8a8b5646f83c6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usart.${OUTPUT_EXTENSION_C}.d 
	@${RM} ${OBJECTDIR}/usart.${OUTPUT_EXTENSION_C} 
	   -fshort-double -fshort-float ${code-model-external.prefix}wordwrite -O1 ${optimization-options.prefix}+asm${optimization-options.separator}${optimization-assembler-files.true}${optimization-options.separator}-speed,+space${optimization-options.separator}${optimization-debug.false}${optimization-options.separator}${local-generation.false} ${what-to-do.prefix}ignore --mode=free ${preprocess-assembler.trueemission} -N255 ${warn-level.prefix}-3 ${asmlist.true} -D=$(CND_CONF)  ${linker-report-options.prefix}-psect${linker-report-options.separator}-class${linker-report-options.separator}+mem${linker-report-options.separator}-hex${linker-report-options.separator}-file ${ld-extra.prefix}${ld-extra.false} ${linker-runtime-options.prefix}+clear${linker-runtime-options.separator}${initialize-data.true}${linker-runtime-options.separator}${keep-generated-startup.as.false}${linker-runtime-options.separator}${opt-xc8-linker-link_startup.false}${linker-runtime-options.separator}${calibrate-oscillator.false}${linker-runtime-options.separator}${backup-reset-condition-flags.false}${linker-runtime-options.separator}${format-hex-file-for-download.false}${linker-runtime-options.separator}${managed-stack.false}${linker-runtime-options.separator}${program-the-device-with-default-config-words.false}${linker-runtime-options.separator}-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs ${stack-options.prefix}compiled    usart.c 
	
${OBJECTDIR}/ac.${OUTPUT_EXTENSION_C}: ac.c  .generated_files/flags/default/809497151db3aa39d5fbace237e964727c125a05 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ac.${OUTPUT_EXTENSION_C}.d 
	@${RM} ${OBJECTDIR}/ac.${OUTPUT_EXTENSION_C} 
	   -fshort-double -fshort-float ${code-model-external.prefix}wordwrite -O1 ${optimization-options.prefix}+asm${optimization-options.separator}${optimization-assembler-files.true}${optimization-options.separator}-speed,+space${optimization-options.separator}${optimization-debug.false}${optimization-options.separator}${local-generation.false} ${what-to-do.prefix}ignore --mode=free ${preprocess-assembler.trueemission} -N255 ${warn-level.prefix}-3 ${asmlist.true} -D=$(CND_CONF)  ${linker-report-options.prefix}-psect${linker-report-options.separator}-class${linker-report-options.separator}+mem${linker-report-options.separator}-hex${linker-report-options.separator}-file ${ld-extra.prefix}${ld-extra.false} ${linker-runtime-options.prefix}+clear${linker-runtime-options.separator}${initialize-data.true}${linker-runtime-options.separator}${keep-generated-startup.as.false}${linker-runtime-options.separator}${opt-xc8-linker-link_startup.false}${linker-runtime-options.separator}${calibrate-oscillator.false}${linker-runtime-options.separator}${backup-reset-condition-flags.false}${linker-runtime-options.separator}${format-hex-file-for-download.false}${linker-runtime-options.separator}${managed-stack.false}${linker-runtime-options.separator}${program-the-device-with-default-config-words.false}${linker-runtime-options.separator}-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs ${stack-options.prefix}compiled    ac.c 
	
${OBJECTDIR}/led.${OUTPUT_EXTENSION_C}: led.c  .generated_files/flags/default/203afd8cdf48619f2b4138f666fce9ef58d18bc3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/led.${OUTPUT_EXTENSION_C}.d 
	@${RM} ${OBJECTDIR}/led.${OUTPUT_EXTENSION_C} 
	   -fshort-double -fshort-float ${code-model-external.prefix}wordwrite -O1 ${optimization-options.prefix}+asm${optimization-options.separator}${optimization-assembler-files.true}${optimization-options.separator}-speed,+space${optimization-options.separator}${optimization-debug.false}${optimization-options.separator}${local-generation.false} ${what-to-do.prefix}ignore --mode=free ${preprocess-assembler.trueemission} -N255 ${warn-level.prefix}-3 ${asmlist.true} -D=$(CND_CONF)  ${linker-report-options.prefix}-psect${linker-report-options.separator}-class${linker-report-options.separator}+mem${linker-report-options.separator}-hex${linker-report-options.separator}-file ${ld-extra.prefix}${ld-extra.false} ${linker-runtime-options.prefix}+clear${linker-runtime-options.separator}${initialize-data.true}${linker-runtime-options.separator}${keep-generated-startup.as.false}${linker-runtime-options.separator}${opt-xc8-linker-link_startup.false}${linker-runtime-options.separator}${calibrate-oscillator.false}${linker-runtime-options.separator}${backup-reset-condition-flags.false}${linker-runtime-options.separator}${format-hex-file-for-download.false}${linker-runtime-options.separator}${managed-stack.false}${linker-runtime-options.separator}${program-the-device-with-default-config-words.false}${linker-runtime-options.separator}-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs ${stack-options.prefix}compiled    led.c 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
