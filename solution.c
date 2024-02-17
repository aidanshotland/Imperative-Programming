#include <stdio.h>
#include <stdlib.h>

// Structure to hold grayscale pixel data
typedef struct {
    unsigned char value;
} PixelGray;

// Function to read a PGM image into a 2D array
PixelGray** readPGM(const char* filename, int* width, int* height){
    FILE *file =fopen(filename, "r");
    if (file ==NULL){
        return NULL;
    }

    char eat[3];
    if(fscanf(file,"%2s",eat) !=1) {
        fclose(file);
        return NULL;
    }

    if(fscanf(file,"%d %d", width, height) != 2) {
        fclose(file);
        return NULL;
    }


    int max;
    if(fscanf(file, "%d", &max)!=1){
        fclose(file);
        return NULL;
    }
    fgetc(file);

    PixelGray **lenna = (PixelGray **)malloc(*height * sizeof(PixelGray *));

    for(int i =0; i<*height; i++){
        lenna[i]=(PixelGray *)malloc(*width * sizeof(PixelGray));
    }

    for(int i=0; i< *height; i++) {
        for (int j = 0; j < *width; j++) {
            fread(&lenna[i][j].value, sizeof(unsigned char), 1, file);
        }
    }
    fclose(file);
    return lenna;

}

// Function to write a 2D matrix as a PGM image
void writePGM(const char* filename, PixelGray** matrix, int* width, int* height){
    FILE *file = fopen(filename,"w");
    if (file ==NULL){
        return;
    }

    fprintf(file, "P5\n%d %d\n255\n", *width, *height);

    for(int i =0; i< *height; i++ ){
        for(int j =0; j< *width; j++){
            fwrite(&matrix[i][j].value, sizeof(unsigned char), 1, file);
        }
    }
    fclose(file);
}

// Function to threshold the image matrix
PixelGray** threshold(PixelGray** matrix, int* width, int* height){
    PixelGray **thresh_Matrix = (PixelGray **)malloc(*height * sizeof(PixelGray *));
    if(thresh_Matrix ==NULL){
        return NULL;
    }
    for(int i =0; i<*height; i++){
        thresh_Matrix[i]=(PixelGray *)malloc(*width * sizeof(PixelGray));
        if(thresh_Matrix[i]==NULL){
            return NULL;
        }
    }

    for(int i =0; i< *height; i++){
        for(int j =0; j< *width; j++) {
            if (matrix[i][j].value > 80) {
                thresh_Matrix[i][j].value = 255;
            }
            else{
                thresh_Matrix[i][j].value =0;
            }
        }
    }

    return thresh_Matrix;
}

// Function to rotate the image matrix
PixelGray** rotate(PixelGray** matrix, int* width, int* height){
    PixelGray **rot_Matrix = (PixelGray **) malloc(*height * sizeof(PixelGray *));
    if(rot_Matrix==NULL){
        return NULL;
    }

    for (int i = 0; i < *height; i++) {
        rot_Matrix[i] = (PixelGray *) malloc(*width * sizeof(PixelGray));
        if(rot_Matrix[i]==NULL){
            return NULL;
        }
    }

    for (int i =0; i < *height; i++) {
        for (int j =0; j < *width; j++) {
            rot_Matrix[j][i].value = matrix[i][j].value;
        }
    }

    int temp = *width;
    *width = *height;
    *height = temp;


    return rot_Matrix;
}


//main function - DO NOT MODIFY
int main() {
    int width, height;  // variable to hold width and height. Use reference in other functions

    PixelGray** image_original = readPGM("lenna.pgm", &width, &height);
    // Now you have the grayscale image data in the 'image_original' 2D array

    // Access pixel data using image[row][col].value
    // For example, to access the pixel at row=2, col=3:
    // unsigned char pixel_value = image[2][3].value;

    // Create a new 2D array 'image_thresh' to store the threshold image
    PixelGray** image_thresh = threshold(image_original, &width, &height);
    //write the image data as "threshold.pgm"
    writePGM("threshold.pgm", image_thresh, &width, &height);

    // Create a new 2D array 'image_rotate' to store the rotated image
    PixelGray** image_rotate = rotate(image_original, &width, &height);
    //write the image data as "rotate.pgm"
    writePGM("rotate.pgm", image_rotate, &width, &height);

    image_rotate = rotate(image_rotate, &width, &height);
    //write the image data as "rotate_again.pgm"
    writePGM("rotate_again.pgm", image_rotate, &width, &height);

    // Free the allocated memory when you're done
    for (int i = 0; i < height; ++i) {
        free(image_original[i]);
        free(image_thresh[i]);
        free(image_rotate[i]);
    }
    free(image_original);
    free(image_thresh);
    free(image_rotate);
    return 0;
}