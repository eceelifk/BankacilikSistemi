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
            islemler.push_back(getZaman() + " | +" + to_string(miktar) + " TL yat�r�ld�.");
            cout << "Yeni bakiye: " << bakiye << " TL\n";
        } else {
            cout << "Ge�ersiz miktar!\n";
        }
    }

    void paraCek(double miktar) {
        if (miktar > 0 && miktar <= bakiye) {
            bakiye -= miktar;
            islemler.push_back(getZaman() + " | -" + to_string(miktar) + " TL �ekildi.");
            cout << "Yeni bakiye: " << bakiye << " TL\n";
        } else {
            cout << "Yetersiz bakiye veya ge�ersiz miktar!\n";
        }
    }

    void krediCek(double miktar) {
        if (miktar > 0) {
            bakiye += miktar;
            islemler.push_back(getZaman() + " | +" + to_string(miktar) + " TL kredi �ekildi.");
            cout << "Yeni bakiye: " << bakiye << " TL\n";
        } else {
            cout << "Ge�ersiz kredi miktar�!\n";
        }
    }

    void paraGonder(Hesap &alici, double miktar) {
        if (miktar > 0 && miktar <= bakiye) {
            bakiye -= miktar;
            alici.bakiye += miktar;
            islemler.push_back(getZaman() + " | -" + to_string(miktar) + " TL g�nderildi --> Hesap No: " + to_string(alici.getHesapNo()));
            alici.islemler.push_back(getZaman() + " | +" + to_string(miktar) + " TL al�nd� <-- Hesap No: " + to_string(this->hesapNo));
            cout << miktar << " TL ba�ar�yla g�nderildi.\n";
        } else {
            cout << "Yetersiz bakiye veya ge�ersiz miktar!\n";
        }
    }

    void hesapBilgileri() {
        cout << "\n--- Hesap Bilgileri ---\n";
        cout << "M��teri Ad�: " << musteriAdi << endl;
        cout << "Hesap No: " << hesapNo << endl;
        cout << "Bakiye: " << bakiye << " TL\n";
    }

    void islemGecmisi() {
        cout << "\n--- ��lem Ge�mi�i ---\n";
        if (islemler.empty()) {
            cout << "Hen�z i�lem yok.\n";
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

    cout << "=== Bankac�l�k Sistemine Ho� Geldiniz ===\n�ifreyi girin: ";
    cin >> sifreGirisi;

    if (sifreGirisi != sistemSifresi) {
        cout << "Hatal� �ifre. Programdan ��k�l�yor.\n";
        return 0;
    }

    vector<Hesap> hesaplar;
    int secim;

   
    cout << "\n--- Deneme Ba�lat�l�yor ---\n";

  
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


    cout << "\nElif'in G�ncel ��lem Ge�mi�i:\n";
    hesapElif->islemGecmisi();

   
    do {
        cout << "\n--- Ana Men� ---\n";
        cout << "1. Yeni Hesap Olu�tur\n";
        cout << "2. Hesaba Giri� Yap\n";
        cout << "3. ��k��\n";
        cout << "Se�iminiz: ";
        cin >> secim;

        if (secim == 1) {
            string ad;
            int no;
            cout << "M��teri ad�: ";
            cin.ignore(); getline(cin, ad);
            cout << "Yeni hesap numaras�: ";
            cin >> no;
            if (hesapBul(hesaplar, no)) {
                cout << "Bu hesap numaras� zaten mevcut!\n";
            } else {
                hesaplar.push_back(Hesap(ad, no));
                cout << "Hesap ba�ar�yla olu�turuldu!\n";
            }
        }

        else if (secim == 2) {
            int no;
            cout << "Hesap numaras�: ";
            cin >> no;
            Hesap* aktifHesap = hesapBul(hesaplar, no);
            if (!aktifHesap) {
                cout << "Hesap bulunamad�!\n";
            } else {
                int sec;
                do {
                    cout << "\n--- Hesap Men�s� ---\n";
                    cout << "1. Para Yat�r\n";
                    cout << "2. Para �ek\n";
                    cout << "3. Hesap Bilgileri\n";
                    cout << "4. ��lem Ge�mi�i\n";
                    cout << "5. Para G�nder\n";
                    cout << "6. Kredi �ek\n";
                    cout << "7. ��k�� Yap\n";
                    cout << "Se�iminiz: ";
                    cin >> sec;
                    double miktar;
                    int hedefNo;

                    switch (sec) {
                        case 1:
                            cout << "Yat�r�lacak miktar: ";
                            cin >> miktar;
                            aktifHesap->paraYatir(miktar);
                            break;
                        case 2:
                            cout << "�ekilecek miktar: ";
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
                            cout << "G�nderilecek hesap numaras�: ";
                            cin >> hedefNo;
                            cout << "Miktar: ";
                            cin >> miktar;
                            if (Hesap* alici = hesapBul(hesaplar, hedefNo)) {
                                aktifHesap->paraGonder(*alici, miktar);
                            } else {
                                cout << "Al�c� hesap bulunamad�!\n";
                            }
                            break;
                        case 6:
                            cout << "Kredi miktar�: ";
                            cin >> miktar;
                            aktifHesap->krediCek(miktar);
                            break;
                        case 7:
                            cout << "Hesaptan ��k�l�yor...\n";
                            break;
                        default:
                            cout << "Ge�ersiz se�im!\n";
                    }
                } while (sec != 7);
            }
        }

    } while (secim != 3);

    cout << "Programdan ��k�l�yor. G�r��mek �zere!\n";
    return 0;
}

