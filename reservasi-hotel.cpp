#include <iostream>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <regex>
#include <cmath>

using namespace std;

// Struct Pelanggan
struct Pelanggan {
    char nama[100];
    string alamat;
    string telepon;
    string dari_tanggal;
    string hingga_tanggal;
    float pembayaran_di_muka;
    int id_pemesanan;
    int tarif;
};

// Struct Kamar
struct Kamar {
    char tipe_ac;
    char tipe_kenyamanan;
    char ukuran;
    int nomor_kamar;
    int tarif;
    int status; // 0: tersedia 1: sudah dipesan
    vector<Pelanggan> daftar_pelanggan; // Daftar pelanggan yang menempati kamar
    Pelanggan pel; // Informasi pelanggan yang menempati kamar
    Kamar* next; // Pointer untuk linked list
};

// Pointer head untuk linked list
Kamar* head = nullptr;

void enqueue(int data);

// Struktur untuk Stack Node
struct StackNode {
    int nomorKamar;
    StackNode* next;
};

// Struktur untuk Queue Node
struct QueueNode {
    int nomorKamar;
    QueueNode* next;
};

// Top untuk Stack
StackNode* top = nullptr;

// Front dan Rear untuk Queue
QueueNode* front = nullptr;
QueueNode* rear = nullptr;

// Fungsi untuk menambah riwayat ke Stack (push)
void push(int nomorKamar) {
    StackNode* newNode = new StackNode();
    newNode->nomorKamar = nomorKamar;
    newNode->next = top;
    top = newNode;
}

// Fungsi untuk menghapus riwayat dari Stack (pop)
void pop() {
    if (top == nullptr) {
        cout << "\nStack kosong!";
        return;
    }
    StackNode* temp = top;
    top = top->next;
    delete temp;
}

// Fungsi untuk menampilkan riwayat Stack
void displayStack() {
    StackNode* temp = top;
    if (temp == nullptr) {
        cout << "\nRiwayat check-in kosong!";
        return;
    }
    cout << "\nRiwayat Check-In (LIFO):";
    while (temp != nullptr) {
        cout << "\nNomor Kamar: " << temp->nomorKamar;
        temp = temp->next;
    }
}

// Fungsi untuk menambah antrian ke Queue (enqueue)
void enqueue(int nomorKamar) {
    QueueNode* newNode = new QueueNode();
    newNode->nomorKamar = nomorKamar;
    newNode->next = nullptr;

    if (rear == nullptr) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
}

// Fungsi untuk menghapus dari antrian Queue (dequeue)
void dequeue() {
    if (front == nullptr) {
        cout << "\nQueue kosong!";
        return;
    }
    QueueNode* temp = front;
    front = front->next;

    if (front == nullptr) {
        rear = nullptr;
    }

    delete temp;
}

// Fungsi untuk menampilkan antrian Queue
void displayQueue() {
    QueueNode* temp = front;
    if (temp == nullptr) {
        cout << "\nAntrian check-out kosong!";
        return;
    }
    cout << "\nAntrian Check-Out (FIFO):";
    while (temp != nullptr) {
        cout << "\nNomor Kamar: " << temp->nomorKamar;
        temp = temp->next;
    }
}

// Fungsi untuk validasi ID pemesanan agar unik
bool isUniqueID(int id) {
    Kamar* temp = head;
    while (temp != nullptr) {
        for (const auto& pelanggan : temp->daftar_pelanggan) {
            if (pelanggan.id_pemesanan == id) {
                return false;  // ID sudah ada
            }
        }
        temp = temp->next;
    }
    return true;  // ID unik
}

// Fungsi untuk memeriksa apakah format tanggal valid (DD/MM/YYYY)
bool isValidDate(const string& date) {
    // Regex untuk format tanggal DD/MM/YYYY
    regex datePattern("(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[0-2])/(19|20)\\d\\d");
    return regex_match(date, datePattern);
}

