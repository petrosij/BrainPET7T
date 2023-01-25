/**
 * \file lauda_global.h (/BrainPET7TSystem/src/lauda_global.h)
 * \ingroup BrainPET7TSystem
 * \brief
 *
 * \date:   Created on: Jan 16, 2020
 * \author: Skudarnov Petr (pskudarnov)
 * \version: 0.0
 *
 * \remarks 
 *
 * \todo TODO
 */
#ifndef BRAINPET7TCOOLINGUNITGLOBAL_H_
#define BRAINPET7TCOOLINGUNITGLOBAL_H_

//Constants
/// Port for communication
#define LAUDA_PORT 54321

///Messages for communication with cooling station
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


///Errors that could be received from the cooling station
#define LAUDA_ERRENTRY 208 			///Wrong entry
#define LAUDA_ERRSPEL 209			///Wrong command spelling
#define LAUDA_ERRVAL1 210			///Syntax error in value
#define LAUDA_ERRVAL2 211			///Impermissible value
#define LAUDA_ERRMODULE 212			///Module or value not available
#define LAUDA_ERROCCUP 213			///All segments in programmer are occupied
#define LAUDA_ERRSETPOINT1 214		///Not possible to specify set point value, analog set point input set to ON
#define LAUDA_ERRTEMPMIS 215		///An external temperature probe is missing
#define LAUDA_ERRANALOGVAL 216		///Analog value not present
#define LAUDA_ERRAUTO 217			///Automatically configured
#define LAUDA_ERRSETPOINT2 218		///Not possible to specify set point value, programmer is running or was paused
#define LAUDA_ERRSETPOINT3 219		///Impossible to start the programmer, analog set point value input is turned ON
#define LAUDA_ERRSAVE 220			///Not possible to activate from safety mode

/**
 * For mistake 211: Be sure to send value from permitted range. Look for it in documentation on cooling station.
 */

#endif /* BRAINPET7TCOOLINGUNITGLOBAL_H_ */
