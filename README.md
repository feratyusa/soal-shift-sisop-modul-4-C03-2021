# Penyelesaian Soal Shift Modul 4

- Ega Prabu Pamungkas (05111940000014)
- A. Zidan Abdillah Majid (05111940000070)
- Jundullah Hanif Robbani (05111940000144)

-------------------------------------------
Pada FUSE ini kelompok kami menggunakan beberapa fungsi dari libfuse sebagai berikut.

```c
static struct fuse_operations xmp_oper = {
	.getattr = xmp_getattr,
	.mkdir = xmp_mkdir,
	.readdir = xmp_readdir,
	.rmdir = xmp_rmdir,
	.symlink = xmp_symlink,
	.read = xmp_read,
	.readlink = xmp_readlink,
	.link = xmp_link,
	.unlink = xmp_unlink,
	.rename = xmp_rename,
	.create = xmp_create,
	.open = xmp_open,
	.write = xmp_write,
	.utimens = xmp_utimens,
};
```

Fungsi-fungsi tersebut dipilih sehingga di dalam fuse dapat dilakukan operasi untuk membuat (`mkdir` dan `touch`), menghapus (`rm` dan `rm -r`), dan mengganti nama (`mv`) folder atau file.

## 1
## 2
## 3
### Penjelasan Solusi
Pada soal ini terdapat sebuah enkripsi nama file yang harus dilakukan. Enkripsi ini adalah nama file dibuat lowercase semua lalu hasil dari lowercase dibandingkan dengan nama aslinya jika terdapat perbedaan maka nilainya 1, jika sama maka nilainya 0. Sehingga, misal terdapat nama file "FiLe_CoNtoH.txt" maka lowercasenya "file_contoh.txt" dan hasil perbandingannya adalah "10100101001". Hasil perbandingan tersebut dibuat dari bilangan biner menjadi bilangan desimal sehingga nilai desimalnya adalah 1321 lalu diletakkan setealah ekstensi nama file tersebut sehingga enkripsinya menjadi `file_contoh.txt.1321`.
```c
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
```
Untuk medecode enkripsinya hanya tinggal mengubah desimal yang ada menjadi deret bilangan 1 dan 0 lalu dibandingkan dengan nama file enkripsi, jika 1 maka diubah menjadi uppercase dan selain itu tetap.
```c
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
```
### Kendala
Masih kurang paham bagaimana fuse melakukan rekursif untuk menghubungkan file-file dan folder-folder pada path yang ingin dimount sehingga kurang memahami dalam mengenkripsi setiap folder dan file pada fuse.

## 4
### Penjelasan
Dalam melakukan logging file cukup dengan meletakkan fungsi logging pada setiap fungsi fuse dengan memberikan nama command, path yang dibawa, dan jenis dari fungsi tersebut apakah INFO atau WARNING.

```c
void logActivity(char *status, char *comm, char *desc){
	FILE *fp = fopen(pathLog, "a+");
	time_t t;
	struct tm *tmp;
	char tbuff[100];
	time(&t);
	tmp = localtime(&t);
	strftime(tbuff, sizeof(tbuff), "%d%m%y-%H:%M:%S", tmp);

	fprintf(fp, "%s::%s:%s::%s\n", status, tbuff, comm, desc);
	fclose(fp);
}

```

Contoh pemanggilan fungsi logging INFO
```c
static int xmp_create(const char *path, mode_t mode, struct fuse_file_info *fi){
	...
	...
	...

	logActivity("INFO", "CREATE", desc);

	return 0;
}
```

Contoh pemanggilan fungsi logging WARNING.
```c
static int xmp_rmdir(const char *path){
	...
	...
	...

	logActivity("WARNING", "RMDIR", desc);

	return 0;
}
```

### Kendala
Sama dengan kendala nomor 3, masih kurang paham bagaimana rekursif dalam pembuatan fuse sehingga kurang memahami untuk membuat enkripsi dari setiap folder dan file.