# image-processing
## This readme is now just a task task and basic explanation it will be updated for the sake of understanding and clarity
## What is the image processing?   
Image processing is the manipulation of digital images using algorithms to improve their quality (enhancement), extract useful information (analysis), or alter them. It involves treating images as two-dimensional signals to perform tasks like noise reduction, contrast adjustment, segmentation, and compression for better storage or interpretation.    

## 1. Image Represantation in Memory  
In general, two-dimensional images are stored in a linear memory buffer, so that all   
image rows are placed in memory consecutively.    
The position of the image in memory is stored in pointer first refers to the first pixel.   
## 2. Accessing an Image   
We access and manipulate the image by scaling down half width and half height of the original.  
We declare variables to be const in order to make clear we do not want the value to be changed.   
Using consts make code easier to understand and read by others and also it will avoid errors.    
The scaled image is calculated by simply copying every second pixel from every second
row of the original image to the new one. This is the easiest and fastest possible way to
do the desired scaling. Although it does not yield the best possible results, it allows to
show nicely how image pixels are accessed.    
## 3. Filtering   
The scaled image may look hard-edged or grainy.   
We can make use of a popular smoothing filter to improve this:    
The Gaussian filter performs a convolution using weights approximating a 2-dimensional Gaussian (bell-shaped) distribution   
We use filterGauss 1x3 for starting second and finishing before-last row   
then for columns filterGauss 3x1 and then for border handling filterGauss3x3 is used.    
## 4. Histograms and Image Enhancment   
For further computations it makes sense to improve quality of the image and we use histograms for that.  
#### What is the Histogram   
An image histogram is a graph representing the distribution of pixel intensities, plotting the number of pixels (y-axis) for each brightness level (x-axis). It provides a concise summary of an image's tonal range, contrast, and brightness, enabling techniques like thresholding and contrast enhancement.   
## 5. Gradients, energy and segmentation    
In image processing, Gradients identify edges by measuring sharp changes in pixel intensity, acting as the fundamental building blocks for shape detection. These gradients are often used to calculate a pixel's Energy, a value that maps the "importance" of information to determine which areas should be preserved or can be compressed. Finally, Segmentation uses these features to partition the image into meaningful groups, allowing computers to isolate and identify specific objects rather than just raw pixels. Together, they transform a grid of numbers into a structured understanding of a visual scene.     


