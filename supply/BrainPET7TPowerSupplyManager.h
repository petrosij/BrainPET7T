/**
 * \file BrainPET7TPowerSupplyManager.h (/MPOD_steering/src/BrainPET7TPowerSupplyManager.h)
 * \ingroup BrainPET7TSystem
 * \brief Class for MPOD power crate manipulating
 *
 * That class is made for MPOD power crate steering. Reading out voltages/currents from channels, setting voltages/currents on channels.
 * For using that class 4 additional settings should be done.
 * 1)"WIENER-CRATE-MIB" text file should lie in that directory: /home/homeGlobal/{username}/.snmp/mibs
 * \todo change default directory
 * 2)"netsnmp" softaware should be installed.
 * 3)Libraries <net-snmp/net-snmp-config.h> and <net-snmp/net-snmp-includes.h> should be installed
 * 4)In project settings netsnmp library should be linked
 * 5)Specify the CHANNEL_AMMOUNT constant as an actual that you have on the power crate.
 *
 * \date:   Created on: Nov 27, 2019
 * \author: Skudarnov Petr (pskudarnov)
 * \version: 0.0
 *
 * \warning Specify with CHANNEL_AMMOUNT the real number of channels that are available.
 *
 * \todo implement messages sending and answer receiving into a nonblocking thread with message queue
 * \todo prepare configuration class for power supply
 */
#ifndef BRAINPET7TPOWERSUPPLYMANAGER_H_
#define BRAINPET7TPOWERSUPPLYMANAGER_H_

/**
 * Default host name for the Wiener PS crate. Configured with IP address in the /etc/hosts file
 * To be overwritten by XML configuration
 * Empty String is treated as not configured and is precondition for the init method
 */
#define BRAINPET7T_POWERSUPPLY_MANAGER_PEERNAME ""
/**
 * Default access mode. Possible alternatives are (see <a href="Manual_MPOD_LV-HV_2.9.pdf" target="_blank"><b>MPOD Manual</b></a> page 23)
 * The following community groups are used:
 * “public”: for all read operations
 * “private”: to switch crate on or off
 * “admin”: to change parameters as fan speed or temperature limits
 * “guru”: to change HV and LV channel parameters as voltages, current limits, ramps, ...
 * Can be overwritten by XML configuration
 */
#define BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_ACCESS "guru"

