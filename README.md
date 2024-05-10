# Sisop-3-2024-MH-IT30
Praktikum Sistem Operasi Modul 3

## Praktikum [Modul 3](https://github.com/lab-kcks/Modul-Sisop/tree/main/Modul-3)

Mata Kuliah Sistem Operasi

Dosen pengampu : Ir. Muchammad Husni, M.Kom.


## Kelompok Praktikum [IT-30]

- [Fico Simhanandi - 50272310](https://github.com/PuroFuro)
- [Jody Hezekiah - 5027221050](https://github.com/imnotjs)
- [Nabiel Nizar Anwari - 5027231087](https://github.com/bielnzar)




## [SOAL 4](https://docs.google.com/document/d/11l-PLiVaPjNwZawNiakcBfg39T_s_yluUffeMMb--Ac)

Lewis Hamilton 🏎 seorang wibu akut dan sering melewatkan beberapa episode yang karena sibuk menjadi asisten. Maka dari itu dia membuat list anime yang sedang ongoing (biar tidak lupa) dan yang completed (anime lama tapi pengen ditonton aja). Tapi setelah Lewis pikir-pikir malah kepikiran untuk membuat list anime. Jadi dia membuat [file](https://drive.google.com/file/d/10p_kzuOgaFY3WT6FVPJIXFbkej2s9f50/view?usp=drive_link) (harap diunduh) dan ingin menggunakan socket yang baru saja dipelajarinya untuk melakukan CRUD pada list animenya. 

a. Client dan server terhubung melalui socket. 

b. client.c di dalam folder client dan server.c di dalam folder server

c. Client berfungsi sebagai pengirim pesan dan dapat menerima pesan dari server.

d. Server berfungsi sebagai penerima pesan dari client dan hanya menampilkan pesan perintah client saja. 

e. Server digunakan untuk membaca myanimelist.csv. Dimana terjadi pengiriman data antara client ke server dan server ke client.
- Menampilkan seluruh judul
- Menampilkan judul berdasarkan genre
- Menampilkan judul berdasarkan hari
- Menampilkan status berdasarkan berdasarkan judul
- Menambahkan anime ke dalam file myanimelist.csv
- Melakukan edit anime berdasarkan judul
- Melakukan delete berdasarkan judul
- Selain command yang diberikan akan menampilkan tulisan “Invalid Command”

f. Karena Lewis juga ingin track anime yang ditambah, diubah, dan dihapus. Maka dia membuat server dapat mencatat anime yang dihapus dalam sebuah log yang diberi nama change.log.
- Format: [date] [type] [massage]
- Type: ADD, EDIT, DEL
- Ex:
[29/03/24] [ADD] Kanokari ditambahkan.

[29/03/24] [EDIT] Kamis,Comedy,Kanokari,completed diubah menjadi Jumat,Action,Naruto,completed.

[29/03/24] [DEL] Naruto berhasil dihapus.

g. Koneksi antara client dan server tidak akan terputus jika ada kesalahan input dari client, cuma terputus jika user mengirim pesan “exit”. Program exit dilakukan pada sisi client.

h. Hasil akhir:
soal_4/
    ├── change.log
    ├── client/
    │   └── client.c
    ├── myanimelist.csv
    └── server/
        └── server.c



### Membuat Source Code client.c

Program ini berfungsi untuk mengirim pesan ke server.c yang mana server bertugas sebagai pengeksekusi perintah, jadi dia yang akan membaca file myanimelist.csv dan memiliki kendali penuh atasnya. Nah, program client.c juga memiliki kemampuan untuk menampilkan hasil dari pengolahan perintah oleh server

client.c dan server.c terhubung menggunakan socket, yang mana socket tersebut harus di config dari kedua program agar bisa terhubung satu sama lain. Isi dari program client.c kurang lebih hanya config socket beserta jajarannya, dan config untuk menerima input dari pengguna, mengirim serta menerima pesan dari server, lalu menampilkannya.

berikut adalah isi dari source code client.c

```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[MAXLINE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    while (1) {
        printf("You : ");
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        send(sock, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "exit") == 0) {
            printf("Exiting the client\n");
            break;
        }

        memset(buffer, 0, sizeof(buffer));
        valread = read(sock, buffer, sizeof(buffer) - 1);
        printf("Server : \n%s\n", buffer);
    }

    close(sock);
    return 0;
}
```

Kami juga menmabahkan pendeteksi" error di beberapa kondisi, agar code bisa berjalan dengan maksimal dan mudah untuk melakukan manajemen dan maintenance error code.



### Membuat server.c

Seperti yang sudah dikatakan sebelumnya, server.c berfungsi sebagai code pengeksekusi perintah dari client.c

banyak sekali fungsi yang berada dalam code server.c ini, karena kami membuat sebuah fungsi untuk satu command. Kami menggunakan struct untuk menyimpan isi dari myanimelist.csv agar mudah dalam mengelola data, digunakan juga fopen untuk membaca melakukan dan melakukan perubahan pada file .csv, diluar header config socker, kami menambahkan `#include <time.h>`, yang berguna untuk mengetahui waktu, yang akan dipakai di penulisan change.log sesuai permintaan soal.

kurang lebih alur dari program server.c itu seperti ini :

membaca file .csv dan menyimpan isinya di struct -> menerima input dari client -> mengeksekusi perintah sesuai command -> menampilkan command dari client ke terminal -> mengirim hasil eksekusi command ke server -> menuliskan hasil perubahan ke change.log apabila sesuai kriteria yang ditentukan

code server.c tidak akan kami cantumkan disini karena terlalu panjang, bisa di baca dan di lihat di repo kami
### Hasil Kinerja

Hasil kolaborasi code client.c dengan server.c berjalan dengan maksimal, semua sudah sesuai dengan apa yang diminta soal, code juga tidak ada yang error, tidak ada revisi dari soal no. 4 ini. Kemudian kami akan menampilkan hasil program apabila dijalankan di terminal.

![github-small](https://github.com/bielnzar/sisop/blob/main/Modul3/1.png)

![github-small](https://github.com/bielnzar/sisop/blob/main/Modul3/2.png)

program sudah bisa menampilkan anime dengan mantap, baik secara keseluruhan, sesuai hari, genre, dan permintaan soal lainnya juga sudah terpenuhi seperti add, edit, delete, dkk. Program juga bisa menampilkan invalid command apabila command yang dimasukkan tidak sesuai, untuk memberitahu pengguna.

![github-small](https://github.com/bielnzar/sisop/blob/main/Modul3/3.png)

sesuai permintaan soal yaitu : "Koneksi antara client dan server tidak akan terputus jika ada kesalahan input dari client, cuma terputus jika user mengirim pesan “exit”. Program exit dilakukan pada sisi client."

![github-small](https://github.com/bielnzar/sisop/blob/main/Modul3/4.png)

Hasil akhir sudah sesuai dengan apa yang diminta soal, bisa dilihat isi dari file change.log dan struktur repo yang sudah sangat ajib dan mantap, alhamdulillah.

## Sekian Laporan Resmi Praktikum Sisop Modul 3
## TERIMAKASIH :)
