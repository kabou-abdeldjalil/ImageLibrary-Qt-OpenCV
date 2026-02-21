#ifndef IMAGE_DESCRIPTOR_H
#define IMAGE_DESCRIPTOR_H

#include <string>

enum class AccessLevel {
    PUBLIC,
    EDITOR,
    ADMIN
};

class ImageDescriptor {
private:
    int numero;
    std::string titre;
    std::string source;
    std::string motCle;
    double cout;
    AccessLevel access;

public:
    ImageDescriptor(int numero,
                    const std::string& titre,
                    const std::string& source,
                    const std::string& motCle,
                    double cout,
                    AccessLevel access);

    int getNumero() const;
    std::string getMotCle() const;
    AccessLevel getAccessLevel() const;


    void display() const;

    double getCout() const;
    const std::string& getTitre() const;
    const std::string& getSource() const;
    std::string toString() const;

};

#endif

