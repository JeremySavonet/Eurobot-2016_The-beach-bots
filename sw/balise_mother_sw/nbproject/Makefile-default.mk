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
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/balise_mother_sw.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/balise_mother_sw.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=appl/utils/base64Encode.c appl/utils/delay.c appl/utils/crc.c appl/global_variables.c hal/ADXL362.c hal/console.c hal/battery.c hal/pwm.c hw/adc.c hw/gpio.c hw/spi.c hw/system.c hw/timer.c hw/uart.c irq.c main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/appl/utils/base64Encode.o ${OBJECTDIR}/appl/utils/delay.o ${OBJECTDIR}/appl/utils/crc.o ${OBJECTDIR}/appl/global_variables.o ${OBJECTDIR}/hal/ADXL362.o ${OBJECTDIR}/hal/console.o ${OBJECTDIR}/hal/battery.o ${OBJECTDIR}/hal/pwm.o ${OBJECTDIR}/hw/adc.o ${OBJECTDIR}/hw/gpio.o ${OBJECTDIR}/hw/spi.o ${OBJECTDIR}/hw/system.o ${OBJECTDIR}/hw/timer.o ${OBJECTDIR}/hw/uart.o ${OBJECTDIR}/irq.o ${OBJECTDIR}/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/appl/utils/base64Encode.o.d ${OBJECTDIR}/appl/utils/delay.o.d ${OBJECTDIR}/appl/utils/crc.o.d ${OBJECTDIR}/appl/global_variables.o.d ${OBJECTDIR}/hal/ADXL362.o.d ${OBJECTDIR}/hal/console.o.d ${OBJECTDIR}/hal/battery.o.d ${OBJECTDIR}/hal/pwm.o.d ${OBJECTDIR}/hw/adc.o.d ${OBJECTDIR}/hw/gpio.o.d ${OBJECTDIR}/hw/spi.o.d ${OBJECTDIR}/hw/system.o.d ${OBJECTDIR}/hw/timer.o.d ${OBJECTDIR}/hw/uart.o.d ${OBJECTDIR}/irq.o.d ${OBJECTDIR}/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/appl/utils/base64Encode.o ${OBJECTDIR}/appl/utils/delay.o ${OBJECTDIR}/appl/utils/crc.o ${OBJECTDIR}/appl/global_variables.o ${OBJECTDIR}/hal/ADXL362.o ${OBJECTDIR}/hal/console.o ${OBJECTDIR}/hal/battery.o ${OBJECTDIR}/hal/pwm.o ${OBJECTDIR}/hw/adc.o ${OBJECTDIR}/hw/gpio.o ${OBJECTDIR}/hw/spi.o ${OBJECTDIR}/hw/system.o ${OBJECTDIR}/hw/timer.o ${OBJECTDIR}/hw/uart.o ${OBJECTDIR}/irq.o ${OBJECTDIR}/main.o

