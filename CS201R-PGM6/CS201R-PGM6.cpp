// CS201R-PGM7.cpp : Media Library Part 2 - Derived Classes Implementation
// Adeeba Nizam

#include "Functions.h"
#include "Media.h"
#include <iostream>
#include <exception>
#include <sstream>
#include <vector>
#include <fstream>

using namespace std;

int main() {
    vector<Media*> mediaLib;
    ifstream inList("mediaList.txt");
    ifstream inCommands("mediaCommands.txt");
    ofstream outFile("mediaReport.txt");
    ofstream outErr("mediaError.txt");

    // Check each file individually after attempting to open
    if (!inList.is_open()) {
        cerr << "Error: Unable to open mediaList.txt" << endl;
        return 1;
    }
    if (!inCommands.is_open()) {
        cerr << "Error: Unable to open mediaCommands.txt" << endl;
        return 1;
    }
    if (!outFile.is_open()) {
        cerr << "Error: Unable to open mediaReport.txt" << endl;
        return 1;
    }
    if (!outErr.is_open()) {
        cerr << "Error: Unable to open mediaError.txt" << endl;
        return 1;
    }

    vector<Media*> myLib;

    // Read data into myLib
    readMediaList(inList, outErr, myLib);
    inList.close();

    // Process commands
    string commandRecord;
    while (getline(inCommands, commandRecord) && !commandRecord.empty()) {
        char choice = commandRecord[0];

        if (choice == 'Q') {
            break;
        }

        // Parse command parameters if any
        string parameter;
        if (commandRecord.length() > 2) {
            parameter = commandRecord.substr(2);
        }
        else if (commandRecord.length() > 1 && isdigit(commandRecord[1])) {
            parameter = commandRecord.substr(1);
        }

        switch (choice) {
        case 'M':
            if (!parameter.empty() && isdigit(parameter[0])) {
                printMoviesByRating(outFile, myLib, stoi(parameter));
            }
            else {
                printMovies(outFile, myLib);
            }
            break;

        case 'B':
            if (!parameter.empty() && isdigit(parameter[0])) {
                // Add rating filter for books if needed
                printBooks(outFile, myLib);
                //printBooksByRating(outFile, myLib, stoi(parameter));
            }
            else {
                printBooks(outFile, myLib);
            }
            break;

        case 'S':
            if (!parameter.empty()) {
                printSongsByGenre(outFile, myLib, parameter);
            }
            else {
                printSongs(outFile, myLib);
            }
            break;

        case 'A':
            printAllMedia(outFile, myLib);
            break;

        case 'T':
            printTotals(outFile, myLib);
            break;

        case 'L':
            if (!parameter.empty()) {
                printMovieStars(outFile, outErr, myLib, parameter);
            }
            break;

        case 'F':
            if (!parameter.empty()) {
                findMoviesByActor(outFile, outErr, myLib, parameter);
            }
            break;

        case 'K':
            if (!parameter.empty()) {
                findMediaByKeyName(outFile, outErr, myLib, parameter);
            }
            break;

        case 'N':
            processNewMedia(commandRecord, outErr, myLib);
            break;

        default:
            outErr << "Invalid command found: " << commandRecord << endl;
        }
    }

    cout << "Thank You for Using Media Everywhere" << endl;

    // Clean up
    inCommands.close();
    outFile.close();
    outErr.close();

    // Delete dynamically allocated objects
    for (auto media : myLib) {
        delete media;
    }
    myLib.clear();

    return 0;
}