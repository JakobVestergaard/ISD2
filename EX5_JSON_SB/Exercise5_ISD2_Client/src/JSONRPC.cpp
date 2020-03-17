/**
 * @file JSON-RPC.cpp
 * @author Steffen Breinbjerg
 * Elektronik ingeniør, Århus universitet Birk herning.
 * Indlejsytem design 2 (E4ISD2)
 * @copyright Free for everyone
 * @date 15-3-2020
 * @brief Implementation of JSONRPC.h
 *
 */

#include "JSONRPC.h"

JSONRPC::~JSONRPC() {
	// TODO Auto-generated destructor stub
}
/**
 *
 * @param in method to call on remote computer
 * @return string object - JSON String.
 */
string JSONRPC::RPC_Call(string in) {

	/** RPC CALL parameters
	 * {"jsonrpc": "2.0", "method": "name", "params": [arg, arg], "id": 1}
	 * */

	// Prepare an initial document without any content
	char json[] = "{}";
	doc.Parse(json);
	// First the jsonrcp version
	char version[] = "2.0";
	val.SetString(version, static_cast<SizeType>(strlen(version)),
			doc.GetAllocator());
	doc.AddMember("jsonrpc", val, doc.GetAllocator());

	/*************************
	 *Add Method
	 ************************/
	val.SetString(in.c_str(), static_cast<SizeType>(in.length()),
			doc.GetAllocator());
	doc.AddMember("method", val, doc.GetAllocator());

	/*************************
	 *Add ID
	 ************************/
	val.SetInt(id);
	doc.AddMember("id", val, doc.GetAllocator());

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);

	return buffer.GetString();

}
/**
 *
 * @param String object - Received JSON String
 * @return string Object - Method to call on computer
 */
string JSONRPC::RPC_Receive(string in) {

	/** Expected RPC CALL parameters
	 * {"jsonrpc": "2.0", "method": "name", "params": [arg, arg], "id": 1}
	 * */

//	doc.Clear();
	char receivedStr[100];
	strcpy(receivedStr, in.c_str());

	if (doc.ParseInsitu(receivedStr).HasParseError()) {
		return nullptr;
	}
	/** So the received document is correct formated JSON */
	/** Check if it is a RPC call */
	if (!doc.HasMember("jsonrpc")) {
		return nullptr;
	}

	/*************************
	 *Find ID number
	 ************************/

	/** Get an iterator pointing at the member */
	Value::MemberIterator jsonrpcIter = doc.FindMember("id");
	/** Check if the iterator is not pointing to the end */
	if (jsonrpcIter == doc.MemberEnd()) {
		return nullptr;
	}
	/** See of the member is an int */
	if (!jsonrpcIter->value.IsInt()) {
		return nullptr;
	}
	/** Save ID */
	received_id = jsonrpcIter->value.GetInt();

	/*************************
	 *Find Method
	 ************************/

	jsonrpcIter = doc.FindMember("method");
	/** Check if the iterator is not pointing to the end */
	if (jsonrpcIter == doc.MemberEnd()) {
		return nullptr;
	}
	/** Check that it is string we received */
	if (!jsonrpcIter->value.IsString()) {
		return nullptr;
	}

	return jsonrpcIter->value.GetString();
}
/**
 *
 * @param in String object - Result from method
 * @return string object - JSON String
 */
string JSONRPC::RPC_Answer(string in) {

	/** RPC Result parameteres
	 *{"jsonrpc": "2.0", "result": 19, "id": 1}
	 */

//	doc.Clear();
	// Prepare an initial document without any content
	char json[] = "{}";
	doc.Parse(json);
	// First the jsonrcp version
	char version[] = "2.0";
	val.SetString(version, static_cast<SizeType>(strlen(version)),
			doc.GetAllocator());
	doc.AddMember("jsonrpc", val, doc.GetAllocator());

	/*************************
	 *Add Result
	 ************************/
	val.SetString(in.c_str(), static_cast<SizeType>(strlen(version)),
			doc.GetAllocator());
	doc.AddMember("result", val, doc.GetAllocator());

	/*************************
	 *Add ID
	 ************************/
	val.SetInt(received_id);
	doc.AddMember("id", val, doc.GetAllocator());

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);

	return buffer.GetString();

}
/**
 *
 * @param in string object - Received Result JSON String
 * @return String object - Result from called method.
 */
string JSONRPC::RPC_Result(string in) {

	/** Expected RPC Result parameteres
	 *{"jsonrpc": "2.0", "result": 19, "id": 1}
	 */

//	doc.Clear();
	char receivedStr[100];
	strcpy(receivedStr, in.c_str());

	if (doc.ParseInsitu(receivedStr).HasParseError()) {
		return nullptr;
	}
	/** So the received document is correct formated JSON */
	/** Check if it is a RPC call */
	if (!doc.HasMember("jsonrpc")) {
		return nullptr;
	}

	/*************************
	 *Find ID number
	 ************************/

	/** Get an iterator pointing at the member */
	Value::MemberIterator jsonrpcIter = doc.FindMember("id");
	/** Check if the iterator is not pointing to the end */
	if (jsonrpcIter == doc.MemberEnd()) {
		return nullptr;
	}
	/** See of the member is an int */
	if (!jsonrpcIter->value.IsInt()) {
		return nullptr;
	}

	/** Compare ID */

	/*************************
	 *Find Result
	 ************************/

	jsonrpcIter = doc.FindMember("result");
	/** Check if the iterator is not pointing to the end */
	if (jsonrpcIter == doc.MemberEnd()) {
		return nullptr;
	}
	/** Check that it is string we received */
	if (!jsonrpcIter->value.IsString()) {
		return nullptr;
	}

	return jsonrpcIter->value.GetString();
}