#define BRAINPET7T_POWERSUPPLY_MANAGER_TAG_NAME	"WIENER-CRATE-MIB::outputName" //!< Wiener specific name tag, (see <a href="Manual_MPOD_LV-HV_2.9.pdf" target="_blank"><b>MPOD Manual</b></a>)
#define BRAINPET7T_POWERSUPPLY_MANAGER_TAG_SETVOLTAGE "WIENER-CRATE-MIB::outputVoltage." //!< Wiener specific name tag, (see <a href="Manual_MPOD_LV-HV_2.9.pdf" target="_blank"><b>MPOD Manual</b></a>)
#define BRAINPET7T_POWERSUPPLY_MANAGER_TAG_SETCURRENT "WIENER-CRATE-MIB::outputCurrent." //!< Wiener specific name tag, (see <a href="Manual_MPOD_LV-HV_2.9.pdf" target="_blank"><b>MPOD Manual</b></a>)
#define BRAINPET7T_POWERSUPPLY_MANAGER_TAG_SWITCH "WIENER-CRATE-MIB::outputSwitch." //!< Wiener specific name tag, (see <a href="Manual_MPOD_LV-HV_2.9.pdf" target="_blank"><b>MPOD Manual</b></a>)
#define BRAINPET7T_POWERSUPPLY_MANAGER_TAG_SENSEVOLTAGE "WIENER-CRATE-MIB::outputMeasurementSenseVoltage." //!< Wiener specific name tag, (see <a href="Manual_MPOD_LV-HV_2.9.pdf" target="_blank"><b>MPOD Manual</b></a>)
#define BRAINPET7T_POWERSUPPLY_MANAGER_TAG_TERMINALVOLTAGE 	"WIENER-CRATE-MIB::outputMeasurementTerminalVoltage." //!< Wiener specific name tag, (see <a href="Manual_MPOD_LV-HV_2.9.pdf" target="_blank"><b>MPOD Manual</b></a>)
#define BRAINPET7T_POWERSUPPLY_MANAGER_TAG_CURRENT 	"WIENER-CRATE-MIB::outputMeasurementCurrent." //!< Wiener specific name tag, (see <a href="Manual_MPOD_LV-HV_2.9.pdf" target="_blank"><b>MPOD Manual</b></a>)
#define BRAINPET7T_POWERSUPPLY_MANAGER_TAG_FALLRATE "WIENER-CRATE-MIB::outputVoltageFallRate." //!< Wiener specific name tag, (see <a href="Manual_MPOD_LV-HV_2.9.pdf" target="_blank"><b>MPOD Manual</b></a>)
#define BRAINPET7T_POWERSUPPLY_MANAGER_TAG_RISERATE "WIENER-CRATE-MIB::outputVoltageRiseRate." //!< Wiener specific name tag, (see <a href="Manual_MPOD_LV-HV_2.9.pdf" target="_blank"><b>MPOD Manual</b></a>)
#define BRAINPET7T_POWERSUPPLY_MANAGER_TAG_CONFIG "WIENER-CRATE-MIB::outputUserConfig." //!< Wiener specific name tag, (see <a href="Manual_MPOD_LV-HV_2.9.pdf" target="_blank"><b>MPOD Manual</b></a>)
#define BRAINPET7T_POWERSUPPLY_MANAGER_TAG_SUPERVISION	"WIENER-CRATE-MIB::outputSupervisionBehavior." //!< Wiener specific name tag, (see <a href="Manual_MPOD_LV-HV_2.9.pdf" target="_blank"><b>MPOD Manual</b></a>)
#define BRAINPET7T_POWERSUPPLY_MANAGER_TAG_MAXTERMINALVOLTAGE "WIENER-CRATE-MIB::outputSupervisionMaxTerminalVoltage." //!< Wiener specific name tag, (see <a href="Manual_MPOD_LV-HV_2.9.pdf" target="_blank"><b>MPOD Manual</b></a>)
#define BRAINPET7T_POWERSUPPLY_MANAGER_TAG_MAXSENSEVOLTAGE "WIENER-CRATE-MIB::outputSupervisionMaxSenseVoltage." //!< Wiener specific name tag, (see <a href="Manual_MPOD_LV-HV_2.9.pdf" target="_blank"><b>MPOD Manual</b></a>)
#define BRAINPET7T_POWERSUPPLY_MANAGER_TAG_MINSENSEVOLTAGE "WIENER-CRATE-MIB::outputSupervisionMinSenseVoltage." //!< Wiener specific name tag, (see <a href="Manual_MPOD_LV-HV_2.9.pdf" target="_blank"><b>MPOD Manual</b></a>)
#define BRAINPET7T_POWERSUPPLY_MANAGER_TAG_MAXCURRENT "WIENER-CRATE-MIB::outputSupervisionMaxCurrent." //!< Wiener specific name tag, (see <a href="Manual_MPOD_LV-HV_2.9.pdf" target="_blank"><b>MPOD Manual</b></a>)
#define BRAINPET7T_POWERSUPPLY_MANAGER_TAG_MAXTEMPERATURE "WIENER-CRATE-MIB::outputSupervisionMaxTemperature." //!< Wiener specific name tag, (see <a href="Manual_MPOD_LV-HV_2.9.pdf" target="_blank"><b>MPOD Manual</b></a>)
#define BRAINPET7T_POWERSUPPLY_MANAGER_TAG_MAINSWITCH "WIENER-CRATE-MIB::sysMainSwitch.0" //!< Wiener specific name tag, (see <a href="Manual_MPOD_LV-HV_2.9.pdf" target="_blank"><b>MPOD Manual</b></a>)


/** Wiener power supply general description
 *
 * Each number has its number and name. Number: 0..CHANNEL_NUM - 1
 * Names are defined by the power crate configurations. Each group
 * of channel has as many channels as presented in the module.
 * The enumeration of each group differs by the factor n*100.
 * "n" is the slot place in the power crate.
 * For instance u100..u107 u400..u407. That means two modules are
 * installed on the place 1 and 4 (zero place also possible) and each
 * module has 8 channels.
 * */
#define BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_CHANNEL_PER_CRATE 		16
#define BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_MSG_BUFLEN				256
#define BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_CHANEL_NAME_LENGTH		5
#define BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_OIDS_NUM 				15
#define BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_MAX_VOLTAGE_CHANGE_RATE 	500.0f
#define BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_MIN_VOLTAGE_CHANGE_RATE 	1.0f

#include <iostream>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>


