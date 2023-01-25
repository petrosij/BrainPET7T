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
#include "BrainPET7TCoolingUnitGlobal.h"	//for errors
#include "BrainPET7TSocketGlobal.h"

#include <iomanip>			//setprecision()
#include <sstream>			//stringstream
#include <string>			//string
#include <stdlib.h>			//atoi(),strtof(), exit()


BrainPET7TCoolingUnitManager::BrainPET7TCoolingUnitManager() {
	// TODO Auto-generated constructor stub
}

BrainPET7TCoolingUnitManager::~BrainPET7TCoolingUnitManager() {
	// TODO Auto-generated destructor stub

}


bool BrainPET7TCoolingUnitManager::setTemp(float temperature){

	std::stringstream buffer;
	buffer << std::fixed << std::setprecision(2) << temperature;
	std::string msg = LAUDA_SET_TMP_MSG + buffer.str();
	std::string msgrecv;

	if ((msgrecv = sendrecv(msg)) == LAUDA_OKMSG){
		return true;
	}else{
		return false;
	}
}

bool BrainPET7TCoolingUnitManager::setPumpLevel(int pump_level){

	std::stringstream buffer;
	buffer << std::fixed << std::setprecision(0) << pump_level;
	std::string msg = LAUDA_SET_PUMP_LEVEL_MSG + buffer.str();
	std::string msgrecv;

	if ((msgrecv = sendrecv(msg)) == LAUDA_OKMSG){
		return true;
	}else{
		return false;
	}
}

bool BrainPET7TCoolingUnitManager::setCoolingMode(short mode){

	std::stringstream buffer;
	buffer << std::fixed << std::setprecision(0) << mode;
	std::string msg = LAUDA_SET_COOL_MODE_MSG + buffer.str();
	std::string msgrecv;

	if ((msgrecv = sendrecv(msg)) == LAUDA_OKMSG){
		return true;
	}else{
		return false;
	}
}

bool BrainPET7TCoolingUnitManager::setMaxFlowTemp(float temp){

	std::stringstream buffer;
	buffer << std::fixed << std::setprecision(2) << temp;
	std::string msg = LAUDA_SET_MAX_FLOW_TEMP_MSG + buffer.str();
	std::string msgrecv;

	if ((msgrecv = sendrecv(msg)) == LAUDA_OKMSG){
		return true;
	}else{
		return false;
	}
}

bool BrainPET7TCoolingUnitManager::setMinFlowTemp(float temp){

	std::stringstream buffer;
	buffer << std::fixed << std::setprecision(2) << temp;
	std::string msg = LAUDA_SET_MIN_FLOW_TEMP_MSG + buffer.str();
	std::string msgrecv;

	if ((msgrecv = sendrecv(msg)) == LAUDA_OKMSG){
		return true;
	}else{
		return false;
	}
}

bool BrainPET7TCoolingUnitManager::setSafeModeTemp(float temp){

	std::stringstream buffer;
	buffer << std::fixed << std::setprecision(2) << temp;
	std::string msg = LAUDA_SET_SAFE_MODE_TEMP_MSG + buffer.str();
	std::string msgrecv;

	if ((msgrecv = sendrecv(msg)) == LAUDA_OKMSG){
		return true;
	}else{
		return false;
	}
}

bool BrainPET7TCoolingUnitManager::setInterfaceTimeout(short time){

	std::stringstream buffer;
	buffer << std::fixed << std::setprecision(0) << time;
	std::string msg = LAUDA_SET_TIMEOUT_MSG + buffer.str();
	std::string msgrecv;

	if ((msgrecv = sendrecv(msg)) == LAUDA_OKMSG){
		return true;
	}else{
		return false;
	}
}

bool BrainPET7TCoolingUnitManager::targetValueOffsetSource(short state){

	std::stringstream buffer;
	buffer << std::fixed << std::setprecision(0) << state;
	std::string msg = LAUDA_SET_TARGET_VALUE_OFFSET_SOURCE_MSG + buffer.str();
	std::string msgrecv;

	if ((msgrecv = sendrecv(msg)) == LAUDA_OKMSG){
		return true;
	}else{
		return false;
	}
}

bool BrainPET7TCoolingUnitManager::safetyModeSwitch(bool state){

	std::stringstream buffer;
	buffer << std::fixed << std::setprecision(0) << state;
	std::string msg = LAUDA_SET_SAFETY_MODE_MSG + buffer.str();
	std::string msgrecv;

	if ((msgrecv = sendrecv(msg)) == LAUDA_OKMSG){
		return true;
	}else{
		return false;
	}
}

bool BrainPET7TCoolingUnitManager::start(void){
	std::string msgrecv;
	if ((msgrecv = sendrecv(LAUDA_STARTMSG)) == LAUDA_OKMSG){
		return true;
	}else{
		return false;
	}

}

bool BrainPET7TCoolingUnitManager::stop(void){
	std::string msgrecv;
	if ((msgrecv = sendrecv(LAUDA_STOPMSG)) == LAUDA_OKMSG){
		return true;
	}else{
		return false;
	}
}

bool BrainPET7TCoolingUnitManager::programSelect(short prog_num){

	std::stringstream buffer;
	buffer << std::fixed << std::setprecision(0) << prog_num;
	std::string msg = LAUDA_SET_PROGRAM_MSG + buffer.str();
	std::string msgrecv;

	if ((msgrecv = sendrecv(msg)) == LAUDA_OKMSG){
		return true;
	}else{
		return false;
	}
}

