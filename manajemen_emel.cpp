// Program ini mengelola sistem manajemen game online. 
// Program ini kita terinsipirasi dari game moba yaitu Mobile Legends Bang-Bang (MLBB).
// Nantinya, program ini dapat digunakan oleh 2 jenis user yaitu pemain atau player dan admin.
// Nah untuk player sendiri, kita mengadakan berbagai fitur seperti registrasi user, login akun, logout, riwayat match (simulasi match), daftar player online--
// lalu ada fitur sorting untuk pengurutan ranking player, profil player dan fitur pencarian player.
// Sedangkan untuk admin, fitur yang kita berikan adalah menghapus atau reset data player, melihat daftar player serta yang online--
// lalu ada fitur pencarian player, banned player, broadast player, log aktivitas player dan memantau statistik player.
// Untuk sorting sendiri nantinya kita menggunakan bubble sort dan quick sort.
// Untuk pencarian player kita menggunakan binary search.
// Program ini juga sudah kita lengkapi dengan fitur save dan load data player ke file.
// Saat program ini dijalankan, nanti user akan diminta memilih dulu akan login sebagai pemain atau admin. 
// Baru setelah itu, user akan diarahkan ke mu utama sesuai dengan pilihan yang dipilih tadi.

#include <iostream>
#include <fstream> 
#include <sstream> 
#include <vector>
#include <string>
#include <algorithm> 
#include <iomanip> 
#include <cstdlib> // Untuk fungsi exit. 
#include <ctime> // Untuk fungsi waktu.
using namespace std;

struct Pemain {
    string username, password;
    string status, rank, role;
    int match, win, lose;

    // Fungsi boolean untuk mengecek apakah player sedang online / berada didalam game.
    bool isOnline;
};

vector<Pemain> players;
bool LoginBerhasil = false;
string UsernameTertaut;

// Deklarasi semua fungsi yang ada dalam program.
// Fungsi ataupun fitur yang dapat diakses oleh pemain (player).
void RegisterPemain();
void LoginPemain();
void LogoutPemain();
void SimpanDataPemainKeFile();
void MuatDataPemainDariFile();
void TampilkanProfilPemain(const Pemain& players);
void TampilkanDaftarPemain(const vector<Pemain>& players); // Dengan sorting bubble dan quick.
void SimulasiMatch(Pemain players);
void CariPemain(const vector<Pemain>& players);
//----------------------------------------------------------------//
// Fungsi ataupun fitur yang dapat diakses oleh admin.
void LoginAdmin();
void LogoutAdmin();
void HapusDataPemain(const string& username);
void ResetDataPemain(const string& username);
void TampilkanDaftarPemain(const vector<Pemain>& players);
void BroadcastPemain(const string& message);
void LogAktivitasPemain(const string& username, const string& activity);
void StatistikPemain(const vector<Pemain>& players);


//Fungsi untuk mendaftarkan pemain baru.
bool CekLogin(const string& user, const string& pass) {
    ifstream file("data_pemain.txt");
    string u, p;
    while (file >> u >> p) {
        if (u == user & p == pass) return true;
    }
    return false; // Mengembalikan nilai false jika username atau password salah.
}


// Fungsi untuk mengecek apakah username sudah terdaftar.
bool CekUsernameTerdaftar(const string& user) {
    ifstream file("data_pemain.txt"); // Membuka file data_pemain.txt
    string u, pass; // untuk menyimpan usn dan pass sementara karena kita akan membaca file tersebut.
    while (file >> u >> pass) {  // Membaca setiap line dari file.
        if (user == user) return true; // Mengembalikan nilai true jika username ditemukan.
            
    }
    return false; // Mengembalikan nilai false jika username tidak ditemukan.
}

void RegisterPemain() {
    string user, password;
    cout << "=====================================\n";
    cout << "|         REGISTRASI PEMAIN         |\n";
    cout << "=====================================\n";
    cout << "Masukkan username : ";
    cin >> user;

    if (CekUsernameTerdaftar(user)) {
        cout << "Username sudah digunakan! Silahkan gunakan username lain.\n";
        return;
    }

    cout << "Masukkan password : ";
    cin >> password;

    ofstream file("data_pemain.txt", ios::app);
    file << user << " " << password << endl;

    cout << "Pendaftaran berhasil! Silakan login untuk masuk.\n";
}

