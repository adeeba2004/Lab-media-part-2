#pragma once
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Media.h"
using namespace std;

// Input validation function
bool isDigits(const string& str);

// File reading functions
int readMediaList(istream& inFile, ostream& errFile, vector<Media*>& mediaLib);

// Display functions
void printMovies(ofstream& outFile, const vector<Media*>& mediaLib);
void printBooks(ofstream& outFile, const vector<Media*>& mediaLib);
void printSongs(ofstream& outFile, const vector<Media*>& mediaLib);
void printAllMedia(ofstream& outFile, const vector<Media*>& mediaLib);
void printTotals(ofstream& outFile, const vector<Media*>& mediaLib);
void printMoviesByRating(ofstream& outFile, const vector<Media*>& mediaLib, int rating);
void printSongsByGenre(ofstream& outFile, const vector<Media*>& mediaLib, const string& genre);

// New functions for Part 2
void printMovieStars(ofstream& outFile, ofstream& errFile, const vector<Media*>& mediaLib, const string& movieTitle);
void findMoviesByActor(ofstream& outFile, ofstream& errFile, const vector<Media*>& mediaLib, const string& actorName);
void findMediaByKeyName(ofstream& outFile, ofstream& errFile, const vector<Media*>& mediaLib, const string& keyName);

// New functions for extended functionality (add these function declarations)
void printStarsByMovie(ofstream& outFile, const vector<Media*>& mediaLib);
void printMoviesByStar(ofstream& outFile, const vector<Media*>& mediaLib);
void printMoviesByDirector(ofstream& outFile, const vector<Media*>& mediaLib);
void printSongsByArtist(ofstream& outFile, const vector<Media*>& mediaLib);

// Process new media addition
void processNewMedia(const string& commandRecord, ofstream& errFile, vector<Media*>& mediaLib);