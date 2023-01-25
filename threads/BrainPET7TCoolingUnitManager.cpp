/**
 * \file BrainPET7TCoolingUnitManager.cpp (/BrainPET7TSystem/src/BrainPET7TCoolingUnitManager.cpp)
 * \ingroup BrainPET7TSystem
 * \brief
 *
 * \date:   Created on: Jan 13, 2020
 * \author: Skudarnov Petr (pskudarnov)
 * \version: 0.0
 *
 * \remarks 
 *
 * \todo TODO
 */

#include "BrainPET7TCoolingUnitManager.h"
#include "BrainPET7TCoolingUnitGlobal.h"
#include "BrainPET7TCoolingUnitException.h"
#include "BrainPET7TSocketCommunicationException.h"

#include <stdio.h>

#include <iomanip>			//setprecision()
#include <sstream>			//stringstream
#include <string>			//string
#include <stdlib.h>			//atoi(),strtof(), exit()
#include <chrono>


BrainPET7TCoolingUnitManager::BrainPET7TCoolingUnitManager() {
	// TODO Auto-generated constructor stub
	pumpLevel_.data.integerValue = 0;
	pumpLevel_.lastUpdate =  std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>{});

	setPoint_.data.floatValue = 0.0f;
	setPoint_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>{});

	coolingMode_.data.integerValue = 0;
	coolingMode_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>{});

	maxFlowTemp_.data.floatValue = 0.0f;
	maxFlowTemp_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>{});

	minFlowTemp_.data.floatValue = 0.0f;
	minFlowTemp_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>{});

	safeModeTemp_.data.floatValue = 0.0f;
	safeModeTemp_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>{});

	interfaceTimeout_.data.integerValue = 0;
	interfaceTimeout_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>{});

	mainState_.data.boolValue = false;
	mainState_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>{});


	bathFillLevel_.data.integerValue =0;
	bathFillLevel_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>{});

	intTemp_.data.floatValue = 0.0;
	intTemp_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>{});

	controlledTemp_.data.floatValue = 0.0;
	controlledTemp_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>{});

	overtemperature_.data.floatValue = 0.0;
	overtemperature_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>{});

	memset(controlSystemVersion_.data.stringValue,0,LAUDA_BUFLEN);
	controlSystemVersion_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>{});

	memset(protectionSystemVersion_.data.stringValue,0,LAUDA_BUFLEN);
	protectionSystemVersion_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>{});

	memset(commandRemoteControlVersion_.data.stringValue,0,LAUDA_BUFLEN);
	commandRemoteControlVersion_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>{});

	memset(coolingSystemVersion_.data.stringValue,0,LAUDA_BUFLEN);
	coolingSystemVersion_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>{});

	deviceStatus_.data.boolValue = false;
	deviceStatus_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>{});

	faultReason_.data.integerValue = 0;
	faultReason_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>{});

	future_ = promise_.get_future();
	thread_ = std::thread(&BrainPET7TCoolingUnitManager::makeThread,this);
}


BrainPET7TCoolingUnitManager::~BrainPET7TCoolingUnitManager() {
	// TODO Auto-generated destructor stub

	promise_.set_value(true);
	thread_.join();

}

void BrainPET7TCoolingUnitManager::initializePrivateMembers(){
	getSetPoint();
	getPumpLevel();
	getCoolingMode();
	getBathFillLevel();
	getMaxFlowTemp();
	getMinFlowTemp();
	getSafeModeTemp();
	getInterfaceTimeout();
	stop();
	getIntTemp();
	getControlledTemp();
	getOvertemperature();
	getControlSystemVersion();
	getProtectionSystemVersion();
	getCommandRemoteControlVersion();
	getCoolingSystemVersion();
	getDeviceStatus();
	getFaultReason();
}

void BrainPET7TCoolingUnitManager::setPumpLevel(int pump_level){
	std::stringstream buffer;
	queueMember newMember;
	buffer << std::fixed << std::setprecision(0) << pump_level;
	newMember.message = LAUDA_SET_PUMP_LEVEL_MSG + buffer.str();
	newMember.messageID = memberID::pumpLevelSet;
	newMember.data.integerValue = pump_level;
	pushMember(newMember);
}

