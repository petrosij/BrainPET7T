#include "BrainPET7TPowerSupplyManager.h"

//#include <malloc/_malloc.h>
//#include <net-snmp/library/asn1.h>
//#include <net-snmp/library/default_store.h>
//#include <net-snmp/library/mib.h>
//#include <net-snmp/library/oid.h>
//#include <net-snmp/library/snmp.h>
//#include <net-snmp/library/snmp_api.h>
//#include <net-snmp/library/snmp_client.h>
//#include <net-snmp/library/tools.h>
//#include <net-snmp/mib_api.h>
//#include <net-snmp/output_api.h>
//#include <net-snmp/pdu_api.h>
//#include <net-snmp/session_api.h>
//#include <net-snmp/types.h>
//#include <net-snmp/varbind_api.h>
//#include <sys/_types/_u_char.h>
//#include <cstdio>
//#include <cstring>
#include <string>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <iostream>

#include "BrainPET7TPowerSupplyException.h"



BrainPET7TPowerSupplyManager::BrainPET7TPowerSupplyManager() {
	//initialize all variables

	mainSwitch_len = MAX_OID_LEN;
	sess_handle = NULL;
	vars = NULL;
	status = 0;
	response = NULL;
	pdu = NULL;
	crateHostName = BRAINPET7T_POWERSUPPLY_MANAGER_PEERNAME;
	crateAcessMode = BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_ACCESS;


	for (int i = 0; i < (BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_CHANNEL_PER_CRATE); i++) {
		channels[i].len = MAX_OID_LEN;
		channels[i].number = i;
		channels[i].name.clear();
	}

}

BrainPET7TPowerSupplyManager::~BrainPET7TPowerSupplyManager() {
	snmp_close(sess_handle);
}

