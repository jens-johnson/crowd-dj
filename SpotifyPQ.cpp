#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "SpotifyPQ.hpp"
#include <map>

using namespace std;

SpotifyPQ::SpotifyPQ(){
}

void SpotifyPQ::addPlaylist(string username, string playlistID){
	this->playlists[username] = playlistID;
}

void SpotifyPQ::deletePlaylist(string username){
	string playlistToDel = playlists[username];
	this->songLists.erase(playlistToDel);
	this->playlists.erase(username);
}

void SpotifyPQ::addSong(string playlistID, string songID){
	if(songLists[playlistID].find(songID) == songLists[playlistID].end()){
		songLists[playlistID][songID] = 1;
	}else{
		int temp = songLists[playlistID][songID];
		songLists[playlistID][songID] = temp + 1;
	}
}

vector<pair<string, int> > SpotifyPQ::sortSongs(string playlistID){
	vector<pair<string, int> > myVec(songLists[playlistID].begin(), songLists[playlistID].end());
	sort(myVec.begin(),myVec.end(),[](pair<string,int>elem1,pair<string,int>elem2){return elem1.second > elem2.second;});
	return myVec;

}

bool SpotifyPQ::playlistExists(string playlist){
	if ( playlists.find(playlist) == playlists.end() ) {
		return false;
	} else {
		return true;
	}
}




