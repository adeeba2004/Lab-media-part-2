#include "Media.h"
#include <iomanip>
// Media Implementation
Media::Media() : type(' '), title(""), keyName(""), rating(-1), genre(""), length(-1), yearReleased(-1) {}

Media::Media(char t, string ttl, string n, int r, string g, int l, int yr)
    : type(t), title(ttl), keyName(n), rating(r), genre(g), length(l), yearReleased(yr) {}

void Media::setType(char c) { type = c; }
void Media::setTitle(string t) { title = t; }
void Media::setKeyName(string n) { keyName = n; }
void Media::setRating(int r) { rating = r; }
void Media::setGenre(string g) { genre = g; }
void Media::setLength(int l) { length = l; }
void Media::setYearReleased(int yr) { yearReleased = yr; }

char Media::getType() const { return type; }
string Media::getTitle() const { return title; }
string Media::getKeyName() const { return keyName; }
int Media::getRating() const { return rating; }
string Media::getGenre() const { return genre; }
int Media::getLength() const { return length; }
int Media::getYearReleased() const { return yearReleased; }

// Movie Implementation
Movie::Movie() : Media(), stars() {}

Movie::Movie(char type, string title, string keyName, int rating, string genre,
    int length, int year, vector<string> s)
    : Media(type, title, keyName, rating, genre, length, year), stars(s) {}

vector<string> Movie::getStars() const { return stars; }
void Movie::setStars(vector<string> s) { stars = s; }

void Movie::print(ostream& out, const vector<Media*>& mediaLib) const {
    out << left << setw(45) << title
        << setw(10) << yearReleased
        << setw(10) << rating
        << setw(30) << genre
        << "Stars: ";
    for (size_t i = 0; i < stars.size(); i++) {
        out << stars[i];
        if (i < stars.size() - 1) out << ", ";
    }
    out << endl;
}

// Book Implementation
Book::Book() : Media(), weeksNYT(0) {}

Book::Book(char type, string title, string keyName, int rating, string genre,
    int length, int year, int weeks)
    : Media(type, title, keyName, rating, genre, length, year), weeksNYT(weeks) {}

int Book::getWeeks() const { return weeksNYT; }
void Book::setWeeks(int w) { weeksNYT = w; }

void Book::print(ostream& out, const vector<Media*>& mediaLib) const {
    out << left << setw(45) << title
        << setw(10) << yearReleased
        << setw(10) << rating;
    if (weeksNYT > 0) {
        out << "Weeks NYT: " << weeksNYT;
    }
    out << endl;
}

// Song Implementation
Song::Song() : Media(), top40(false) {}

Song::Song(char type, string title, string keyName, int rating, string genre,
    int length, int year, bool isTop40)
    : Media(type, title, keyName, rating, genre, length, year), top40(isTop40) {}

bool Song::getTop40() const { return top40; }
void Song::setTop40(bool t) { top40 = t; }

void Song::print(ostream& out, const vector<Media*>& mediaLib) const {
    out << left << setw(45) << title
        << setw(10) << yearReleased
        << setw(10) << rating
        << setw(30) << genre;
    if (top40) out << "(Top 40 Hit)";
    out << endl;
}