void BrainPET7TPowerSupplyManager::session_init(void) {

	/// first check precondition, if the host name has been set (NOT DONE IN CONSTRUCTOR)

    if(crateHostName == "") {
    	throw BrainPET7TPowerSupplyException(BRAINPET7T_POWERSUPPLY_MANAGER_HOST_NOT_CONFIGURED_ERROR_STRING,BrainPET7TPowerSupplyException::PSError::HostNotConfigured);
    }

	// Initialize the SNMP library
	init_snmp("APC Check");
	snmp_sess_init(&session);

	//Initialize a "session" that defines who we're going to talk to
    netsnmp_ds_toggle_boolean(NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_QUICK_PRINT);
    netsnmp_ds_toggle_boolean(NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_PRINT_BARE_VALUE);
	session.version = SNMP_VERSION_2c;
	session.community = reinterpret_cast<unsigned char*>(const_cast<char*>(crateAcessMode.c_str()));
	session.community_len = (strlen(const_cast<const char*>(reinterpret_cast<char*>(session.community))));
	session.peername = reinterpret_cast<char*>(const_cast<char*>(crateHostName.c_str()));
	status = 0;
	//Open the session and check for proper opening
	sess_handle = snmp_open(&session);
	if (!sess_handle) {
	   	throw BrainPET7TPowerSupplyException(BRAINPET7T_POWERSUPPLY_MANAGER_SESSION_OPENING_ERROR_STRING,BrainPET7TPowerSupplyException::PSError::SNMPSessionOpenError);
	}

	//Find all active channels
    oid             name[MAX_OID_LEN];
    size_t          name_length;
    oid             root[MAX_OID_LEN];
    size_t          rootlen;
    oid             end_oid[MAX_OID_LEN];
    size_t          end_len = 0;
    int             running;
    int             numprinted = 0;
    rootlen = MAX_OID_LEN;
    read_objid(BRAINPET7T_POWERSUPPLY_MANAGER_TAG_NAME, root, &rootlen);

    /// till the end
    memmove(end_oid, root, rootlen*sizeof(oid));
    end_len = rootlen;
    end_oid[end_len-1]++;

    //get first object to start walk
	memmove(name, root, rootlen * sizeof(oid));
	name_length = rootlen;
	running = 1;

	while (running) {

		//create PDU and do the request
		pdu = snmp_pdu_create(SNMP_MSG_GETNEXT);
		snmp_add_null_var(pdu, name, name_length);
		snmp_synch_response(sess_handle, pdu, &response);

		// check resulting variables
		for (vars = response->variables; vars; vars = vars->next_variable) {
			if (snmp_oid_compare(end_oid, end_len,vars->name, vars->name_length) <= 0) {
				running = 0;
				continue;
			}

			channels[numprinted].name = get_var(vars->name,vars->name_length,vars);
			numprinted++;

			if ((vars->type != SNMP_ENDOFMIBVIEW) && (vars->type != SNMP_NOSUCHOBJECT) && (vars->type != SNMP_NOSUCHINSTANCE)) {
				memmove((char *) name, (char *) vars->name, vars->name_length * sizeof(oid));
				name_length = vars->name_length;
			}else{
				running = 0;
			}

		}
	}


	for (int i = 0; i < BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_CHANNEL_PER_CRATE; i++) {
	// making a string to read voltage oid
		std::string outputVoltageName = BRAINPET7T_POWERSUPPLY_MANAGER_TAG_SETVOLTAGE;
		outputVoltageName += channels[i].name;
		read_objid(outputVoltageName.c_str(),
			channels[i].oids[VoltageOID], &channels[i].len);//reading oid of voltage from MIB file
	// making a string to read current oid
		std::string outputCurrentName = BRAINPET7T_POWERSUPPLY_MANAGER_TAG_SETCURRENT;
		outputCurrentName += channels[i].name;
		read_objid(outputCurrentName.c_str(),
			channels[i].oids[CurrentOID], &channels[i].len);//reading oid of current from MIB file
	// making a string to read switch oid
		std::string outputSwitchName = BRAINPET7T_POWERSUPPLY_MANAGER_TAG_SWITCH;
		outputSwitchName += channels[i].name;
		read_objid(outputSwitchName.c_str(),
			channels[i].oids[SwitchOID], &channels[i].len);//reading oid of switch from MIB file
	// making a string to read measured sense voltage oid
		std::string outputMesurementSenseVoltageName = BRAINPET7T_POWERSUPPLY_MANAGER_TAG_SENSEVOLTAGE;
		outputMesurementSenseVoltageName += channels[i].name;
		read_objid(outputMesurementSenseVoltageName.c_str(),
			channels[i].oids[MeasuredSenseVoltageOID], &channels[i].len);//reading oid of measured sense voltage from MIB file
	// making a string to read measured terminal voltage oid
		std::string outputMesurementTerminalVoltageName = BRAINPET7T_POWERSUPPLY_MANAGER_TAG_TERMINALVOLTAGE;
		outputMesurementTerminalVoltageName += channels[i].name;
		read_objid(outputMesurementTerminalVoltageName.c_str(),
			channels[i].oids[MeasuredTerminalVoltageOID], &channels[i].len);//reading oid of measured sense voltage from MIB file
	// making a string to read measured current oid
		std::string outputMesurementCurrentName = BRAINPET7T_POWERSUPPLY_MANAGER_TAG_CURRENT;
		outputMesurementCurrentName += channels[i].name;
		read_objid(outputMesurementCurrentName.c_str(),
			channels[i].oids[MeasuredCurrentOID], &channels[i].len);//reading oid of measured sense voltage from MIB file
	// making a string to read voltage fall rate oid
		std::string outputVoltageFallRate = BRAINPET7T_POWERSUPPLY_MANAGER_TAG_FALLRATE;
		outputVoltageFallRate += channels[i].name;
		read_objid(outputVoltageFallRate.c_str(),
			channels[i].oids[VoltageFallOID], &channels[i].len);
	// making a string to read voltage rise rate oid
		std::string outputVoltageRiseRate = BRAINPET7T_POWERSUPPLY_MANAGER_TAG_RISERATE;
		outputVoltageRiseRate += channels[i].name;
		read_objid(outputVoltageRiseRate.c_str(),
			channels[i].oids[VoltageRiseOID], &channels[i].len);
	// making a string to read user config oid
		std::string outputUserConfig = BRAINPET7T_POWERSUPPLY_MANAGER_TAG_CONFIG;
		outputUserConfig += channels[i].name;
		read_objid(outputUserConfig.c_str(),
			channels[i].oids[UserConfigOID], &channels[i].len);
	// making a string to read supervision behavior oid
		std::string outputSupervisionBehavior = BRAINPET7T_POWERSUPPLY_MANAGER_TAG_SUPERVISION;
		outputSupervisionBehavior += channels[i].name;
		read_objid(outputSupervisionBehavior.c_str(),
			channels[i].oids[SupervisionOID], &channels[i].len);

	// making a string to read min sense voltage oid
		std::string outputMinSenseVoltage = BRAINPET7T_POWERSUPPLY_MANAGER_TAG_MINSENSEVOLTAGE;
		outputMinSenseVoltage += channels[i].name;
		read_objid(outputMinSenseVoltage.c_str(),
			channels[i].oids[MinSenseOID], &channels[i].len);
	// making a string to read max sense voltage oid
		std::string outputMaxSenseVoltage = BRAINPET7T_POWERSUPPLY_MANAGER_TAG_MAXSENSEVOLTAGE;
		outputMaxSenseVoltage += channels[i].name;
		read_objid(outputMaxSenseVoltage.c_str(),
			channels[i].oids[MaxSenseOID], &channels[i].len);
	// making a string to read max terminal voltage oid
		std::string outputMaxTerminalVoltage = BRAINPET7T_POWERSUPPLY_MANAGER_TAG_MAXTERMINALVOLTAGE;
		outputMaxTerminalVoltage += channels[i].name;
		read_objid(outputMaxTerminalVoltage.c_str(),
			channels[i].oids[MaxTerminalOID], &channels[i].len);
	// making a string to read max current oid
		std::string outputMaxCurrent = BRAINPET7T_POWERSUPPLY_MANAGER_TAG_MAXCURRENT;
		outputMaxCurrent += channels[i].name;
		read_objid(outputMaxCurrent.c_str(),
			channels[i].oids[MaxCurrentOID], &channels[i].len);
	// making string to read max temperature oid
		std::string outputMaxTemperature = BRAINPET7T_POWERSUPPLY_MANAGER_TAG_MAXTEMPERATURE;
		outputMaxTemperature += channels[i].name;
		read_objid(outputMaxTemperature.c_str(),
			channels[i].oids[MaxTemperatureOID], &channels[i].len);

		//Check the power crate Main Switch state
		pdu = snmp_pdu_create(SNMP_MSG_GET);
		read_objid(BRAINPET7T_POWERSUPPLY_MANAGER_TAG_MAINSWITCH, mainSwitch_oid, &mainSwitch_len);
		snmp_add_null_var(pdu, mainSwitch_oid, mainSwitch_len);
		status = snmp_synch_response(sess_handle, pdu, &response);

		if (status == STAT_SUCCESS && !(response->errstat == SNMP_ERR_NOERROR)) {
//			throw snmp_errstring(static_cast<int>(response->errstat));
			throw BrainPET7TPowerSupplyException(snmp_errstring(static_cast<int>(response->errstat)),response->errstat);
		}
		if (!(status == STAT_SUCCESS)) {
		   	throw BrainPET7TPowerSupplyException(BRAINPET7T_POWERSUPPLY_MANAGER_SESSION_ERROR_STRING,BrainPET7TPowerSupplyException::PSError::UnknownSNMPSessionError);
		}
		snmp_free_pdu(response);
	}
}


