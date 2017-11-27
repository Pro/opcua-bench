//
// Created by profanter on 27.11.17.
// Copyright (c) 2017 fortiss GmbH. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>  /* srand, rand */
#include <time.h>
#include <open62541.h>

UA_Boolean running = true;
static void stopHandler(int sig) {
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
	running = false;
}

UA_ByteString dataPackage;


static UA_StatusCode
setDataSizeMethodCallback(UA_Server *server,
						 const UA_NodeId *sessionId, void *sessionHandle,
						 const UA_NodeId *methodId, void *methodContext,
						 const UA_NodeId *objectId, void *objectContext,
						 size_t inputSize, const UA_Variant *input,
						 size_t outputSize, UA_Variant *output) {
	UA_UInt32 *newSize = (UA_UInt32 *)input->data;

	if (dataPackage.length > 0)
		UA_ByteString_deleteMembers(&dataPackage);

	UA_ByteString_allocBuffer(&dataPackage, (*newSize)*1024);
	for (size_t i=0; i<dataPackage.length; i++)
		dataPackage.data[i] = (UA_Byte) rand();

	return UA_STATUSCODE_GOOD;
}

static void
addSetDataSizeMethod(UA_Server *server) {
	UA_Argument inputArgument;
	UA_Argument_init(&inputArgument);
	inputArgument.description = UA_LOCALIZEDTEXT("en-US", "Desired package size in KB");
	inputArgument.name = UA_STRING("Size");
	inputArgument.dataType = UA_TYPES[UA_TYPES_UINT32].typeId;
	inputArgument.valueRank = -1; /* scalar */

	UA_MethodAttributes helloAttr = UA_MethodAttributes_default;
	helloAttr.description = UA_LOCALIZEDTEXT("en-US","Say `Hello World`");
	helloAttr.displayName = UA_LOCALIZEDTEXT("en-US","Hello World");
	helloAttr.executable = true;
	helloAttr.userExecutable = true;
	UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(1,6000),
							UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
							UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
							UA_QUALIFIEDNAME(1, "setSize"),
							helloAttr, &setDataSizeMethodCallback,
							1, &inputArgument,0, NULL, NULL, NULL);
}

static UA_StatusCode
getDataMethodCallback(UA_Server *server,
						 const UA_NodeId *sessionId, void *sessionHandle,
						 const UA_NodeId *methodId, void *methodContext,
						 const UA_NodeId *objectId, void *objectContext,
						 size_t inputSize, const UA_Variant *input,
						 size_t outputSize, UA_Variant *output) {
	UA_Variant_setScalarCopy(output, &dataPackage, &UA_TYPES[UA_TYPES_BYTESTRING]);
	//UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Sending data");
	return UA_STATUSCODE_GOOD;
}

static void
addGetDataMethod(UA_Server *server) {
	UA_Argument outputArgument;
	UA_Argument_init(&outputArgument);
	outputArgument.description = UA_LOCALIZEDTEXT("en-US", "The Data");
	outputArgument.name = UA_STRING("data");
	outputArgument.dataType = UA_TYPES[UA_TYPES_BYTESTRING].typeId;
	outputArgument.valueRank = -1; /* scalar */

	UA_MethodAttributes helloAttr = UA_MethodAttributes_default;
	helloAttr.description = UA_LOCALIZEDTEXT("en-US","Get the data from the server");
	helloAttr.displayName = UA_LOCALIZEDTEXT("en-US","getData");
	helloAttr.executable = true;
	helloAttr.userExecutable = true;
	UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(1,6001),
							UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
							UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
							UA_QUALIFIEDNAME(1, "getData"),
							helloAttr, &getDataMethodCallback,
							0, NULL, 1, &outputArgument, NULL, NULL);
}