void LoginPemain() {
    string user, password;
    cout << "=================================\n";
    cout << "|           LOGIN PEMAIN        |\n";
    cout << "=================================\n";
    cout << "Username : "; cin >> user;
    cout << "Password : "; cin >> password;

    if (CekLogin(user, password)) {
        cout << "Login berhasil! Selamat datang, " << user << "." << "Nikmati waktu bermain anda!\n";
        UsernameTertaut = user; // Menyimpan usn yang sukses login.
        LoginBerhasil = true;
        MuatDataPemainDariFile(); // Memuat data pemain dari file.
    } else {
        cout << "Login gagal! Username atau password yang anda masukkan salah.\n";
    }
}

void LogoutPemain() {
    string username;
    cout << "=================================\n";
    cout << "|          LOGOUT PEMAIN         |\n";
    cout << "=================================\n";
    cout << "Masukkan username : ";
    cin >> username;

    for (auto& player : players) {
        if (player.username == username) {
            if (player.isOnline) {
                player.isOnline = false; // Mengubah status pemain menjadi offline.
                cout << "Logout berhasil! Sampai jumpa, " << username << "!\n";
            } else {
                cout << "Pemain belum login hari ini!\n";
            }
            return;
        }
    }
         cout << "Username tidak ditemukan!\n";
}   

void SimpanDataPemainKeFile() {
    ofstream file("data_pemain.txt");
    for (const auto& player : players) {
        file << player.username << " " << player.password << " "
             << player.status << " " << player.rank << " "
             << player.role << " " << player.match << " "
             << player.win << " " << player.lose << endl;
    }
}

void MuatDataPemainDariFile() {  
    ifstream file("data_pemain.txt");
    players.clear();
    Pemain player; // Membuat objek player untuk menyimpan data pemain sementara.
    while (file >> player.username >> player.password >> player.status >> player.rank
                >> player.role >> player.match >> player.win >> player.lose) {
        player.isOnline = false; // Untuk menandai bahwa pemain belum online.
        players.push_back(player); 
    }
}

void TampilkanProfilPemain(const Pemain& player) {
    cout << "=================================\n";
    cout << "|         PROFIL PEMAIN         |\n";
    cout << "=================================\n";
    cout << "Username : " << player.username << endl;
    cout << "Status : " << player.status << endl;
    cout << "Rank : " << player.rank << endl;
    cout << "Role : " << player.role << endl;
    cout << "Match : " << player.match << endl;
    cout << "Win : " << player.win << endl;
    cout << "Lose : " << player.lose << endl;
    cout << "Status Online : " << (player.isOnline ? "Online" : "Offline") << endl;
}

// Fungsi bubble sort.
void BubbleSortbyMatch(vector<Pemain>& data) {
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < data.size() - i - 1; ++j) {
            if (data[j].match < data[j + 1].match) {
                swap(data[j], data[j + 1]);
            }
        }
    }
}      
   
int PartisibyWin(vector<Pemain>& data, int low, int high) {
    int pivot = data[high].win; // Mengambil nilai win dari elemen terakhir sebagai pivot.
    int i = low - 1; // Indeks untuk elemen yang lebih kecil dari pivot.

    for (int j = low; j < high; ++j) {
        if (data[j].win > pivot) { 
            i++;
            swap(data[i], data[j]); 
        }
    }
    swap(data[i + 1], data[high]); // Tempatkan pivot di posisi yang benar.
    return i + 1; 
}

void QuickSortbyWin(vector<Pemain>& data, int low, int high) {
    if (low < high) {
        int pi = PartisibyWin(data, low, high); 
        QuickSortbyWin(data, low, pi - 1); // Rekursif untuk bagian kiri.
        QuickSortbyWin(data, pi + 1, high); // Rekursif untuk bagian kanan.
    }
}

void TampilkanDaftarPemain(const vector<Pemain>& players) {
    // User akan memilih ingin melihat daftar pemain berdasarkan sorting bubble atau quick sort.
    int pilihan;
    cout << "Tampilkan daftar pemain berdasarkan :\n";
    cout << "1. Banyaknya jumlah match\n";
    cout << "2. Banyaknya jumlah kemenangan (win)\n";
    cout << "Masukkan pilihan (1/2) : ";
    cin >> pilihan;

    vector<Pemain> sortedPemain = players; // Membuat salinan dari daftar pemain untuk diurutkan.

    if (pilihan == 1) {
        BubbleSortbyMatch(sortedPemain); 
    } else if (pilihan == 2) {
        QuickSortbyWin(sortedPemain, 0, sortedPemain.size() - 1); 
    } else {
        cout << "Pilihan yang anda masukkan tidak valid!\n";
        return;
    }

    cout << "=============================================================================================\n";
    cout << "|                                     DAFTAR PEMAIN GAME                                    |\n";
    cout << "=============================================================================================\n";
    cout << "| No |     Username     |  Status  |     Rank     |     Role     |  Match  |  Win  |  Lose  |\n";
    cout << "---------------------------------------------------------------------------------------------\n";
    int no = 1;
    for (const auto& player : players) {
        cout << "| " << setw(3) << no++ << " | " << setw(20) << player.username << " | "
             << setw(8) << player.status << " | " << setw(15) << player.rank << " | "
             << setw(12) << player.role << " | " << setw(7) << player.match << " | " 
             << setw(5) << player.win << " | " << setw(6) << player.lose << " |\n";
    }
    cout << "=============================================================================================\n";
}

