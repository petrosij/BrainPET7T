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
#include <iostream>
using namespace std;
int main(int argc, char ** argv)
{
	struct snmp_session session;
	struct snmp_session *sess_handle;
	struct snmp_pdu *pdu;
	struct snmp_pdu *response;
	oid id_oid[MAX_OID_LEN];
	size_t id_len = MAX_OID_LEN;
	float voltage = 0;

	init_snmp("APC Check");
	snmp_sess_init( &session );
	session.version = SNMP_VERSION_2c;
	session.community = reinterpret_cast<unsigned char*>(const_cast<char*>("guru"));
	session.community_len = (strlen(const_cast<const char*>(reinterpret_cast<char*>(session.community))));
	session.peername = reinterpret_cast<char*>(const_cast<char*>("wiener"));
	sess_handle = snmp_open(&session);
	if (!sess_handle){
		snmp_perror("ack");
		snmp_log(LOG_ERR,"wasn't opened correctly");
		exit(2);
	}
	read_objid("WIENER-CRATE-MIB::outputVoltage.u1", id_oid, &id_len);
	pdu = snmp_pdu_create(SNMP_MSG_GET);
	snmp_add_null_var(pdu, id_oid, id_len);
	snmp_synch_response(sess_handle, pdu, &response);
	voltage = *((response->variables)->val.floatVal);
	snmp_free_pdu(response);
	snmp_close(sess_handle);
}
