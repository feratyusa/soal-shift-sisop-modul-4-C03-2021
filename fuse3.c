#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>
#include <sys/time.h>

// gcc -Wall `pkg-config fuse --cflags` fuse3.c -o fuse `pkg-config fuse --libs`

static const char *dirpath = "/home/prabu/Downloads";

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

static int xmp_getattr(const char *path, struct stat *stbuf){
	int res;
	char fpath[1000];

	sprintf(fpath, "%s%s", dirpath, path);
	res = lstat(fpath, stbuf);

	// printf("%s %d - attr\n", fpath, i);

	if(res == -1) return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){
	char fpath[1000];

	if(strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}
	else{
		sprintf(fpath, "%s%s", dirpath, path);
	}

	int res = 0;

	DIR *dp;
	struct dirent *de;
	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if(dp == NULL) return -errno;

	while((de = readdir(dp))){
		struct stat st;

		memset(&st, 0, sizeof(st));

		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;

		char custPath[1000];
		if(de->d_type == DT_REG){
			encodeSpecialFile(custPath, de->d_name);
		}
		else strcpy(custPath, de->d_name);

		// strcpy(custPath, de->d_name);

		if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
			res = (filler(buf, de->d_name, &st, 0));
		else 
			res = (filler(buf, custPath, &st, 0));

		if(res!=0) break;
	}
	closedir(dp);

	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	char fpath[1000];

	if(strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}
	else sprintf(fpath, "%s%s", dirpath, path);

	int res = 0;
	int fd = 0;

	(void) fi;

	fd = open(fpath, O_RDONLY);

	if (fd == -1) return -errno;

	res = pread(fd, buf, size, offset);

	if(res == -1) res = -errno;

	close(fd);

	return res;
}

static struct fuse_operations xmp_oper = {
	.getattr = xmp_getattr,
	.readdir = xmp_readdir,
	.read = xmp_read,
};

int main(int argc, char *argv[]){
	char specialFile[] = "FiLe_CoNtoH.txt";

	char enc[1000];
	encodeSpecialFile(enc, specialFile);
	printf("%s\n", enc);

	char dec[1000];
	decodeSpecialFile(dec, enc);
	printf("%s\n", dec);

	umask(0);

	return fuse_main(argc, argv, &xmp_oper, NULL);
}