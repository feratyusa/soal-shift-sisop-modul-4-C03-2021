#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/xattr.h>
#include <errno.h>
#include <stdlib.h>

static  const  char *dirpath = "/home/[USER]/Downloads";


int vigenere_verify(char *str, const char *key, size_t key_length) {
//     size_t i, strlength = strlen(str);
//     for (i = 0; i < key_length || i < strlength; ++i) {

//     }

    return 1; // untuk sementara selalu benar dahulu
}

void encodeSpecialFile(char *custPath, char *path){
	char namaFile[500], ext[500], *token, temp[500];
	strcpy(temp, path);
	// Nama file
	token = strtok(temp, ".");
	strcpy(namaFile, token);
	// Exstension
	token = strtok(NULL, ".");
	strcpy(ext, token);
	// strcat(ext, ".");

	// Encrypt
	int i;
	char lowNamaFile[500], enc[500];
	memset(enc, '\0', 500);
	strcpy(lowNamaFile, namaFile);
	for(i=0;i<strlen(lowNamaFile);i++) lowNamaFile[i] = tolower(lowNamaFile[i]);
	for(i=0;i<strlen(namaFile);i++){
		if(namaFile[i] != lowNamaFile[i]) strcat(enc, "1");
		else strcat(enc, "0");
	}
		
	// // Biner to Dec
	int num=0;
	double a = 0.00;
	for(i=strlen(enc)-1;i>=0;i--){
		if(enc[i] == '1') num = num + (int)pow(2.00, a);
		a += 1;
	}

	sprintf(custPath, "%s.%s.%d", lowNamaFile, ext, num);
}

void decodeSpecialFile(char *custPath, char *path){
	char namaFile[500], ext[500], encryption[500], *token, temp[500];
	strcpy(temp, path);
	// Nama file
	token = strtok(temp, ".");
	strcpy(namaFile, token);
	// Exstension
	token = strtok(NULL, ".");
	strcpy(ext, token);
	// encryption
	token = strtok(NULL, ".");
	strcpy(encryption, token);

	int i;
	// Decimal to Biner
	
	int ten=1, num=0;
	for(i=strlen(encryption)-1;i>=0;i--){
		num = num + ((int)encryption[i]-48) * ten;
		ten *= 10;
	}

	char binerRev[500];
	memset(binerRev, '\0', 500);
	while(num > 0){
		if(num%2 == 1) strcat(binerRev, "1");
		else strcat(binerRev, "0");
		num /= 2;
	}
	char biner[500];
	int j = 0;
	for(i=strlen(binerRev)-1;i>=0;i--) biner[j++] = binerRev[i];

	// Decrypt
	char decodeFile[500];
	for(i=0;i<strlen(biner);i++)
		if(biner[i] == '1') decodeFile[i] = toupper(namaFile[i]);
		else decodeFile[i] = namaFile[i];

	sprintf(custPath, "%s.%s", decodeFile, ext);
}

int atbash_encoder(char *str) {
    size_t i, strlength = strlen(str);
    // char *tmp = malloc(sizeof(char) * strlength);
    int position;

    for (i = 0; i < strlength; ++i) {
        if (str[i] >= 65 && str[i] <= 90) {
            position = str[i] - 65;
            position = 25 - position;
            // tmp[i] = position + 65;
            str[i] = position + 65;
        } else if (str[i] >= 97 && str[i] <= 122) {
            position = str[i] - 97;
            position = 25 - position;
            // tmp[i] = position + 97;
            str[i] = position + 97;
        } else {
            // tmp[i] = str[i];
        }
    }

    return 0;
}

int rot13_encoder(char *str) {
    size_t i, strlength = strlen(str);
    // char *tmp = malloc(sizeof(char) * strlength);
    int position;

    for (i = 0; i < strlength; ++i) {
        if (str[i] >= 65 && str[i] <= 90) {
            position = str[i] - 65;
            position = (position + 13) % 26;
            // tmp[i] = position + 65;
            str[i] = position + 65;
        } else if (str[i] >= 97 && str[i] <= 122) {
            position = str[i] - 97;
            position = (position + 13) % 26;
            // tmp[i] = position + 97;
            str[i] = position + 97;
        } else {
            // tmp[i] = str[i];
        }
    }

    return 0;
}

int vigenere_encoder(char *str, const char *key, size_t key_length) {
    size_t i, strlength = strlen(str);
    // char *tmp = malloc(sizeof(char) * strlength);
    int position, key_pos, key_subtractor;

    if (key[0] >= 97) key_subtractor = 97;
    else key_subtractor = 65;

    for (i = 0; i < strlength; ++i) {
        key_pos = i % key_length;
        if (str[i] >= 65 && str[i] <= 90) {
            position = str[i] - 65;
            position = (position + key[key_pos] - key_subtractor) % 26;
            // tmp[i] = position + 65;
            str[i] = position + 65;
        } else if (str[i] >= 97 && str[i] <= 122) {
            position = str[i] - 97;
            position = (position + key[key_pos] - key_subtractor) % 26;
            // tmp[i] = position + 97;
            str[i] = position + 97;
        } else {
            // tmp[i] = str[i];
        }
    }

    return 0;
}