void CariPemain(const vector<Pemain>& players) {
    string keyword;
    cout << "===================================\n";
    cout << "|           CARI PEMAIN           |\n";
    cout << "===================================\n";
    cout << "Masukkan username : ";
    cin.ignore(); getline(cin, keyword);

    cout << "========================================================================================\n";
    cout << "|                                     HASIL PENCARIAN                                  |\n";
    cout << "========================================================================================\n";
    cout << "|     Username     |  Status  |     Rank     |     Role     |  Match  |  Win  |  Lose  |\n";
    cout << "----------------------------------------------------------------------------------------\n";
    for (const auto& player : players) {
        cout << " | " << setw(20) << player.username << " | "
             << setw(8) << player.status << " | " << setw(15) << player.rank << " | "
             << setw(12) << player.role << " | " << setw(7) << player.match << " | " 
             << setw(5) << player.win << " | " << setw(6) << player.lose << " |\n";
    }
    cout << "========================================================================================\n";
}

void SimulasiMatch(Pemain& player) {
    if (!player.isOnline) {
        cout << "Anda harus login untuk bermain match.\n";
        return;
    }

    int pilihan;
    cout << "==================================================================\n";
    cout << "|                           MATCHMAKING                          |\n";
    cout << "==================================================================\n";
    cout << "| Selamat datang di match!                                       |\n";
    cout << "| Anda akan bermain sebagai " << player.username << "." <<      "|\n";
    cout << "| Sebelum memulai match, silakan pilih role anda.                |\n";
    cout << "------------------------------------------------------------------\n";
    cout << "| Pilih role yang anda inginkan. (1-5)                           |\n";
    cout << "| 1. Tank                                                        |\n";
    cout << "| 2. Mage                                                        |\n";
    cout << "| 3. Marksman                                                    |\n";
    cout << "| 4. Assassin                                                    |\n";
    cout << "| 5. Support                                                     |\n";
    cout << "==================================================================\n";
    cout << "Masukkan pilihan (1-5) : ";
    cin >> pilihan;

    string role;
    switch (pilihan) {
        case 1: role = "Tank"; break;
        case 2: role = "Mage"; break;
        case 3: role = "Marksman"; break;
        case 4: role = "Assassin"; break;
        case 5: role = "Support"; break;
        default:
            cout << "Pilihan anda tidak valid. Program gagal memulai match.\n";
            return;
    }

    // Simulasi hasil match (menang/kalah secara acak)
    srand(time(0)); 
    cout << "Anda telah memilih role : " << role << ".\n";
    cout << "Match dimulai...\n";
    cout << "========================================\n";
    cout << "|              MATCHMAKING             |\n";
    cout << "========================================\n";
    cout << "Anda bermain sebagai " << role << ".\n";
    cout << "Match sedang berlangsung...\n";
    cout << "----------------------------------------\n";
   
    cout << "Match selesai!\n";
    cout << "========================================\n";
    cout << "|            HASIL MATCH                |\n";
    cout << "========================================\n";

    // Menggunakan rand() untuk menentukan hasil match
    bool menang = rand() % 2 == 0; // 50% chance win (peluang).

    player.role = role;
    player.match++;
    if (menang) {
        player.win++;
        cout << "Selamat! kamu MENANG dalam match ini sebagai " << role << ".\n";
    } else {
        player.lose++;
        cout << "Yaahhh, kamu KALAH dalam match ini sebagai " << role << ".\n";
    }

    // Simpan riwayat match ke file
    ofstream riwayat("riwayat_match.txt", ios::app);
    riwayat << player.username << " | Role: " << role
            << " | Hasil: " << (menang ? "Menang" : "Kalah") << "|" << endl;

    // Simpan perubahan data pemain ke file
    SimpanDataPemainKeFile();
}

//struct Admin {
    //string username, password;
//};
 
//vector<Admin> admins;


