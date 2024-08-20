#include <iostream>  
#include <opencv2/opencv.hpp>          

// Function to build an image pyramid
void imagePyramid(const cv::Mat& originalImage, int levels)
{
    cv::Mat currentImage = originalImage; // Start with the original image
    for (int i = 0; i < levels; ++i) // For-loop to iterate through the number of levels specified
    {
        {
            // Create a window name for the current level
            std::string windowName = "Pyramid Level " + std::to_string(i);
            // Display the image at the current level
            cv::imshow(windowName, currentImage);
            cv::waitKey(0); // Wait for a key press

            // Apply 2x2 block filtering and downsample the image to the next level
            cv::resize(currentImage, currentImage, cv::Size(), 0.5, 0.5, cv::INTER_NEAREST);
        }
    }
}

int main()
{
    // Load the image from the specified file path (Replace the path with one to a photo on your computer.)
    cv::Mat theImage = cv::imread("C:\\Users\\aaron\\Desktop\\Fam_Pic.jpeg", cv::IMREAD_COLOR); 
    if (theImage.empty()) // Check if the image was loaded successfully
    {
        std::cerr << "Error loading image!" << std::endl;
        return -1; // Exit if the image couldn't be loaded
    }

    // Prompt the user to enter the number of pyramid levels
    int levels;
    std::cout << "Enter the number of pyramid levels that you would like: ";
    std::cin >> levels; // Read the number of levels from user input

    // Build the pyramid using the specified number of levels
    imagePyramid(theImage, levels);

    // Perform 2x2 block filtering (downsampling by half) - already done in the pyramid
    cv::Mat blockFilter = theImage.clone(); // Clone the image to create a copy
    cv::resize(theImage, blockFilter, cv::Size(), 0.5, 0.5, cv::INTER_NEAREST);

    // Apply Burt and Adelson’s binomial kernel filter
    cv::Mat binomialFilter;
    cv::Mat kernel = (cv::Mat_<float>(1, 5) << 1, 4, 6, 4, 1) / 16; // Define the binomial kernel
    cv::sepFilter2D(theImage, binomialFilter, -1, kernel, kernel); // Apply separable filtering
    cv::resize(binomialFilter, binomialFilter, cv::Size(), 0.5, 0.5, cv::INTER_LINEAR); // Downsample the filtered image

    // Apply a high-quality 7-tap filter
    cv::Mat sevenTapFilter;
    float sevenTapData[] = { 0.004, 0.054, 0.242, 0.4, 0.242, 0.054, 0.004 }; // Define the 7-tap filter coefficients
    cv::Mat sevenTapKernel(1, 7, CV_32F, sevenTapData); // Create the kernel matrix
    cv::sepFilter2D(theImage, sevenTapFilter, -1, sevenTapKernel, sevenTapKernel); // Apply separable filtering
    cv::resize(sevenTapFilter, sevenTapFilter, cv::Size(), 0.5, 0.5, cv::INTER_LINEAR); // Downsample the filtered image

    // Shift the image by 1 pixel in both x and y directions
    cv::Mat shiftedImage;
    cv::Mat M = (cv::Mat_<double>(2, 3) << 1, 0, 1, 0, 1, 1); // Define the transformation matrix for shifting
    cv::warpAffine(theImage, shiftedImage, M, theImage.size()); // Apply the affine transformation to shift the image

    // Display the filtered and shifted images
    cv::imshow("2x2 Block Filter", blockFilter);
    cv::imshow("Binomial Kernel Filter", binomialFilter);
    cv::imshow("7-Tap Filter", sevenTapFilter);
    cv::imshow("Shifted Image", shiftedImage);

    cv::waitKey(0); // Wait for a key press

    // Save the filtered and shifted images to files
    cv::imwrite("blockFilter.png", blockFilter);
    cv::imwrite("binomialFilter.png", binomialFilter);
    cv::imwrite("sevenTapFilter.png", sevenTapFilter);
    cv::imwrite("shiftedImg.png", shiftedImage);
}