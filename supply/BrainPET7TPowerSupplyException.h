/**
 * \file BrainPET7TException.h (/BrainPET7TSystem/src/BrainPET7TException.h)
 * \ingroup BrainPET7TSystem
 * \brief
 *
 * \date:   Created on: Feb 4, 2020
 * \author: Christoph Lerche (clerche)
 * \version: 0.0
 *
 * \remarks 
 *
 * \todo TODO
 */
#ifndef BRAINPET7TPOWERSUPPLYEXCEPTION_H_
#define BRAINPET7TPOWERSUPPLYEXCEPTION_H_

#define BRAINPET7T_POWERSUPPLY_MANAGER_SESSION_ERROR_STRING "Unknown problem in SNMP session."
#define BRAINPET7T_POWERSUPPLY_MANAGER_SESSION_OPENING_ERROR_STRING "SNMP Session could not be opened."
#define BRAINPET7T_POWERSUPPLY_MANAGER_VOLTAGE_ERROR_STRING	"Illegal values. Allowed range for rate setting is 1..500 [V/s]"
#define BRAINPET7T_POWERSUPPLY_MANAGER_MEMORY_ALLOCATION_ERROR_STRING "Could not allocate memory"
#define BRAINPET7T_POWERSUPPLY_MANAGER_HOST_NOT_CONFIGURED_ERROR_STRING "Host name for power supply crate was not yet configured."

#include <stdexcept>
#include <iostream>
#include <string>
/**
 * \brief Required exceptions for the BrainPET7T runtime library
 * \remarks Example given in https://riptutorial.com/cplusplus/example/23640/custom-exception
 */
class BrainPET7TPowerSupplyException: virtual public std::runtime_error {
public:
	explicit BrainPET7TPowerSupplyException(const std::string& err_msg, long err_num):
	std::runtime_error(err_msg),
	errorNumber(err_num),
	errorMessage(err_msg)
	{}
	virtual ~BrainPET7TPowerSupplyException() throw () {}

	/**
	 * \enum PSError
	 * Values from -1 to -99 are reserved for
	 * Values from -100 to -999 are reserved for the communication protocol (BrainPET7TProtocolLibrary)
	 * Values from -1000 to -1999 are reserved for the BrainPETSystem library
	 * Values from -1100 to -1199 are reserved for the BrainPETSystem Power supply manager class
	 * Values from -1200 to -1299 are reserved for the BrainPETSystem Command stack classes
	 *
	 */

	enum PSError : long {
		UnknownSNMPSessionError = -1100,           //!< A needed temporary file cannot be opened
				SNMPSessionOpenError = -1101,        //!< Internal hit buffer not writable
				MemoryAllocationError = -1102,        //!< Internal hit buffer not writable
				IllegalRateSetting = -1103,        //!< Internal hit buffer not writable
				HostNotConfigured = -1104        //!< Internal hit buffer not writable
	};

	/**
	 * Getter for the error message
	 * @return returns pointer to a const * char
	 * The corresponding memory is in possession of the BrainPET7TPowerSupplyException object.
	 * Callers must not free the memory.
	 */
	virtual const char* what() const throw () {
		return errorMessage.c_str();
	}

	/**
	 * Getter for the error number
	 * 	 * @return returns errorNumber
	 */
	virtual long getErrorNumber() const throw() {
		return errorNumber;
	}

protected:

	long errorNumber;               ///< Error number (of typ long, since merged with internal SNMP errors which have type long)
	std::string errorMessage;      ///< Error message

private:

};

#endif /* BRAINPET7TPOWERSUPPLYEXCEPTION_H_ */