int atbash_decoder(char *str) {
    size_t i, strlength = strlen(str);
    // char *tmp = malloc(sizeof(char) * strlength);
    int position;

    for (i = 0; i < strlength; ++i) {
        if (str[i] >= 65 && str[i] <= 90) {
            position = str[i] - 65;
            position = 25 - position;
            // tmp[i] = position + 65;
            str[i] = position + 65;
        } else if (str[i] >= 97 && str[i] <= 122) {
            position = str[i] - 97;
            position = 25 - position;
            // tmp[i] = position + 97;
            str[i] = position + 97;
        } else {
            // tmp[i] = str[i];
        }
    }

    return 0;
}

int rot13_decoder(char *str) {
    size_t i, strlength = strlen(str);
    // char *tmp = malloc(sizeof(char) * strlength);
    int position;

    for (i = 0; i < strlength; ++i) {
        if (str[i] >= 65 && str[i] <= 90) {
            position = str[i] - 65;
            position = (position + 13) % 26;
            // tmp[i] = position + 65;
            str[i] = position + 65;
        } else if (str[i] >= 97 && str[i] <= 122) {
            position = str[i] - 97;
            position = (position + 13) % 26;
            // tmp[i] = position + 97;
            str[i] = position + 97;
        } else {
            // tmp[i] = str[i];
        }
    }

    return 0;
}

int vigenere_decoder(char *str, const char *key, size_t key_length) {
    size_t i, strlength = strlen(str);
    // char *tmp = malloc(sizeof(char) * strlength);
    int position, key_pos, key_subtractor;

    if (key[0] >= 97) key_subtractor = 97;
    else key_subtractor = 65;

    for (i = 0; i < strlength; ++i) {
        key_pos = i % key_length;
        if (str[i] >= 65 && str[i] <= 90) {
            position = str[i] - 65;
            position = (position - (key[key_pos] - key_subtractor));
            if (position < 0) position = 26 + position;
            // tmp[i] = position + 65;
            str[i] = position + 65;
        } else if (str[i] >= 97 && str[i] <= 122) {
            position = str[i] - 97;
            position = (position - (key[key_pos] - key_subtractor));
            if (position < 0) position = 26 + position;
            // tmp[i] = position + 97;
            str[i] = position + 97;
        } else {
            // tmp[i] = str[i];
        }
    }

    return 0;
}

static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];

    sprintf(fpath,"%s%s",dirpath,path);

    res = lstat(fpath, stbuf);

    if (res == -1) return -errno;

    return 0;
}



static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];

    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else sprintf(fpath, "%s%s",dirpath,path);

    int res = 0;

    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;

    dp = opendir(fpath);

    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        res = (filler(buf, de->d_name, &st, 0));
        if(res!=0) break;
    }

    closedir(dp);

    return 0;
}



static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;

        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);

    int res = 0;
    int fd = 0 ;

    (void) fi;

    fd = open(fpath, O_RDONLY);

    if (fd == -1) return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1) res = -errno;

    close(fd);

    return res;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
};


int  main(int  argc, char *argv[])
{
    /* Testing encode decoder nomor3 */
    char specialFile[] = "FiLe_CoNtoH.txt";

	char enc[1000];
	encodeSpecialFile(enc, specialFile);
	printf("Encode Nomor 3: %s\n", enc);

	char dec[1000];
	decodeSpecialFile(dec, enc);
	printf("Decode Nomor 3: %s\n", dec);
    
    /* Testing encoder decoder */
    char test_atbash[] = "TEST_ATBASH";
    char test_rot13[] = "TEST_ROT13";
    char test_vigenere[] = "TEST_VIGENERE";

    printf("###INIT TEST:\nTEST_ATBASH: %s\nTEST ROT13: %s\nTEST VIGENERE (SISOP): %s\n", test_atbash, test_rot13, test_vigenere);

    atbash_encoder(test_atbash);
    rot13_encoder(test_rot13);
    vigenere_encoder(test_vigenere, "SISOP", 5);

    printf("###ENCODED TEST:\nTEST_ATBASH: %s\nTEST ROT13: %s\nTEST VIGENERE (SISOP): %s\n", test_atbash, test_rot13, test_vigenere);

    atbash_decoder(test_atbash);
    rot13_decoder(test_rot13);
    vigenere_decoder(test_vigenere, "SISOP", 5);

    printf("###DECODED TEST:\nTEST_ATBASH: %s\nTEST ROT13: %s\nTEST VIGENERE (SISOP): %s\n", test_atbash, test_rot13, test_vigenere);

    printf("\n\n###STARTING FUSE...\n\n");
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