// Fungsi Tambah Kamar
void tambahKamar(int noKamar) {
    system("cls");
    cout << "\n============ Tambah Kamar ============";

    // Validasi input nomor kamar hanya menerima angka
    while (true) {
        cout << "\nMasukkan nomor kamar : ";
        cin >> noKamar;
        
        if (cin.fail()) {  // Memeriksa jika input bukan angka
            cout << "Error: Masukkan hanya angka untuk nomor kamar.\n";
            cin.clear(); // Menghapus flag error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Membuang input yang salah
        } else {
            break;  // Input valid, keluar dari loop
        }
    }

    // Pengecekan apakah nomor kamar sudah ada
    Kamar* temp = head;
    while (temp != nullptr) {
        if (temp->nomor_kamar == noKamar) {
            cout << "\nError: Nomor kamar " << noKamar << " sudah ada. Silakan masukkan nomor lain.\n";
            return; // Keluar dari fungsi jika nomor kamar sudah ada
        }
        temp = temp->next;
    }

    // Jika nomor kamar belum ada, lanjutkan penambahan kamar
    Kamar* newKamar = new (nothrow) Kamar;
    if (newKamar == nullptr) {
        cout << "\nError: Memori tidak cukup untuk menambahkan kamar baru.\n";
        return;
    }
    newKamar->nomor_kamar = noKamar;

    // Input tipe AC/Non-AC dengan validasi
    while (true) {
        cout << "\nTipe AC/Non-AC (A/N): ";
        cin >> newKamar->tipe_ac;
        if (newKamar->tipe_ac == 'A' || newKamar->tipe_ac == 'N') {
            break;
        } else {
            cout << "Error: Masukkan 'A' untuk AC atau 'N' untuk Non-AC.\n";
        }
    }

    // Input tipe kenyamanan dengan validasi
    while (true) {
        cout << "\nTipe Kenyamanan (S/N): ";
        cin >> newKamar->tipe_kenyamanan;
        if (newKamar->tipe_kenyamanan == 'S' || newKamar->tipe_kenyamanan == 'N') {
            break;
        } else {
            cout << "Error: Masukkan 'S' untuk Standar atau 'N' untuk Nyaman.\n";
        }
    }

    // Input ukuran dengan validasi
    while (true) {
        cout << "\nUkuran (B/S): ";
        cin >> newKamar->ukuran;
        if (newKamar->ukuran == 'B' || newKamar->ukuran == 'S') {
            break;
        } else {
            cout << "Error: Masukkan 'B' untuk Besar atau 'S' untuk Sedang.\n";
        }
    }

    // Input tarif harian dengan validasi input numerik
    while (true) {
        cout << "\nTarif Harian: ";
        cin >> newKamar->tarif;
        
        if (cin.fail() || newKamar->tarif < 0) {
            cout << "Error: Masukkan nilai numerik positif untuk tarif.\n";
            cin.clear(); // Menghapus flag error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Membuang input yang salah
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Membersihkan buffer
            break;
        }
    }

    newKamar->status = 0; // Status awal: tersedia
    newKamar->next = nullptr;

    // Tambahkan ke linked list
    if (head == nullptr) {
        head = newKamar;
    } else {
        Kamar* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newKamar;
    }
    cout << "\nKamar berhasil ditambahkan!\n";
    system("pause");
}

