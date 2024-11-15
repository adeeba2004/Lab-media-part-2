#include "Movie.h"

Movie::Movie(const std::string& title, const std::string& director, int releaseYear)
    : Media(title, releaseYear), director(director) {}

std::string Movie::getDirector() const {
    return director;  // Implement this function
}
