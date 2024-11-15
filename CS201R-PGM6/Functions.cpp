#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include "Functions.h"
#include "Media.h"


bool isDigits(const string& str) {
    return str.find_first_not_of("0123456789") == string::npos;
}

int readMediaList(istream& inFile, ostream& errFile, vector<Media*>& mediaLib) {
    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string type, title, name, ratingStr, genre, lengthStr, yearStr, additionalInfo;

        // Read each field
        getline(ss, type, ',');
        getline(ss, title, ',');
        getline(ss, name, ',');
        getline(ss, ratingStr, ',');
        getline(ss, genre, ',');
        getline(ss, lengthStr, ',');
        getline(ss, yearStr, ',');
        getline(ss, additionalInfo); // Get rest of line for additional info

        int rating = -1, length = -1, year = -1;

        // Attempt to convert rating, length, and year fields
        try {
            if (isDigits(ratingStr)) {
                rating = stoi(ratingStr);
            }
            else {
                throw invalid_argument("Invalid stoi argument error");
            }
        }
        catch (const invalid_argument&) {
            errFile << "ERROR: " << line << endl;
            errFile << "Previous record has an invalid stoi argument error" << endl;
            continue;  // Skip this record
        }

        try {
            if (isDigits(lengthStr)) {
                length = stoi(lengthStr);
            }
            else {
                throw invalid_argument("Invalid length");
            }
        }
        catch (const invalid_argument&) {
            errFile << "ERROR: " << line << endl;
            errFile << "Previous record has an invalid stoi argument error" << endl;
            continue;  // Skip this record
        }

        try {
            if (isDigits(yearStr)) {
                year = stoi(yearStr);
            }
            else {
                throw invalid_argument("Invalid year");
            }
        }
        catch (const invalid_argument&) {
            errFile << "ERROR: " << line << endl;
            errFile << "There was an invalid value entered for year." << endl;
            continue;  // Skip this record
        }

        // Check for other validation errors
        if ((type != "M" && type != "B" && type != "S") ||
            rating < 0 || rating > 10 ||
            length <= 0 ||
            year < 1920 || year > 2024) {
            errFile << "ERROR: " << line << endl;
            if (rating < 0 || rating > 10) {
                errFile << "The rating provided: " << rating << " is invalid" << endl;
            }
            if (length <= 0) {
                errFile << "There was an invalid value entered for length." << endl;
            }
            if (year < 1920 || year > 2024) {
                errFile << "There was an invalid value entered for year." << endl;
            }
            continue;  // Skip this record
        }

        Media* newMedia = nullptr;

        // Create appropriate derived class based on type
        if (type == "M") {
            vector<string> stars;
            if (!additionalInfo.empty()) {
                stringstream starsSS(additionalInfo);
                string star;
                while (getline(starsSS, star, ',')) {
                    // Remove leading/trailing spaces
                    star.erase(0, star.find_first_not_of(" "));
                    star.erase(star.find_last_not_of(" ") + 1);
                    if (!star.empty()) {
                        stars.push_back(star);
                    }
                }
            }
            newMedia = new Movie('M', title, name, rating, genre, length, year, stars);
        }
        else if (type == "B") {
            int weeks = 0;
            if (!additionalInfo.empty()) {
                try {
                    weeks = stoi(additionalInfo);
                    if (weeks < 0) throw invalid_argument("Invalid weeks");
                }
                catch (...) {
                  errFile << "ERROR: " << line << endl;
                  errFile << "Unable to process command. There was an invalid value entered for weeks: " << title << endl;
                  continue;
                 // weeks = 0; // Default to 0 if invalid
                }
            }
            newMedia = new Book('B', title, name, rating, genre, length, year, weeks);
        }
        else if (type == "S") {
            bool isTop40 = false;
            if (!additionalInfo.empty()) {
                isTop40 = (additionalInfo == "1" ||
                    additionalInfo == "true" ||
                    additionalInfo == "yes");
            }
            newMedia = new Song('S', title, name, rating, genre, length, year, isTop40);
        }

        if (newMedia) {
            mediaLib.push_back(newMedia);
        }
    }
    return mediaLib.size();
}