void editKamar() {
    int noKamar;
    system("cls");
    cout << "\n============ Edit Kamar ============";
    // Validasi input nomor kamar hanya menerima angka
    while (true) {
        cout << "\nMasukkan nomor kamar : ";
        cin >> noKamar;
        
        if (cin.fail()) {  // Memeriksa jika input bukan angka
            cout << "Error: Masukkan hanya angka untuk nomor kamar.\n";
            cin.clear(); // Menghapus flag error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Membuang input yang salah
        } else {
            break;  // Input valid, keluar dari loop
        }
    }

    Kamar* temp = head;
    while (temp != nullptr) {
        if (temp->nomor_kamar == noKamar) {
            cout << "\nMengedit kamar nomor: " << temp->nomor_kamar;

            // Edit tipe AC
            while (true) {
                cout << "\nTipe AC/Non-AC saat ini: " << temp->tipe_ac;
                cout << "\nMasukkan tipe baru (A untuk AC, N untuk Non-AC): ";
                cin >> temp->tipe_ac;
                if (temp->tipe_ac == 'A' || temp->tipe_ac == 'N') {
                    break;
                } else {
                    cout << "Error: Masukkan 'A' atau 'N' saja.\n";
                }
            }

            // Edit tipe kenyamanan
            while (true) {
                cout << "\nTipe Kenyamanan saat ini: " << temp->tipe_kenyamanan;
                cout << "\nMasukkan tipe baru (S untuk Standar, N untuk Nyaman): ";
                cin >> temp->tipe_kenyamanan;
                if (temp->tipe_kenyamanan == 'S' || temp->tipe_kenyamanan == 'N') {
                    break;
                } else {
                    cout << "Error: Masukkan 'S' atau 'N' saja.\n";
                }
            }

            // Edit ukuran
            while (true) {
                cout << "\nUkuran saat ini: " << temp->ukuran;
                cout << "\nMasukkan ukuran baru (B untuk Besar, S untuk Sedang): ";
                cin >> temp->ukuran;
                if (temp->ukuran == 'B' || temp->ukuran == 'S') {
                    break;
                } else {
                    cout << "Error: Masukkan 'B' atau 'S' saja.\n";
                }
            }

            // Edit tarif
            while (true) {
                cout << "\nTarif saat ini: " << temp->tarif;
                cout << "\nMasukkan tarif baru: ";
                cin >> temp->tarif;
                if (cin.fail() || temp->tarif < 0) {
                    cout << "Error: Masukkan nilai numerik positif untuk tarif.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                } else {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
            }

            cout << "\nKamar berhasil diperbarui!\n";
            system("pause");
            return;
        }
        temp = temp->next;
    }

    cout << "\nKamar dengan nomor " << noKamar << " tidak ditemukan.\n";
    system("pause");
}



// Fungsi Tampilkan Kamar Tersedia
void tampilkanKamarTersedia() {
    Kamar* temp = head;
    bool ditemukan = false;
    system("cls");
    cout << "\n============ Daftar Kamar Tersedia ============";
    while (temp != nullptr) {
        if (temp->status == 0) { // Kamar tersedia
            cout << "\nNomor Kamar: \t" << temp->nomor_kamar;
            cout << "\nTipe AC/Non-AC (A/N): " << temp->tipe_ac;
            cout << "\nTipe Kenyamanan (S/N): " << temp->tipe_kenyamanan;
            cout << "\nUkuran (B/S): " << temp->ukuran;
            cout << "\nTarif: " << temp->tarif;
            cout << "\n\n";
            ditemukan = true;
        }
        temp = temp->next;
    }
    if (!ditemukan) {
        cout << "\nSemua kamar sudah dipesan\n" << endl; 
    }
    system("pause");
}

// Fungsi Check-In
void checkIn() {
    tampilkanKamarTersedia(); // Menampilkan kamar yang tersedia terlebih dahulu
    int noKamar;
    cout << "\nMasukkan nomor kamar untuk check-in: ";
    cin >> noKamar;

    Kamar* temp = head;
    while (temp != nullptr) {
        if (temp->nomor_kamar == noKamar) {
            if (temp->status == 1) {
                cout << "\nKamar sudah dipesan\n";
                return;
            }
            
            Pelanggan pel;
            cout << "\nMasukkan ID pemesanan: ";

            // Menangani input ID pemesanan
            while (true) {
                cin >> pel.id_pemesanan;

                // Jika input tidak valid (misalnya huruf atau simbol), beri pesan dan minta input ulang
                if (cin.fail()) {
                    cin.clear(); // Bersihkan status error cin
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Abaikan input yang salah
                    cout << "\nID pemesanan hanya boleh berupa angka! Coba lagi: ";
                } else {
                    break; // Keluar dari loop jika input valid
                }
            }

            // Validasi ID Pemesanan Unik
            if (!isUniqueID(pel.id_pemesanan)) {
                cout << "\nID pemesanan sudah digunakan! Masukkan ID yang unik.\n";
                return;
            }

            cout << "\nMasukkan Nama Pelanggan: ";
            cin >> pel.nama;
            cin.ignore(); 

            cout << "\nMasukkan Alamat (kota saja): ";
            getline(cin, pel.alamat);

            cout << "\nMasukkan Telepon: ";
            getline(cin, pel.telepon);      

            // Validasi Tanggal Mulai
            bool valid = false;
            while (!valid) {
                cout << "\nMasukkan Tanggal Mulai (DD/MM/YYYY): ";
                cin >> pel.dari_tanggal;
                if (isValidDate(pel.dari_tanggal)) {
                    valid = true;
                } else {
                    cout << "\nFormat tanggal tidak valid! Pastikan formatnya adalah DD/MM/YYYY.\n";
                }
            }

            // Validasi Tanggal Selesai
            valid = false;
            while (!valid) {
                cout << "\nMasukkan Tanggal Selesai (DD/MM/YYYY): ";
                cin >> pel.hingga_tanggal;
                if (isValidDate(pel.hingga_tanggal)) {
                    valid = true;
                } else {
                    cout << "\nFormat tanggal tidak valid! Pastikan formatnya adalah DD/MM/YYYY.\n";
                }
            }

            cout << "\nMasukkan Pembayaran Di Muka: ";
            cin >> pel.pembayaran_di_muka;

            temp->daftar_pelanggan.push_back(pel); // Tambahkan pelanggan ke daftar

            temp->status = 1; // Kamar di-check-in
            push(noKamar); // Tambahkan ke riwayat check-in
            cout << "\nCheck-in berhasil!\n";
            cin.get();
            system("pause");

            return;
        }
        temp = temp->next;
    }
    cout << "\nKamar tidak ditemukan!\n";
    system("pause");
}

// fungsi check out
void checkOut() {
    int nomorKamar;
    system("cls");
    cout << "\nMasukkan Nomor Kamar untuk Check-Out: ";
    cin >> nomorKamar;

    Kamar* temp = head;
    while (temp != nullptr) {
        if (temp->nomor_kamar == nomorKamar && temp->status == 1) {
            // Proses pembayaran
            int hari;
            float jumlahTagihan;
            cout << "\nMasukkan Jumlah Hari Menginap: ";
            cin >> hari;
            jumlahTagihan = hari * temp->tarif;

            cout << "\n\t######## Detail Check-Out ########\n";
            cout << "\nNama Pelanggan: " << temp->daftar_pelanggan.back().nama;
            cout << "\nNomor Kamar: " << temp->nomor_kamar;
            cout << "\nAlamat: " << temp->daftar_pelanggan.back().alamat;
            cout << "\nTelepon: " << temp->daftar_pelanggan.back().telepon;
            cout << "\nJumlah Tagihan: " << jumlahTagihan << " /";
            cout << "\nPembayaran Di Muka: " << temp->daftar_pelanggan.back().pembayaran_di_muka << " /";
            cout << "\n*** Jumlah Pembayaran: " << jumlahTagihan - temp->daftar_pelanggan.back().pembayaran_di_muka << "/ saja";

            temp->status = 0; // Ubah status menjadi tersedia kembali
            enqueue(nomorKamar); // Tambahkan ke antrian check-out
            cout << "\nProses check-out berhasil!\n";
            cin.get();
            system("pause");
            return;
        }
        temp = temp->next;
    }

    cout << "\nKamar tidak ditemukan atau belum dipesan!\n";
    system("pause");
}


// Fungsi untuk mengurutkan tarif menggunakan Shell Sort
void shellSort() {
    // Menemukan panjang linked list terlebih dahulu
    Kamar* temp = head;
    int n = 0;
    while (temp != nullptr) {
        n++;
        temp = temp->next;
    }

    // Menggunakan Shell Sort berdasarkan tarif kamar
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            temp = head;
            for (int j = 0; j < i; j++) {
                temp = temp->next;
            }
            Kamar* temp2 = temp;
            int j = i;
            while (j >= gap) {
                Kamar* kamar1 = head;
                for (int k = 0; k < j - gap; k++) {
                    kamar1 = kamar1->next;
                }
                Kamar* kamar2 = kamar1->next;
                if (kamar1->tarif > kamar2->tarif) {
                    swap(kamar1->tarif, kamar2->tarif);
                    swap(kamar1->nomor_kamar, kamar2->nomor_kamar);
                    // Swap lainnya jika diperlukan
                }
                j -= gap;
            }
        }
    }
}

