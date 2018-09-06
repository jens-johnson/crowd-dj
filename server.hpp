#ifndef SERVER_CPP_
#define SERVER_CPP_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include "SpotifyAPI.cpp"
#include "SpotifyPQ.cpp"


class Server
{
public:
	Server(int port);
	~Server();

	void parseUrl();
	pair<char*, int> parsefile(std::string filename);

	void displaySongs();

	void throwError();

	void buildPlaylist();

	string fixString(string s);

	char url[500];

	string currentPlaylist;

private:
	struct sockaddr_in server_address, client_address;
	socklen_t sin_len;
	int fd_server, fd_client, on, PORT;
	char buff[2048];
	char auth_token[346];
	std::string authString = "";
	SpotifyAPI spotifyAPI;
	SpotifyPQ spotifyPQ;
	
	int siteIndex = 0;
	pair<char*, int> siteArray[7];
	string names[7] = {"htmlFiles/main.html", "htmlFiles/host.html", "htmlFiles/guest.html", "htmlFiles/addSongs.html", "htmlFiles/newPlaylist.html", "htmlFiles/temp.html", "htmlFiles/temp2.html"};


};

#endif /* SERVER_HPP_ */