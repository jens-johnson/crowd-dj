#ifndef SPOTIFY_API_
#define SPOTIFY_API_

#include <string>
#include "json.hpp"
#include "curl/curl.h"

using namespace std;



class SpotifyAPI{

public:

	SpotifyAPI();
	
	static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
	{
	    ((std::string*)userp)->append((char*)contents, size * nmemb);
	    return size * nmemb;
	}

	string ReplaceAll(std::string str, const std::string& from, const std::string& to)
	{
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
	}
	void setAuthToken(string authToken);

	void authorize();

	void authorizeHost(string token);

	nlohmann::json spotifyCurlRequest(string request, string endpoint, map<string, string> options, string authToken, string body, string headerTwo);

	nlohmann::json spotifyGETRequest(string endpoint, map<string, string> options, string authToken, string body, string headerTwo);
	
	nlohmann::json spotifyPOSTRequest(string endpoint, map<string, string> options, string authToken, string body, string headerTwo);

	void searchForSong(string name);

	void createPlaylist(string name, string userID);

	string getUserID();

	string getPlaylistID(string userID);

	void addSongToPlaylist(string userID, string playlistID, string songID);

	pair<string, pair<string, string>> currentSongs[5];

private:

	string authToken;
	bool isHost = false;
	map<string, string> users;


};

#endif