// Fungsi untuk mengurutkan berdasarkan nomor kamar menggunakan Quick Sort
void quickSort(Kamar* low, Kamar* high) {
    if (low == nullptr || low == high) {
        return;
    }
    Kamar* pivot = low;
    Kamar* temp = low;
    Kamar* current = low->next;

    while (current != nullptr) {
        if (current->nomor_kamar < pivot->nomor_kamar) {
            temp = temp->next;
            swap(temp->nomor_kamar, current->nomor_kamar);
            swap(temp->tarif, current->tarif); // swap tarif jika perlu
        }
        current = current->next;
    }
    swap(low->nomor_kamar, temp->nomor_kamar);
    swap(low->tarif, temp->tarif);

    quickSort(low, temp);
    quickSort(temp->next, high);
}

int fibonacciSearch(Kamar* head, int target) {
    Kamar* temp = head;
    while (temp != nullptr) {
        if (temp->nomor_kamar == target) {
            if (temp->status == 1) { // Hanya tampilkan jika kamar dipesan
                // Tampilkan detail kamar yang sudah dipesan
                cout << "\nDetail Kamar\n";
                cout << "\nKamar sudah dipesan";
                cout << "\nNomor Kamar: \t" << temp->nomor_kamar;
                cout << "\nTipe AC/Non-AC (A/N): " << temp->tipe_ac;
                cout << "\nTipe Kenyamanan (S/N): " << temp->tipe_kenyamanan;
                cout << "\nUkuran (B/S): " << temp->ukuran;
                cout << "\nTarif: " << temp->tarif;
                sleep(5);
                return 1;
            } else {
                cout << "\nKamar tidak ditemukan atau belum dipesan.\n";
                system("pause");
                return -1; // Kamar ada tetapi statusnya belum dipesan
            }
        }
        temp = temp->next;
    }
    cout << "\nKamar tidak ditemukan.\n";
    system("pause");
    return -1; // Tidak ditemukan
}