/**
 * \struct channel BrainPET7TPowerSupplyManager.h /MPOD_steering/src/BrainPET7TPowerSupplyManager.h
 * \brief Stores information about a power crate channel
 */
typedef struct {
	int number;										//!< Personal number of a channel
	std::string name;								//!< Personal name of each channel (uX) X: 0...1999
	size_t len;										//!< Needed for netsnmp library functions
	oid oids[BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_OIDS_NUM][MAX_OID_LEN];				//!< Stores all oids (each oid give an access to channel parameters)
}channel;


/**
 * \class BrainPET7TPowerSupplyManager BrainPET7TPowerSupplyManager.h /MPOD_steering/src/BrainPET7TPowerSupplyManager.h
 * \brief Begin a SNMP session and communicate with power crate.
 *
 * \attention
 * The presence of the packages net-snmp and libsnmp-dev (on ubuntu systems) is required.
 *
 * \attention
 * The Wiener power supply MPOD series communicates via SNMP. Thus, installation of SNMP libraries is required. For correct function, the
 * file WIENER-CRATE-MIB.txt, must be available and readable.
 *
 * \attention
 * When useing MAC with mac ports net-snmp library, compilation with c++17 standard is not possible, because of some deprecated code.
 *
 * \attention
 * libsnmp function calls need to read the WIENER-CRATE-MIB.txt file. Since an path to the WIENER-CRATE-MIB.txt file cannot be specified, the file must be copied
 * to one of the default mib file directories (to get the whole list, run net-snmp-config --configure-options from shell and look for --with-mibdirs option).
 * See man net-snmp-config for more details.
 *
 * \remarks TCPIP_over_Ethernet.pdf
 * - See detailed information on power supply communication in <a href="Manual_MPOD_LV-HV_2.9.pdf" target="_blank"><b>MPOD Manual</b></a>
 * - See detailed information for alternative power supply steering software (Windows) <a href="Short_Instruction_to_update_Muhse_(rev_2010.02.08).pdf" target="_blank"><b>Mhuse Manual</b></a>
 * - See detailed information on TCP communication <a href="TCPIP_over_Ethernet.pdf" target="_blank"><b>TCP over Ethernet</b></a>
 *
 */
class BrainPET7TPowerSupplyManager {
private:

	/**
	 *
	 */
	enum ramp {rampON, rampOFF};
	/**
	 *
	 */
	enum mode {shortCable, mediumCable, longCable};
	/**
	 *
	 */
	enum behavior {ignore, channelOFF, groupOFF, crateOFF};
	/**
	 *
	 */
	enum oidName {
		VoltageOID, CurrentOID, SwitchOID, MeasuredSenseVoltageOID,
		MeasuredTerminalVoltageOID, MeasuredCurrentOID, VoltageRiseOID,
		VoltageFallOID, UserConfigOID, SupervisionOID, MinSenseOID,
		MaxSenseOID, MaxTerminalOID, MaxCurrentOID, MaxTemperatureOID
	};

	channel channels[BRAINPET7T_POWERSUPPLY_MANAGER_DEFAULT_CHANNEL_PER_CRATE];						//!< Array that represent all channels
	snmp_session session;									//!< A structure that holds information about who we're going to be talking to.
	snmp_session* sess_handle;								//!< Pointer on the same structure
	snmp_pdu* pdu;											//!< This structure will hold all of the information that we're going to send to the remote host.
	snmp_pdu* response;										//!< This structure will hold the information that is received.
	variable_list* vars;									//!< This will hold the variables that we want to manipulate via SNMP
	oid mainSwitch_oid[MAX_OID_LEN];						//!< Object identifier for main switch of the power crate
	size_t mainSwitch_len;
	int status;
	std::string crateHostName;
	std::string crateAcessMode;


	void setFloatMessage(int& chan, char* st, oidName OID, int& stat);		//!< Used to sent float set messages
	void setIntMessage(int& chan, char* st, oidName OID, int& stat);		//!< Used to sent integer set messages
	float getFloatMessage(int& chan, oidName OID);							//!< Used to sent float get message
	long getIntMessage(int& chan, oidName OID);								//!< Used to sent integer get message
	std::string get_var(const oid*, size_t, const netsnmp_variable_list *); //!< Used to print name of the channel found in mib file


public:

	///Default constructor. Install a connection with power crate and initalize all private members.
	BrainPET7TPowerSupplyManager();

	///Default destructor
	virtual ~BrainPET7TPowerSupplyManager();