int BrainPET7TPowerSupplyManager::setVoltage(int channel, float value) {
	status = 0;
	setFloatMessage(channel, const_cast<char*>((std::to_string(value)).c_str()), VoltageOID, status);
	return status;
}

float BrainPET7TPowerSupplyManager::getVoltage(int channel) {
	return getFloatMessage(channel, VoltageOID);
}

int BrainPET7TPowerSupplyManager::setCurrent(int channel, float value) {
	status = 0;
	setFloatMessage(channel, const_cast<char*>((std::to_string(value)).c_str()), CurrentOID, status);
	return status;
}

float BrainPET7TPowerSupplyManager::getCurrent(int channel) {
	return getFloatMessage(channel, CurrentOID);
}


void BrainPET7TPowerSupplyManager::channelSwitch(int channel, int state) {
	setIntMessage(channel, const_cast<char*>((std::to_string(state)).c_str()), SwitchOID, status);
}

float BrainPET7TPowerSupplyManager::getMeasuredCurrent(int channel) {
	return getFloatMessage(channel, MeasuredCurrentOID);
}

float BrainPET7TPowerSupplyManager::getMeasuredSenseVoltage(int channel) {
	return getFloatMessage(channel, MeasuredSenseVoltageOID);
}

float BrainPET7TPowerSupplyManager::getMeasuredTerminalVoltage(int channel) {
	return getFloatMessage(channel, MeasuredTerminalVoltageOID);
}

