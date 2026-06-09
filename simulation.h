#define READER_H

//=======file-reader.c functions=======//
int read_num_dims(const char *);
int *read_dims(const char *, int);
float *read_array(const char *, const int *, int);
int write_to_output_file(const char *, float *, int *, int);

#endif