float BrainPET7TCoolingUnitManager::getIntTemp(void){
	return strtof(sendrecv(LAUDA_GET_INT_TMP_MSG).c_str(),NULL);
}

int BrainPET7TCoolingUnitManager::getPumpLevel(void){
	return stoi (sendrecv(LAUDA_GET_PUMP_LEVEL_MSG));
}

float BrainPET7TCoolingUnitManager::getControlledTemp(void){
	return strtof(sendrecv(LAUDA_GET_CONTROLLED_TEMP_MSG).c_str(),NULL);
}

int BrainPET7TCoolingUnitManager::getBathFillLevel(void){
	return stoi (sendrecv(LAUDA_GET_BATH_FILL_LEVEL_MSG));
}

float BrainPET7TCoolingUnitManager::getPreciseIntTemp(void){
	return strtof(sendrecv(LAUDA_GET_PRECISE_INT_TMP_MSG).c_str(),NULL);
}

float BrainPET7TCoolingUnitManager::getSetPoint(void){
	return strtof(sendrecv(LAUDA_GET_SETPOINT_MSG).c_str(),NULL);
}

int BrainPET7TCoolingUnitManager::getCoolingMode(void){
	return stoi (sendrecv(LAUDA_GET_COOLING_MODE_MSG));
}

float BrainPET7TCoolingUnitManager::getOvertemperature(void){
	return strtof(sendrecv(LAUDA_GET_OVERTEMPERATURE_SWITCH_OFF_POINT).c_str(),NULL);
}

float BrainPET7TCoolingUnitManager::getMaxFloatTemp(void){
	return strtof(sendrecv(LAUDA_GET_MAX_FLOW_TEMP_MSG).c_str(),NULL);
}

float BrainPET7TCoolingUnitManager::getMinFloatTemp(void){
	return strtof(sendrecv(LAUDA_GET_MIN_FLOW_TEMP_MSG).c_str(),NULL);
}

float BrainPET7TCoolingUnitManager::getSafeModeTemp(void){
	return strtof(sendrecv(LAUDA_GET_SAFE_MODE_TEMP_MSG).c_str(),NULL);
}

int BrainPET7TCoolingUnitManager::getInterfaceTimeout(void){
	return stoi (sendrecv(LAUDA_GET_TIMEOUT_MSG));
}

std::string BrainPET7TCoolingUnitManager::controlSystemVersion(){
	return sendrecv(LAUDA_VERSION_R);
}

std::string BrainPET7TCoolingUnitManager::protectionSystemVersion(){
	return sendrecv(LAUDA_VERSION_S);
}

std::string BrainPET7TCoolingUnitManager::commandRemoteControlVersion(){
	return sendrecv(LAUDA_VERSION_B);
}

std::string BrainPET7TCoolingUnitManager::coolingSystemVersion(){
	return sendrecv(LAUDA_VERSION_T);
}

bool BrainPET7TCoolingUnitManager::deviceStatus(){
	std::string msgrecv;
	msgrecv = sendrecv(LAUDA_STATUS);
	if (msgrecv.find(LAUDA_ZERO_MSG) != std::string::npos){
		return true;
	}else if(msgrecv.find(LAUDA_ONE_MSG) != std::string::npos){
		return false;
	}else{
		throw SOCKET_ERRUNEXPECT;
	}
}

std::string BrainPET7TCoolingUnitManager::faultDiagnosis(){
	std::string msgrecv,msgreturn;
	msgrecv = sendrecv(LAUDA_STAT);
	msgrecv.erase(msgrecv.find(LAUDA_CRLF),strlen(LAUDA_CRLF));
	for (uint i = 0; i < msgrecv.length(); i++){
		if (msgrecv[i] == '1'){
			switch (i){
			case 0: msgreturn += "error\n";
					break;
			case 1: msgreturn += "alarm\n";
					break;
			case 2: msgreturn += "warning\n";
					break;
			case 3: msgreturn += "overtemperature\n";
					break;
			case 4: msgreturn += "sublevel\n";
					break;
			case 5: msgreturn += "higher level (alarm setting)\n";
					break;
			case 6: msgreturn += "external control value missing\n";
					break;
			}
		}else
			continue;

	}

	return msgrecv + '\n' + msgreturn;

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
		case 2: throw LAUDA_ERRENTRY;
		case 3: throw LAUDA_ERRSPEL;
		case 5: throw LAUDA_ERRVAL1;
		case 6: throw LAUDA_ERRVAL2;
		case 8: throw LAUDA_ERRMODULE;
		case 30: throw LAUDA_ERROCCUP;
		case 31: throw LAUDA_ERRSETPOINT1;
		case 33: throw LAUDA_ERRTEMPMIS;
		case 34: throw LAUDA_ERRANALOGVAL;
		case 35: throw LAUDA_ERRAUTO;
		case 36: throw LAUDA_ERRSETPOINT2;
		case 37: throw LAUDA_ERRSETPOINT3;
		case 38: throw LAUDA_ERRSAVE;
		default: throw SOCKET_ERRRECV;
		}
	}

	if (msgrecv.empty()){
		throw SOCKET_ERRNOREPLY;
	}

	return msgrecv;
}





















