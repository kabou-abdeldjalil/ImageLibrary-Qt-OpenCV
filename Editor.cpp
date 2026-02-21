#include "Editor.h"
#include <opencv2/opencv.hpp>

Editor::Editor(int id, const std::string& name)
    : User(id, name, "Editor") {}

void Editor::convertToGray(Image& image) const {
    if (!image.isLoaded()) return;

    cv::Mat& src = image.getDataMutable();

    // Création d'une image grayscale
    cv::Mat gray(src.rows, src.cols, CV_8UC1);

    for (int y = 0; y < src.rows; ++y) {
        for (int x = 0; x < src.cols; ++x) {
            cv::Vec3b pixel = src.at<cv::Vec3b>(y, x);

            unsigned char grayValue =
                static_cast<unsigned char>(
                    0.299 * pixel[2] +
                    0.587 * pixel[1] +
                    0.114 * pixel[0]
                );

            gray.at<unsigned char>(y, x) = grayValue;
        }
    }

    // On remplace l'image originale
    src = gray;
}
void Editor::blurMean3x3(Image& image) const {
    if (!image.isLoaded()) return;

    cv::Mat src = image.getData();           // copie
    cv::Mat& dst = image.getDataMutable();   // image résultat

    for (int y = 1; y < src.rows - 1; ++y) {
        for (int x = 1; x < src.cols - 1; ++x) {

            int sum = 0;

            for (int j = -1; j <= 1; ++j) {
                for (int i = -1; i <= 1; ++i) {
                    sum += src.at<cv::Vec3b>(y + j, x + i)[0];
                }
            }

            uchar mean = static_cast<uchar>(sum / 9);
            dst.at<cv::Vec3b>(y, x) = cv::Vec3b(mean, mean, mean);
        }
    }
}


void Editor::resizeImage(Image& image, int newWidth, int newHeight) const {
    if (!image.isLoaded()) return;

    const cv::Mat& src = image.getData();
    cv::Mat dst(newHeight, newWidth, src.type());

    float scaleX = static_cast<float>(src.cols) / newWidth;
    float scaleY = static_cast<float>(src.rows) / newHeight;

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            int srcX = static_cast<int>(x * scaleX);
            int srcY = static_cast<int>(y * scaleY);

            dst.at<cv::Vec3b>(y, x) = src.at<cv::Vec3b>(srcY, srcX);
        }
    }

    image.getDataMutable() = dst;
}



void Editor::displayInfo() const {
    User::displayInfo();
    std::cout << "Permissions: image editing" << std::endl;
}
void Editor::threshold(Image& image, unsigned char T) const {
    if (!image.isLoaded()) return;

    cv::Mat& img = image.getDataMutable();

    for (int y = 0; y < img.rows; ++y) {
        for (int x = 0; x < img.cols; ++x) {
            unsigned char pixel = img.at<unsigned char>(y, x);

            if (pixel >= T)
                img.at<unsigned char>(y, x) = 255;
            else
                img.at<unsigned char>(y, x) = 0;
        }
    }
}
