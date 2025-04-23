#include "Utils.h"

sf::Color Utils::getColorForType(ParticleType type)
{
    // TODO: Make a random variation in brightness of color 
    switch (type) {
    case ParticleType::SAND:  return sf::Color(194, 178, 128);
    case ParticleType::SANDWET:  return sf::Color(144, 128, 78);
    case ParticleType::DIRT:  return sf::Color(133, 94, 66);
    case ParticleType::GRASS: return sf::Color(40, 140, 40);
    case ParticleType::WATER: return sf::Color(60, 120, 180);
    case ParticleType::SILT:  return sf::Color(115, 105, 90);
    case ParticleType::OIL:  return sf::Color(90, 30, 30);

    case ParticleType::EMPTY: return sf::Color::White;
    default:                  return sf::Color::Black; // Unknown particles are black
    }
}

std::string Utils::particleTypeToString(ParticleType type)
{
    std::string particleTypeName;
    switch (type) {
    case ParticleType::SAND:  particleTypeName = "Sand"; break;
    case ParticleType::SANDWET:  particleTypeName = "Wet Sand"; break;
    case ParticleType::DIRT:  particleTypeName = "Dirt"; break;
    case ParticleType::GRASS: particleTypeName = "Grass"; break;
    case ParticleType::WATER: particleTypeName = "Water"; break;
    case ParticleType::SILT:  particleTypeName = "Silt"; break;
    case ParticleType::OIL:  particleTypeName = "Oil"; break;
    case ParticleType::EMPTY: particleTypeName = "Empty"; break;
    default:                  particleTypeName = "Unknown"; break;
    }
    return particleTypeName;
}