void  BrainPET7TCoolingUnitManager::getPumpLevel(){
	queueMember newMember;
	newMember.message = LAUDA_GET_PUMP_LEVEL_MSG;
	newMember.messageID = memberID::pumpLevelGet;
	pushMember(newMember);

}

void BrainPET7TCoolingUnitManager::setSetPoint(float temperature){
	queueMember newMember;
	std::stringstream buffer;
	buffer << std::fixed << std::setprecision(2) << temperature;
	newMember.message = LAUDA_SET_TMP_MSG + buffer.str();
	newMember.messageID = memberID::setPointSet;
	newMember.data.floatValue = temperature;
	pushMember(newMember);
}

void BrainPET7TCoolingUnitManager::getSetPoint(void){
	queueMember newMember;
	newMember.message = LAUDA_GET_SETPOINT_MSG;
	newMember.messageID = memberID::setPointGet;
	pushMember(newMember);
}


void BrainPET7TCoolingUnitManager::setCoolingMode(short mode){
	queueMember newMember;
	std::stringstream buffer;
	buffer << std::fixed << std::setprecision(0) << mode;
	newMember.message = LAUDA_SET_COOL_MODE_MSG + buffer.str();
	newMember.messageID = memberID::coolingModeSet;
	newMember.data.integerValue = mode;
	pushMember(newMember);
}

void BrainPET7TCoolingUnitManager::getCoolingMode(void){
	queueMember newMember;
	newMember.message = LAUDA_GET_COOLING_MODE_MSG;
	newMember.messageID = memberID::coolingModeGet;
	pushMember(newMember);
}


void BrainPET7TCoolingUnitManager::setMaxFlowTemp(float temp){
	queueMember newMember;
	std::stringstream buffer;
	buffer << std::fixed << std::setprecision(2) << temp;
	newMember.message = LAUDA_SET_MAX_FLOW_TEMP_MSG + buffer.str();
	newMember.messageID = memberID::maxFlowTempSet;
	newMember.data.floatValue = temp;
	pushMember(newMember);
}

void BrainPET7TCoolingUnitManager::getMaxFlowTemp(void){
	queueMember newMember;
	newMember.message = LAUDA_GET_MAX_FLOW_TEMP_MSG;
	newMember.messageID = memberID::maxFlowTempGet;
	pushMember(newMember);
}

void BrainPET7TCoolingUnitManager::setMinFlowTemp(float temp){
	queueMember newMember;
	std::stringstream buffer;
	buffer << std::fixed << std::setprecision(2) << temp;
	newMember.message = LAUDA_SET_MIN_FLOW_TEMP_MSG + buffer.str();;
	newMember.messageID = memberID::minFlowTempSet;
	newMember.data.floatValue = temp;
	pushMember(newMember);
}

void BrainPET7TCoolingUnitManager::getMinFlowTemp(void){
	queueMember newMember;
	newMember.message = LAUDA_GET_MIN_FLOW_TEMP_MSG;
	newMember.messageID = memberID::minFlowTempGet;
	pushMember(newMember);
}

void BrainPET7TCoolingUnitManager::setSafeModeTemp(float temp){
	queueMember newMember;
	std::stringstream buffer;
	buffer << std::fixed << std::setprecision(2) << temp;
	newMember.message = LAUDA_SET_SAFE_MODE_TEMP_MSG + buffer.str();;
	newMember.messageID = memberID::safeModeTempSet;
	newMember.data.floatValue = temp;
	pushMember(newMember);
}

void BrainPET7TCoolingUnitManager::getSafeModeTemp(void){
	queueMember newMember;
	newMember.message = LAUDA_GET_SAFE_MODE_TEMP_MSG;
	newMember.messageID = memberID::safeModeTempGet;
	pushMember(newMember);
}

void BrainPET7TCoolingUnitManager::setInterfaceTimeout(short time){

	queueMember newMember;
	std::stringstream buffer;
	buffer << std::fixed << std::setprecision(0) << time;
	newMember.message = LAUDA_SET_TIMEOUT_MSG + buffer.str();
	newMember.messageID = memberID::interfaceTimeoutSet;
	newMember.data.integerValue = time;
	pushMember(newMember);
}

void BrainPET7TCoolingUnitManager::getInterfaceTimeout(void){
	queueMember newMember;
	newMember.message = LAUDA_GET_TIMEOUT_MSG;
	newMember.messageID = memberID::interfaceTimeoutGet;
	pushMember(newMember);
}