void printMovies(ofstream& outFile, const vector<Media*>& mediaLib) {
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    outFile << "              YOUR MOVIE LIST" << endl;
    outFile << "#           TITLE                             YEAR  RATING  GENRE                 TYPE" << endl;
    int count = 1;
    for (const auto& media : mediaLib) {
        if (media->getType() == 'M') {
            outFile << left << setw(5) << count++
                << setw(45) << media->getTitle()
                << setw(10) << media->getYearReleased()
                << setw(10) << media->getRating()
                << setw(30) << media->getGenre()
                << media->getType() << endl;
            media->print(outFile, mediaLib); // Calling the print metod of the movie class
        }
    }
}

void printMoviesByRating(ofstream& outFile, const vector<Media*>& mediaLib, int rating) {
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    outFile << "           MOVIES WITH RATING " << rating << endl;
    int count = 1;
    for (const auto& media : mediaLib) {
        if (media->getType() == 'M' && media->getRating() == rating) {
            outFile << count++ << " " << media->getTitle() << " " << media->getYearReleased() << " " << media->getRating() << " " << media->getGenre() << endl;
        }
    }
}

void printSongsByGenre(ofstream& outFile, const vector<Media*>& mediaLib, const string& genre) {
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    outFile << "           SONGS IN GENRE " << genre << endl;
    int count = 1;
    for (const auto& media : mediaLib) {
        if (media->getType() == 'S' && media->getGenre() == genre) {
            outFile << count++ << " " << media->getTitle() << " " << media->getYearReleased() << " " << media->getRating() << endl;
        }
    }
}

void printSongs(ofstream& outFile, const vector<Media*>& mediaLib) {
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    outFile << "              YOUR SONG LIST" << endl;
    int count = 1;
    for (const auto& media : mediaLib) {
        if (media->getType() == 'S') {
            outFile << count++ << " " << media->getTitle() << " " << media->getYearReleased() << " " << media->getRating() << endl;
            media->print(outFile, mediaLib);
        }
    }
}

void printBooks(ofstream& outFile, const vector<Media*>& mediaLib) {
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    outFile << "              YOUR BOOK LIST" << endl;
    outFile << "#           TITLE                             YEAR  RATING" << endl;
    int count = 1;
    for (const auto& media : mediaLib) {
        if (media->getType() == 'B') {
            outFile << left << setw(5) << count++
                << setw(45) << media->getTitle()
                << setw(10) << media->getYearReleased()
                << setw(10) << media->getRating() << endl;
            media->print(outFile, mediaLib); 
        }
    }
}



void printAllMedia(ofstream& outFile, const vector<Media*>& mediaLib) {
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    outFile << "              YOUR MEDIA LIST" << endl;
    outFile << "#           TITLE                             YEAR  RATING  GENRE                 TYPE" << endl;
    int count = 1;
    for (const auto& media : mediaLib) {
        outFile << left << setw(5) << count++
            << setw(45) << media->getTitle()
            << setw(10) << media->getYearReleased()
            << setw(10) << media->getRating()
            << setw(30) << media->getGenre()
            << media->getType() << endl;
        media->print(outFile, mediaLib);

    }
}

void printTotals(ofstream& outFile, const vector<Media*>& mediaLib) {
    int movieCount = 0, bookCount = 0, songCount = 0;
    for (const auto& media : mediaLib) {
        if (media->getType() == 'M') movieCount++;
        else if (media->getType() == 'B') bookCount++;
        else if (media->getType() == 'S') songCount++;
    }

    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    outFile << "              YOUR MEDIA LIBRARY" << endl;
    outFile << "#   TYPE" << endl;
    outFile << movieCount << "   Movies" << endl;
    outFile << bookCount << "   Books" << endl;
    outFile << songCount << "   Songs" << endl;
    outFile << (movieCount + bookCount + songCount) << "   Items" << endl;
}




// Helper function for case-insensitive string comparison
bool containsIgnoreCase(const string& str, const string& substr) {
    string strLower, substrLower;
    // Convert both strings to lowercase manually
    for (char c : str) {
        strLower += tolower(c);
    }
    for (char c : substr) {
        substrLower += tolower(c);
    }
    return strLower.find(substrLower) != string::npos;
}

