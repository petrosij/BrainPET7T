//============================================================================
// Name        : snmp_test2.cpp
// Author      : Petr Skudarnov
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <chrono>
#include <unistd.h>
#include <sys/time.h>
using namespace std;

int main(int argc, char ** argv)
{
	struct snmp_session session;
	struct snmp_session *sess_handle;
	struct snmp_pdu *pdu;
	struct snmp_pdu *response;
	oid id_oid[MAX_OID_LEN];
	size_t id_len = MAX_OID_LEN;
	init_snmp("APC Check");
	snmp_sess_init( &session );
	session.version = SNMP_VERSION_2c;
	session.community = reinterpret_cast<unsigned char*>(const_cast<char*>("guru"));
	session.community_len = (strlen(const_cast<const char*>(reinterpret_cast<char*>(session.community))));
	session.peername = reinterpret_cast<char*>(const_cast<char*>("wiener"));
	sess_handle = snmp_open(&session); // use sess_handle only in future requests
	if (!sess_handle){
		snmp_perror("ack");
		snmp_log(LOG_ERR,"wasn't opened correctly");
		exit(2);
	}
	read_objid("WIENER-CRATE-MIB::outputVoltage.u1", id_oid, &id_len);
//-------------------------------------------------------------------//

//Sending a get request with opening new session each time
	float voltage = 0;
	auto start = chrono::steady_clock::now();
	for (int i = 0; i< 10000; i++){
		sess_handle = snmp_open(&session);
		pdu = snmp_pdu_create(SNMP_MSG_GET);
		snmp_add_null_var(pdu, id_oid, id_len);
		snmp_synch_response(sess_handle, pdu, &response);
		voltage = *((response->variables)->val.floatVal);
		snmp_free_pdu(response);
		snmp_close(sess_handle);
	}
	auto end = chrono::steady_clock::now();
	float time = 0;
	time  = chrono::duration_cast<chrono::milliseconds>(end-start).count();
	time /= 10000;
	cout << "Get request (with new session opening) in ms: "<< time << endl;

//Sending a get request without closing the session
	sess_handle = snmp_open(&session);
	start = chrono::steady_clock::now();
	for (int i = 0; i< 10000; i++){
		pdu = snmp_pdu_create(SNMP_MSG_GET);
		snmp_add_null_var(pdu, id_oid, id_len);
		snmp_synch_response(sess_handle, pdu, &response);
		voltage = *((response->variables)->val.floatVal);
		snmp_free_pdu(response);
	}
	end = chrono::steady_clock::now();
	time  = chrono::duration_cast<chrono::milliseconds>(end-start).count();
	time /= 10000;
	cout << "Get request (without new session opening) in ms: "<< time << endl;
	snmp_close(sess_handle);

//Sending a set request with opening new session each time
	char str[] = "3.0";
	start = chrono::steady_clock::now();
	for (int i = 0; i< 10000; i++){
		sess_handle = snmp_open(&session);
		pdu = snmp_pdu_create(SNMP_MSG_SET);
		snmp_add_var(pdu, id_oid, id_len, 'F', str);
		snmp_synch_response(sess_handle, pdu, &response);
		snmp_free_pdu(response);
		snmp_close(sess_handle);
	}
	end = chrono::steady_clock::now();
	time = 0;
	time  = chrono::duration_cast<chrono::milliseconds>(end-start).count();
	time /= 10000;
	cout << "Set request (with new session opening) in ms: "<< time << endl;

//Sending a set request without opening new session each time
	chrono::time_point<chrono::system_clock> start_c,end_c;
	sess_handle = snmp_open(&session);
	start_c = chrono::steady_clock::now();
	for (int i = 0; i< 10000; i++){
		pdu = snmp_pdu_create(SNMP_MSG_SET);
		snmp_add_var(pdu, id_oid, id_len, 'F', str);
		snmp_synch_response(sess_handle, pdu, &response);
		snmp_free_pdu(response);
	}
	end_c = chrono::system_clock::now();
	time = 0;
	time  = chrono::duration_cast<chrono::milliseconds>(end_c-start_c).count();
	time /= 10000;
	cout << "Set request (without new session opening) in ms: "<< time << endl;
	snmp_close(sess_handle);
}
