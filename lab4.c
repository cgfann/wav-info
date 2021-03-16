/**
 * This program reads a WAV audio file and prints statistics about the audio samples. The file name 
 * is provided using command line arguments. If the file name is not provided or the file is not readable, 
 * the program will exit and provide an error message.
 *
 * @author Charlotte Fanning {@literal <fanncg18@wfu.edu>}
 * @date Mar. 15, 2021
 * @assignment Lab 4  
 * @course CSC 250
 **/

#include <math.h>  
#include <stdio.h>
#include <stdlib.h>  
#include <string.h>  


int read_wav_header(FILE *in_file, short *sample_size_ptr, int *num_samples_ptr, int *sample_rate_ptr, int *num_channels_ptr);
void read_wav_data(FILE* in_file, short sample_size, int num_samples, int sample_rate, int num_channels);


int main(int argc, char *argv[]) 
{
    FILE *in_file;      /* WAV file */
    short sample_size;  /* size of an audio sample (bits) */
    int num_samples;    /* number of audio samples */ 
    int sample_rate;    /* sample rate (samples/second) */
    int num_channels;
    int wav_ok = 0;     /* 1 if the WAV file is ok, 0 otherwise */

    if (argc < 2) {
        printf("usage: %s wav_file \n", argv[0]);
        return 1;
    }

    in_file = fopen(argv[1], "rbe"); 
    if (!in_file) {
        printf("could not open wav file %s \n", argv[1]);
        return 2;
    }

    wav_ok = read_wav_header(in_file, &sample_size, &num_samples, &sample_rate, &num_channels);
    if (!wav_ok) {
       printf("wav file %s has incompatible format \n", argv[1]);   
       return 3;
    }
    read_wav_data(in_file, sample_size, num_samples, sample_rate, num_channels);   

    if (in_file) {
        fclose(in_file);
    }
    return 0;
}


/**
 *  function reads the RIFF, fmt, and start of the data chunk, prints relevant information 
 **/
int read_wav_header(FILE *in_file, short *sample_size_ptr, int *num_samples_ptr, int *sample_rate_ptr, int *num_channels_ptr) 
{
    char chunk_id[] = "    ";  /* chunk id, note initialize as a C-string */
    char data[] = "    ";      /* chunk data */
    int chunk_size = 0;        /* number of bytes remaining in chunk */
    short audio_format = 0;    /* audio format type, PCM = 1 */
    short num_channels = 0;    /* number of audio channels */ 
    int sample_rate = 0;
    short bits_per_smp = 0; 
    int num_samples = 0;

    /* read info in RIFF chunk, chunk id, chunk size, chunk format */  
    fread(chunk_id, 4, 1, in_file);
    fread(&chunk_size, 4, 1, in_file);
    printf("chunk: %s \n", chunk_id);
    fread(data, 4, 1, in_file);
    printf("  data: %s \n", data);

    /* let's try to read the next chunk, it always starts with an id */
    fread(chunk_id, 4, 1, in_file);
    /* if the next chunk is not "fmt " then skip over it */  
    while(strcmp(chunk_id, "fmt ") != 0) {
        fread(&chunk_size, 4, 1, in_file);
        /* skip to the end of this chunk */  
        fseek(in_file, chunk_size, SEEK_CUR);
        /* read the id of the next chuck */  
        fread(chunk_id, 4, 1, in_file);
    }  

    /* if we are here, then we must have the fmt chunk, now read that data */  
    fread(&chunk_size, 4, 1, in_file);
    fread(&audio_format, 1,  sizeof(audio_format), in_file);
    fread(&num_channels, 1,  sizeof(num_channels), in_file);
    fread(&sample_rate, 1, sizeof(sample_rate), in_file);
    fseek(in_file, 6, SEEK_CUR);    /* skip byte rate and block align */
    fread(&bits_per_smp, 1, sizeof(bits_per_smp), in_file);

    printf("chunk: %s \n", chunk_id);
    printf(" audio format: %d \n", audio_format);
    printf(" num channels: %d \n", num_channels);
    printf(" sample rate: %d \n", sample_rate);
    printf(" bits per sample: %d \n", bits_per_smp);

    fread(&chunk_id, 4, 1, in_file);
    while(strcmp(chunk_id, "data") != 0) {
        fread(&chunk_size, 4, 1, in_file);
        /* skip to the end of this chunk */  
        fseek(in_file, chunk_size, SEEK_CUR);
        /* read the id of the next chuck */  
        fread(chunk_id, 4, 1, in_file);
    }

    fread(&num_samples, 1, sizeof(num_samples), in_file);   /* total bytes of data*/
    *sample_rate_ptr = sample_rate;
    *sample_size_ptr = bits_per_smp / 8;                    /* sample size in bytes */
    *num_samples_ptr = (num_samples / num_channels) / (*sample_size_ptr); 
    *num_channels_ptr = num_channels;
    printf("chunk: %s \n", chunk_id);

    return (audio_format == 1);
}


/**
 *  function reads the WAV audio data (last part of the data chunk)
 **/
void read_wav_data(FILE *in_file, short sample_size, int num_samples, int sample_rate, int num_channels) 
{
    float duration;
    int right = 1;               /* 0 if left channel, 1 if right channel */
    int curr_sample = 0;
    int max_left = 0;
    int max_right = 0;
    int i;

    /* read samples, byte by byte depending on number of channels and sample size */
    for (i = 0; i < num_samples * num_channels; i++) {
        right = !right;
        fread(&curr_sample, sample_size, 1, in_file);

        if (sample_size == 2) {
            curr_sample = abs((short)curr_sample);
        }

        /* only the left sample array is modified for one channel */
        if (!right || (num_channels == 1)) {
            if (curr_sample > max_left) {
                max_left = curr_sample;
            }
        }
        else {
            if (curr_sample > max_right ) {
                max_right = curr_sample;
            }
        }
    }

    duration = (float)num_samples / (float)sample_rate;
    printf(" num_samples: %d \n", num_samples);
    printf(" duration: %f (sec) \n", duration);

    if (max_right == 0) {
        printf(" max abs mono sample: %d \n", max_left);
    } 
    else {
        printf(" max abs left sample: %d \n", max_left);
        printf(" max abs right sample: %d \n", max_right);
    }
    
}



