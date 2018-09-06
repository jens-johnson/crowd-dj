#include <iostream>
#include "SpotifyAPI.hpp"
#include "json.hpp"
#include "curl/curl.h"
#include <string>

using namespace std;




SpotifyAPI::SpotifyAPI(){
	curl_global_init( CURL_GLOBAL_ALL );
}


void SpotifyAPI::setAuthToken(string authToken){
	this-> authToken = authToken;
}


void SpotifyAPI::authorize(){
	
	string buffer;
	
	CURL * curl;
	curl = curl_easy_init();

	string url = "https://accounts.spotify.com/api/token";
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

	/* Spotify API Key goes below */
	string apiKey = "<API KEY HERE>"

	string header = "Authorization: Basic "+apiKey+"=";
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, header.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	string postData = "grant_type=client_credentials";
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());

    int rc = curl_easy_perform(curl);
    if (rc != CURLE_OK)
    	cout << "oops" << endl;
    curl_easy_cleanup(curl);

    nlohmann::json json = nlohmann::json::parse(buffer);
    string token = json["access_token"];

    setAuthToken(token);
}

void SpotifyAPI::authorizeHost(string token){
	this-> isHost = true;
	setAuthToken(token);
}


nlohmann::json SpotifyAPI::spotifyCurlRequest(string request, string endpoint, map<string, string> options, string authToken, string body, string headerTwo){
	
	CURL * curl;
	curl = curl_easy_init();

	string buffer;

	string url = "https://api.spotify.com" + endpoint;

	if(!options.empty())
    {
        url += "?";
        for(auto option : options)
        {
            url += option.first + "=" + option.second + '&';
        }
    }
    url = ReplaceAll(url, " ", "%20");

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, request.c_str());

	string header = "Authorization: Bearer " + authToken;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, header.c_str());

    if(!headerTwo.empty()){
    	
    	headers = curl_slist_append(headers, headerTwo.c_str());
    }

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    if(!body.empty())
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());

    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    int rc = curl_easy_perform(curl);
    if (rc != CURLE_OK)
    	cout << "curl error" << endl;
    curl_easy_cleanup(curl);

    return nlohmann::json::parse(buffer);
}

nlohmann::json SpotifyAPI::spotifyGETRequest(string endpoint, map<string, string> options, string authToken, string body="", string headerTwo=""){
	return spotifyCurlRequest("GET", endpoint, options, authToken, body, headerTwo);
}

nlohmann::json SpotifyAPI::spotifyPOSTRequest(string endpoint, map<string, string> options, string authToken, string body="", string headerTwo=""){
	return spotifyCurlRequest("POST", endpoint, options, authToken, body, headerTwo);
}

void SpotifyAPI::searchForSong(string query){
	map<string, string> options;
	options["q"] = query;
	options["type"] = "track";
	options["limit"] = "5";
	nlohmann::json songInfo = spotifyGETRequest("/v1/search", options, this->authToken);
	

    nlohmann::json json = songInfo["tracks"]["items"];

    
    int i = 0;
    for (auto it = json.begin(); it != json.end(); ++it) {
  		currentSongs[i].second.first = it.value().at("name");
  		currentSongs[i].first = it.value().at("id");
  		nlohmann::json subJson = it.value().at("artists");
  		string temp = "";
  		for (auto it2 = subJson.begin(); it2 != subJson.end(); ++it2){
  			string cast = it2.value().at("name");
  			temp += cast;
  			temp += ",";
  		}
  		temp.pop_back();
  		currentSongs[i].second.second = temp;
  		
  		i += 1;
	}
	
}

void SpotifyAPI::createPlaylist(string name, string userID){
	if(this-> isHost == false){
		cout << "you cant do this" << endl;
		return;
	}
	nlohmann::json bodyInfo;
	bodyInfo["name"] = name;
	map<string, string> options;
	nlohmann::json playlistInfo = spotifyPOSTRequest("/v1/users/" + userID + "/playlists", options, authToken, bodyInfo.dump(4));

	this->users[userID] = playlistInfo["id"];


}

string SpotifyAPI::getUserID(){
	if(this-> isHost == false){
		cout << "you are not a host" << endl;
		return "error: not host";
	}

	map<string, string> options;
	nlohmann::json userInfo = spotifyGETRequest("/v1/me", options, this->authToken);
	

	return userInfo["id"];

}

string SpotifyAPI::getPlaylistID(string userID){
	return users[userID];
}

void SpotifyAPI::addSongToPlaylist(string userID, string playlistID, string songID){
	map<string, string> options;
	options["uris"] = songID;
	nlohmann::json info = spotifyPOSTRequest("/v1/users/" + userID + "/playlists/" + playlistID + "/tracks", options, authToken, "", "Accept: application/json");
	
}
