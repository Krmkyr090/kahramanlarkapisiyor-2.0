#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>
#include <cctype>
#include <vector>
using namespace std;

// === RENKLER ===
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

// === Yardımcı Fonksiyonlar ===
void bekle_ms(int ms) { this_thread::sleep_for(chrono::milliseconds(ms)); }
void noktaAnim(int n = 3, int ms = 250) { for(int i = 0; i < n; i++) { cout << '.'; cout.flush(); bekle_ms(ms); } cout << endl; }

string trim(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

string toLowerNoSpace(const string &s) {
    string t;
    for (char c : s)
        if (!isspace((unsigned char)c))
            t += char(tolower((unsigned char)c));
    return t;
}

string safeLine() {
    string line;
    if (!getline(cin, line)) {
        cin.clear();
        getline(cin, line);
    }
    return line;
}

int safeIntFromLine(const string &prompt) {
    while (true) {
        cout << prompt;
        string line = safeLine();
        line = trim(line);
        try {
            return stoi(line);
        } catch (...) {
            cout << RED << "❌ Geçersiz sayı, tekrar dene.\n" << RESET;
        }
    }
}

// === ASCII Efektler ===
void asciiIntro() {
    cout << BOLD << CYAN;
    cout << "==============================================\n";
    cout << " SÜPER KAHRAMANLAR ARENASI\n";
    cout << " GODLY EDITION (6 KAHRAMAN!)\n";
    cout << "   💥 EPİK CANLAR AKTİF 💥\n";
    cout << "==============================================\n" << RESET;
    bekle_ms(600);
    cout << YELLOW << "Previously on... Godly Arena"; noktaAnim(3, 300);
}

void efektSpiderman() { cout << MAGENTA << R"( .--.
 ( ) THWIP!
  '--' )" << RESET << "\n"; bekle_ms(500); cout << "🕸️ Spiderman: Ağ tuzağı!\n"; bekle_ms(400); }
void efektBatman() { cout << BLUE << R"( /\
  /__\ BOOM!
 (| |) )" << RESET << "\n"; bekle_ms(500); cout << "💣 Batman: Batarang!\n"; bekle_ms(400); }
void efektIronMan() { cout << RED << R"( /-\
  |o o| PEW PEW!
   \_/ )" << RESET << "\n"; bekle_ms(500); cout << "⚡ Iron Man: Lazer!\n"; bekle_ms(400); }
void efektHulk() { cout << GREEN << R"( .----.
 / \ RAAAGH!
 \ / )" << RESET << "\n"; bekle_ms(500); cout << "💪 Hulk: SMASH!\n"; bekle_ms(400); }
void efektSuperman() { cout << CYAN << R"( ☄️
 /\
/ \ ZAP!
 )" << RESET << "\n"; bekle_ms(500); cout << "💥 Superman: Heat Vision!\n"; bekle_ms(400); }
void efektThor() { cout << YELLOW << R"( ⚡
  /|\
 / | \ THOR!
  / \
  Mjolnir )" << RESET << "\n"; bekle_ms(500); cout << "🔨 Thor: Thunder Strike!\n"; bekle_ms(400); }

// === Element Sistemi ===
enum Element { ATES, SU, ELEKTRIK, DOGA, YOK };
string elementAdi(Element e) {
    switch(e) {
        case ATES: return "Ateş";
        case SU: return "Su";
        case ELEKTRIK: return "Elektrik";
        case DOGA: return "Doğa";
        default: return "Yok";
    }
}

double elementCarpani(Element a, Element b) {
    if (a == YOK || b == YOK) return 1.0;
    if (a == ATES && b == DOGA) return 1.5;
    if (a == DOGA && b == ELEKTRIK) return 1.5;
    if (a == ELEKTRIK && b == SU) return 1.5;
    if (a == SU && b == ATES) return 1.5;
    if (a == DOGA && b == ATES) return 0.5;
    if (a == ELEKTRIK && b == DOGA) return 0.5;
    if (a == SU && b == ELEKTRIK) return 0.5;
    if (a == ATES && b == SU) return 0.5;
    return 1.0;
}

// === Kahraman Sınıfı ===
class Kahraman {
public:
    string ad;
    int can, hasar, maxCan;
    int ozelCooldown, ultimateCooldown;
    bool stunlu;
    Element element;
    // İstatistik
    int toplamHasar, kritikSayisi, yenilemeSayisi, vurusSayisi, ozelKullanmaSayisi, ultimateKullanmaSayisi, selfHasar, toplamAldigiHasar;

    Kahraman() {}
    
    // 💥 EPİK CANLAR AKTİF 💥
    Kahraman(const string &isim, int c, int h, Element el = YOK) {
        ad = isim; 
        can = c; 
        hasar = h; 
        maxCan = c; 
        element = el;
        ozelCooldown = ultimateCooldown = 0; 
        stunlu = false;
        toplamHasar = kritikSayisi = yenilemeSayisi = vurusSayisi = ozelKullanmaSayisi = ultimateKullanmaSayisi = selfHasar = toplamAldigiHasar = 0;
    }

    string getRenk() {
        if (ad == "Spiderman") return MAGENTA;
        if (ad == "Batman") return BLUE;
        if (ad == "Iron Man") return RED;
        if (ad == "Hulk") return GREEN;
        if (ad == "Superman") return CYAN;
        if (ad == "Thor") return YELLOW;
        return WHITE;
    }

    void durumGoster() {
        int bar = (can * 20) / maxCan; if (bar < 0) bar = 0;
        string renk = getRenk();
        cout << renk << BOLD << ad << RESET << " [";
        for (int i = 0; i < bar; i++) cout << "▓";
        for (int i = bar; i < 20; i++) cout << "░";
        cout << "] " << BOLD << can << "/" << maxCan << RESET << " HP";
        if (ozelCooldown > 0) cout << " | Özel:" << ozelCooldown;
        else cout << " | Özel:✔";
        if (ultimateCooldown > 0) cout << " | Ult:" << ultimateCooldown;
        else cout << " | Ult:✔";
        if (stunlu) cout << " | ⚡STUN";
        if (element != YOK) cout << " | " << elementAdi(element)[0];
        cout << endl;
    }

    void saldir(Kahraman &rakip, const string &hava, int kritikSans) {
        int baseDmg = hasar;
        if (hava == "Gunesli") baseDmg += 10;
        else if (hava == "Sisli") baseDmg = baseDmg * 75 / 100;
        double carp = elementCarpani(element, rakip.element);
        baseDmg = int(baseDmg * carp);
        int kritik = rand() % 100;
        int dmg = baseDmg;
        if (kritik < kritikSans) {
            dmg *= 2;
            kritikSayisi++;
            cout << YELLOW << "🔥 KRİTİK! " << RESET;
        }
        if (carp > 1.0) cout << GREEN << "(x1.5!) " << RESET;
        else if (carp < 1.0) cout << RED << "(x0.5) " << RESET;
        cout << ad << " > " << dmg << " hasar!\n";
        rakip.can -= dmg; if (rakip.can < 0) rakip.can = 0;
        toplamHasar += dmg; vurusSayisi++; rakip.toplamAldigiHasar += dmg;
    }

    void ozelGuc(Kahraman &rakip) {
        if (ozelCooldown > 0) { cout << getRenk() << ad << " özel CD:" << ozelCooldown << RESET << "\n"; return; }
        cout << getRenk() << ad << " ÖZEL! " << RESET; noktaAnim();
       
        if (ad == "Spiderman") efektSpiderman();
        else if (ad == "Batman") efektBatman();
        else if (ad == "Iron Man") efektIronMan();
        else if (ad == "Hulk") efektHulk();
        else if (ad == "Superman") efektSuperman();
        else if (ad == "Thor") efektThor();

        int ozelDmg = hasar * 2;
        if (ad == "Hulk") ozelDmg = hasar * 3;
        if (ad == "Iron Man") ozelDmg += 20;
        if (ad == "Superman") ozelDmg = hasar * 2 + 25;
        if (ad == "Thor") ozelDmg = hasar * 2 + 30;
        double carp = elementCarpani(element, rakip.element);
        ozelDmg = int(ozelDmg * carp);

        bool stun = false;
        if (ad == "Spiderman") {
            stun = true;
            cout << "🕸️ Ağ! Stun + " << ozelDmg << "!\n";
        } else if (ad == "Batman") {
            int self = 10;
            can -= self; if (can < 0) can = 0;
            selfHasar += self; toplamAldigiHasar += self;
            cout << "💣 Bomba! " << ozelDmg << " (self:" << self << ")\n";
        } else if (ad == "Superman") {
            rakip.ozelCooldown = max(0, rakip.ozelCooldown - 1);
            cout << "🔥 Heat Vision! " << ozelDmg << " + savunma kır!\n";
        } else if (ad == "Thor") {
            stun = (rand() % 100 < 30);
            cout << "⚡ Thunder! " << ozelDmg << " hasar" << (stun ? " + STUN!" : "") << "\n";
        } else {
            cout << "💥 Özel! " << ozelDmg << "!\n";
        }
       
        rakip.can -= ozelDmg; if (rakip.can < 0) rakip.can = 0;
        if (stun) rakip.stunlu = true;
        toplamHasar += ozelDmg; ozelKullanmaSayisi++; vurusSayisi++; rakip.toplamAldigiHasar += ozelDmg;
        ozelCooldown = 3;
    }

    void ultimateGuc(Kahraman &rakip, string &havaDurum) {
        if (ultimateCooldown > 0) { cout << getRenk() << ad << " ult CD:" << ultimateCooldown << RESET << "\n"; return; }
        cout << BOLD << RED << ad << " ULTIMATE! ⚔️" << RESET; noktaAnim(4, 400);

        int dmg = 0; string efekt = ""; bool yeniHava = false;
        if (ad == "Spiderman") { dmg = 50; rakip.stunlu = true; efekt = "AĞ FIRTINASI! Stun+50"; }
        else if (ad == "Batman") { dmg = 100; efekt = "GÖLGE! 100 hasar"; }
        else if (ad == "Iron Man") { dmg = 150; efekt = "UNIBEAM! 150"; }
        else if (ad == "Hulk") { dmg = 200; int self = 50; can -= self; if(can<0)can=0; selfHasar+=self; toplamAldigiHasar+=self; efekt = "DÜNYA KIRICI! 200(self50)"; }
        else if (ad == "Superman") { dmg = 150; rakip.stunlu = true; efekt = "SPEED PUNCH! 150+Stun"; }
        else if (ad == "Thor") {
            dmg = 180;
            efekt = "RAGNAROK! 180 hasar + FIRTINA 2 tur!";
            yeniHava = true;
            havaDurum = "Firtinali";
        }
        double carp = elementCarpani(element, rakip.element);
        dmg = int(dmg * carp);
        rakip.can -= dmg; if (rakip.can < 0) rakip.can = 0;
       
        cout << BOLD << YELLOW << efekt << " (" << dmg << ")" << RESET << "\n";
        if (yeniHava) cout << RED << "🌩️ HAVA: FIRTINALI (2 tur heal yok!)" << RESET << "\n";
        toplamHasar += dmg; ultimateKullanmaSayisi++; vurusSayisi++; rakip.toplamAldigiHasar += dmg;
        ultimateCooldown = 3;
    }

    void heal(const string &hava) {
        if (hava == "Firtinali") { cout << "🌩️ Fırtına! Heal yok!\n"; return; }
        int yenile = 25 + rand() % 16; // Heal de artırdım!
        if (ad == "Hulk") yenile = 20 + rand() % 21;
        else if (ad == "Superman") yenile += 15;
        else if (ad == "Thor") yenile += 8;
        can += yenile; if (can > maxCan) can = maxCan;
        yenilemeSayisi++;
        cout << GREEN << ad << " +" << yenile << " HP (" << can << ")\n" << RESET;
    }

    void turSonuGuncelle() {
        if (ozelCooldown > 0) ozelCooldown--;
        if (ultimateCooldown > 0) ultimateCooldown--;
        stunlu = false;
    }

    void istatistikGoster() {
        string renk = getRenk();
        cout << "\n" << renk << BOLD << ad << " STATS" << RESET << "\n";
        cout << "DMG:" << toplamHasar << " | HIT:" << vurusSayisi << " | CRIT:" << kritikSayisi << "\n";
        cout << "HL:" << yenilemeSayisi << " | SP:" << ozelKullanmaSayisi << " | UL:" << ultimateKullanmaSayisi << "\n";
        cout << "SELF:" << selfHasar << " | RCVD:" << toplamAldigiHasar << "\n";
    }
};

// === Fonksiyonlar ===
string havaSec() {
    string havalar[] = {"Gunesli", "Yagmurlu", "Firtinali", "Sisli"};
    return havalar[rand() % 4];
}

Kahraman karakterSec(int oyuncu) {
    while (true) {
        cout << "\n" << BOLD << GREEN << "💥 EPİK CANLAR AKTİF 💥" << RESET << endl;
        cout << BOLD << "Oyuncu " << oyuncu << " SEÇ!\n" << RESET;
        cout << "1) Spiderman 🕷️ (140/40 Doğa)\n";
        cout << "2) Batman 🦇 (180/30 Yok)\n";
        cout << "3) Iron Man ⚡ (130/50 Elektrik)\n";
        cout << "4) Hulk 💪 (250/60 Ateş)\n";
        cout << "5) Superman 🦸 (320/55 Ateş)\n";
        cout << "6) Thor 🔨 (290/65 Elektrik)\n";
        cout << "(r) Rastgele: ";
        
        string girdi = safeLine(); girdi = trim(girdi);
        string key = toLowerNoSpace(girdi);

        // 💥 YENİ EPİK CANLAR 💥
        if (key == "1" || key.find("spider") != string::npos) { cout << "🕷️ Spiderman (140 HP)\n"; return Kahraman("Spiderman", 140, 40, DOGA); }
        else if (key == "2" || key == "batman") { cout << "🦇 Batman (180 HP)\n"; return Kahraman("Batman", 180, 30, YOK); }
        else if (key == "3" || key.find("iron") != string::npos) { cout << "⚡ Iron Man (130 HP)\n"; return Kahraman("Iron Man", 130, 50, ELEKTRIK); }
        else if (key == "4" || key == "hulk") { cout << "💪 Hulk (250 HP)\n"; return Kahraman("Hulk", 250, 60, ATES); }
        else if (key == "5" || key.find("super") != string::npos) { cout << "🦸 Superman (320 HP)\n"; return Kahraman("Superman", 320, 55, ATES); }
        else if (key == "6" || key.find("thor") != string::npos) { cout << "🔨 THOR! (290 HP)\n"; return Kahraman("Thor", 290, 65, ELEKTRIK); }
        else if (key == "r") {
            vector<Kahraman> liste = {
                Kahraman("Spiderman",140,40,DOGA), 
                Kahraman("Batman",180,30,YOK),
                Kahraman("Iron Man",130,50,ELEKTRIK), 
                Kahraman("Hulk",250,60,ATES),
                Kahraman("Superman",320,55,ATES), 
                Kahraman("Thor",290,65,ELEKTRIK)
            };
            int rast = rand() % 6;
            cout << "🎲 " << liste[rast].ad << "!\n";
            return liste[rast];
        } else {
            cout << RED << "❌ 1-6/isim/r!\n" << RESET;
        }
    }
}

int kararAI(Kahraman &ai, Kahraman &rakip, const string &hava, int kritikSans) {
    int ol = rand() % 100;
    int agresiflik = (ai.ad == "Hulk" || ai.ad == "Thor" || ai.ad == "Superman") ? 92 : (ai.ad == "Batman") ? 55 : 75;
    
    int ultDmg = (ai.ad == "Thor") ? 180 : (ai.ad == "Hulk") ? 200 : (ai.ad == "Superman") ? 150 : 120;
    if (ai.ultimateCooldown == 0 && ultDmg >= rakip.can && ol < 98) return 5;
   
    int ozelDmg = ai.hasar * 2 + 25;
    if (ai.ozelCooldown == 0 && ozelDmg >= rakip.can && ol < 95) return 2;
    
    // AI heal eşiği artırıldı (yeni canlar için)
    if (ai.can < ai.maxCan * 0.25 && hava != "Firtinali" && ol < 65) return 3;
    
    if (ai.ultimateCooldown == 0 && ol < 28) return 5;
    if (ai.ozelCooldown == 0 && ol < agresiflik) return 2;
    if (ol < 88) return 1;
    return 4;
}

// === MAIN ===
int main() {
    srand((unsigned)time(0));
    char tekrar;
    do {
        // system("clear");
        asciiIntro();

        cout << "\nMod: 1) PvP 2) vs AI\n> ";
        string mod = safeLine(); mod = trim(mod);
        bool vsAI = (mod == "2" || toLowerNoSpace(mod) == "ai");

        Kahraman p1 = karakterSec(1);
        Kahraman p2 = vsAI ? karakterSec(2) : karakterSec(2);

        cout << "\n" << BOLD << GREEN << "⚔️ EPİK ARENA AÇILDI! ⚔️" << RESET; noktaAnim(3,350);

        int tur = 1, siradaki = 1;
        while (p1.can > 0 && p2.can > 0) {
            cout << "\n" << BOLD << CYAN << "=== TUR " << tur << " ===" << RESET << "\n";
            string hava = havaSec();
            int kritik = (hava == "Yagmurlu") ? 20 : 30;
            cout << "🌦️ " << hava << " (Krit %" << kritik << ")\n";

            Kahraman *aktif = (siradaki == 1) ? &p1 : &p2;
            Kahraman *rakip = (siradaki == 1) ? &p2 : &p1;

            cout << "\n--- BAŞLANGIÇ ---\n"; 
            p1.durumGoster(); 
            p2.durumGoster();

            if (aktif->stunlu) {
                cout << RED << aktif->ad << " ⚡STUN! Pas\n" << RESET;
            } else {
                if (vsAI && siradaki == 2) {
                    bekle_ms(900);
                    int sec = kararAI(*aktif, *rakip, hava, kritik);
                    cout << "[AI] " << aktif->ad << ":\n";
                    if (sec == 1) { cout << "Saldırı\n"; aktif->saldir(*rakip, hava, kritik); }
                    else if (sec == 2) { cout << "ÖZEL\n"; aktif->ozelGuc(*rakip); }
                    else if (sec == 3) { cout << "Heal\n"; aktif->heal(hava); }
                    else if (sec == 5) { cout << "ULTIMATE\n"; aktif->ultimateGuc(*rakip, hava); }
                    else cout << "Pas\n";
                } else {
                    cout << "\n" << aktif->ad << ":\n1=Attack 2=Özel 3=Heal 4=Pas 5=ULT\n> ";
                    int sec = safeIntFromLine("");
                    if (sec == 1) aktif->saldir(*rakip, hava, kritik);
                    else if (sec == 2) aktif->ozelGuc(*rakip);
                    else if (sec == 3) aktif->heal(hava);
                    else if (sec == 5) aktif->ultimateGuc(*rakip, hava);
                    else if (sec == 4) cout << "Pas\n";
                    else cout << RED << "Geçersiz! Pas\n" << RESET;
                }
            }

            p1.turSonuGuncelle(); p2.turSonuGuncelle();

            cout << "\n--- SONUC ---\n"; 
            p1.durumGoster(); 
            p2.durumGoster();

            siradaki = 3 - siradaki;
            if (siradaki == 1) tur++;
            bekle_ms(600);
        }

        string kazanan = (p1.can > 0 ? p1.ad : p2.ad);
        cout << "\n🏆 " << BOLD << YELLOW << kazanan << " 💥EPİK ŞAMPİYON!💥 🏆\n" << RESET;
        p1.istatistikGoster(); p2.istatistikGoster();

        cout << "\nTekrar? (e/h): "; cin >> tekrar; cin.ignore();
    } while (tolower(tekrar) == 'e');

    cout << BOLD << MAGENTA << "\n Odin seni çağırıyor kanka! 🔥⚡\n" << RESET;
    return 0;
}