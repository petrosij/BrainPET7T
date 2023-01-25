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
 *	Sample example how to manipulate class could be found in text file lauda_example.txt
 *	/data/BrainPET_User/Former_User/pskudarnov/eclipse-workspace/client_lauda_threads
 *
 * \date:   Created on: Jan 13, 2020
 * \author: Skudarnov Petr (pskudarnov)
 * \version: 0.0
 *
 * \remarks 
 *
 * \todo TODO
 */
#ifndef BRAINPET7TCOOLINGUNITMANAGER_H_
#define BRAINPET7TCOOLINGUNITMANAGER_H_

#include <iostream>
#include <poll.h>			//struct pollfd
#include <string>			//string
#include <arpa/inet.h>		//inet_pton(), inet_addr, sockaddr_in
#include <unordered_map>	//unordered_map
#include <mutex>			//mutex
#include <future>			//future
#include <thread>			//thread
#include <queue>			//queue

#include "BrainPET7TCoolingUnitGlobal.h"
#include "BrainPET7TSocketCommunication.h"
#include "BrainPET7TCoolingUnitConfiguration.h"


    union value{
    	int integerValue;
    	float floatValue;
    	char stringValue[LAUDA_BUFLEN];
    	bool boolValue;
    };

	struct privateMember {
		union value data;
		std::time_t lastUpdate;
	};


	/**
	 * \class BrainPET7TCoolingUnitManager BrainPET7TCoolingUnitManager.h /client_lauda/src/BrainPETCoolingUnitManager.h
	 * \brief Begin a socket session and communicate with cooling station.
	 *
	 */

class BrainPET7TCoolingUnitManager {
public:
	BrainPET7TCoolingUnitManager();
	virtual ~BrainPET7TCoolingUnitManager();
//=================================================
	/// Init all private members
	void initializePrivateMembers();
//=================================================
	/// Set pump action level (1..9)
	void setPumpLevel(int);
	/// Get already set pump action level (1..8)
	void getPumpLevel();
	/// Read pump level from private members
	struct privateMember readPumpLevel() {std::lock_guard<std::mutex> lock(mutexDownstream_);return pumpLevel_;};
//================================================
	///Set the temperature setpoint (-50..200)
	void setSetPoint(float);
	/// Get the temperature setpoint (-50..200)
	void getSetPoint();
	/// Read the temperature setpoint
	struct privateMember readSetPoint (){std::lock_guard<std::mutex> lock(mutexDownstream_);return setPoint_;};
//=================================================
	/// Set the cooling mode use MACROS (LAUDA_MODE_OFF/LAUDA_MODE_ON/LAUDA_MODE_AUTO)
	void setCoolingMode(short);
	/// Get the cooling mode (0 = OFF/1 = ON/2 = AUTO)
	void getCoolingMode();
	/// Read the cooling mode
	struct privateMember readCoolingMode (){std::lock_guard<std::mutex> lock(mutexDownstream_);return coolingMode_;};

//=================================================
	/// Set the flow temperature limit - upper value
	void setMaxFlowTemp(float);
	/// Get the flow temperature limit - upper value
	void getMaxFlowTemp();
	// Read the flow temperature limit - upper value
	struct privateMember readMaxFlowTemp (){std::lock_guard<std::mutex> lock(mutexDownstream_);return maxFlowTemp_;};
//=================================================
	/// Set the flow temperature limit - lower value
	void setMinFlowTemp(float);
	/// Get the flow temperature limit - lower value
	void getMinFlowTemp();
	/// Read the flow temperature limit - lower value
	struct privateMember readMinFlowTemp (){std::lock_guard<std::mutex> lock(mutexDownstream_);return minFlowTemp_;};
//=================================================
	/// Set the safety mode temperature set point
	void setSafeModeTemp(float);
	/// Get the safety mode temperature set point
	void getSafeModeTemp();
	/// Read the safety mode temperature set point
	struct privateMember readSafeModeTemp(){std::lock_guard<std::mutex> lock(mutexDownstream_);return safeModeTemp_;};
//=================================================
	/// Set interface communication timeout 0 = OFF / 1..60 sec (only integer values)
	void setInterfaceTimeout(short);
	/// Get interface communication timeout
	void getInterfaceTimeout();
	/// Read interface communication timeout
	struct privateMember readInterfaceTimeout (){std::lock_guard<std::mutex> lock(mutexDownstream_);return interfaceTimeout_;};

//=================================================

	/// Turn the device ON (from standby mode)
	void start(void);
	/// Switches the device to standby mode (pump,cooling,heating - OFF)
	void stop(void);

