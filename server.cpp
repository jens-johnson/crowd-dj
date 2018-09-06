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
#include "server.hpp"
#include <algorithm>

#define MAX_FILE_SIZE 10000
using namespace std;

void Server::parseUrl(){
	this->authString = "";
	int sizeOfArray = sizeof(auth_token) / sizeof(auth_token[0]);
	for(int i = 0; i < sizeOfArray; i++){
		if(auth_token[i] == '='){
			i++;
			while(auth_token[i] != '&'){
				authString += auth_token[i];
				i++;
			}	
			break;
		}
	}
}

void Server::buildPlaylist(){
	string userID = spotifyAPI.getUserID();
	vector<pair<string, int> > currentVec = spotifyPQ.sortSongs(currentPlaylist);
	for(int i = 0; i < currentVec.size(); i++){
		spotifyAPI.addSongToPlaylist(userID, currentPlaylist, "spotify:track:" + currentVec[i].first);
	}
}

string Server::fixString(string s){
	string newString = s;
	newString.erase(remove(newString.begin(), newString.end(), '"'), newString.end());
	newString.erase(remove(newString.begin(), newString.end(), '\n'), newString.end());
	return newString;
}

pair<char*, int> Server::parsefile(string filename){
	ifstream file(filename);
	file.seekg(0, ios::end);
	int flength = file.tellg();
	file.seekg(0, ios::beg);
	char *file_array = new char[flength];
	file.read(file_array, flength);
	file.close();
	pair<char*, int> ret;
	ret.first = file_array;
	ret.second = flength;
	return ret;
}

void Server::displaySongs(){
	ifstream  src("htmlFiles/searchSongs.html", ios::binary);
    ofstream  dst("htmlFiles/temp.html",   ios::binary);

    dst << src.rdbuf();
    dst << "<br>" << endl << "<div>" << endl << "<ol>";
    for(int i = 0; i < 5; i++){
    	dst << "<li>Song: " << spotifyAPI.currentSongs[i].second.first << " &nbsp Artist(s): " << spotifyAPI.currentSongs[i].second.second << "</li>";
    	dst << "<button name=\"button" << i << "\" id=\"" << spotifyAPI.currentSongs[i].first << "\" onClick=\"send_song(this.id)\">Add Song</button><br><br>" << endl;
    }
    dst << "</ol>" << endl << "</div>" << endl;
    dst << "<script type=\"text/javascript\">" << endl;
    dst << "\tfunction send_song(button_id){" << endl;
    dst << "\tvar songnumber = JSON.stringify(button_id);" << endl;
    dst << "\tsongnumber += \"/\";" << endl;
    dst << "\tvar song_request = new XMLHttpRequest();" << endl;
    dst << "\tsong_request.open(\"POST\", \"/send_song\", false);" << endl;
    dst << "\tsong_request.send(songnumber)};" << endl;
    dst << "</script>" << endl;
    dst << "</body>" << endl << "</html>";

    pair<char*, int> main_file_info = parsefile(names[5]);
	siteArray[5] = main_file_info;
}

void Server::throwError(){
	
	ifstream  src("htmlFiles/guestError.html", ios::binary);
	ofstream  dst("htmlFiles/temp2.html",   ios::binary);

    dst << src.rdbuf();
    dst << "<div><p>ERROR: invalid username</p></div>" << endl;
    dst << "</body>" << endl;
    dst << "</html>";

    pair<char*, int> main_file_info = parsefile(names[6]);
	siteArray[6] = main_file_info;
}


