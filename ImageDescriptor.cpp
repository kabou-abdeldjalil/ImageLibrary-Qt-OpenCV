#include "ImageDescriptor.h"
#include <iostream>
#include <sstream>

ImageDescriptor::ImageDescriptor(int numero,
                                 const std::string& titre,
                                 const std::string& source,
                                 const std::string& motCle,
                                 double cout,
                                 AccessLevel access)
    : numero(numero),
      titre(titre),
      source(source),
      motCle(motCle),
      cout(cout),
      access(access) {}

int ImageDescriptor::getNumero() const {return numero;}
const std::string& ImageDescriptor::getTitre() const { return titre; }
const std::string& ImageDescriptor::getSource() const { return source; }
std::string ImageDescriptor::getMotCle() const {return motCle;}
double ImageDescriptor::getCout() const { return cout; }
AccessLevel ImageDescriptor::getAccessLevel() const {return access;}

void ImageDescriptor::display() const {
    std::cout << "Numero: " << numero << std::endl;
    std::cout << "Titre: " << titre << std::endl;
    std::cout << "Source: " << source << std::endl;
    std::cout << "Mot-cle: " << motCle << std::endl;
    std::cout << "Cout: " << cout << std::endl;
}


std::string ImageDescriptor::toString() const {
    std::ostringstream os;
    os << "Numero: " << numero << "\n"
       << "Titre: " << titre << "\n"
       << "Source: " << source << "\n"
       << "Mot-cle: " << motCle << "\n"
       << "Cout: " << cout << "\n";
    return os.str();
}