// Fungsi Jump Search (Mencari berdasarkan tarif)
int jumpSearch(Kamar* head, int target) {
    Kamar* temp = head;
    while (temp != nullptr) {
        if (temp->tarif == target) {
            // Tampilkan detail kamar setelah ditemukan
            cout << "\nDetail Kamar\n";
            if (temp->status == 1) {
                cout << "\nKamar sudah dipesan";
            } else {
                cout << "\nKamar tersedia";
            }
            cout << "\nNomor Kamar: \t" << temp->nomor_kamar;
            cout << "\nTipe AC/Non-AC (A/N): " << temp->tipe_ac;
            cout << "\nTipe Kenyamanan (S/N): " << temp->tipe_kenyamanan;
            cout << "\nUkuran (B/S): " << temp->ukuran;
            cout << "\nTarif: " << temp->tarif;
            sleep(5);    
            return 1;
        }
        temp = temp->next;
    }
    cout << "\nKamar tidak ditemukan.\n";
    system("pause");
    return -1;  // Tidak ditemukan
}

// Fungsi Boyer-Moore Search (Mencari berdasarkan nama pelanggan)
int boyerMooreSearch(Kamar* head, const string& target) {
    Kamar* temp = head;
    while (temp != nullptr) {
        if (target == temp->pel.nama) {
            // Tampilkan detail kamar setelah ditemukan
            cout << "\nDetail Kamar\n";
            if (temp->status == 1) {
                cout << "\nKamar sudah dipesan";
            } else {
                cout << "\nKamar tersedia";
            }
            cout << "\nNomor Kamar: \t" << temp->nomor_kamar;
            cout << "\nTipe AC/Non-AC (A/N): " << temp->tipe_ac;
            cout << "\nTipe Kenyamanan (S/N): " << temp->tipe_kenyamanan;
            cout << "\nUkuran (B/S): " << temp->ukuran;
            cout << "\nTarif: " << temp->tarif;
            sleep(5);
            return 1;
        }
        temp = temp->next;
    }
    cout << "\nKamar tidak ditemukan.\n";
    system("pause");
    return -1;  // Tidak ditemukan
}

// Fungsi Menu Pencarian
void menuSearch() {
    int searchOption;
    while (true) {
    system("cls");
        cout << "\n===== MENU PENCARIAN =====";
        cout << "\n1. Cari berdasarkan nomor kamar";
        cout << "\n2. Cari berdasarkan tarif";
        cout << "\n3. Cari berdasarkan nama pelanggan";
        cout << "\nPilih pencarian: ";
        cin >> searchOption;

        if (cin.fail()) { // Memeriksa apakah input valid
            cout << "Input Tidak Valid! Coba Lagi...\n"; // Menampilkan pesan kesalahan input
            sleep(1);
            cin.clear(); // Membersihkan status kesalahan input
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Mengabaikan sisa input yang salah
            continue; // Melanjutkan loop untuk input ulang
        }
        switch (searchOption) {
            case 1:
                int nomorKamar;
                cout << "\nMasukkan nomor kamar yang dicari: ";
                cin >> nomorKamar;
                fibonacciSearch(head, nomorKamar);
                break;
            case 2:
                int tarif;
                cout << "\nMasukkan tarif yang dicari: ";
                cin >> tarif;
                jumpSearch(head, tarif);
                break;
            case 3:
                string namaTarget;
                cout << "\nMasukkan nama pelanggan yang dicari: ";
                cin >> namaTarget;
                boyerMooreSearch(head, namaTarget);
                break;
                cout << "\nPilihan tidak valid!";
        }
    }
}

