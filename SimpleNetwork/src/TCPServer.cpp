#include "TCPServer.h" 

string TCPServer::Message;

void* TCPServer::Task(void *arg)
{
	int n;
	int newsockfd = (long)arg;
	char msg[MAXPACKETSIZE];
	pthread_detach(pthread_self());
	while(1)
	{
		n=recv(newsockfd,msg,MAXPACKETSIZE,0);
		if(n==0)
		{
		   close(newsockfd);
		   break;
		}
		msg[n]=0;
		//send(newsockfd,msg,n,0);
		Message = string(msg);
        }
	return 0;
}

// This function is a manual initialisation
void TCPServer::setup(int port)
{
	sockfd=socket(AF_INET,SOCK_STREAM,0); //  AF_INET = IPv4 - stream = TCP - 0 = IP
 	memset(&serverAddress,0,sizeof(serverAddress)); // Sets a lot of 0's in serverAddress
	serverAddress.sin_family=AF_INET; // Set IPv4
	serverAddress.sin_addr.s_addr=htonl(INADDR_ANY); // host to network long
	serverAddress.sin_port=htons(port); // Set Port
    // binds socket filedescriptor to port in port-table 
    // bind(int sockfd, const struct sockaddr * addr, socklen_t addrLen)
	bind(sockfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress));
    // Starts listening to socket
    // listen(int sockfd, int backlog)
 	listen(sockfd,5);
}

// Add error-handling

// This function creates a socket from incoming connections and threads them
string TCPServer::receive()
{
	string str;
	while(1)
	{
        // Sets size of clientAddress
		socklen_t sosize  = sizeof(clientAddress);
        // Accept() is a blocking call
        // add errorhandling
		newsockfd = accept(sockfd,(struct sockaddr*)&clientAddress,&sosize);
        /*     The inet_ntoa() function converts the Internet host address in, given
       in network byte order, to a string in IPv4 dotted-decimal notation.
       The string is returned in a statically allocated buffer, which
       subsequent calls will overwrite.*/
		str = inet_ntoa(clientAddress.sin_addr);
        // add error handling
		pthread_create(&serverThread,NULL,&Task,(void *)newsockfd);
	}
    // This doesn't return ¯\_(ツ)_/¯
	return str;
}

string TCPServer::getMessage()
{
	return Message;
}

void TCPServer::Send(string msg)
{
	send(newsockfd,msg.c_str(),msg.length(),0);
}

void TCPServer::clean()
{
	Message = "";
	memset(msg, 0, MAXPACKETSIZE);
}

void TCPServer::detach()
{
	close(sockfd);
	close(newsockfd);
} 