void BrainPET7TCoolingUnitManager::start(void){
	queueMember newMember;
	newMember.message = LAUDA_STARTMSG;
	newMember.messageID = memberID::startSet;
	newMember.data.boolValue = true;
	pushMember(newMember);
}

void BrainPET7TCoolingUnitManager::stop(void){
	queueMember newMember;
	newMember.message = LAUDA_STOPMSG;
	newMember.messageID = memberID::stopSet;
	newMember.data.boolValue = false;
	pushMember(newMember);
}


void BrainPET7TCoolingUnitManager::getIntTemp(void){

	queueMember newMember;
	newMember.message = LAUDA_GET_INT_TMP_MSG;
	newMember.messageID = memberID::intTempGet;
	pushMember(newMember);

}

void BrainPET7TCoolingUnitManager::getControlledTemp(void){

	queueMember newMember;
	newMember.message = LAUDA_GET_CONTROLLED_TEMP_MSG;
	newMember.messageID = memberID::controlledTempGet;
	pushMember(newMember);
}

void BrainPET7TCoolingUnitManager::getBathFillLevel(void){

	queueMember newMember;
	newMember.message = LAUDA_GET_BATH_FILL_LEVEL_MSG;
	newMember.messageID = memberID::bathFillLevelGet;
	pushMember(newMember);
}

void BrainPET7TCoolingUnitManager::getOvertemperature(void){

	queueMember newMember;
	newMember.message = LAUDA_GET_OVERTEMPERATURE_SWITCH_OFF_POINT;
	newMember.messageID = memberID::overtemperatureGet;
	pushMember(newMember);
}

void BrainPET7TCoolingUnitManager::getControlSystemVersion(){

	queueMember newMember;
	newMember.message = LAUDA_VERSION_R;
	newMember.messageID = memberID::controlSystemVersionGet;
	pushMember(newMember);
}

void BrainPET7TCoolingUnitManager::getProtectionSystemVersion(){

	queueMember newMember;
	newMember.message = LAUDA_VERSION_S;
	newMember.messageID = memberID::protectionSystemVersionGet;
	pushMember(newMember);
}

void BrainPET7TCoolingUnitManager::getCommandRemoteControlVersion(){

	queueMember newMember;
	newMember.message = LAUDA_VERSION_B;
	newMember.messageID = memberID::commandRemoteControlVersionGet;
	pushMember(newMember);
}

void BrainPET7TCoolingUnitManager::getCoolingSystemVersion(){

	queueMember newMember;
	newMember.message = LAUDA_VERSION_T;
	newMember.messageID = memberID::coolingSystemVersionGet;
	pushMember(newMember);
}

void BrainPET7TCoolingUnitManager::getDeviceStatus(){

	queueMember newMember;
	newMember.message = LAUDA_STATUS;
	newMember.messageID = memberID::deviceStatusGet;
	pushMember(newMember);


}

void BrainPET7TCoolingUnitManager::getFaultReason(){

	queueMember newMember;
	newMember.message = LAUDA_STAT;
	newMember.messageID = memberID::faultReasonGet;
	pushMember(newMember);
}

std::string BrainPET7TCoolingUnitManager::faultDecode(){
	int status;
	std::string str;
	status = faultReason_.data.integerValue;
	for (u_short i = 0; i < faultNrToStr.size(); i++){
		if (status & (1 << i)){
			str += faultNrToStr.at(i);
		}

		if (str.empty()){
			str += LAUDA_NO_FAULTS;
		}
	}
	return str;
}