static int throughput_server() {
	srand ((unsigned int) time(NULL));
	UA_ByteString_init(&dataPackage);

	UA_ServerConfig *config = UA_ServerConfig_new_default();
	UA_Server *server = UA_Server_new(config);

	addSetDataSizeMethod(server);
	addGetDataMethod(server);
	UA_StatusCode retval = UA_Server_run(server, &running);

	if (dataPackage.length > 0)
		UA_ByteString_deleteMembers(&dataPackage);
	UA_Server_delete(server);
	UA_ServerConfig_delete(config);
	return (int)retval;
}

time_t startPackageTime = 0;
time_t lastPrintTime = 0;
size_t totalBytes = 0;


static void client_gotData(UA_ByteString *data) {
	time_t curr = time(NULL);
	if (startPackageTime == 0) {
		lastPrintTime = curr;
		startPackageTime = curr;
		printf("Got first package. Size %ld bytes\n", data->length);
		return;
	}
	totalBytes += data->length;

	if (curr != lastPrintTime && (curr- startPackageTime) % 2 == 0) {
		double bytePerSec = totalBytes / (double)(curr - startPackageTime);
		double mibPerSec = bytePerSec / (1024 * 1024);
		printf("Rate = %.3f Mebibit/s = %.3f Mebibyte/s Total Size = %ldMiB, Time = %ld s\n", mibPerSec*8.0, mibPerSec, totalBytes/(1024*1024), curr-startPackageTime);
		lastPrintTime = curr;
	}

}

static int throughput_client(char *endpoint, unsigned int size) {
	UA_Client *client = UA_Client_new(UA_ClientConfig_default);
	UA_StatusCode retval = UA_Client_connect(client, endpoint);
	if(retval != UA_STATUSCODE_GOOD) {
		UA_Client_delete(client);
		printf("Could not connect to %s. Error: %s", endpoint, UA_StatusCode_name(retval));
		return (int)retval;
	}

	/* Set the size on the server */
	UA_Variant input;
	UA_UInt32 newSize = size;
	UA_Variant_init(&input);
	UA_Variant_setScalarCopy(&input, &size, &UA_TYPES[UA_TYPES_UINT32]);
	retval = UA_Client_call(client, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
							UA_NODEID_NUMERIC(1, 6000), 1, &input, 0, NULL);
	if(retval == UA_STATUSCODE_GOOD) {
		printf("Method call was successfull, and package is set to %d KB\n",newSize);
	} else {
		printf("Method call setSize was unsuccessfull, and the error is: %s\n", UA_StatusCode_name(retval));
		UA_Client_delete(client);
		return retval;
	}
	UA_Variant_deleteMembers(&input);

	while (running) {
		/* Get the data */
		size_t outputSize;
		UA_Variant *output;
		retval = UA_Client_call(client, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
								UA_NODEID_NUMERIC(1, 6001), 0, NULL, &outputSize, &output);
		if(retval == UA_STATUSCODE_GOOD) {

			UA_ByteString *data = (UA_ByteString *)output->data;
			client_gotData(data);
			UA_Array_delete(output, outputSize, &UA_TYPES[UA_TYPES_VARIANT]);
		} else {
			printf("Method call getData was unsuccessfull, and the error is: %s\n", UA_StatusCode_name(retval));
			break;
		}
	}

	UA_Client_delete(client);
	return UA_STATUSCODE_GOOD;
}

int main(int argc, char *argv[]) {
	if (argc != 2 && argc != 4) {
		printf("Usage: program [server][client ENDPOINT SIZE_MB]");
		exit(1);
	}

	signal(SIGINT, stopHandler);
	signal(SIGTERM, stopHandler);


	if (argc == 2 && strcmp(argv[1], "server") == 0)
		return throughput_server();

	if (argc == 4 && strcmp(argv[1], "client") == 0) {
		long size = strtol(argv[3], NULL, 10);
		if (size == 0) {
			printf("Invalid package size");
			return 1;
		}
		return throughput_client(argv[2], (unsigned int)size);
	}
	else
		printf("Invalid parameter");
	return 1;
}