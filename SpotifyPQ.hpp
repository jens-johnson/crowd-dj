#ifndef SPOTIFYPQ_CPP_
#define SPOTIFYPQ_CPP_

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>

using namespace std;

class SpotifyPQ{

public:

	SpotifyPQ();

	void addPlaylist(string username, string playlistID);

	void deletePlaylist(string username);

	void addSong(string playlistID, string songID);

	vector<pair<string, int> > sortSongs(string playlistID);

	bool playlistExists(string username);

	map<string, string> playlists;	//username, playlistID
	map<string, map<string, int>> songLists;  //playlistID, ordered list of songs
};


#endif