std::string BrainPET7TCoolingUnitManager::sendrecv(std::string msg){
	std::string msgrecv;
	int errnum = 0;
	msg += LAUDA_CRLF;

	laudaSocket.send_msg(msg);
	msgrecv = laudaSocket.recv_msg();

	if (msgrecv.find(LAUDA_ERR) != std::string::npos){
		msgrecv.erase(msgrecv.find(LAUDA_ERR),strlen(LAUDA_ERR));
		errnum = atoi(msgrecv.c_str());
		switch (errnum){
		case 2: throw BrainPET7TCoolingUnitException(BRAINPET7T_COOLINGUNIT_MANAGER_ENTRY_ERROR_STRING,
				BrainPET7TCoolingUnitException::CUError::WrongEntry);
		case 3: throw BrainPET7TCoolingUnitException(BRAINPET7T_COOLINGUNIT_MANAGER_SPELLING_ERROR_STRING,
				BrainPET7TCoolingUnitException::CUError::WrongCommandSpelling);
		case 5: throw BrainPET7TCoolingUnitException(BRAINPET7T_COOLINGUNIT_MANAGER_VALUE_SYNTAX_ERROR_STRING,
				BrainPET7TCoolingUnitException::CUError::SyntaxErrorInValue);
		case 6: throw BrainPET7TCoolingUnitException(BRAINPET7T_COOLINGUNIT_MANAGER_VALUE_RANGE_ERROR_STRING,
				BrainPET7TCoolingUnitException::CUError::ImperissibleValue);
		case 8: throw BrainPET7TCoolingUnitException(BRAINPET7T_COOLINGUNIT_MANAGER_MODULE_ERROR_STRING,
				BrainPET7TCoolingUnitException::CUError::ModuleOrValueNotAvailible);
		case 30: throw BrainPET7TCoolingUnitException(BRAINPET7T_COOLINGUNIT_MANAGER_OCCUPIED_ERROR_STRING,
				BrainPET7TCoolingUnitException::CUError::ProgrammerOqupied);
		case 31: throw BrainPET7TCoolingUnitException(BRAINPET7T_COOLINGUNIT_MANAGER_SETPOINT1_ERROR_STRING,
				BrainPET7TCoolingUnitException::CUError::SetpointNotSpecified1);
		case 33: throw BrainPET7TCoolingUnitException(BRAINPET7T_COOLINGUNIT_MANAGER_PROBE_ERROR_STRING,
				BrainPET7TCoolingUnitException::CUError::ProbeMissing);
		case 34: throw BrainPET7TCoolingUnitException(BRAINPET7T_COOLINGUNIT_MANAGER_VALUE_MISSING_ERROR_STRING,
				BrainPET7TCoolingUnitException::CUError::NoAnalogValue);
		case 35: throw BrainPET7TCoolingUnitException(BRAINPET7T_COOLINGUNIT_MANAGER_AUTOCONFIGURED_ERROR_STRING,
				BrainPET7TCoolingUnitException::CUError::AutoConfig);
		case 36: throw BrainPET7TCoolingUnitException(BRAINPET7T_COOLINGUNIT_MANAGER_SETPOINT2_ERROR_STRING,
				BrainPET7TCoolingUnitException::CUError::SetpointNotSpecified2);
		case 37: throw BrainPET7TCoolingUnitException(BRAINPET7T_COOLINGUNIT_MANAGER_PROGRAMMER_START_ERROR_STRING,
				BrainPET7TCoolingUnitException::CUError::ProgrammerNotStarted);
		case 38: throw BrainPET7TCoolingUnitException(BRAINPET7T_COOLINGUNIT_MANAGER_SAFE_MODE_ERROR_STRING,
				BrainPET7TCoolingUnitException::CUError::RestrictedInSafetyMode);
		default: throw BrainPET7TCoolingUnitException(BRAINPET7T_COOLINGUNIT_MANAGER_UNEXPECTED_MSG_ERROR_STRING,
				BrainPET7TCoolingUnitException::CUError::UnexpectedMessageReceived);

		}
	}

	if (msgrecv.empty()){
		throw BrainPET7TSocketCommunicationException(BRAINPET7T_SOCKET_COMMUNICATION_NOREPLY_ERROR_STRING,
				BrainPET7TSocketCommunicationException::SCError::NoReplyFromHost);
	}

	return msgrecv;
}

