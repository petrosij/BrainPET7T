/**
 * \file BrainPETCoolingUnitManager.h (/BrainPET7TSystem/src/BrainPETCoolingUnitManager.h)
 * \ingroup BrainPET7TSystem
 * \brief An class to manipulate cooling station lauda
 *
 *	That class is created o manipulate cooling station lauda. Before startig
 *	it is needed to specify a host name and bind it to the MAC address of the device.
 *	Also it is possible to get acccess via plain ip. The port is configured automatically.
 *	Usually it is 54321. If in other installation the port differs, change it in the
 *	definitions in lauda_global file.
 *
 * \date:   Created on: Jan 13, 2020
 * \author: Skudarnov Petr (pskudarnov)
 * \version: 0.0
 *
 * \remarks 
 *
 * \todo TODO
 * put sendrecv to the private members
 */
#ifndef BRAINPET7TCOOLINGUNITMANAGER_H_
#define BRAINPET7TCOOLINGUNITMANAGER_H_

#include <poll.h>			//struct pollfd
#include <string>			//string
#include <arpa/inet.h>		//inet_pton(), inet_addr, sockaddr_in

#include "BrainPET7TCoolingUnitGlobal.h"
#include "BrainPET7TSocketCommunication.h"

/**
 * \class BrainPET7TCoolingUnitManager BrainPET7TCoolingUnitManager.h /client_lauda/src/BrainPETCoolingUnitManager.h
 * \brief Begin a socket session and communicate with cooling station.
 *
 */
class BrainPET7TCoolingUnitManager {
public:
	BrainPET7TCoolingUnitManager();
	virtual ~BrainPET7TCoolingUnitManager();

	/** Set the temperature setpoint (-50..200)
	 * \return true for success, false for error */
	bool setTemp(float);
	/** Set pump action level (1..9)
	 * \return true for success, false for error */
	bool setPumpLevel(int);
	/** Set the cooling mode (0 = OFF/1 = ON/2 = AUTO)
	 * \return true for success, false for error */
	bool setCoolingMode(short);
	/** Set the flow temperature limit - upper value
	 * \return true for success, false for error */
	bool setMaxFlowTemp(float);
	/** Set the flow temperature limit - lower value
	 * \return true for success, false for error */
	bool setMinFlowTemp(float);
	/** Set the safety mode temperature set point
	 * \return true for success, false for error */
	bool setSafeModeTemp(float);
	/** Set interface communication timeout 0 = OFF / 1..60 sec (only integer values)
	 * \return true for success, false for error */
	bool setInterfaceTimeout(short);



	/** Set the target value offset source (0 = normal/1 = external Pt/2 = external analog)
	 * \return true for success, false for error */
	bool targetValueOffsetSource(short);
	/** Set the safety mode temperature set point
	 * \return true for success, false for error */
	bool safetyModeSwitch(bool);
	/** Turn the device ON (from standby mode)
	 * \return true for success, false for error */
	bool start(void);
	/** Switches the device to standby mode (pump,cooling,heating - OFF)
	 * \return true for success, false for error */
	bool stop(void);
	/** Set the safety mode temperature set point
	 * \return true for success, false for error */
	bool programSelect(short);



	/// Get outflow temperature (bath temp)
	float getIntTemp(void);
	/// Get already set pump action level (1..8)
	int getPumpLevel(void);
	/// Get controlled temperature (internal/external Pt/external analog)
	float getControlledTemp();
	/// Get bath fill level
	int getBathFillLevel();
	/// Get precise outflow (bath) temperature (format: XX.XXX)
	float getPreciseIntTemp();
	/// Get the temperature setpoint (-50..200)
	float getSetPoint();
	/// Get the cooling mode (0 = OFF/1 = ON/2 = AUTO)
	int getCoolingMode();
	/// Get the overtemperature switch-off point
	float getOvertemperature();
	/// Get the flow temperature limit - upper value
	float getMaxFloatTemp();
	/// Get the flow temperature limit - lower value
	float getMinFloatTemp();
	/// Get the safety mode temperature set point
	float getSafeModeTemp();
	/// Get interface communication timeout
	int getInterfaceTimeout();



	/// Get software version number of control system
	std::string controlSystemVersion();
	/// Get software version number of protection system
	std::string protectionSystemVersion();
	/// Get software version number of command remote control
	std::string commandRemoteControlVersion();
	/// Get software version number of cooling system
	std::string coolingSystemVersion();



	/// Get the device status (0 - OK/1 - fault)
	bool deviceStatus();
	/** Get the reason of device mallfunction
	 *\return XXXXXXX message with further description
	 *\return of each failure
	 *\return In case of an error (1st character is "1") the cooling unit has to be reloaded
	 *\return for further info read the manual (CHAPTER 8 - FAULTS)
	 */
	std::string faultDiagnosis();


	BrainPET7TSocketCommunication laudaSocket;

private:

    std::string sendrecv(std::string);

};


#endif /* BRAINPET7TCOOLINGUNITMANAGER_H_ */
