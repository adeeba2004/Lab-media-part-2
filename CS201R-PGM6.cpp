//Adeeba Nizam
//PROGRAM 7 PART 2 MEDIA

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
    vector<Media*> myLib; 
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

    // Read data into mediaLib
    readMediaList(inList, outErr, mediaLib);
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
        case 'M': // Movies
            if (!parameter.empty() && isdigit(parameter[0])) {
                printMoviesByRating(outFile, mediaLib, stoi(parameter));
            }
            else {
                printMovies(outFile, mediaLib);
            }
            break;

        case 'B': // Books
            if (!parameter.empty() && isdigit(parameter[0])) {
                printBooks(outFile, mediaLib);
            }
            else {
                printBooks(outFile, mediaLib);
            }
            break;

        case 'S': // Songs
            if (!parameter.empty()) {
                printSongsByGenre(outFile, mediaLib, parameter);
            }
            else {
                printSongs(outFile, mediaLib);
            }
            break;

        case 'A': // All Media
            printAllMedia(outFile, mediaLib);
            break;

        case 'T': // Totals
            printTotals(outFile, mediaLib);
            break;

        case 'L': // List stars of a specific movie
            if (!parameter.empty()) {
                printStarsByMovie(outFile, mediaLib, parameter);
            }
            else {
                outErr << "ERROR: Missing movie title for 'L' command." << endl;
            }
            break;

        case 'F': // Find movies by actor
            if (!parameter.empty()) {
                findMoviesByActor(outFile, outErr, myLib, parameter); // Add outErr
            }
            else {
                outErr << "ERROR: Missing actor name for 'F' command." << endl;
            }
            break;

        case 'K': // Find media by key name
            if (!parameter.empty()) {
                findMediaByKeyName(outFile, outErr, myLib, parameter); // Add outErr
            }
            else {
                outErr << "ERROR: Missing keyword for 'K' command." << endl;
            }
            break;


        case 'N': // Add new media
            processNewMedia(commandRecord, outErr, mediaLib);
            break;

        default:
            outErr << "ERROR: Invalid command found: " << commandRecord << endl;
            break;
        }
    }

    cout << "Thank You for Using Media Everywhere" << endl;

    // Clean up
    inCommands.close();
    outFile.close();
    outErr.close();

    // Delete dynamically allocated objects
    for (auto media : mediaLib) {
        delete media;
    }
    mediaLib.clear();

    return 0;
}