void BrainPET7TCoolingUnitManager::makeThread(){

	while(1){
		static queueMember frontMember;
		mutexUpstream_.lock();
		if(isData(queueUp_)){

			frontMember = queueUp_.front();		//copy of queue member
			queueUp_.pop();                      // delete front member from queue
			mutexUpstream_.unlock();

			mutexDownstream_.lock();
			switch(frontMember.messageID){
			case memberID::pumpLevelSet:
				checkIfSetterWorked(sendrecv(frontMember.message));
				pumpLevel_.data.integerValue = frontMember.data.integerValue;
				pumpLevel_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				break;
			case memberID::pumpLevelGet:
				setPumpLevelMemberFromGetter(sendrecv(frontMember.message));
				break;
			case memberID::setPointSet:
				checkIfSetterWorked(sendrecv(frontMember.message));
				setPoint_.data.floatValue = frontMember.data.floatValue;
				setPoint_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				break;
			case memberID::setPointGet:
				setSetPointMemberFromGetter(sendrecv(frontMember.message));
				break;
			case memberID::coolingModeSet:
				checkIfSetterWorked(sendrecv(frontMember.message));
				coolingMode_.data.integerValue = frontMember.data.integerValue;
				coolingMode_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				break;
			case memberID::coolingModeGet:
				setCoolingModeMemberFromGetter(sendrecv(frontMember.message));
				break;
			case memberID::maxFlowTempSet:
				checkIfSetterWorked(sendrecv(frontMember.message));
				maxFlowTemp_.data.floatValue = frontMember.data.floatValue;
				maxFlowTemp_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				break;
			case memberID::maxFlowTempGet:
				setMaxFlowTempMemberFromGetter(sendrecv(frontMember.message));
				break;
			case memberID::minFlowTempSet:
				checkIfSetterWorked(sendrecv(frontMember.message));
				minFlowTemp_.data.floatValue = frontMember.data.floatValue;
				minFlowTemp_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				break;
			case memberID::minFlowTempGet:
				setMinFlowTempMemberFromGetter(sendrecv(frontMember.message));
				break;
			case memberID::safeModeTempSet:
				checkIfSetterWorked(sendrecv(frontMember.message));
				safeModeTemp_.data.floatValue = frontMember.data.floatValue;
				safeModeTemp_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				break;
			case memberID::safeModeTempGet:
				setSafeModeTempMemberFromGetter(sendrecv(frontMember.message));
				break;
			case memberID::interfaceTimeoutSet:
				checkIfSetterWorked(sendrecv(frontMember.message));
				interfaceTimeout_.data.integerValue = frontMember.data.integerValue;
				interfaceTimeout_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				break;
			case memberID::interfaceTimeoutGet:
				setInterfaceTimeoutMemberFromGetter(sendrecv(frontMember.message));
				break;
			case memberID::startSet:
			case memberID::stopSet:
				checkIfSetterWorked(sendrecv(frontMember.message));
				mainState_.data.boolValue = frontMember.data.boolValue;
				mainState_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				break;
			case memberID::bathFillLevelGet:
				setBathFillLevelMemberFromGetter(sendrecv(frontMember.message));
				break;
			case memberID::intTempGet:
				setIntTempMemberFromGetter(sendrecv(frontMember.message));
				break;
			case memberID::controlledTempGet:
				setControlledTempMemberFromGetter(sendrecv(frontMember.message));
				break;
			case memberID::overtemperatureGet:
				setOvertemperatureMemberFromGetter(sendrecv(frontMember.message));
				break;
			case memberID::controlSystemVersionGet:
				setControlSystemVersionMemberFromGetter(sendrecv(frontMember.message));
				break;
			case memberID::protectionSystemVersionGet:
				setProtectionSystemVersionMemberFromGetter(sendrecv(frontMember.message));
				break;
			case memberID::commandRemoteControlVersionGet:
				setCommandRemoteControlVersionMemberFromGetter(sendrecv(frontMember.message));
				break;
			case memberID::coolingSystemVersionGet:
				setCoolingSystemVersionMemberFromGetter(sendrecv(frontMember.message));
				break;
			case memberID::deviceStatusGet:
				setDeviceStatusMemberFromGetter(sendrecv(frontMember.message));
				break;
			case memberID::faultReasonGet:
				setFaultReasonMemberFromGetter(sendrecv(frontMember.message));
				break;
			}
			mutexDownstream_.unlock();
		}else{
			mutexUpstream_.unlock();
		}

		if (future_.wait_for(std::chrono::seconds(0)) == std::future_status::ready){
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(LAUDA_THREAD_LOOP_TIME));
	}
}

void BrainPET7TCoolingUnitManager::checkIfSetterWorked(std::string str){

	if (str != LAUDA_OKMSG){
		throw BrainPET7TCoolingUnitException(BRAINPET7T_COOLINGUNIT_MANAGER_NOT_REOGNIZED_ERROR_STRING,
				BrainPET7TCoolingUnitException::CUError::ReceivedMessageNotRecognized);
	}
}

