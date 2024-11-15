
#pragma once
#include "Media.h"
#include <string>

class Movie : public Media {
private:
    std::string director;  // Make sure this attribute exists
public:
    Movie(const std::string& title, const std::string& director, int releaseYear);

    // Getter for director
    std::string getDirector() const;  // Add this declaration if missing
};

