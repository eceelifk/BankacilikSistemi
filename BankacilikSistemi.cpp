#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

class Hesap {
private:
    string musteriAdi;
    int hesapNo;
    double bakiye;
    vector<string> islemler;

public:
    Hesap(string ad, int no) {
        musteriAdi = ad;
        hesapNo = no;
        bakiye = 0.0;
    }

    int getHesapNo() { return hesapNo; }

    void paraYatir(double miktar) {
        if (miktar > 0) {
            bakiye += miktar;
            islemler.push_back(getZaman() + " | +" + to_string(miktar) + " TL yatirildi.");
            cout << "Yeni bakiye: " << bakiye << " TL\n";
        } else {
            cout << "Geçersiz miktar!\n";
        }
    }

    void paraCek(double miktar) {
        if (miktar > 0 && miktar <= bakiye) {
            bakiye -= miktar;
            islemler.push_back(getZaman() + " | -" + to_string(miktar) + " TL çekildi.");
            cout << "Yeni bakiye: " << bakiye << " TL\n";
        } else {
            cout << "Yetersiz bakiye veya geçersiz miktar!\n";
        }
    }

    void krediCek(double miktar) {
        if (miktar > 0) {
            bakiye += miktar;
            islemler.push_back(getZaman() + " | +" + to_string(miktar) + " TL kredi çekildi.");
            cout << "Yeni bakiye: " << bakiye << " TL\n";
        } else {
            cout << "Geçersiz kredi miktari!\n";
        }
    }

    void paraGonder(Hesap &alici, double miktar) {
        if (miktar > 0 && miktar <= bakiye) {
            bakiye -= miktar;
            alici.bakiye += miktar;
            islemler.push_back(getZaman() + " | -" + to_string(miktar) + " TL gönderildi --> Hesap No: " + to_string(alici.getHesapNo()));
            alici.islemler.push_back(getZaman() + " | +" + to_string(miktar) + " TL alýndý <-- Hesap No: " + to_string(this->hesapNo));
            cout << miktar << " TL basariyla gönderildi.\n";
        } else {
            cout << "Yetersiz bakiye veya geçersiz miktar!\n";
        }
    }

    void hesapBilgileri() {
        cout << "\n--- Hesap Bilgileri ---\n";
        cout << "Müsteri Adi: " << musteriAdi << endl;
        cout << "Hesap No: " << hesapNo << endl;
        cout << "Bakiye: " << bakiye << " TL\n";
    }

    void islemGecmisi() {
        cout << "\n--- İslem Geçmisi ---\n";
        if (islemler.empty()) {
            cout << "Henüz islem yok.\n";
        } else {
            for (const string &islem : islemler) {
                cout << islem << endl;
            }
        }
    }

    string getZaman() {
        time_t simdi = time(0);
        tm *ltm = localtime(&simdi);
        char zaman[20];
        sprintf(zaman, "%02d/%02d/%04d %02d:%02d", 
                ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900,
                ltm->tm_hour, ltm->tm_min);
        return string(zaman);
    }
};

Hesap* hesapBul(vector<Hesap>& hesaplar, int no) {
    for (auto& h : hesaplar) {
        if (h.getHesapNo() == no) return &h;
    }
    return nullptr;
}

int main() {
    string sifreGirisi;
    const string sistemSifresi = "1234";

    cout << "=== Bankacilik Sistemine Hos Geldiniz ===\nSifreyi girin: ";
    cin >> sifreGirisi;

    if (sifreGirisi != sistemSifresi) {
        cout << "Hatali sifre.\n";
        return 0;
    }

    vector<Hesap> hesaplar;
    int secim;

   
    cout << "\n--- Deneme Baslatiliyor ---\n";

  
    hesaplar.push_back(Hesap("Elif", 1001));
    hesaplar.push_back(Hesap("Ahmet", 1002));

    Hesap* hesapElif = &hesaplar[0];
    hesapElif->paraYatir(1000);

    
    Hesap* hesapAhmet = &hesaplar[1];
    hesapElif->paraGonder(*hesapAhmet, 200);

   
    cout << "\nElif'in Hesap Bilgileri:\n";
    hesapElif->hesapBilgileri();
    hesapElif->islemGecmisi();

    
    cout << "\nAhmet'in Hesap Bilgileri:\n";
    hesapAhmet->hesapBilgileri();
    hesapAhmet->islemGecmisi();

  
    hesapElif->krediCek(500);


    cout << "\nElif'in Güncel İslem Geçmisi:\n";
    hesapElif->islemGecmisi();

   
    do {
        cout << "\n--- Ana Menü ---\n";
        cout << "1. Yeni Hesap Olustur\n";
        cout << "2. Hesaba Giriþ Yap\n";
        cout << "3. Çikis\n";
        cout << "Seçiminiz: ";
        cin >> secim;

        if (secim == 1) {
            string ad;
            int no;
            cout << "Müsteri adi: ";
            cin.ignore(); getline(cin, ad);
            cout << "Yeni hesap numarasi: ";
            cin >> no;
            if (hesapBul(hesaplar, no)) {
                cout << "Bu hesap numarasi zaten mevcut!\n";
            } else {
                hesaplar.push_back(Hesap(ad, no));
                cout << "Hesap basariyla olusturuldu!\n";
            }
        }

        else if (secim == 2) {
            int no;
            cout << "Hesap numarasi: ";
            cin >> no;
            Hesap* aktifHesap = hesapBul(hesaplar, no);
            if (!aktifHesap) {
                cout << "Hesap bulunamadý!\n";
            } else {
                int sec;
                do {
                    cout << "\n--- Hesap Menüsü ---\n";
                    cout << "1. Para Yatir\n";
                    cout << "2. Para Çek\n";
                    cout << "3. Hesap Bilgileri\n";
                    cout << "4. İslem Geçmisi\n";
                    cout << "5. Para Gönder\n";
                    cout << "6. Kredi Çek\n";
                    cout << "7. Çikis Yap\n";
                    cout << "Seçiminiz: ";
                    cin >> sec;
                    double miktar;
                    int hedefNo;

                    switch (sec) {
                        case 1:
                            cout << "Yatirilacak miktar: ";
                            cin >> miktar;
                            aktifHesap->paraYatir(miktar);
                            break;
                        case 2:
                            cout << "Çekilecek miktar: ";
                            cin >> miktar;
                            aktifHesap->paraCek(miktar);
                            break;
                        case 3:
                            aktifHesap->hesapBilgileri();
                            break;
                        case 4:
                            aktifHesap->islemGecmisi();
                            break;
                        case 5:
                            cout << "Gönderilecek hesap numarasi: ";
                            cin >> hedefNo;
                            cout << "Miktar: ";
                            cin >> miktar;
                            if (Hesap* alici = hesapBul(hesaplar, hedefNo)) {
                                aktifHesap->paraGonder(*alici, miktar);
                            } else {
                                cout << "Alici hesap bulunamadi!\n";
                            }
                            break;
                        case 6:
                            cout << "Kredi miktari: ";
                            cin >> miktar;
                            aktifHesap->krediCek(miktar);
                            break;
                        case 7:
                            cout << "Hesaptan çikiliyor...\n";
                            break;
                        default:
                            cout << "Geçersiz seçim!\n";
                    }
                } while (sec != 7);
            }
        }

    } while (secim != 3);

    cout << "Programdan çikiliyor. Görüsmek üzere!\n";
    return 0;
}