void BrainPET7TCoolingUnitManager::setPumpLevelMemberFromGetter(std::string str){
	pumpLevel_.data.integerValue = stoi (str);
	pumpLevel_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

void BrainPET7TCoolingUnitManager::setSetPointMemberFromGetter(std::string str){
	setPoint_.data.floatValue = strtof(str.c_str(),NULL);
	setPoint_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

void BrainPET7TCoolingUnitManager::setCoolingModeMemberFromGetter(std::string str){
	coolingMode_.data.integerValue = stoi (str);
	coolingMode_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}


void BrainPET7TCoolingUnitManager::setMaxFlowTempMemberFromGetter(std::string str){
	maxFlowTemp_.data.floatValue = strtof(str.c_str(),NULL);
	maxFlowTemp_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

void BrainPET7TCoolingUnitManager::setMinFlowTempMemberFromGetter(std::string str){
	minFlowTemp_.data.floatValue = strtof(str.c_str(),NULL);
	minFlowTemp_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

void BrainPET7TCoolingUnitManager::setSafeModeTempMemberFromGetter(std::string str){
	safeModeTemp_.data.floatValue = strtof(str.c_str(),NULL);
	safeModeTemp_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}


void BrainPET7TCoolingUnitManager::setInterfaceTimeoutMemberFromGetter(std::string str){
	interfaceTimeout_.data.integerValue = stoi (str);
	interfaceTimeout_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

void BrainPET7TCoolingUnitManager::setBathFillLevelMemberFromGetter(std::string str){
	bathFillLevel_.data.integerValue = stoi(str);
	bathFillLevel_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}
void BrainPET7TCoolingUnitManager::setIntTempMemberFromGetter(std::string str){
	intTemp_.data.floatValue = strtof(str.c_str(),NULL);
	intTemp_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}
void BrainPET7TCoolingUnitManager::setControlledTempMemberFromGetter(std::string str){
	controlledTemp_.data.floatValue = strtof(str.c_str(),NULL);
	controlledTemp_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}
void BrainPET7TCoolingUnitManager::setOvertemperatureMemberFromGetter(std::string str){
	overtemperature_.data.floatValue = strtof(str.c_str(),NULL);
	overtemperature_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}
void BrainPET7TCoolingUnitManager::setControlSystemVersionMemberFromGetter(std::string str){
	strcpy(controlSystemVersion_.data.stringValue, str.c_str());
	controlSystemVersion_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}
void BrainPET7TCoolingUnitManager::setProtectionSystemVersionMemberFromGetter(std::string str){
	strcpy(protectionSystemVersion_.data.stringValue, str.c_str());
	protectionSystemVersion_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}
void BrainPET7TCoolingUnitManager::setCommandRemoteControlVersionMemberFromGetter(std::string str){
	strcpy(commandRemoteControlVersion_.data.stringValue, str.c_str());
	commandRemoteControlVersion_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

void BrainPET7TCoolingUnitManager::setCoolingSystemVersionMemberFromGetter(std::string str){
	strcpy(coolingSystemVersion_.data.stringValue, str.c_str());
	coolingSystemVersion_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}
void BrainPET7TCoolingUnitManager::setDeviceStatusMemberFromGetter(std::string str){

	if (str.find(LAUDA_ZERO_MSG) != std::string::npos){
		deviceStatus_.data.boolValue = false;
		deviceStatus_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	}else if(str.find(LAUDA_ONE_MSG) != std::string::npos){
		deviceStatus_.data.boolValue = true;
		deviceStatus_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	}else{
		throw BrainPET7TCoolingUnitException(BRAINPET7T_COOLINGUNIT_MANAGER_UNEXPECTED_MSG_ERROR_STRING,
				BrainPET7TCoolingUnitException::CUError::UnexpectedMessageReceived);;
	}
}
void BrainPET7TCoolingUnitManager::setFaultReasonMemberFromGetter(std::string str){
	int status = 0;
	str.erase(str.find(LAUDA_CRLF),strlen(LAUDA_CRLF));
	for (u_short i = 0; i < str.length(); i++){
		if (str[i] == '1'){
			status = status | 1 << i;
		}
	}
	faultReason_.data.integerValue = status;
	faultReason_.lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}