# Source Files
SOURCEFILES=appl/utils/base64Encode.c appl/utils/delay.c appl/utils/crc.c appl/global_variables.c hal/ADXL362.c hal/console.c hal/battery.c hal/pwm.c hw/adc.c hw/gpio.c hw/spi.c hw/system.c hw/timer.c hw/uart.c irq.c main.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/balise_mother_sw.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ128GA306
MP_LINKER_FILE_OPTION=,--script=balise_mother_linker.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/appl/utils/base64Encode.o: appl/utils/base64Encode.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/appl/utils" 
	@${RM} ${OBJECTDIR}/appl/utils/base64Encode.o.d 
	@${RM} ${OBJECTDIR}/appl/utils/base64Encode.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  appl/utils/base64Encode.c  -o ${OBJECTDIR}/appl/utils/base64Encode.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/appl/utils/base64Encode.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/appl/utils/base64Encode.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/appl/utils/delay.o: appl/utils/delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/appl/utils" 
	@${RM} ${OBJECTDIR}/appl/utils/delay.o.d 
	@${RM} ${OBJECTDIR}/appl/utils/delay.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  appl/utils/delay.c  -o ${OBJECTDIR}/appl/utils/delay.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/appl/utils/delay.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/appl/utils/delay.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/appl/utils/crc.o: appl/utils/crc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/appl/utils" 
	@${RM} ${OBJECTDIR}/appl/utils/crc.o.d 
	@${RM} ${OBJECTDIR}/appl/utils/crc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  appl/utils/crc.c  -o ${OBJECTDIR}/appl/utils/crc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/appl/utils/crc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/appl/utils/crc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/appl/global_variables.o: appl/global_variables.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/appl" 
	@${RM} ${OBJECTDIR}/appl/global_variables.o.d 
	@${RM} ${OBJECTDIR}/appl/global_variables.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  appl/global_variables.c  -o ${OBJECTDIR}/appl/global_variables.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/appl/global_variables.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/appl/global_variables.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/hal/ADXL362.o: hal/ADXL362.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal" 
	@${RM} ${OBJECTDIR}/hal/ADXL362.o.d 
	@${RM} ${OBJECTDIR}/hal/ADXL362.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hal/ADXL362.c  -o ${OBJECTDIR}/hal/ADXL362.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hal/ADXL362.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/hal/ADXL362.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/hal/console.o: hal/console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal" 
	@${RM} ${OBJECTDIR}/hal/console.o.d 
	@${RM} ${OBJECTDIR}/hal/console.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hal/console.c  -o ${OBJECTDIR}/hal/console.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hal/console.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/hal/console.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/hal/battery.o: hal/battery.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal" 
	@${RM} ${OBJECTDIR}/hal/battery.o.d 
	@${RM} ${OBJECTDIR}/hal/battery.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hal/battery.c  -o ${OBJECTDIR}/hal/battery.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hal/battery.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/hal/battery.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/hal/pwm.o: hal/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal" 
	@${RM} ${OBJECTDIR}/hal/pwm.o.d 
	@${RM} ${OBJECTDIR}/hal/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hal/pwm.c  -o ${OBJECTDIR}/hal/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hal/pwm.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/hal/pwm.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/hw/adc.o: hw/adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hw" 
	@${RM} ${OBJECTDIR}/hw/adc.o.d 
	@${RM} ${OBJECTDIR}/hw/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hw/adc.c  -o ${OBJECTDIR}/hw/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hw/adc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/hw/adc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/hw/gpio.o: hw/gpio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hw" 
	@${RM} ${OBJECTDIR}/hw/gpio.o.d 
	@${RM} ${OBJECTDIR}/hw/gpio.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hw/gpio.c  -o ${OBJECTDIR}/hw/gpio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hw/gpio.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/hw/gpio.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/hw/spi.o: hw/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hw" 
	@${RM} ${OBJECTDIR}/hw/spi.o.d 
	@${RM} ${OBJECTDIR}/hw/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hw/spi.c  -o ${OBJECTDIR}/hw/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hw/spi.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/hw/spi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/hw/system.o: hw/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hw" 
	@${RM} ${OBJECTDIR}/hw/system.o.d 
	@${RM} ${OBJECTDIR}/hw/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hw/system.c  -o ${OBJECTDIR}/hw/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hw/system.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/hw/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/hw/timer.o: hw/timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hw" 
	@${RM} ${OBJECTDIR}/hw/timer.o.d 
	@${RM} ${OBJECTDIR}/hw/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hw/timer.c  -o ${OBJECTDIR}/hw/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hw/timer.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/hw/timer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/hw/uart.o: hw/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hw" 
	@${RM} ${OBJECTDIR}/hw/uart.o.d 
	@${RM} ${OBJECTDIR}/hw/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hw/uart.c  -o ${OBJECTDIR}/hw/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hw/uart.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/hw/uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/irq.o: irq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/irq.o.d 
	@${RM} ${OBJECTDIR}/irq.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  irq.c  -o ${OBJECTDIR}/irq.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/irq.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/irq.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/appl/utils/base64Encode.o: appl/utils/base64Encode.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/appl/utils" 
	@${RM} ${OBJECTDIR}/appl/utils/base64Encode.o.d 
	@${RM} ${OBJECTDIR}/appl/utils/base64Encode.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  appl/utils/base64Encode.c  -o ${OBJECTDIR}/appl/utils/base64Encode.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/appl/utils/base64Encode.o.d"      -mno-eds-warn  -g -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/appl/utils/base64Encode.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/appl/utils/delay.o: appl/utils/delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/appl/utils" 
	@${RM} ${OBJECTDIR}/appl/utils/delay.o.d 
	@${RM} ${OBJECTDIR}/appl/utils/delay.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  appl/utils/delay.c  -o ${OBJECTDIR}/appl/utils/delay.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/appl/utils/delay.o.d"      -mno-eds-warn  -g -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/appl/utils/delay.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/appl/utils/crc.o: appl/utils/crc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/appl/utils" 
	@${RM} ${OBJECTDIR}/appl/utils/crc.o.d 
	@${RM} ${OBJECTDIR}/appl/utils/crc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  appl/utils/crc.c  -o ${OBJECTDIR}/appl/utils/crc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/appl/utils/crc.o.d"      -mno-eds-warn  -g -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/appl/utils/crc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/appl/global_variables.o: appl/global_variables.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/appl" 
	@${RM} ${OBJECTDIR}/appl/global_variables.o.d 
	@${RM} ${OBJECTDIR}/appl/global_variables.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  appl/global_variables.c  -o ${OBJECTDIR}/appl/global_variables.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/appl/global_variables.o.d"      -mno-eds-warn  -g -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/appl/global_variables.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/hal/ADXL362.o: hal/ADXL362.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal" 
	@${RM} ${OBJECTDIR}/hal/ADXL362.o.d 
	@${RM} ${OBJECTDIR}/hal/ADXL362.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hal/ADXL362.c  -o ${OBJECTDIR}/hal/ADXL362.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hal/ADXL362.o.d"      -mno-eds-warn  -g -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/hal/ADXL362.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/hal/console.o: hal/console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal" 
	@${RM} ${OBJECTDIR}/hal/console.o.d 
	@${RM} ${OBJECTDIR}/hal/console.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hal/console.c  -o ${OBJECTDIR}/hal/console.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hal/console.o.d"      -mno-eds-warn  -g -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/hal/console.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/hal/battery.o: hal/battery.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal" 
	@${RM} ${OBJECTDIR}/hal/battery.o.d 
	@${RM} ${OBJECTDIR}/hal/battery.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hal/battery.c  -o ${OBJECTDIR}/hal/battery.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hal/battery.o.d"      -mno-eds-warn  -g -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/hal/battery.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/hal/pwm.o: hal/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hal" 
	@${RM} ${OBJECTDIR}/hal/pwm.o.d 
	@${RM} ${OBJECTDIR}/hal/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hal/pwm.c  -o ${OBJECTDIR}/hal/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hal/pwm.o.d"      -mno-eds-warn  -g -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/hal/pwm.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/hw/adc.o: hw/adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hw" 
	@${RM} ${OBJECTDIR}/hw/adc.o.d 
	@${RM} ${OBJECTDIR}/hw/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hw/adc.c  -o ${OBJECTDIR}/hw/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hw/adc.o.d"      -mno-eds-warn  -g -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/hw/adc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/hw/gpio.o: hw/gpio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hw" 
	@${RM} ${OBJECTDIR}/hw/gpio.o.d 
	@${RM} ${OBJECTDIR}/hw/gpio.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hw/gpio.c  -o ${OBJECTDIR}/hw/gpio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hw/gpio.o.d"      -mno-eds-warn  -g -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/hw/gpio.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/hw/spi.o: hw/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hw" 
	@${RM} ${OBJECTDIR}/hw/spi.o.d 
	@${RM} ${OBJECTDIR}/hw/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hw/spi.c  -o ${OBJECTDIR}/hw/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hw/spi.o.d"      -mno-eds-warn  -g -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/hw/spi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/hw/system.o: hw/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hw" 
	@${RM} ${OBJECTDIR}/hw/system.o.d 
	@${RM} ${OBJECTDIR}/hw/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hw/system.c  -o ${OBJECTDIR}/hw/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hw/system.o.d"      -mno-eds-warn  -g -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/hw/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/hw/timer.o: hw/timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hw" 
	@${RM} ${OBJECTDIR}/hw/timer.o.d 
	@${RM} ${OBJECTDIR}/hw/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hw/timer.c  -o ${OBJECTDIR}/hw/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hw/timer.o.d"      -mno-eds-warn  -g -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/hw/timer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/hw/uart.o: hw/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hw" 
	@${RM} ${OBJECTDIR}/hw/uart.o.d 
	@${RM} ${OBJECTDIR}/hw/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hw/uart.c  -o ${OBJECTDIR}/hw/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hw/uart.o.d"      -mno-eds-warn  -g -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/hw/uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/irq.o: irq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/irq.o.d 
	@${RM} ${OBJECTDIR}/irq.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  irq.c  -o ${OBJECTDIR}/irq.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/irq.o.d"      -mno-eds-warn  -g -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/irq.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -O1 -I"appl" -I"hal" -I"hw" -I"hal/miwi" -I"appl/utils" -I"appl/test" -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/balise_mother_sw.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    balise_mother_linker.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/balise_mother_sw.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf  -mreserve=data@0x800:0x81B -mreserve=data@0x81C:0x81D -mreserve=data@0x81E:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x827 -mreserve=data@0x82A:0x84F   -Wl,--local-stack,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/balise_mother_sw.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   balise_mother_linker.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/balise_mother_sw.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,--local-stack,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}/xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/balise_mother_sw.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
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