	/**
	 * Initialization of the snmp session
	 * /pre hostname must be correctly configured!
	 */
	void session_init(void);

	/**
	 * \brief Set voltage on one channel to a specified value.
	 * Call each channel by its number (not name).
	 *
	 * \param channel number of a channel (not name)
	 * \param value a desired setting voltage value [V]
	 * \return 0 for success, 1 for error in package, 2 for unknown reason
	 */
	int setVoltage(int channel, float value);

	/**
	 * \brief Set current limit on one channel to a specified value.
	 * Call each channel by its number (not name).
	 *
	 * \param channel number of a channel (not name)
	 * \param value a desired setting current limit value [A]
	 * \return 0 for success, 1 for error in package, 2 for unknown reason
	 */
	int setCurrent(int channel, float value);

	/**
	 * \brief Return already set voltage value from channel.
	 * \param channel number of a channel (not name)
	 * \return a voltage, that is wanted on a channel [V]
	 * \warning set value and actual value on the channel can differ. For actual value, look for getMeasuredSenseVoltage() or getMeasuredTerminalVoltage() members
	 */
	float getVoltage(int channel);

	/**
	 * \brief Return already set current value from channel.
	 * \param channel number of a channel (not name)
	 * \return a current limit, that is wanted on a channel [A]
	 * \warning set value and actual value on the channel can differ. For actual value, look for getMeasuredCurrent() member
	 */
	float getCurrent(int channel);

	/**
	 * \brief Return measured sense voltage on a channel.
	 * \param channel number of a channel (not name)
	 * \return measured sense voltage [V]
	 */
	float getMeasuredSenseVoltage(int channel);

	/**
	 * \brief Return measured terminal voltage on a channel.
	 * \param channel number of a channel (not name)
	 * \return measured terminal voltage [V]
	 */
	float getMeasuredTerminalVoltage(int channel);

	/**
	 * \brief Return measured current on a channel.
	 * \param channel number of a channel (not name)
	 * \return measured Current on a channel [A]
	 */
	float getMeasuredCurrent(int channel);

	/**
	 * \brief Print information about one channel
	 * \param channel number of a channel (not name)
	 */
	void printCommand(int channel) { printCommand(channel, std::cout); };

	void printCommand(int, std::ostream&);

	void printChannelsINFO(std::ostream&);

	void printChannelsINFO(void) { printChannelsINFO(std::cout); };
	/**
	 * \brief Switch on/off a particular channel
	 * \param channel number of a channel (not name)
	 * \param state pass 0 to switch channel off, 1 to turn off
	 */
	void channelSwitch(int channel, int state);

	/**
	 * \brief Switch on a particular channel
	 * \param channel number of a channel (not name)
	 */
	void switchChannelON(int channel);

	/**
	 * \brief Switch off a particular channel
	 * \param channel number of a channel (not name)
	 */
	void switchChannelOFF(int channel);

	/// Switch all channels on
	void allChannelsON();

	/// Switch all channels off
	void allChannelsOFF();

	/**
	 * \brief Switch on/off a particular channel
	 * \param channel number of a channel (not name)
	 * \return state of the channel O(OFF)/1(ON)
	 */
	long getChannelState(int channel);

	/**
	 * \brief Set a voltage falling rate [V/s]
	 *
	 * Only integer number from range 1..500 can be set
	 * \param channel number of a channel (not name)
	 * \param value a desired falling rate [V/s]
	 * \return 0 for success, 1 for error in package, 2 for unknown reason
	 */
	long setVoltageFallRate(int, float);

	/**
	 * \brief Set a rising rate [V/s]
	 *
	 * Only integer number from range 1..500 can be set
	 * \param channel number of a channel (not name)
	 * \param value a desired rising rate [V/s]
	 * \return 0 for success, 1 for error in package, 2 for unknown reason
	 */
	long setVoltageRiseRate(int, float);

	/**
	 * \brief Return a falling rate [V/s]
	 * \param channel number of a channel (not name)
	 * \return falling voltage rate on a channel [V/s]
	 */
	float getVoltageFallRate(int);
	/**
	 * \brief Return a rising rate [V/s]
	 * \param channel number of a channel (not name)
	 * \return rising voltage rate on a channel [V/s]
	 */
	float getVoltageRiseRate(int);

	/**
	 * \brief Define configuration of the channel
	 * \param channel number of a channel (not name)
	 * \param m (short_cable <1[m] medium_cable 1..30[m] long_cable >30[m])
	 * \param r turns on/off ramping of the voltage (for the case of turning off of the channel itself)
	 * \warning doesn't influence on ramp for voltage change on the channels during working mode
	 */
	long outputUserConfig(int, mode, ramp);

