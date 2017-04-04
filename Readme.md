Qt GUI program using that compare's the performance of basic Filters done by OpenCV functions and without them.

Filters Included on the Program
	Sobel Derivative
		Gx
		Gy
		Gx-Gy
	Grayscale Filter
	Binary Filter
	Erode Filter
	Dilate Filter

In the GUI appears two different images.
	On the left side will be the image processed by OpenCV Functions
	On the Right side will be the image processed Without OpenCV Functions

On the middle parte will appear the Execution time done by both of the functions to allow the user see the contrast between the functions.


The images that will appear on the program are stored on the images/ folder where more images can be added to try with different images.

Setup
	Open .pro with QtCreator
	Change the main_directory = "/YourPath/images"
	

The program Still on Development and have some missing filters and also had some buttons without directions.

