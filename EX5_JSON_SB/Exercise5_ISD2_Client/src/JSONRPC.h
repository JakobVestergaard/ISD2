/**
 * @file JSONRPC.h
 * @author Steffen Breinbjerg
 * Elektronik ingeniør, Århus universitet Birk herning.
 * Indlejsytem design 2 (E4ISD2)
 * @copyright Free for everyone
 * @date 15-3-2020
 * @brief JSONRPC header file.
 *
 */
#ifndef JSONRPC_H_
#define JSONRPC_H_

/** 	Includes 	*/
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"
#include <cstdio>

using namespace rapidjson;
using namespace std;

class JSONRPC {
public:
	JSONRPC(){id = 1;received_id = 0;}

	virtual ~JSONRPC();
	string RPC_Call(string);
	string RPC_Receive(string);
	string RPC_Answer(string);
	string RPC_Result(string);

private:
	Document doc;
	Value val;
	int id;
	int received_id;


};


#endif /* JSONRPC_H_ */
