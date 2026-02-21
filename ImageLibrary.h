#ifndef IMAGE_LIBRARY_H
#define IMAGE_LIBRARY_H

#include "ImageDescriptor.h"
#include "User.h"
#include <string>

class ImageLibrary {
private:
    struct Node {
        ImageDescriptor descriptor;
        Node* next;
        Node(const ImageDescriptor& d) : descriptor(d), next(nullptr) {}
    };

    Node* head;

public:
    ImageLibrary();
    ~ImageLibrary();

    void addImage(const ImageDescriptor& descriptor);
    bool removeImage(int numero, const User& user);

    void displayAll(const User& user) const;
    void displayByKeyword(const std::string& keyword, const User& user)
    const;

    ImageDescriptor* findByNumero(int numero);

    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
    void displayByCostRange(double minC, double maxC, const User& user) const;
    bool canAccess(const ImageDescriptor& desc, const User& user) const;

    std::string dumpAllAccessible(const User& user) const;
    std::string dumpByCostRange(double minC, double maxC, const User& user) const;

};

#endif
