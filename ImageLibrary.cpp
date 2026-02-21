#include "ImageLibrary.h"
#include "User.h"
#include "ImageDescriptor.h"

#include <iostream>   
#include <fstream>    
#include <sstream>    



// Constructeur
ImageLibrary::ImageLibrary() : head(nullptr) {}

// Destructeur
ImageLibrary::~ImageLibrary() {
    Node* current = head;
    while (current) {
        Node* tmp = current;
        current = current->next;
        delete tmp;
    }
}

bool ImageLibrary::canAccess(const ImageDescriptor& desc, const User& user) const
{
    // Niveau 2 : tout
    if (user.getLevel() == UserLevel::LEVEL2) return true;

    // Niveau 1 : seulement PUBLIC
    if (user.getLevel() == UserLevel::LEVEL1) {
        return desc.getAccessLevel() == AccessLevel::PUBLIC;
    }

    // Niveau 0 : rien
    return false;
}

bool ImageLibrary::removeImage(int numero, const User& user)
{
    if (user.getLevel() != UserLevel::LEVEL2) {
        std::cout << "Acces refuse : suppression interdite\n";
        return false;
    }

    Node* current = head;
    Node* previous = nullptr;

    while (current) {
        if (current->descriptor.getNumero() == numero) {
            if (previous) previous->next = current->next;
            else head = current->next;

            delete current;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;
}

void ImageLibrary::addImage(const ImageDescriptor& descriptor) {
    Node* newNode = new Node(descriptor);
    newNode->next = head;
    head = newNode;
}



void ImageLibrary::displayAll(const User& user) const {
    Node* current = head;
    while (current) {
        if (canAccess(current->descriptor, user)) {
            current->descriptor.display();
            std::cout << "------------------\n";
        }
        current = current->next;
    }
}

void ImageLibrary::displayByKeyword(const std::string& keyword, const User& user) const {
    Node* current = head;
    while (current) {
        if (current->descriptor.getMotCle() == keyword &&
            canAccess(current->descriptor, user)) {
            current->descriptor.display();
            std::cout << "------------------\n";
        }
        current = current->next;
    }
}

void ImageLibrary::displayByCostRange(double minCost, double maxCost, const User& user) const {
    Node* current = head;
    bool found = false;

    while (current) {
        const auto& d = current->descriptor;
        const double c = d.getCout();

        if (c >= minCost && c <= maxCost && canAccess(d, user)) {
            d.display();
            std::cout << "------------------\n";
            found = true;
        }
        current = current->next;
    }

    if (!found) {
        std::cout << "(Aucune image accessible dans cette plage de cout)\n";
    }
}

ImageDescriptor* ImageLibrary::findByNumero(int numero) {
    Node* current = head;
    while (current) {
        if (current->descriptor.getNumero() == numero)
            return &current->descriptor;
        current = current->next;
    }
    return nullptr;
}

bool ImageLibrary::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur ouverture fichier (ecriture)\n";
        return false;
    }

    Node* current = head;
    while (current) {
        const auto& img = current->descriptor;
        file << img.getNumero() << ";"
             << img.getTitre() << ";"
             << img.getSource() << ";"
             << img.getMotCle() << ";"
             << img.getCout() << ";"
             << static_cast<int>(img.getAccessLevel())
             << "\n";
        current = current->next;
    }

    return true;
}

bool ImageLibrary::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur ouverture fichier (lecture)\n";
        return false;
    }

    // vider la liste chainee
    Node* current = head;
    while (current) {
        Node* tmp = current;
        current = current->next;
        delete tmp;
    }
    head = nullptr;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;

        int numero;
        std::string titre, source, motCle;
        double cout;
        int accessInt;

        std::getline(ss, token, ';'); numero = std::stoi(token);
        std::getline(ss, titre, ';');
        std::getline(ss, source, ';');
        std::getline(ss, motCle, ';');
        std::getline(ss, token, ';'); cout = std::stod(token);
        std::getline(ss, token, ';'); accessInt = std::stoi(token);

        ImageDescriptor img(numero, titre, source, motCle, cout,
                            static_cast<AccessLevel>(accessInt));

        addImage(img);
    }

    return true;
}


std::string ImageLibrary::dumpAllAccessible(const User& user) const
{
    std::ostringstream os;
    Node* current = head;
    while (current) {
        if (canAccess(current->descriptor, user)) {
            os << current->descriptor.toString();
            os << "------------------\n";
        }
        current = current->next;
    }
    return os.str();
}

std::string ImageLibrary::dumpByCostRange(double minCost, double maxCost, const User& user) const
{
    std::ostringstream os;
    Node* current = head;
    bool found = false;

    while (current) {
        const auto& d = current->descriptor;
        double c = d.getCout();

        if (c >= minCost && c <= maxCost && canAccess(d, user)) {
            os << d.toString();
            os << "------------------\n";
            found = true;
        }
        current = current->next;
    }

    if (!found) {
        os << "(Aucune image accessible dans cette plage de coût)\n";
    }
    return os.str();
}