// Fungsi Menu Pengurutan
void menuSort() {
    int sortOption;
    cout << "\n===== MENU PENGURUTAN =====";
    cout << "\n1. Shell Sort (Urutkan berdasarkan Tarif)";
    cout << "\n2. Quick Sort (Urutkan berdasarkan Nomor Kamar)";
    cout << "\nPilih pengurutan: ";
    cin >> sortOption;

    switch (sortOption) {
        case 1:
            shellSort();
            tampilkanKamarTersedia();
            break;
        case 2:
            quickSort(head, nullptr);
            tampilkanKamarTersedia();
            break;
        default:
            cout << "\nPilihan tidak valid!";
    }
}

void lihatRiwayatCheckIn() {
    system("cls");
    displayStack();
}

void lihatAntrianCheckOut() {
    system("cls");
    displayQueue();
}


void lihatRiwayat() {
    int pilihan;
    system("cls");
    do {
        cout << "\n======== Lihat Riwayat ========\n";
        cout << "1. Riwayat Check-In\n";
        cout << "2. Antrian Check-Out\n";
        cout << "3. Kembali ke Menu Utama\n";
        cout << "================================\n";
        cout << "Masukkan pilihan: ";
        
        // Memeriksa input apakah valid atau tidak
        cin >> pilihan;

        // Jika input gagal (misalnya simbol atau karakter), bersihkan input dan tampilkan pesan error
        if (cin.fail()) {
            cout << "\nInput tidak valid!\n";
            sleep(1);
            cin.clear(); // Membersihkan status error pada cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Mengabaikan input yang salah
            continue; // Kembali ke awal loop untuk meminta input ulang
        }

        // Proses berdasarkan pilihan yang dimasukkan
        switch (pilihan) {
            case 1:
                lihatRiwayatCheckIn(); // Menampilkan riwayat Check-In
                break;
            case 2:
                lihatAntrianCheckOut(); // Menampilkan antrian Check-Out
                break;
            case 3:
                return; // Kembali ke menu utama
            default:
                cout << "\nPilihan tidak valid! Silakan pilih lagi.\n";
        }
    } while (pilihan != 3);
}


// Fungsi Main
int main() {
    int pilihan;
    int noKamar;
    while (true) {
        system("cls");
        cout << "\n============ MENU UTAMA ============";
        cout << "\n1. Tambah Kamar";
        cout << "\n2. Edit Kamar";
        cout << "\n3. Check-In";
        cout << "\n4. Check-Out";
        cout << "\n5. Tampilkan Kamar Tersedia";
        cout << "\n6. Menu Pencarian";
        cout << "\n7. Menu Pengurutan";
        cout << "\n8. Lihat Riwayat";
        cout << "\n0. Keluar";
        cout << "\nPilih menu: ";
        cin >> pilihan;

        if (cin.fail()) { // Memeriksa apakah input valid
        cout << "Input Tidak Valid! Coba Lagi...\n"; // Menampilkan pesan kesalahan input
        sleep(1);
        cin.clear(); // Membersihkan status kesalahan input
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Mengabaikan sisa input yang salah
        continue; // Melanjutkan loop untuk input ulang
        }

        switch (pilihan) {
            case 1:
                tambahKamar(noKamar);
                break;
            case 2:
                editKamar();
                break;
            case 3:
                checkIn();
                break;
            case 4:
                checkOut();
                break;
            case 5:
                tampilkanKamarTersedia();
                break;
            case 6:
                menuSearch();
                break;
            case 7: 
            	menuSort();
                break;
            case 8:
                lihatRiwayat();
                break;
            case 0:
                cout << "\nTerima kasih, sampai jumpa!";
                return 0;
                break;
            default:
                cout << "\nPilihan tidak valid!";
        }
    } while (pilihan != 0);

    return 0;
}