// Function to print stars of a specific movie
void printMovieStars(ofstream& outFile, ofstream& errFile, const vector<Media*>& mediaLib, const string& movieTitle) {
    bool found = false;
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    outFile << "Stars in Movie: " << movieTitle << endl;

    for (const auto& media : mediaLib) {
        if (media->getType() == 'M' && media->getTitle() == movieTitle) {
            // We need to cast the Media pointer to a Movie pointer to access stars
            Movie* movie = dynamic_cast<Movie*>(media);
            if (movie) {
                vector<string> stars = movie->getStars();
                if (stars.empty()) {
                    outFile << "No stars listed for this movie." << endl;
                }
                else {
                    for (size_t i = 0; i < stars.size(); i++) {
                        outFile << (i + 1) << ". " << stars[i] << endl;
                    }
                }
                found = true;
                break;
            }
        }
    }

    if (!found) {
        errFile << "ERROR: Movie not found: " << movieTitle << endl;
    }
}


// Function to print stars by movie title
void printStarsByMovie(ofstream& outFile, const vector<Media*>& mediaLib, const string& movieTitle) {
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    outFile << "THE STARS OF THE MOVIE " << movieTitle << " ARE:" << endl;
    for (const auto& media : mediaLib) {
        if (media->getType() == 'M' && media->getTitle() == movieTitle) {
            const Movie* movie = dynamic_cast<const Movie*>(media);
            if (movie) {
                for (const auto& star : movie->getStars()) {
                    outFile << star << endl;
                }
            }
        }
    }
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++\n" << endl;
}

// Function to print movies by star
void printMoviesByStar(ofstream& outFile, const vector<Media*>& mediaLib, const string& starName) {
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    outFile << starName << " appears in the following movie(s):" << endl;
    for (const auto& media : mediaLib) {
        if (media->getType() == 'M') {
            const Movie* movie = dynamic_cast<const Movie*>(media);
            if (movie && find(movie->getStars().begin(), movie->getStars().end(), starName) != movie->getStars().end()) {
                outFile << movie->getTitle() << endl;
            }
        }
    }
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++\n" << endl;
}

// Function to print movies by a specific director
void printMoviesByDirector(ofstream& outFile, const vector<Media*>& mediaLib, const string& directorName) {
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    outFile << "The movie(s) directed by " << directorName << " are:" << endl;
    for (const auto& media : mediaLib) {
        if (media->getType() == 'M') {
            const Movie* movie = dynamic_cast<const Movie*>(media);
            if (movie && movie->getDirector() == directorName) {
                outFile << movie->getTitle() << endl;
            }
        }
    }
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++\n" << endl;
}

// Sample function to print songs by a particular artist (e.g., "Bee Gees" or "Beatles")
void printSongsByArtist(ofstream& outFile, const vector<Media*>& mediaLib, const string& artistName) {
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    outFile << artistName << "'s songs in the media library are:" << endl;
    for (const auto& media : mediaLib) {
        if (media->getType() == 'S') {
            const Song* song = dynamic_cast<const Song*>(media);
            if (song && song->getArtist() == artistName) {
                outFile << song->getTitle() << endl;
            }
        }
    }
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++\n" << endl;
}

// Function to find movies by actor name
void findMoviesByActor(ofstream& outFile, ofstream& errFile, const vector<Media*>& mediaLib, const string& actorName) {
    bool found = false;
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    outFile << "Movies starring: " << actorName << endl;
    outFile << "#    TITLE                             YEAR  RATING  GENRE" << endl;

    int count = 1;
    for (const auto& media : mediaLib) {
        if (media->getType() == 'M') {
            Movie* movie = dynamic_cast<Movie*>(media);
            if (movie) {
                vector<string> stars = movie->getStars();
                // Check if the actor is in the movie's star list
                for (const string& star : stars) {
                    if (star == actorName) {
                        outFile << left << setw(5) << count++
                            << setw(45) << movie->getTitle()
                            << setw(10) << movie->getYearReleased()
                            << setw(10) << movie->getRating()
                            << movie->getGenre() << endl;
                        found = true;
                        break;
                    }
                }
            }
        }
    }

    if (!found) {
        errFile << "ERROR: No movies found with actor: " << actorName << endl;
    }
}