Server::Server(int port){
	this->PORT = port;

	for(int i = 0; i < 5; i++){
		pair<char*, int> main_file_info = parsefile(names[i]);
		siteArray[i] = main_file_info;
	}
	

	/* Establish file descriptor for the server (IPv4 Protocol) */
	fd_server = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_server < 0){
		perror("socket");
		exit(1);
	}


	/* Configure socket */
	on = 1;
	setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

	/* Configure server adress and port */
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);

	/* Bind the server descriptor to its address struct (using our local IP and port) */
	if (::bind(fd_server, (struct sockaddr *) &server_address, sizeof(server_address))==-1){
		perror("bind");
		close(fd_server);
		exit(1);
	}

	/* Listen for connections (up to 10), before closing */
	if (listen(fd_server, 10) == -1){
		perror("listen");
		close(fd_server);
		exit(1);
	}

	bool auth_token_accepted = false;
	bool test = false;

	/* Main process, will exit with errors */
	while(1){
		/* Accept server connection from client side */
		sin_len = sizeof(client_address);
		fd_client = accept(fd_server, (struct sockaddr *) &client_address, &sin_len);

		if (fd_client == -1){
			perror("Connection failed\n");
			continue;
		}

		

			/* At this point, we have accepted the server, and we are in the child process */
			
			memset(buff, 0, 2048);
			read(fd_client, buff, 2047);

			/* Client request control flow: Read in all POST requests into the buffer */
			if (!strncmp(buff, "POST", 4)){
				
				if ((!strncmp(buff, "POST /auth_token", 16)) && !auth_token_accepted){
					
					for (int i=0; i<346; i++){
						auth_token[i] = buff[418+i];
					}
					
					parseUrl();
					
					if(authString.length() > 0){
						
						siteIndex = 1;
						spotifyAPI.authorizeHost(authString);
					}
						
				}
				if ((!strncmp(buff, "POST /playlist_search", 21))){
					string user = "";
					int i = 0;
					while(buff[418+i+1] != '/'){
						user += buff[418+i];
						i += 1;
					}
					user = fixString(user);
					user.erase(0, 1);
					
					if(spotifyPQ.playlistExists(user)){
						cout << "found playlist" << endl;
						siteIndex = 3;
						
					}
					else{
						cout << "did not find playlist" << endl;
						throwError();
						siteIndex = 6;
					}
					
					
				}
				if ((!strncmp(buff, "POST /new_playlist", 18))){
					string playlistName = "";
					int i = 0;
					while(buff[418+i+1] != '/'){
						playlistName += buff[418+i];
						i += 1;
					}
					playlistName = fixString(playlistName);
					cout << "creating playlist: " << playlistName << endl;
					string username = spotifyAPI.getUserID();
					
					spotifyAPI.createPlaylist(playlistName, username);
					currentPlaylist = spotifyAPI.getPlaylistID(username);

					spotifyPQ.addPlaylist(username, currentPlaylist);
					siteIndex = 3;

				}
				if ((!strncmp(buff, "POST /search_song", 17))){
					string songName = "";
					int i = 0;
					while(buff[417+i+1] != '/'){
						songName += buff[417+i];
						i += 1;
					}
					songName = fixString(songName);
					spotifyAPI.authorize();
					cout << "searching for: " << songName << endl;
					spotifyAPI.searchForSong(songName);
					displaySongs();
					siteIndex = 5;
				}

				if ((!strncmp(buff, "POST /send_song", 15))){
					string songId = "";
					int i = 0;
					while(buff[415+i+1] != '/'){
						songId += buff[415+i];
						i += 1;
					}
					songId = fixString(songId);
					cout << "SONG ID: " << songId << endl;
					spotifyPQ.addSong(currentPlaylist, songId);

				}
			}

			if (!strncmp(buff, "GET", 3)){
				if (!strncmp(buff, "GET /playlist_redirect", 22)){
					siteIndex = 2;
				}
				if (!strncmp(buff, "GET /new_playlist", 17)){
					siteIndex = 4;
				}
				if (!strncmp(buff, "GET /build_playlist", 19)){
					cout << "building playlist" << endl;
					buildPlaylist();
				}
				if (!strncmp(buff, "GET /send_home", 14)){
					siteIndex = 0;
				}
			}
			write(fd_client, siteArray[siteIndex].first, siteArray[siteIndex].second - 1);
		
		close(fd_client);
	}
}

Server::~Server(){
	for (int i=0; i<7; i++){
		delete[] siteArray[i].first;
	}
}

int main(){
	cout << "Establishing connection" << endl;
	Server s = Server(8888);
}

