long BrainPET7TPowerSupplyManager::getChannelState(int channel) {
	return getIntMessage(channel, SwitchOID);
}

void BrainPET7TPowerSupplyManager::allChannelsON() {
	for (int i = 0; i < BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_CHANNEL_PER_CRATE; i++) {
		switchChannelON(i);
	}
}

void BrainPET7TPowerSupplyManager::allChannelsOFF() {
	for (int i = 0; i < BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_CHANNEL_PER_CRATE; i++) {
		switchChannelOFF(i);
	}
}

void BrainPET7TPowerSupplyManager::switchChannelON(int channel) {
	channelSwitch(channel, 1);
}

void BrainPET7TPowerSupplyManager::switchChannelOFF(int channel) {
	channelSwitch(channel, 0);
}

float BrainPET7TPowerSupplyManager::getFloatMessage(int& chan, oidName OID) {
	pdu = snmp_pdu_create(SNMP_MSG_GET);
	snmp_add_null_var(pdu, channels[chan].oids[OID], channels[chan].len);
	snmp_synch_response(sess_handle, pdu, &response);
	snmp_free_pdu(response);
	return *((response->variables)->val.floatVal);
}



void BrainPET7TPowerSupplyManager::setFloatMessage(int& chan, char* st, oidName OID, int& stat) {
	pdu = snmp_pdu_create(SNMP_MSG_SET);
	snmp_add_var(pdu, channels[chan].oids[OID], channels[chan].len, 'F', st);
	stat = snmp_synch_response(sess_handle, pdu, &response);
	if (stat == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {
		stat = 0; // return 0 for success
	} else {
		if (stat == STAT_SUCCESS) {
			throw snmp_errstring(static_cast<int>(response->errstat));
			stat = 1; /// return 1 for error in packet
		} else {
			stat = 2;/// return 2 for unknown reason
		   	throw BrainPET7TPowerSupplyException(BRAINPET7T_POWERSUPPLY_MANAGER_SESSION_ERROR_STRING,BrainPET7TPowerSupplyException::PSError::UnknownSNMPSessionError);
		}
	}
	snmp_free_pdu(response);
}

void BrainPET7TPowerSupplyManager::setIntMessage(int& chan, char* st, oidName OID, int& stat) {
	pdu = snmp_pdu_create(SNMP_MSG_SET);
	snmp_add_var(pdu, channels[chan].oids[OID], channels[chan].len, 'i', st);
	stat = snmp_synch_response(sess_handle, pdu, &response);
	if (stat == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {
		stat = 0; /// return 0 for success
	} else {
		if (stat == STAT_SUCCESS) {
			throw snmp_errstring(static_cast<int>(response->errstat));
			stat = 1; /// return 1 for error in packet
		} else {
			stat = 2;/// return 2 for unknown reason
		   	throw BrainPET7TPowerSupplyException(BRAINPET7T_POWERSUPPLY_MANAGER_SESSION_ERROR_STRING,BrainPET7TPowerSupplyException::PSError::UnknownSNMPSessionError);
		}
	}
	snmp_free_pdu(response);
}

long BrainPET7TPowerSupplyManager::getIntMessage(int& chan, oidName OID) {
	pdu = snmp_pdu_create(SNMP_MSG_GET);
	snmp_add_null_var(pdu, channels[chan].oids[OID], channels[chan].len);
	snmp_synch_response(sess_handle, pdu, &response);
	snmp_free_pdu(response);
	return *((response->variables)->val.integer);
}

std::string BrainPET7TPowerSupplyManager::get_var(const oid* objid,
		size_t objidlen, const netsnmp_variable_list * variable){
	u_char* buf = NULL;
	size_t buf_len = 256,out_len = 0;
	char new_buf[BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_MSG_BUFLEN]; 	/// To match the format to convert to string class (u_char* does not work properly)
	std::string str;		// String class

	if ((buf = (u_char*) calloc(buf_len,1)) == NULL){
	   	throw BrainPET7TPowerSupplyException(BRAINPET7T_POWERSUPPLY_MANAGER_MEMORY_ALLOCATION_ERROR_STRING,BrainPET7TPowerSupplyException::PSError::MemoryAllocationError);
	}else{
		if(sprint_realloc_variable(&buf, &buf_len,
				&out_len, 1,objid,objidlen,variable)){
			sprintf(new_buf,"%s",buf);
			new_buf[0] = 'u';
			str = new_buf;
		}else{
		   	throw BrainPET7TPowerSupplyException(BRAINPET7T_POWERSUPPLY_MANAGER_MEMORY_ALLOCATION_ERROR_STRING,BrainPET7TPowerSupplyException::PSError::MemoryAllocationError);
		}
	}

	SNMP_FREE(buf);
	return str;
}

void BrainPET7TPowerSupplyManager::printCommand(int channel, std::ostream& os) {
	os << " channel " << channel << ':' << std::endl;
	os << "\t name: " << channels[channel].name << std::endl;
	os << "\t state: " << getChannelState(channel) << " [off(0)/on(1)]" << std::endl;
	os << "\t set voltage: " << getVoltage(channel) << 'V' << std::endl;
	os << "\t measured sense voltage: " << getMeasuredSenseVoltage(channel) << 'V' << std::endl;
	os << "\t measured terminal voltage: " << getMeasuredTerminalVoltage(channel) << 'V' << std::endl;
	os << "\t set current: " << getCurrent(channel) << 'A' << std::endl;
	os << "\t measured current: " << getMeasuredCurrent(channel) << 'A' << std::endl;
	os << "\t voltage fall rate: " << getVoltageFallRate(channel) << "V/s" << std::endl;
	os << "\t voltage rise rate: " << getVoltageRiseRate(channel) << 'A' << std::endl;
	os << "\t Minimum sense voltage: " << getMinSenseVoltage(channel) << 'V' << std::endl;
	os << "\t Maximum sense voltage: " << getMaxSenseVoltage(channel) << 'V' << std::endl;
	os << "\t Maximum terminal voltage: " << getMaxTerminalVoltage(channel) << 'V' << std::endl;
	os << "\t Maximum current: " << getMaxCurrent(channel) << 'A' << std::endl;
	os << "\t Maximum temperature: " << getMaxTemperature(channel) << " degrees" << std::endl;

}

void BrainPET7TPowerSupplyManager::printChannelsINFO(std::ostream& os) {
	for (int i = 0; i < BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_CHANNEL_PER_CRATE; i++) {
		printCommand(i, os);
	}
}

long BrainPET7TPowerSupplyManager::setVoltageFallRate(int channel, float value) {
	status = 0;
	if (value < BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_MIN_VOLTAGE_CHANGE_RATE) {
		value = BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_MIN_VOLTAGE_CHANGE_RATE;
	   	throw BrainPET7TPowerSupplyException(BRAINPET7T_POWERSUPPLY_MANAGER_VOLTAGE_ERROR_STRING,BrainPET7TPowerSupplyException::PSError::IllegalRateSetting);
	} else if (value > BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_MAX_VOLTAGE_CHANGE_RATE) {
		value = BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_MAX_VOLTAGE_CHANGE_RATE;
	   	throw BrainPET7TPowerSupplyException(BRAINPET7T_POWERSUPPLY_MANAGER_VOLTAGE_ERROR_STRING,BrainPET7TPowerSupplyException::PSError::IllegalRateSetting);
	}
	setFloatMessage(channel, const_cast<char*>((std::to_string(value)).c_str()), VoltageFallOID, status);
	return status;
}

long BrainPET7TPowerSupplyManager::setVoltageRiseRate(int channel, float value) {
	status = 0;

	if (value < BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_MIN_VOLTAGE_CHANGE_RATE) {
		value = BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_MIN_VOLTAGE_CHANGE_RATE;
	   	throw BrainPET7TPowerSupplyException(BRAINPET7T_POWERSUPPLY_MANAGER_VOLTAGE_ERROR_STRING,BrainPET7TPowerSupplyException::PSError::IllegalRateSetting);
	}
	else if (value > BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_MAX_VOLTAGE_CHANGE_RATE) {
		value = BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_MAX_VOLTAGE_CHANGE_RATE;
	   	throw BrainPET7TPowerSupplyException(BRAINPET7T_POWERSUPPLY_MANAGER_VOLTAGE_ERROR_STRING,BrainPET7TPowerSupplyException::PSError::IllegalRateSetting);
	}

	setFloatMessage(channel,  const_cast<char*>((std::to_string(value)).c_str()), VoltageRiseOID, status);
	return status;
}

float BrainPET7TPowerSupplyManager::getVoltageFallRate(int channel) {
	return getFloatMessage(channel, VoltageFallOID);
}

float BrainPET7TPowerSupplyManager::getVoltageRiseRate(int channel) {
	return getFloatMessage(channel, VoltageRiseOID);
}

long BrainPET7TPowerSupplyManager::outputUserConfig(int channel, mode m, ramp r) {
	status = 0;
	int message = 0x0;
	message |= (m << 1) | (r << 0);
	setIntMessage(channel, const_cast<char*>((std::to_string(message)).c_str()), UserConfigOID, status);
	return status;
}

long BrainPET7TPowerSupplyManager::setMaxTerminalVoltage(int channel, float value) {
	status = 0;
	setFloatMessage(channel, const_cast<char*>((std::to_string(value)).c_str()), MaxTerminalOID, status);
	return status;
}

long BrainPET7TPowerSupplyManager::setMaxSenseVoltage(int channel, float value) {
	status = 0;
	setFloatMessage(channel, const_cast<char*>((std::to_string(value)).c_str()), MaxSenseOID, status);
	return status;
}

long BrainPET7TPowerSupplyManager::setMinSenseVoltage(int channel, float value) {
	status = 0;
	setFloatMessage(channel, const_cast<char*>((std::to_string(value)).c_str()), MinSenseOID, status);
	return status;
}

long BrainPET7TPowerSupplyManager::setMaxCurerent(int channel, float value) {
	status = 0;
	setFloatMessage(channel, const_cast<char*>((std::to_string(value)).c_str()), MaxCurrentOID, status);
	return status;
}

long BrainPET7TPowerSupplyManager::setMaxTemerature(int channel, int value) {
	status = 0;
	setIntMessage(channel, const_cast<char*>((std::to_string(value)).c_str()), MaxTemperatureOID, status);
	return status;
}

float BrainPET7TPowerSupplyManager::getMinSenseVoltage(int channel){
	return getFloatMessage(channel, MinSenseOID);
}
float BrainPET7TPowerSupplyManager::getMaxSenseVoltage(int channel){
	return getFloatMessage(channel, MaxSenseOID);
}
float BrainPET7TPowerSupplyManager::getMaxTerminalVoltage(int channel){
	return getFloatMessage(channel, MaxTerminalOID);
}
float BrainPET7TPowerSupplyManager::getMaxCurrent(int channel){
	return getFloatMessage(channel, MaxCurrentOID);
}
long BrainPET7TPowerSupplyManager::getMaxTemperature(int channel){
	return getIntMessage(channel, MaxTemperatureOID);
}

long BrainPET7TPowerSupplyManager::setSupervisionBehavior(int channel, behavior minSenseVoltageBehavior, behavior maxSenseVoltageBehavior,
	behavior maxTerminalVoltageBehavior, behavior maxCurrentBehavior, behavior maxTemperatureBehavior, behavior maxPowerBehavior,
	behavior inhibitBehavior, behavior timeoutBehavior) {

	int message = 0x0;
	status = 0;
	message |= (minSenseVoltageBehavior << 0) | (maxSenseVoltageBehavior << 2) | (maxTerminalVoltageBehavior << 4) |
		(maxCurrentBehavior << 6) | (maxTemperatureBehavior << 8) | (maxPowerBehavior << 10) | (inhibitBehavior << 12) |
		(timeoutBehavior << 14);
	setIntMessage(channel, const_cast<char*>((std::to_string(message)).c_str()), SupervisionOID, status);
	return status;
}


