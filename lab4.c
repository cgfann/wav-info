/**
 * This program reads a WAV audio file and prints statistics about the audio samples. The file name 
 * is provided using command line arguments. If the file name is not provided or the file is not readable, 
 * the program will exit and provide an error message.
 *
 * @author Your Name Here {@literal <pluf@wfu.edu>}
 * @date Mar. 7, 2021
 * @assignment Lab 4  
 * @course CSC 250
 **/

#include <stdio.h>
#include <stdlib.h>  
#include <string.h>  
#include <math.h>  


int read_wav_header(FILE* in_file, short *sample_size_ptr, int *num_samples_ptr, int *sample_rate_ptr);
int read_wav_data(FILE* in_file, short sample_size, int num_samples, int sample_rate);


int main(int argc, char *argv[]) {
    FILE *in_file;      /* WAV file */
    short sample_size;  /* size of an audio sample (bits) */
    int sample_rate;    /* sample rate (samples/second) */
    int num_samples;    /* number of audio samples */ 
    int wav_ok = 0;     /* 1 if the WAV file si ok, 0 otherwise */

    if(argc < 2) {
        printf("usage: %s wav_file \n", argv[0]);
        return 1;
    }

    in_file = fopen(argv[1], "rbe"); 
    if(!in_file) {
        printf("could not open wav file %s \n", argv[1]);
        return 2;
    }

    wav_ok = read_wav_header(in_file, &sample_size, &num_samples, &sample_rate);
    if(!wav_ok) {
       printf("wav file %s has incompatible format \n", argv[1]);   
       return 3;
    }
    else
        read_wav_data(in_file, sample_size, num_samples, sample_rate);

    if(in_file) fclose(in_file);
    return 0;
}


/**
 *  function reads the RIFF, fmt, and start of the data chunk. 
 */
int read_wav_header(FILE* in_file, short *sample_size_ptr, int *num_samples_ptr, int *sample_rate_ptr) {
    char chunk_id[] = "    ";  /* chunk id, note initialize as a C-string */
    char data[] = "    ";      /* chunk data */
    int chunk_size = 0;        /* number of bytes remaining in chunk */
    short audio_format = 0;    /* audio format type, PCM = 1 */
    short num_channels = 0;    /* number of audio channels */ 

    /* first chunk is the RIFF chunk, let's read that info */  
    fread(chunk_id, 1, 4, in_file);
    fread(&chunk_size, 1, 4, in_file);
    printf("chunk: %s, size: %d \n", chunk_id, chunk_size);
    fread(data, 1, 4, in_file);
    printf("  data: %s \n", data);

    /* let's try to read the next chunk, it always starts with an id */
    fread(chunk_id, 1, 4, in_file);
    /* if the next chunk is not "fmt " then let's skip over it */  
    while(strcmp(chunk_id, "fmt ") != 0) {
        fread(&chunk_size, 1, 4, in_file);
        /* skip to the end of this chunk */  
        fseek(in_file, chunk_size, SEEK_CUR);
        /* read the id of the next chuck */  
        fread(chunk_id, 1, 4, in_file);
    }  

    /* if we are here, then we must have the fmt chunk, now read that data */  
    fread(&chunk_size, 1, 4, in_file);
    fread(&audio_format, 1,  sizeof(audio_format), in_file);
    fread(&num_channels, 1,  sizeof(num_channels), in_file);
    /* you'll need more reads here, hear? */  

    printf("chunk: %s, size: %d \n", chunk_id, chunk_size);
    printf(" audio_format: %d \n", audio_format);

    /* read the data chunk next, use another while loop (like above) */
    /* visit http://goo.gl/rxnHB1 for helpful advice */

    return (audio_format == 1);
}


/**
 *  function reads the WAV audio data (last part of the data chunk)
 */
int read_wav_data(FILE* in_file, short sample_size, int num_samples, int sample_rate) {

   return 1;
}