	/**
	 * \brief Set a min sense voltage [V]
	 *
	 * If the measured sense voltage is below that value, the power supply
	 * performs an action defined by supervision action
	 * \param channel number of a channel (not name)
	 * \param value a desired min sense voltage [V]
	 * \return 0 for success, 1 for error in package, 2 for unknown reason
	 */
	long setMinSenseVoltage(int, float);

	/**
	 * \brief Set a max terminal voltage [V]
	 *
	 * If the measured terminal voltage is above that value, the power supply
	 * performs an action defined by supervision action
	 * \param channel number of a channel (not name)
	 * \param value a desired max terminal voltage [V]
	 * \return 0 for success, 1 for error in package, 2 for unknown reason
	 */
	long setMaxTerminalVoltage(int, float);

	/**
	 * \brief Set a max sense voltage [V]
	 *
	 * If the measured sense voltage is above that value, the power supply
	 * performs an action defined by supervision action
	 * \param channel number of a channel (not name)
	 * \param value a desired max sense voltage [V]
	 * \return 0 for success, 1 for error in package, 2 for unknown reason
	 */
	long setMaxSenseVoltage(int, float);

	/**
	 * \brief Set a max current [V]
	 *
	 * If the measured current is above that value, the power supply
	 * performs an action defined by supervision action
	 * \param channel number of a channel (not name)
	 * \param value a desired max current [V]
	 * \return 0 for success, 1 for error in package, 2 for unknown reason
	 */
	long setMaxCurerent(int, float);

	/**
	 * \brief Set a max Temperature [C]
	 *
	 * If the temperature is above that value, the power supply
	 * performs an action defined by supervision action
	 * \param channel number of a channel (not name)
	 * \param value a max Temperature [C]
	 * \return 0 for success, 1 for error in package, 2 for unknown reason
	 * \warning for temperature only integer values are available
	 */
	long setMaxTemerature(int, int);

	/**
		 * \brief Read an already set min sense voltage [V]
		 *
		 * If the sense voltage is below that value, the power supply
		 * performs an action defined by supervision action
		 * \param channel number of a channel (not name)
		 * \return already set min sense voltage [V]
		 */
	float getMinSenseVoltage(int);

	/**
		 * \brief Read an already set max sense voltage [V]
		 *
		 * If the sense voltage is above that value, the power supply
		 * performs an action defined by supervision action
		 * \param channel number of a channel (not name)
		 * \return already set max sense voltage [V]
		 */
	float getMaxSenseVoltage(int);

	/**
		 * \brief Read an already set max terminal voltage [V]
		 *
		 * If the terminal voltage is above that value, the power supply
		 * performs an action defined by supervision action
		 * \param channel number of a channel (not name)
		 * \return already set max terminal voltage [V]
		 */
	float getMaxTerminalVoltage(int);

	/**
		 * \brief Read an already set max Current [A]
		 *
		 * If the current is above that value, the power supply
		 * performs an action defined by supervision action
		 * \param channel number of a channel (not name)
		 * \return already set max Current [A]
		 */
	float getMaxCurrent(int);

	/**
		 * \brief Read an already set max Temperature [C]
		 *
		 * If the temperature is above that value, the power supply
		 * performs an action defined by supervision action
		 * \param channel number of a channel (not name)
		 * \return already set max Temperature [C]
		 * \warning for temperature only integer values are available
		 */
	long getMaxTemperature(int);


	/**
	 * \brief Set supervision behavior.
	 *
	 * Define an action for failure occurrence.
	 * Four possible behavior could be set (ignore, channelOFF, groupOFF, crateOFF).
	 * There are 8 possible failure that can occur.
	 * 1) Min sense voltage exceeding
	 * 2) Max sense voltage exceeding
	 * 3) Max terminal voltage exceeding
	 * 4) Max current exceeding
	 * 5) Max temperature exceeding
	 * 6) Max power exceeding
	 * 7) Inhibit error
	 * 8) Timeout error
	 * \param channel number of a channel (not name)
	 * \return 0 for success, 1 for error in package, 2 for unknown reason
	 */
	long setSupervisionBehavior(int, behavior, behavior, behavior, behavior, behavior, behavior, behavior, behavior);

};

#endif /* BRAINPET7TPOWERSUPPLYMANAGER_H_ */