// Function to find media by key name (title search)
void findMediaByKeyName(ofstream& outFile, ofstream& errFile, const vector<Media*>& mediaLib, const string& keyName) {
    bool found = false;
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    outFile << "Media containing keyword: " << keyName << endl;
    outFile << "#    TITLE                             YEAR  RATING  GENRE                 TYPE" << endl;

    int count = 1;
    for (const auto& media : mediaLib) {
        // Use our case-insensitive search function
        if (containsIgnoreCase(media->getTitle(), keyName)) {
            outFile << left << setw(5) << count++
                << setw(45) << media->getTitle()
                << setw(10) << media->getYearReleased()
                << setw(10) << media->getRating()
                << setw(30) << media->getGenre()
                << media->getType() << endl;
            found = true;
        }
    }

    if (!found) {
        errFile << "ERROR: No media found containing: " << keyName << endl;
    }
}


void processNewMedia(const string& commandRecord, ofstream& errFile, vector<Media*>& mediaLib) {
    stringstream ss(commandRecord.substr(2)); // Skip 'N ' at start
    string mediaInfo;
    if (!getline(ss, mediaInfo)) {
        errFile << "ERROR: Invalid new media command format" << endl;
        return;
    }

    stringstream mediaSS(mediaInfo);
    string type, title, name, ratingStr, genre, lengthStr, yearStr;
    string additionalInfo = ""; // For stars (movies), weeks (books), or top40 (songs)

    // Parse the comma-separated values
    if (!getline(mediaSS, type, ',') ||
        !getline(mediaSS, title, ',') ||
        !getline(mediaSS, name, ',') ||
        !getline(mediaSS, ratingStr, ',') ||
        !getline(mediaSS, genre, ',') ||
        !getline(mediaSS, lengthStr, ',') ||
        !getline(mediaSS, yearStr, ',')) {
        errFile << "ERROR: Invalid new media format: Missing required fields" << endl;
        return;
    }

    // Get the rest of the line for additional info
    getline(mediaSS, additionalInfo);

    // Validate and convert numeric values
    int rating, length, year;
    try {
        rating = stoi(ratingStr);
        length = stoi(lengthStr);
        year = stoi(yearStr);

        // Basic validation
        if (rating < 0 || rating > 10) throw invalid_argument("Invalid rating range");
        if (length <= 0) throw invalid_argument("Invalid length");
        if (year < 1920 || year > 2024) throw invalid_argument("Invalid year");
    }
    catch (const exception&) {
        errFile << "ERROR: Invalid numeric values in new media entry" << endl;
        return;
    }

    Media* newMedia = nullptr;

    // Process based on media type
    if (type == "M") {
        vector<string> stars;
        if (!additionalInfo.empty()) {
            stringstream starsSS(additionalInfo);
            string star;
            // Parse comma-separated stars
            while (getline(starsSS, star, ',')) {
                // Remove leading/trailing spaces
                star.erase(0, star.find_first_not_of(" "));
                star.erase(star.find_last_not_of(" ") + 1);
                if (!star.empty()) {
                    stars.push_back(star);
                }
            }
        }
        newMedia = new Movie('M', title, name, rating, genre, length, year, stars);
    }
    else if (type == "B") {
        int weeks = 0;
        if (!additionalInfo.empty()) {
            try {
                weeks = stoi(additionalInfo);
                if (weeks < 0) throw invalid_argument("Invalid weeks");
            }
            catch (const exception&) {
                errFile << "ERROR: Invalid weeks value for new book" << endl;
                
                return;
            }
        }
        newMedia = new Book('B', title, name, rating, genre, length, year, weeks);
    }
    else if (type == "S") {
        bool isTop40 = false;
        if (!additionalInfo.empty()) {
            isTop40 = (additionalInfo == "1" ||
                additionalInfo == "true" ||
                additionalInfo == "yes");
        }
        newMedia = new Song('S', title, name, rating, genre, length, year, isTop40);
    }
    else {
        errFile << "ERROR: Invalid media type '" << type << "'" << endl;
        return;
    }

    if (newMedia) {
        mediaLib.push_back(newMedia);
    }
}