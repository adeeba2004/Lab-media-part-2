#pragma once
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Media {
protected:
    char type;
    string title;
    string keyName;
    int rating;
    string genre;
    int length;
    int yearReleased;

public:
    Media();
    Media(char, string, string, int, string, int, int);
    virtual ~Media() = default;

    void setType(char c);
    void setTitle(string t);
    void setKeyName(string n);
    void setRating(int r);
    void setGenre(string g);
    void setLength(int l);
    void setYearReleased(int yr);

    char getType() const;
    string getTitle() const;
    string getKeyName() const;
    int getRating() const;
    string getGenre() const;
    int getLength() const;
    int getYearReleased() const;

    // Pure virtual function making Media an abstract class
    virtual void print(ostream& out, const vector<Media*>& mediaLib) const = 0;
};

class Movie : public Media {
private:
    vector<string> stars;

public:
    Movie();
    Movie(char type, string title, string keyName, int rating, string genre,
        int length, int year, vector<string> stars);

    vector<string> getStars() const;
    void setStars(vector<string> s);
    virtual void print(ostream& out, const vector<Media*>& mediaLib) const override;
};

class Book : public Media {
private:
    int weeksNYT;

public:
    Book();
    Book(char type, string title, string keyName, int rating, string genre,
        int length, int year, int weeks);

    int getWeeks() const;
    void setWeeks(int w);
    virtual void print(ostream& out, const vector<Media*>& mediaLib) const override;
};

class Song : public Media {
private:
    bool top40;

public:
    Song();
    Song(char type, string title, string keyName, int rating, string genre,
        int length, int year, bool isTop40);

    bool getTop40() const;
    void setTop40(bool t);
    virtual void print(ostream& out, const vector<Media*>& mediaLib) const override;
};