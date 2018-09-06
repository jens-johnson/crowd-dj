# Crowd DJ 

### Created by: Josh Tanke and Christopher Jens Johnson

#### *An exploration of C/C++ server-client interactions using the Spotify API*

___

Have you ever been in a restaurant, bar, or party where the music just plain and simple sucks? Do you wish there was a way to put the music in the hands of the crowd? Crowd DJ is our exploration of how we can leverage one of the world's largest music libraries, Spotify, to replace the sentimental yet archaic jukebox. Crowd DJ allows users to create, manage, and vote on music that goes into a dynamic playlist, allowing the highest-priority music to be played next.


*How does it work?*
We implemented a simple local C/C++ server that allows users (clients) to interact with Crowd DJ as either a guest (someone who is contributing to a playlist) or a host (someone who is creating and managing the playlist). We use the Spotify API to manage user profiles, create and manage playlists, and index music from Spotify's library.

---

##### *Landing Page*
![Landing Page](https://github.com/jens-johnson/crowddj/blob/master/misc/images/landing.png "Landing Page")

##### *Spotify Authorization*
Authenticates users and obtains a redirect URL with their spotify access token
![Spotify Authorization](https://github.com/jens-johnson/crowddj/blob/master/misc/images/spotify-access.png "Spotify Authorization")

##### *Playlist Creation*
Hosts can create a playlist for their guests to contribute to
![Playlist Creation](https://github.com/jens-johnson/crowddj/blob/master/misc/images/playlist-creation.png "Playlist Creation")

##### *Song Search*
Users can search the Spotify library for songs to add to their playlist
![Song Search](https://github.com/jens-johnson/crowddj/blob/master/misc/images/song-search.png "Song Search")

##### *Guest Access*
Guests can search for a playlst to contribute to
![Guest Access](https://github.com/jens-johnson/crowddj/blob/master/misc/images/guest-search.png "Guest Access")

---

### To Access
Crowd DJ is currently configured for local access. Using a Spotify API key (line 37, SpotifyAPI.cpp), we can get access to the Spotify API needed to run the app. With the API key configured, we can simply compile our server program and run it (in this case accessing it through our local port on 8888):

```
make main
./server
```