	struct privateMember readMainState (){std::lock_guard<std::mutex> lock(mutexDownstream_);return mainState_;};
//=================================================<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	/// Get bath fill level
	void getBathFillLevel();
	struct privateMember readBathFillLevel (){std::lock_guard<std::mutex> lock(mutexDownstream_);return bathFillLevel_;};

//=================================================
	/// Get outflow temperature (bath temp)
	void getIntTemp(void);
	struct privateMember readIntTemp (){std::lock_guard<std::mutex> lock(mutexDownstream_);return intTemp_;};

//=================================================
	/// Get controlled temperature (internal/external Pt/external analog)
	void getControlledTemp(void);
	struct privateMember readControlledTemp (){std::lock_guard<std::mutex> lock(mutexDownstream_);return controlledTemp_;};
//=================================================
	/// Get the overtemperature switch-off point
	void getOvertemperature();
	struct privateMember readOvertemperature (){std::lock_guard<std::mutex> lock(mutexDownstream_);return overtemperature_;};
//=================================================
	/// Get software version number of control system;
	void getControlSystemVersion();
	struct privateMember readControlSystemVersion (){std::lock_guard<std::mutex> lock(mutexDownstream_);return controlSystemVersion_;};
//=================================================
	/// Get software version number of protection system
	void getProtectionSystemVersion();
	struct privateMember readProtectionSystemVersion (){std::lock_guard<std::mutex> lock(mutexDownstream_);return protectionSystemVersion_;};
//=================================================
	/// Get software version number of command remote control
	void getCommandRemoteControlVersion();
	struct privateMember readCommandRemoteControlVersion (){std::lock_guard<std::mutex> lock(mutexDownstream_);return commandRemoteControlVersion_;};
//=================================================
	/// Get software version number of cooling system
	void getCoolingSystemVersion();
	struct privateMember readCoolingSystemVersion (){std::lock_guard<std::mutex> lock(mutexDownstream_);return coolingSystemVersion_;};
//=================================================
	/// Get the device status (0 - OK/1 - fault)
	void getDeviceStatus();
	struct privateMember readDeviceStatus() {std::lock_guard<std::mutex> lock(mutexDownstream_);return deviceStatus_;};
//=================================================
	/** Get the reason of device mallfunction
	 *\return an integer number. Only first 7 bits are relevant. For decoding use faultDecode()
	 *\for further info read the manual (CHAPTER 8 - FAULTS)
	 */
	void getFaultReason();
	struct privateMember readFaultReason (){std::lock_guard<std::mutex> lock(mutexDownstream_);return faultReason_;};

//=================================================<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	/// decoding an integer value about faults on cooling station.
	std::string faultDecode();

//=================================================


	BrainPET7TSocketCommunication laudaSocket;

private:
    privateMember setPoint_;
    privateMember pumpLevel_;
    privateMember coolingMode_;
	privateMember bathFillLevel_;
    privateMember maxFlowTemp_;
	privateMember minFlowTemp_;
	privateMember safeModeTemp_;
	privateMember interfaceTimeout_;
	privateMember mainState_;
	privateMember intTemp_;
	privateMember controlledTemp_;
	privateMember overtemperature_;
	privateMember controlSystemVersion_;
	privateMember commandRemoteControlVersion_;
	privateMember protectionSystemVersion_;
	privateMember coolingSystemVersion_;
	privateMember deviceStatus_;
	privateMember faultReason_;

    std::string sendrecv(std::string);
    std::unordered_map<u_short,std::string> faultNrToStr {
    	{0,"error\n"},
    	{1,"alarm\n"},
    	{2,"warning\n"},
    	{3,"overtemperature\n"},
    	{4,"sublevel\n"},
    	{5,"higher level (alarm setting)\n"},
    	{6,"external control value missing\n"}
    };

    enum memberID : u_short{
    	pumpLevelSet = 1,
		pumpLevelGet = 2,
		setPointSet  = 3,
		setPointGet  = 4,
		coolingModeSet = 5,
		coolingModeGet = 6,
		maxFlowTempSet = 7,
		maxFlowTempGet = 8,
		minFlowTempSet = 9,
		minFlowTempGet = 10,
		safeModeTempSet = 11,
		safeModeTempGet = 12,
		interfaceTimeoutSet = 13,
		interfaceTimeoutGet = 14,
		startSet = 15,
		stopSet = 16,
		bathFillLevelGet = 17,
		intTempGet = 18,
		controlledTempGet = 19,
		overtemperatureGet = 20,
		controlSystemVersionGet = 21,
		protectionSystemVersionGet = 22,
		commandRemoteControlVersionGet = 23,
		coolingSystemVersionGet = 24,
		deviceStatusGet = 25,
		faultReasonGet = 26
    };


    struct queueMember {
    	union value data;
    	std::string message;
    	u_short		messageID;
    };

    std::thread 			thread_;
    std::mutex 				mutexUpstream_,mutexDownstream_;
    std::future<bool>		future_;
    std::promise<bool> 		promise_;
    std::queue<queueMember> queueUp_;


	void checkIfSetterWorked(std::string);
	void setPumpLevelMemberFromGetter(std::string);
	void setSetPointMemberFromGetter(std::string);
	void setCoolingModeMemberFromGetter(std::string);
	void setMaxFlowTempMemberFromGetter(std::string);
	void setMinFlowTempMemberFromGetter(std::string);
	void setSafeModeTempMemberFromGetter(std::string);
	void setInterfaceTimeoutMemberFromGetter(std::string);
	void setBathFillLevelMemberFromGetter(std::string );
	void setIntTempMemberFromGetter(std::string );
	void setControlledTempMemberFromGetter(std::string );
	void setOvertemperatureMemberFromGetter(std::string );
	void setControlSystemVersionMemberFromGetter(std::string );
	void setProtectionSystemVersionMemberFromGetter(std::string );
	void setCommandRemoteControlVersionMemberFromGetter(std::string );
	void setCoolingSystemVersionMemberFromGetter(std::string );
	void setDeviceStatusMemberFromGetter(std::string );
	void setFaultReasonMemberFromGetter(std::string );
    void pushMember(queueMember newMember){
    	mutexUpstream_.lock();
    	queueUp_.push(newMember);
    	mutexUpstream_.unlock();
    };
    ///return true if there is members in queue
    bool isData(std::queue<queueMember>& que){return !(que.empty());};
    void makeThread();
};


#endif /* BRAINPET7TCOOLINGUNITMANAGER_H_ */



























