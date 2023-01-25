/**
 * \file lauda_global.h (/BrainPET7TSystem/src/lauda_global.h)
 * \ingroup BrainPET7TSystem
 * \brief
 *
 * \date:   Created on: Jan 16, 2020
 * \author: Skudarnov Petr (pskudarnov)
 * \version: 0.0
 *
 * \remarks There SHOULD be a space in the end of setter message, and there SHOULD NOT be a space
 * at the end of getter message. It is requirement for messages structures defined by cooling unit.
 * The space is needed, because the message is incomplete. It is beeing finnaly concatenated when
 * a number is added to the end.
 *
 * \todo TODO
 */
#ifndef BRAINPET7TCOOLINGUNITGLOBAL_H_
#define BRAINPET7TCOOLINGUNITGLOBAL_H_

//Constants
/// Port for communication
#define LAUDA_PORT 54321
/// Cooling mode ON
#define LAUDA_MODE_ON 1
///  Cooling mode OFF
#define LAUDA_MODE_OFF 0
///  Cooling mode AUTO
#define LAUDA_MODE_AUTO 2
/// Target value offset source - normal
#define LAUDA_OFFSET_NORMAL 0
/// Target value offset source - external Pt
#define LAUDA_OFFSET_EXTERNAL_PT 1
/// Target value offset source - external analog
#define LAUDA_OFFSET_EXTERNAL_ANALOG 2

#define LAUDA_BUFLEN 30

#define LAUDA_THREAD_LOOP_TIME 100 ///!< in [ms]

/**Messages for communication with cooling station
 * See an list of all commands on link <a href="YACE0103_PRO Base_V4R15_US_translation_19_12.pdf" target="_blank"><b>LAUDA Manual</b></a> page 44
 */

#define LAUDA_STARTMSG 				"start"
#define LAUDA_STOPMSG 				"stop"
#define LAUDA_STATUS				"status"
#define LAUDA_STAT					"stat"
#define LAUDA_VERSION_R				"version r"
#define LAUDA_VERSION_S				"version s"
#define LAUDA_VERSION_B				"version b"
#define LAUDA_VERSION_T				"version t"

#define LAUDA_SET_TMP_MSG 				"out sp 00 "
#define LAUDA_SET_PUMP_LEVEL_MSG 		"out sp 01 "
#define LAUDA_SET_COOL_MODE_MSG 		"out sp 02 "
#define LAUDA_SET_MAX_FLOW_TEMP_MSG		"out sp 04 "
#define LAUDA_SET_MIN_FLOW_TEMP_MSG 	"out sp 05 "
#define LAUDA_SET_SAFE_MODE_TEMP_MSG 	"out sp 07 "
#define LAUDA_SET_TIMEOUT_MSG 			"out sp 08 "

#define LAUDA_SET_TARGET_VALUE_OFFSET_SOURCE_MSG 	"out mode 04 "
#define LAUDA_SET_SAFETY_MODE_MSG 					"out mode 05 "
#define LAUDA_SET_PROGRAM_MSG 						"rmp select "

#define LAUDA_GET_INT_TMP_MSG 			"in pv 00"
#define LAUDA_GET_CONTROLLED_TEMP_MSG 	"in pv 01"
#define LAUDA_GET_BATH_FILL_LEVEL_MSG 	"in pv 05"
#define LAUDA_GET_PRECISE_INT_TMP_MSG  	"in pv 10"

#define LAUDA_GET_SETPOINT_MSG 						"in sp 00"
#define LAUDA_GET_PUMP_LEVEL_MSG 					"in sp 01"
#define LAUDA_GET_COOLING_MODE_MSG 					"in sp 02"
#define LAUDA_GET_OVERTEMPERATURE_SWITCH_OFF_POINT 	"in sp 03"
#define LAUDA_GET_MAX_FLOW_TEMP_MSG 				"in sp 04"
#define LAUDA_GET_MIN_FLOW_TEMP_MSG 				"in sp 05"
#define LAUDA_GET_SAFE_MODE_TEMP_MSG 				"in sp 07"
#define LAUDA_GET_TIMEOUT_MSG 						"in sp 08"

#define LAUDA_OKMSG 			"OK\r\n"
#define LAUDA_CRLF				"\r\n"
#define LAUDA_ZERO_MSG			"0\r\n"
#define LAUDA_ONE_MSG			"1\r\n"
#define LAUDA_ERR				"ERR_"

#define LAUDA_NO_FAULTS			"No faults"

#endif /* BRAINPET7TCOOLINGUNITGLOBAL_H_ */
