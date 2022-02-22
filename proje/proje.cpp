﻿///NOT:
//isimler soyimler txt dosyalarinda 300 satıra askin veri oldugundan rand 300 e kadar deger uretmekte daha az verili txt dosyalarında hata alınabilir.
//Program hiç bir şekilde loopta kalmamaktadır print yaparken bekleme suresi bilgisayarınızın hızına göre değişiklik gösterebilir.
#pragma endregion

#include <iostream>
#include<string> 
#include<fstream>//dosya islemleri icin eklendi. 
#include <ctime>//sabit zamanı srand ile yenilemek icin eklendi.
#include<conio.h>//_getch() fonksiyonunu kullanmak için eklendi
#include <cmath>//round yuvarlama fonksiyonu kullanmak için eklendi.

using namespace std;

string takmaAd(string);//takma ad prototip.
void fiksturOlustur();//fikstur prototip.



#pragma region Linkli Listler

struct FutbolcularListe;//Futbolcu listesi asagida tanimladigi icin prototip tanimlandi.

#pragma region TAKIM

#pragma region Formasyon bilgisi

class Formasyonbilgi//Formasyon bilgisini tutan listedir. Aynı zamanda performans ortamalarini tutar.
{
private:
	int v = 0;
	int kale = 1;
public:

	int defans = 4;
	int ortasaha = 4;
	int forvet = 2;

	int kaleciPerf = 0;
	int defansPerf = 0;
	int ortasahaPerf = 0;
	int forvetPerf = 0;


	float getPerfOrt();
	float getDefansOrt();
	float getOrtasahaOrt();
	float getForvetOrt();

	void clear();
	int* get();

};

void Formasyonbilgi::clear() //Formasyondaki performanlari yeniler.
{
	kaleciPerf = 0;
	defansPerf = 0;
	ortasahaPerf = 0;
	forvetPerf = 0;
}

float Formasyonbilgi::getPerfOrt()//Takimin ortalama performansi. 
{
	return (float)(kaleciPerf + defansPerf + ortasahaPerf + forvetPerf) / (kale + defans + ortasaha + forvet);// toplam perf/11;
}

float Formasyonbilgi::getDefansOrt()//Takimin defans ortalama performansi.
{

	return (float)defansPerf / defans;//toplam defans ort defans oyuncu sayısına bol;
}

float Formasyonbilgi::getOrtasahaOrt() //Takimin ortasaha ortalama performansi.
{
	return (float)ortasahaPerf / ortasaha;
}

float Formasyonbilgi::getForvetOrt()//Takimin forvet ortalama performansi.
{
	return (float)forvetPerf / forvet;
}

///Kullanicidan alinan formasyon bilgisi çekmeten kullandigimiz fonksiyon.(while ile)
/*int* Formasyonbilgi::get() {

	switch (v++ % 3)
	{
	case 0:
		return &defans;
		break;
	case 1:
		return &ortasaha;
		break;
	case 2:
		return &forvet;
		break;


	default:
		break;
	}


}
*/
#pragma endregion


struct ligPuan//Lig tablosu icin icin gerekli gerekli degiskenleri tutugumuz yapi.
{
	int oynadigimac = 0;
	int galibiyet = 0;
	int berabere = 0;
	int maglubiyet = 0;
	int atilangol = 0;
	int yenilengol = 0;
	int averaj = 0;
	int puan = 0;
};

struct Takim//Takim bilgilerin tutan yapi.
{
	int id;
	string adi;
	string teknikDirektor;
	string takmaAd;
	Formasyonbilgi formasyon;
	float perfOrt = -1;
	float yasOrt = -1;
	Takim* next; // Listedeki sonraki dugumu isaret eder.
	Takim* prev; // Listedeki onceki dugumu isaret eder.
	FutbolcularListe* fList;
	ligPuan ligPuan;
};


struct TakimListele//Takim listemizi tutan linkli liste.
{
	Takim* header;									 // Listemizin baslangic pointeri ve bostur Sadece referans degerdir.
	Takim* trailer;									//  Listemizin sonunu gosteren pointeri ve bostur Sadece referans degerdir.

	TakimListele();
	~TakimListele();

	bool empty() const;
	void addBack(const int&, const string&, const string&, const string&);

	void removeFront();
	int count();

	void add(Takim* v, const int&, const string&, const string&, const string&);
	void remove(Takim* v);


	void ordered();
	void listSwipe();
	void printList(bool);
	Takim* printList(string);

private:
	int counter = 0;

};



void TakimListele::ordered() //Lig tablosu puanlarini siralar.
{
	Takim* k1 = header->next;

	while (k1 != trailer)//Tum listedeki dugumleri gezer.
	{
		Takim* k2 = k1->next;
		while (k2 != trailer)
		{
			Takim* tmp = k2->next;

			if (k1->ligPuan.puan < k2->ligPuan.puan)//puanlari karsilastirip swap yapar.
			{

				k2->prev->next = k2->next;
				k2->next->prev = k2->prev;

				k2->next = k1;
				k2->prev = k1->prev;

				k1->prev->next = k2;
				k1->prev = k2;
				k1 = k1->prev;
			}

			k2 = tmp;

		}

		k1 = k1->next;

	}
}







TakimListele::TakimListele()//Takim listesinin bas ve son dugumu olan header ve trailer tanimlanir.
{
	header = new Takim;
	trailer = new Takim;
	header->next = trailer;
	trailer->prev = header;
}

TakimListele::~TakimListele()//Destructor scoptan ciktiktan sonra listedeki tum dugumleri siler.
{
	while (!empty()) removeFront();
	delete header;
	delete trailer;
}

bool TakimListele::empty() const//liste bossa header i trailer a baglar.
{
	return (header->next == trailer);
}

void TakimListele::addBack(const int& id, const string& adi, const string& teknikDirektor, const string& takmaAd)//Listenin sonuna yeni takim ekler.(LIFO) Degerler takimok() fonksiyonundan gelmektedir.
{
	add(trailer, id, adi, teknikDirektor, takmaAd);
}

///Rasgele takim ekleme.(Denemede)
/*
void TakimListele::addRand((rand a),const int& id, const string& adi, const string& teknikDirektor, const string& takmaAd, const float& perfOrt, const float& yasOrt)		// add to back of list
{

	add(c, id, adi, teknikDirektor, takmaAd, perfOrt, yasOrt);
}*/

void TakimListele::add(Takim* v, const int& id, const string& adi, const string& teknikDirektor, const string& takmaAd)//addbackten gelen degerleri linkli listeye ekleriz.
{
	Takim* tempTakim = new Takim;//yeni dugum olusturuldu	
	//dugume takim bilgileri eklendi
	tempTakim->id = id;
	tempTakim->adi = adi;
	tempTakim->teknikDirektor = teknikDirektor;
	tempTakim->takmaAd = takmaAd;

	//dugum baglantilari yapildi
	tempTakim->next = v;
	tempTakim->prev = v->prev;

	v->prev->next = tempTakim;
	v->prev = tempTakim;

	counter++;//takim sayisini tutar.

}

void TakimListele::removeFront()//Listenin onundeki elemani siler.Headerdan baslar trailira kadar.
{
	// Liste boş mu?
	if (empty())
	{
		cout << "List is empty !" << endl;
		return;
	}

	remove(header->next);
}



void TakimListele::remove(Takim* v)//Silinecek takimin dugumleri listeden silinirç
{
	Takim* u = v->prev;//silinen takimin sonraki dugumu gösteren isaret silinir.
	Takim* w = v->next;	//silnen takimin onceki dugumu gösteren isaret silinir.
	u->next = w;//bosta kalan isaretler var olan dugumu isaret eder.					
	w->prev = u;
	delete v;//dugumu siler.
	counter--;//takim sayisini azaltir.
}

int TakimListele::count()//Toplam takim sayisini tutan fonksiyon
{
	return counter;
}


void TakimListele::listSwipe()//Takim listesini ilk dugum sabit tutarak diger dugum elemanlarini basta alip sona ekleyerek listedeki dugumlerin yerleri degistirildi. addBackte fonksiyonundan yararlanilmistir.
{

	Takim* tempTakim = header->next->next;

	tempTakim->prev->next = tempTakim->next;
	tempTakim->next->prev = tempTakim->prev;


	tempTakim->next = trailer;
	tempTakim->prev = trailer->prev;
	trailer->prev->next = tempTakim;
	trailer->prev = tempTakim;


	/*

		Takim* tempTakim = trailer->prev;


		tempTakim->prev->next = tempTakim->next;
		tempTakim->next->prev = tempTakim->prev;



		tempTakim->next = header->next->next;
		tempTakim->prev = header->next;


		header->next->next->prev = tempTakim;
		header->next->next= tempTakim;

		*/




}

//void TakimListele::playerList() {
//	Takim* T = header->next;
//
//	while (T != takimListesi.trailer)
//	{
//
//		Futbolcu* F = T->fList->header->next;
//		T->perfOrt = T->fList->listPerfOrt();
//		T->yasOrt = T->fList->listYasOrt();
//		int syc = 0;
//		cout << to_string(T->id) + "--" + T->adi + "--" + T->takmaAd + "--" + T->teknikDirektor + "--( " + to_string(T->perfOrt) + "--" + to_string(T->yasOrt) + ")" << endl;
//
//
//		while (F != T->fList->trailer)
//		{
//			cout << to_string(syc) + "--" + "--" + F->adi + "--" + F->soyadi + "--" + to_string(F->perf) + "---" + F->pozisyon << endl;
//
//			F = F->next;
//			syc++;
//		}
//
//
//		//cout<< T->fList->listPerfOrt()<< endl;
//		cout << "-----------------------" << endl;
//
//		T = T->next;
//	}
//}
//
//void TakimListele::playerList(string takimAdi) {
//	Takim* T = header->next;
//
//	while (T != takimListesi.trailer)
//	{
//		if (!T->adi.compare(takimAdi))
//		{
//
//			Futbolcu* F = T->fList->header->next;
//			T->perfOrt = T->fList->listPerfOrt();
//			T->yasOrt = T->fList->listYasOrt();
//			int syc = 0;
//			cout << to_string(T->id) + "--" + T->adi + "--" + T->takmaAd + "--" + T->teknikDirektor + "--( " + to_string(T->perfOrt) + "--" + to_string(T->yasOrt) + ")" << endl;
//
//
//			while (F != T->fList->trailer)
//			{
//				cout << to_string(syc) + "--" + "--" + F->adi + "--" + F->soyadi + "--" + to_string(F->perf) + "---" + F->pozisyon << endl;
//
//				F = F->next;
//				syc++;
//			}
//		}
//
//		T = T->next;
//	}
//}




void TakimListele::printList(bool ligDurumTablosu)//Burada 2tane printimiz var. Bool ile 0 veya 1 degeri yollayarak lig durum tablosunu ve takimlari listeliyoruz. Bool bize 2print fonksiyonunu teke indirmeye yardimci oluyor.
{
	if (empty())
	{
		cout << "List is empty !" << endl;
		return;
	}

	if (ligDurumTablosu)
	{
		cout << "Sira\t|" << "\t\t Takim \t\t |" << "Oynadigi \t |" << "Galibiyet \t |" << " Beraberlik \t |" << " Maglubiyet \t |" << "Atilan \t |" << "Yenilen \t |" << "Averaj \t |" << "Puan" << endl;
		cout << "no\t|" << "\t\t Adi \t\t\ |" << "   mac  \t |" << " Sayisi \t |" << "  Sayisi \t | " << "   Sayisi \t |" << " Gol \t\t |" << " Gol \t\t |" << "  \t\t |" << "\t" << endl;
		cout << endl;
	}
	else
		cout << "id\t|" << "\t\t Takim \t\t\t|" << "Teknik Direktor \t |" << "Takma Ad \t |" << " Performans Ortalama \t |" << " Yas Ortalama \t |" << "Lig Puan \t | " << endl;

	int siraNo = 0;
	Takim* takim = header->next;
	while (takim != trailer)
	{
		if (ligDurumTablosu)
		{
			siraNo++;

			//burdaki kosullar bize takim adi karakter uzunluguna gore tab attırmaya yardımcı oluyor.
			cout << siraNo << "\t" << takim->adi.substr(0, 25) << ((takim->adi.length() > 25) ? "..." : " ") << ((takim->adi.length() > 25) ? "\t\t" : (takim->adi.length() >= 15) ? "\t\t\t" : (takim->adi.length() >= 8) ? "\t\t\t\t" : "\t\t\t\t\t") << takim->ligPuan.oynadigimac << "\t\t" << takim->ligPuan.galibiyet << "\t\t" << takim->ligPuan.berabere << "\t\t" << takim->ligPuan.maglubiyet << "\t\t" << takim->ligPuan.atilangol << "\t\t" << takim->ligPuan.yenilengol << "\t\t" << takim->ligPuan.averaj << "\t\t" << takim->ligPuan.puan << endl;




		}
		else//burdaki kosullar bize takim adi ve teknik direktor karakter uzunluguna gore tab attırmaya yardımcı oluyor.
			cout << takim->id << "\t" << takim->adi.substr(0, 25) << ((takim->adi.length() > 25) ? "..." : " ") << ((takim->adi.length() > 25) ? "\t\t" : (takim->adi.length() >= 15) ? "\t\t\t" : (takim->adi.length() >= 8) ? "\t\t\t\t" : "\t\t\t\t\t") << takim->teknikDirektor.substr(0, 25) << ((takim->teknikDirektor.length() > 25) ? "..." : " ") << ((takim->teknikDirektor.length() > 25) ? "\t" : (takim->teknikDirektor.length() >= 15) ? "\t\t" : (takim->teknikDirektor.length() >= 8) ? "\t\t\t" : "\t\t\t\t") << takim->takmaAd << "\t\t " << takim->perfOrt << "\t\t\t" << takim->yasOrt << "\t\t\t" << takim->ligPuan.puan << endl;

		takim = takim->next;

	}
}

#pragma endregion


#pragma region FUTBOLCU




struct Futbolcu//Futbolcu bilgilerin tutan yapi.
{

	int id;
	string adi;
	string soyadi;
	int formaNo;
	int perf;
	int yas;
	char pozisyon;
	Takim* takimi;
	Futbolcu* next;
	Futbolcu* prev;

};

struct FutbolcularListe
{
	Futbolcu* header;										// listenin header pointeri
	Futbolcu* trailer;									// listenin trailer pointeri

	FutbolcularListe();										// bos lite constructor
	~FutbolcularListe();									// destructor

	bool empty() const;										// Liste bos mu?

	void addBack(Takim*, const int&, const string&, const string&, const int&, const int&, const int&, const char&);			// Futbolcu ekleme
	void removeFront();										// Listenin onunden silemeye yarayan method.

	int count();//futbolcu sayisini tutan metot.

	void add(Futbolcu*, Takim*, const int&, const string&, const string&, const int&, const int&, const int&, const char&);	// yukaridaki addBack fonksiyonuna yardımcı asil verilerin eklendigi yer.
	void remove(Futbolcu*);

	Futbolcu* printList(string);//Futbolcu idsine gore listeleme yapan metot.
	const float& listPerfOrt() const;//takim ortalama performans ortalamasi.
	const float& listYasOrt() const;//yas ortalama performans ortalamasi.

private:
	int counter = 0;

};



int FutbolcularListe::count()//futbolcu sayisini tutan metot.
{
	int syc = 0;
	Futbolcu* futbolcu = header->next;
	while (futbolcu != trailer)
	{
		syc++;
		futbolcu = futbolcu->next;

	}
	return syc;
}

const float& FutbolcularListe::listPerfOrt() const//Futbolcu listesinde dolasarak istenen takimin performans ortalmasini donduruyor.
{
	float ort = 0;
	Futbolcu* futbolcu = header->next;
	while (futbolcu != trailer)
	{
		ort += futbolcu->perf;
		futbolcu = futbolcu->next;

	}
	ort = ort / 11;
	return round(ort * 100) / 100;
}
const float& FutbolcularListe::listYasOrt() const//Futbolcu listesinde dolasarak istenen takimin yas ortalmasini donduruyor.
{
	float ort = 0;
	Futbolcu* futbolcu = header->next;
	while (futbolcu != trailer)
	{
		ort += futbolcu->yas;
		futbolcu = futbolcu->next;

	}
	ort = ort / 11;
	return round(ort * 100) / 100;
}

FutbolcularListe::FutbolcularListe()//Constructorlistenin baslangic degerlerini belirler.
{
	header = new Futbolcu;
	trailer = new Futbolcu;
	header->next = trailer;
	trailer->prev = header;
}

FutbolcularListe::~FutbolcularListe()
{
	while (!empty()) removeFront();
	delete header;
	delete trailer;
}

bool FutbolcularListe::empty() const
{
	return (header->next == trailer);
}




//add fonksiyonunan trailer ile yolladigi verileri  il gireni listenin arkasina ekler. 
void FutbolcularListe::addBack(Takim* takim, const int& id, const string& adi, const string& soyadi, const int& formaNo, const int& perf, const int& yas, const char& pozisyon)		// add to back of list
{
	add(trailer, takim, id, adi, soyadi, formaNo, perf, yas, pozisyon);
}


void FutbolcularListe::add(Futbolcu* v, Takim* takim, const int& id, const string& adi, const string& soyadi, const int& formaNo, const int& perf, const int& yas, const char& pozisyon)	// insert new node before v
{
	Futbolcu* tempFutbolcu = new Futbolcu;	//yeni dugum olusturur.
	//Gonderilen verileri yeni dugume ekler
	tempFutbolcu->takimi = takim;
	tempFutbolcu->id = id;
	tempFutbolcu->adi = adi;
	tempFutbolcu->soyadi = soyadi;
	tempFutbolcu->formaNo = formaNo;
	tempFutbolcu->perf = perf;
	tempFutbolcu->yas = yas;
	tempFutbolcu->pozisyon = pozisyon;

	//dugumleri listeye baglar
	tempFutbolcu->next = v;
	tempFutbolcu->prev = v->prev;
	v->prev->next = tempFutbolcu;
	v->prev = tempFutbolcu;

	counter++;
}

void FutbolcularListe::removeFront()//Listenin onundeki dugumu silmek icin remova header->next i yollar.
{
	// Liste boş mu?
	if (empty())
	{
		cout << "Listeniz Bos !" << endl;
		return;
	}

	remove(header->next);
}

void FutbolcularListe::remove(Futbolcu* v)//Listede bulunan dugum ve isretleri siler.
{
	Futbolcu* u = v->prev;
	Futbolcu* w = v->next;
	u->next = w;
	w->prev = u;
	delete v;
	counter--;

}

Futbolcu* FutbolcularListe::printList(string temp)//Futbolcu id'sine gore futbolcu bilgileri listeleme.
{
	if (empty())
	{
		cout << "Listeniz Bos !" << endl;
		return NULL;
	}

	Futbolcu* futbolcu = header->next;
	while (futbolcu != trailer)
	{
		if (stoi(temp) == futbolcu->id)
		{
			cout << futbolcu->id << "-" << futbolcu->adi << "-" << futbolcu->soyadi << "-" << futbolcu->formaNo << "-" << futbolcu->yas << "-" << futbolcu->perf << endl;
			return futbolcu;
		}
		futbolcu = futbolcu->next;
	}
	return futbolcu;
}





#pragma endregion


#pragma region MAC


struct Mac//Mac bilgilerin tutan yapi.
{
	int id;//mac id
	int hafta;//hafta sayisini tutan degisken

	Takim* T1;
	int T1Gol = rand() % 6;//takim 1'in ilk aldigi rasgele gol.

	Takim* T2;
	int T2Gol = rand() % 6;//takim 2'in ilk aldigi rasgele gol.


	Mac* next;
	Mac* prev;
};


struct MacFikstur//Mac fiksturu listesinin struct yapisi
{
	Mac* header;
	Mac* trailer;

	MacFikstur();
	~MacFikstur();

	bool empty() const;

	void addBack(const int&, const int&, Takim*, Takim*);
	void removeFront();

	int count();

	void add(Mac* v, const int&, const int&, Takim*, Takim*);
	void remove(Mac* v);
	void clear();
	Takim* oyna(Mac* v);

	void LigTabloDegerleri();
	void printList(int);
	void printList(string);
	void maclistele();

};

void MacFikstur::clear()//Lig tablosu degerlerini sifirlamak icin kullandigimiz fonksiyon. Tum tabloyu sifirlar.
{

	Mac* tmp = header->next;
	while (tmp != trailer)
	{
		tmp->T1->ligPuan = ligPuan();
		tmp->T2->ligPuan = ligPuan();
		tmp = tmp->next;
		remove(tmp->prev);
	}

}

MacFikstur::MacFikstur()
{
	header = new Mac;
	trailer = new Mac;
	header->next = trailer;
	trailer->prev = header;
}

MacFikstur::~MacFikstur()
{
	while (!empty()) removeFront();
	delete header;
	delete trailer;
}

bool MacFikstur::empty() const
{
	return (header->next == trailer);
}

void MacFikstur::addBack(const int& id, const int& hafta, Takim* T1, Takim* T2)
{
	add(trailer, id, hafta, T1, T2);
}


void MacFikstur::add(Mac* v, const int& id, const int& hafta, Takim* T1, Takim* T2)
{
	Mac* tempMac = new Mac;
	tempMac->id = id;
	tempMac->hafta = hafta;
	tempMac->T1 = T1;
	tempMac->T2 = T2;

	tempMac->next = v;
	tempMac->prev = v->prev;

	v->prev->next = tempMac;
	v->prev = tempMac;
}

void MacFikstur::removeFront()
{
	// Liste boş mu?
	if (empty())
	{
		cout << "Listeniz Bos !" << endl;
		return;
	}

	remove(header->next);
}


void MacFikstur::remove(Mac* v)
{
	Mac* u = v->prev;
	Mac* w = v->next;
	u->next = w;
	w->prev = u;
	delete v;
}

int MacFikstur::count()
{
	return trailer->prev->id + 1;
}


Takim* MacFikstur::oyna(Mac* v)// Mac gollerini belirleyen fonskiyon. Gonderilen her macin golleri perfonrmanslara gore belirliyoruz. 
{

	//FPO
	if (v->T1->formasyon.getForvetOrt() < v->T2->formasyon.getForvetOrt())
	{
		v->T2Gol++;
	} 
	else if (v->T1->formasyon.getForvetOrt() > v->T2->formasyon.getForvetOrt())
	{
		v->T1Gol++;
	}

	//DPO

	if (v->T1->formasyon.getDefansOrt() < v->T2->formasyon.getDefansOrt())
	{


		v->T1Gol--;
	}
	else if (v->T1->formasyon.getDefansOrt() > v->T2->formasyon.getDefansOrt())
	{

		v->T2Gol--;
	}

	//OSOP

	if (v->T1->formasyon.getOrtasahaOrt() < v->T2->formasyon.getOrtasahaOrt())
	{
		v->T2Gol++;
		v->T1Gol--;
	}
	else if (v->T1->formasyon.getOrtasahaOrt() > v->T2->formasyon.getOrtasahaOrt())
	{
		v->T1Gol++;
		v->T2Gol--;
	}

	//TOP

	if (v->T1->formasyon.getPerfOrt() < v->T2->formasyon.getPerfOrt())
	{

		v->T2Gol++;
		v->T1Gol--;
	}
	else if (v->T1->formasyon.getPerfOrt() > v->T2->formasyon.getPerfOrt())
	{
		v->T1Gol++;
		v->T2Gol--;
	}
	// (Hesaplanacak  olacak EN SON gol değerinin negatif olamayacağını unutmayınız)
	v->T1Gol = (v->T1Gol) <= 0 ? 0 : v->T1Gol;
	v->T2Gol = (v->T2Gol) <= 0 ? 0 : v->T2Gol;


	return (v->T1Gol == v->T2Gol) ? NULL : ((v->T1Gol > v->T2Gol) ? v->T1 : v->T2);
}


void MacFikstur::LigTabloDegerleri()//Lig puan tablosuna degerleri gollere gore eklenir.
{

	if (empty())
	{
		cout << "Listeniz Bos !" << endl;
		return;
	}
	Mac* mac = header->next;

	mac = header->next;
	while (mac != trailer)//Her maci dolasarak takimlarin lig puan degerlerini degistirir.
	{

		Takim* kazanan = oyna(mac);

		mac->T1->ligPuan.oynadigimac++;//Takimin oyanadigi mac sayisini arttirir.
		(kazanan == NULL) ? mac->T1->ligPuan.berabere++, mac->T1->ligPuan.puan++ : (mac->T1 == kazanan) ? mac->T1->ligPuan.galibiyet++, mac->T1->ligPuan.puan += 3 : mac->T1->ligPuan.maglubiyet++;//Beraber veya kazanan olma durumuna gore puan arttirir.
		mac->T1->ligPuan.atilangol += mac->T1Gol;//T1 takiminin attigi gol sayisi eklenir.
		mac->T1->ligPuan.yenilengol += mac->T2Gol;//T1 takiminin yedigi gol sayisi eklenir.
		mac->T1->ligPuan.averaj = mac->T1->ligPuan.atilangol - mac->T1->ligPuan.yenilengol;//T1 takiminin averaj sayisi.
		if (mac->T1->ligPuan.averaj < 0)mac->T1->ligPuan.averaj = 0;

		mac->T2->ligPuan.oynadigimac++;
		(kazanan == NULL) ? mac->T2->ligPuan.berabere++, mac->T2->ligPuan.puan++ : (mac->T2 == kazanan) ? mac->T2->ligPuan.galibiyet++, mac->T2->ligPuan.puan += 3 : mac->T2->ligPuan.maglubiyet++;
		mac->T2->ligPuan.atilangol += mac->T1Gol;
		mac->T2->ligPuan.yenilengol += mac->T1Gol;
		mac->T2->ligPuan.averaj = mac->T2->ligPuan.atilangol - mac->T2->ligPuan.yenilengol;
		if (mac->T2->ligPuan.averaj < 0)mac->T2->ligPuan.averaj = 0;

		mac = mac->next;

	}
}

void MacFikstur::maclistele()//Her haftanin maclarini hafta hafta yazdiran fonksiyon.
{
	if (empty())//liste bossa bos yazisi verir
	{
		cout << "Liste Bos !" << endl;
		return;
	}

	Mac* mac = header->next;
	while (mac != trailer)//tum maclari gezer
	{

		int drm = mac->hafta;
		cout << "" << endl;
		cout << "   -----------------	       " << to_string(drm) << ".	     -----------------" << endl;
		cout << "--------------------	     HAFTA	     --------------------" << endl;
		cout << "" << endl;

		cout << "" << endl;
		cout << "\t\t\t   MacId: " << mac->id << endl;//mac id yazdirir.
		cout << "\t\t   " << mac->T1->takmaAd << "-" << mac->T1->formasyon.getPerfOrt() << " -- " << mac->T2->takmaAd << "-" << mac->T2->formasyon.getPerfOrt() << endl;

		cout << mac->T1->adi << " Kazanma Olasiligi  %" << round(((mac->T1->formasyon.getPerfOrt() / (mac->T1->formasyon.getPerfOrt() + mac->T2->formasyon.getPerfOrt())) * 100));//2 takimin kazanma olasiliginin yazdirildigi yer
		cout << "---" << mac->T2->adi << " Kazanma Olasiligi  %" << round(((mac->T2->formasyon.getPerfOrt() / (mac->T1->formasyon.getPerfOrt() + mac->T2->formasyon.getPerfOrt())) * 100)) << endl << endl;//performanslari 2 virgulune kadar yazdirir.


		Futbolcu* f1 = mac->T1->fList->header->next;//T1 in furbolcu listesi
		Futbolcu* f2 = mac->T2->fList->header->next;//T2 nin futbolcu listesi



		while (f1 != mac->T1->fList->trailer && f2 != mac->T2->fList->trailer)//2 takiminda futbolcu bilgilerini yazdirir.
		{
			string tmp = f1->adi + " " + f1->soyadi + "-" + f1->pozisyon + "-" + to_string(f1->perf);

			cout << tmp << (tmp.length() < 15 ? "\t\t\t|\t" : (tmp.length() < 23 ? " \t\t| \t" : "\t| \t")) << f2->adi << " " << f2->soyadi << "-" << f2->pozisyon << "-" << f2->perf << endl;//tmp stringinin uzunluguna gore tab atar.
			f1 = f1->next;//diger futbolcuya gecer.
			f2 = f2->next;//diger futbolcuya gecer.
		}

		cout << " DPO:" << mac->T1->formasyon.getDefansOrt() << " OPO:" << mac->T1->formasyon.getOrtasahaOrt() << " FPO:" << mac->T1->formasyon.getForvetOrt() << "\t\t";//Takim1 in formasyon ortalama performans bilgileri.
		cout << " DPO:" << mac->T2->formasyon.getDefansOrt() << " OPO:" << mac->T2->formasyon.getOrtasahaOrt() << " FPO:" << mac->T2->formasyon.getForvetOrt() << endl;//Takim2 nin formasyon ortalama performans bilgileri.

		cout << "\t\t" << ((mac->T1Gol == mac->T2Gol) ? "Mac Berabere" : ((mac->T1Gol < mac->T2Gol) ? (mac->T1->adi) : (mac->T2->adi))) + "Maci Kazanandi ( " << (to_string(mac->T1Gol) + "-" + to_string(mac->T2Gol)) + " )" << endl;//Kazanma veya beraber durumuna gore degerleri yazdirir.


		mac = mac->next;
	}


}

void MacFikstur::printList(int id)//Mac Id sine göre mac listeleyen fonksiyon.
{
	if (empty())
	{
		cout << "List is empty !" << endl;
		return;
	}

	Mac* mac = header->next;
	while (mac != trailer)//gonderilen id ile mac idlerini karsilatirip buldugunda break eden while.
	{
		if (mac->id == id)
		{
			break;
		}
		mac = mac->next;
	}
	//bulunan mac id yazdirilir.
	int drm = mac->hafta;
	cout << "" << endl;
	cout << "   -----------------	       " << to_string(drm) << ".	     -----------------" << endl;
	cout << "--------------------	     HAFTA	     --------------------" << endl;
	cout << "" << endl;

	cout << "" << endl;

	cout << "\t\t\t   MacId: " << mac->id << endl;
	cout << "\t\t   " << mac->T1->takmaAd << "-" << mac->T1->formasyon.getPerfOrt() << " -- " << mac->T2->takmaAd << "-" << mac->T2->formasyon.getPerfOrt() << endl;

	cout << mac->T1->adi << " Kazanma Olasiligi  %" << round(((mac->T1->formasyon.getPerfOrt() / (mac->T1->formasyon.getPerfOrt() + mac->T2->formasyon.getPerfOrt())) * 100));
	cout << "---" << mac->T2->adi << " Kazanma Olasiligi  %" << round(((mac->T2->formasyon.getPerfOrt() / (mac->T1->formasyon.getPerfOrt() + mac->T2->formasyon.getPerfOrt())) * 100)) << endl << endl;


	Futbolcu* f1 = mac->T1->fList->header->next;
	Futbolcu* f2 = mac->T2->fList->header->next;



	while (f1 != mac->T1->fList->trailer && f2 != mac->T2->fList->trailer)
	{
		string tmp = f1->adi + " " + f1->soyadi + "-" + f1->pozisyon + "-" + to_string(f1->perf);

		cout << tmp << (tmp.length() < 15 ? "\t\t\t|\t" : (tmp.length() < 23 ? " \t\t| \t" : "\t| \t")) << f2->adi << " " << f2->soyadi << "-" << f2->pozisyon << "-" << f2->perf << endl;
		f1 = f1->next;
		f2 = f2->next;
	}

	cout << " DPO:" << mac->T1->formasyon.getDefansOrt() << " OPO:" << mac->T1->formasyon.getOrtasahaOrt() << " FPO:" << mac->T1->formasyon.getForvetOrt() << "\t\t";
	cout << " DPO:" << mac->T2->formasyon.getDefansOrt() << " OPO:" << mac->T2->formasyon.getOrtasahaOrt() << " FPO:" << mac->T2->formasyon.getForvetOrt() << endl;

	cout << "\t\t" << ((mac->T1Gol == mac->T2Gol) ? "Mac Berabere" : ((mac->T1Gol < mac->T2Gol) ? (mac->T1->adi) : (mac->T2->adi))) + "Maci Kazanandi ( " << (to_string(mac->T1Gol) + "-" + to_string(mac->T2Gol)) + " )" << endl;


}
void MacFikstur::printList(string takimad)//Takim adina gore oynadigi tum maclari listeler
{
	if (empty())
	{
		cout << "List is empty !" << endl;
		return;
	}

	Mac* mac = header->next;
	while (mac != trailer)
	{
		if (mac->T1->adi == takimad || mac->T2->adi == takimad)//Istenen takimin T1 ve T2 oldugundaki maclar. Yani o takimin oynadigi tum maclar.
		{



			int drm = mac->hafta;
			cout << "" << endl;
			cout << "   -----------------	       " << to_string(drm) << ".	     -----------------" << endl;
			cout << "--------------------	     HAFTA	     --------------------" << endl;
			cout << "" << endl;

			cout << "" << endl;
			cout << "\t\t\t   MacId: " << mac->id << endl;
			cout << "\t\t " << mac->T1->takmaAd << "-" << mac->T1->formasyon.getPerfOrt() << " -- " << mac->T2->takmaAd << "-" << mac->T2->formasyon.getPerfOrt() << endl;

			cout << mac->T1->adi << " Kazanma Olasiligi  %" << round(((mac->T1->formasyon.getPerfOrt() / (mac->T1->formasyon.getPerfOrt() + mac->T2->formasyon.getPerfOrt())) * 100));
			cout << "---" << mac->T2->adi << " Kazanma Olasiligi  %" << round(((mac->T2->formasyon.getPerfOrt() / (mac->T1->formasyon.getPerfOrt() + mac->T2->formasyon.getPerfOrt())) * 100)) << endl << endl;


			Futbolcu* f1 = mac->T1->fList->header->next;
			Futbolcu* f2 = mac->T2->fList->header->next;



			while (f1 != mac->T1->fList->trailer && f2 != mac->T2->fList->trailer)
			{
				string tmp = f1->adi + " " + f1->soyadi + "-" + f1->pozisyon + "-" + to_string(f1->perf);

				cout << tmp << (tmp.length() < 15 ? "\t\t\t|\t" : (tmp.length() < 23 ? " \t\t| \t" : "\t| \t")) << f2->adi << " " << f2->soyadi << "-" << f2->pozisyon << "-" << f2->perf << endl;
				f1 = f1->next;
				f2 = f2->next;
			}

			cout << " DPO:" << mac->T1->formasyon.getDefansOrt() << " OPO:" << mac->T1->formasyon.getOrtasahaOrt() << " FPO:" << mac->T1->formasyon.getForvetOrt() << "\t\t";
			cout << " DPO:" << mac->T2->formasyon.getDefansOrt() << " OPO:" << mac->T2->formasyon.getOrtasahaOrt() << " FPO:" << mac->T2->formasyon.getForvetOrt() << endl;


			cout << "\t\t" << ((mac->T1Gol == mac->T2Gol) ? "Mac Berabere" : ((mac->T1Gol < mac->T2Gol) ? (mac->T1->adi) : (mac->T2->adi))) + "Maci Kazanandi ( " << (to_string(mac->T1Gol) + "-" + to_string(mac->T2Gol)) + " )" << endl;
		}
		if (mac == trailer)
		{
			cout << takimad << " Bulunamadi!";
			return;
		}
		mac = mac->next;
	}


}



#pragma endregion

#pragma endregion


TakimListele takimListesi;//takimlistesinin prototipi.
MacFikstur macFiksturu;//macFiksturu prototipi.

void takimOku() {

	ifstream dosya1("Takimlar.txt", ios::in);//dosya1 acilir.
	if (dosya1.is_open())
	{
		int syc = 0;
		do //Takimlar txt dosyasindan  Tek tek Okunur.
		{

			string temp;
			getline(dosya1, temp);//txt içindeki satir satir temp degiskenine atar.
			if (temp != "")
			{
				string takimAdi = temp.substr(0, temp.find('-'));//txt dosyasındaki belirtilen takım adını almak için
				string takmaAdi = takmaAd(takimAdi);//txt dosyasındaki belirtilen takım adına uygun takma ad oluşturmak için
				string teknikDir = temp.substr((temp.find('-') + 1));//txt dosyasındaki belirtilen teknik direktor adını almak için

				takimListesi.addBack(syc, takimAdi, teknikDir, takmaAdi);  //Takımlar Listeye Aktarılır...
				syc++;//takim indexi
			}

		} while (!dosya1.eof());
		dosya1.close();
	}

	else
		cout << "Dosya acilamadi";


}


// 11 kişilik bir takim oluşturur.
FutbolcularListe* takimFutbolculari(Takim* v, int id)
{

	FutbolcularListe* futbolcuListesi = new FutbolcularListe;
	int syc = 11;

	while (syc)
	{

		string adi = "", soyadi = "";
		int formaNo = 0, perf = 0, yas = 0;

# pragma region ad soyad oluşturma kısmı


		int rnd = rand() % 8;//0-300 arasinda random deger uretir.


		ifstream dosya1("Isimler.txt", ios::in);//Isimler.txt dosyasini acip okur.
		if (dosya1.is_open())
		{
			do //Takimlar txt dosyasindan  Tek tek Okunur.
			{
				getline(dosya1, adi);//satir satir adlari ceker.

				if (!rnd)//rnd degeri 0a ulastignda if girip break atar.
					break;
				rnd--;
			} while (!dosya1.eof());
			dosya1.close();
		}
		else
			cout << "Dosya acilamadi";



		rnd = rand() % 8;

		ifstream dosya2("Soyisimler.txt", ios::in);
		if (dosya2.is_open())
		{
			do //Takimlar txt dosyasindan  Tek tek Okunur.
			{
				getline(dosya2, soyadi);

				if (!rnd)
					break;
				rnd--;
			} while (!dosya2.eof());
			dosya2.close();
		}
		else
			cout << "Dosya acilamadi";

#pragma endregion

		//forma no 			
		rnd = rand() % 99 + 1;//1-99 a kadar random sayi degeri uretir.
		//Aynı forma numarası varsa yeniden numara üretiyor 
		Futbolcu* f = futbolcuListesi->header->next;
		while (f != futbolcuListesi->trailer)
		{
			if (f->formaNo == rnd)
			{
				rnd = rand() % 99 + 1;

				f = futbolcuListesi->header;
			}
			f = f->next;
		}

		formaNo = rnd;
		//perf		
		rnd = rand() % 70 + 30;//30-100 arasi random deger uretir.
		perf = rnd;
		//yas
		rnd = rand() % 18 + 18;//18-36 arasi random deger uretir.
		yas = rnd;


		char position = ' ';
		if (syc == 11)//Oyuncunun pozisyonlari belirleyen if else yapısı.
		{
			position = 'K';
			v->formasyon.kaleciPerf = perf;//
		}
		//işaretçi olarak tanımlanan formasyon bilgisi azaltamamak için bu tarz else if yapısını kullandık.
		else if (syc - (v->formasyon.forvet + v->formasyon.ortasaha) <= v->formasyon.defans && syc - (v->formasyon.forvet + v->formasyon.ortasaha) > 0)
		{
			position = 'D';
			v->formasyon.defansPerf += perf;

		}
		else if (syc - (v->formasyon.forvet) <= v->formasyon.ortasaha && syc - (v->formasyon.forvet) > 0)
		{
			position = 'O';
			v->formasyon.ortasahaPerf += perf;

		}
		else if (syc <= v->formasyon.forvet && syc > 0)
		{
			position = 'F';
			v->formasyon.forvetPerf += perf;

		}

		syc--;



		futbolcuListesi->addBack(v, id, adi, soyadi, formaNo, perf, yas, position);//takımda oynayan oyuncuların listesini tutar.
		id++;
	}

	//while takıma ait performans yaş bilgileri istenilen aralıkta olana kadar yeniden  takıma ait futbolcuları üretir.
	while (futbolcuListesi->listPerfOrt() < 60 || futbolcuListesi->listYasOrt() < 25 || futbolcuListesi->listYasOrt() > 32)
	{
		v->formasyon.clear();
		id = id - 11;//0-10**11-21
		futbolcuListesi = takimFutbolculari(v, id);//recursive yapi kullanıldı.

	}

	//takima ait perf-yas ortalaması futbolcularının listesinden temin edilir.
	v->perfOrt = futbolcuListesi->listPerfOrt();
	v->yasOrt = futbolcuListesi->listYasOrt();


	return futbolcuListesi;//takima atılacak futbolcu listesi geri döndürülür.

}


void futbolculariOlustur(Takim* v)//Takim listesindeki futbolcularini olusturan fonksiyon.(Paramteresi: Takim listesinin ilk elemanidir.)
{

	srand(time(NULL));//Farkli random degerler uretmek icin srand kullanildi.
	Takim* current = v;

	//Takimlarin hepsi icin ayri ayri kullanicidan alinan formasyon bilgisi alir ve futbolculari formasyona gore olusturur.
	do
	{
		int futbolcularId = current->id * 11;//Futbolcular icin benzersiz id tanimlama.
		Formasyonbilgi formasyon;
		int sayi = 5;
		cout << current->adi + " Formasyon bilgisi: ";
		string  st;
	wrongInput: //yanlış girişte goto noktası

	//	cin.clear();

		getline(cin, st);//st degiskenine formasyon bilgisi alinir.

		if (st == "")//Kullanici bos deger girdiginde 4 4 2 varsayilan degerini atar.
		{
			cout << to_string(formasyon.defans) + "-" + to_string(formasyon.ortasaha) + "-" + to_string(formasyon.forvet) + "  Formasyonunda.." << endl << endl;//var sayilan degerlri yazar.
			current->formasyon = formasyon;//varsayilan degerleri currenta atar.
			current->fList = takimFutbolculari(current, futbolcularId);//flist futbolculari olusturur.
			current = current->next;
			continue;
		}
		if (st.length() > 1 || st.length() < 4)//kullanicidan alinan formasyon bilgi uzunlugu max 4 karaket sinirlandirilmistir.
			st += "   ";
		string dizi = "123456789";
		int sayac = 0;
		for (int i = 0; i < st.length(); i += 2)//dizi degiskeninin icindeki degeler ile girilen degerler karsilastirilir ve eğer dizimizde mevcut ise sayac 1 arttar.
			if (st[i] == dizi[0] || st[i] == dizi[1] || st[i] == dizi[2] || st[i] == dizi[3] || st[i] == dizi[4] || st[i] == dizi[5] || st[i] == dizi[6] || st[i] == dizi[7] || st[i] == dizi[8]) sayac++;

		if (sayac != 3 || (stoi(string(1, char(st[0]))) + stoi(string(1, char(st[2]))) + stoi(string(1, char(st[4])))) != 10)// sayac 3 farkli veya formasyon bilgisi toplami 10 degilse goto ile yeniden formasyon ister. 
		{
			cout << "Gecerli Formasyon Bilgisi Giriniz !" << endl;
			goto wrongInput;
		}

		//kullanicidan alinan formasyon bilgilerini formasyona atar.
		formasyon.defans = stoi(string(1, char(st[0]))); //string(tekrar adet,char);

		formasyon.ortasaha = stoi(string(1, char(st[2])));

		formasyon.forvet = stoi(string(1, char(st[4])));


		//cin >> formasyon.defans >> formasyon.ortasaha >> formasyon.forvet;




		cout << to_string(formasyon.defans) + "-" + to_string(formasyon.ortasaha) + "-" + to_string(formasyon.forvet) + "  Formasyonunda.." << endl << endl;
		current->formasyon = formasyon;//formasyon bilgisi takima eklenir.
		current->fList = takimFutbolculari(current, futbolcularId);//flist futbolculari olusturur.
		current = current->next;//sonraki takima gecer.

		/*	do
	{
		int futbolcularId = current->id * 11;
		Formasyonbilgi formasyon;
		int sayi = 5;
		cout << current->adi + " Formasyon bilgisi: ";
		string  st;
	wrongInput: //yanlış girişte goto noktası

		getline(cin, st);

		if (st != "")
		{

			formasyon.defans = stoi(string(1, char(st[0]))); //string(tekrar adet,char);

			formasyon.ortasaha = stoi(string(1, char(st[2])));

			formasyon.forvet = stoi(string(1, char(st[4])));


			if ((formasyon.defans + formasyon.ortasaha + formasyon.forvet) != 10)
			{
				cout << "Gecersiz bir formasyon girdiniz." << endl;
				goto wrongInput;

			}


		}

		cout << to_string(formasyon.defans) + "-" + to_string(formasyon.ortasaha) + "-" + to_string(formasyon.forvet) + "  Formasyonunda.." << endl << endl;
		current->formasyon = formasyon;


		current->fList = takimFutbolculari(current, futbolcularId);

		current = current->next;

	} while (current != takimListesi.trailer);*/



	/*
	  while (cin >> st && st != "\n")
	  {

		  int* a=formasyon.get();
		  *a = stoi(st);

		  if (*a==formasyon.Forvet)
		  {
			  break;
		  }

	  }	*/
	  /// TODO: Enter basıldığında default yazmasını kontrol et 




	  //cin >> formasyon.Defans >> formasyon.Ortasaha >> formasyon.Forvet;

	 /*
		int d = stoi(st.substr(0, st.find(' ')));
		st = st.substr(st.find(' ') + 1, st.length());
		int o = stoi(st.substr(0, st.find(' ')));
		st = st.substr(st.find(' ') + 1, st.length());
		int f = stoi(st.substr(0, st.find(' ')));*/


		/*	if ((formasyon.defans + formasyon.ortasaha + formasyon.forvet) != 10)
			{
				cout << "Gecersiz bir formasyon girdiniz." << endl;
				goto wrongInput;

			}



			while (cin.fail())
			{
				cin.clear();
				cin.ignore();
				cout << "Defans Formasyonunu Giriniz :";
				cin >> d;
				if (!cin.fail())break;
			}

			cout << "Ortalama Formasyonunu Giriniz :";
			cin >> o;
			while (cin.fail())
			{
				cin.clear();
				cin.ignore();
				cout << "Ortalama Formasyonunu Giriniz :";
				cin >> o;
				if (!cin.fail())break;
			}

			cout << "Forvet Formasyonunu Giriniz :";
			cin >> f;
			while (cin.fail())
			{
				cin.clear();
				cin.ignore();
				cout << "Forvet Formasyonunu Giriniz :";
				cin >> f;
				if (!cin.fail())break;
			}

			if (o + d + f != 10)
			{
				cout << endl;
				cout << "Bir Takimda 10 Kisi Olmali!" << endl;
				goto don;
			}*/


			//st = st.substr(0, st.find(' ') + 1);
			//cout << "st" << endl;


		//	cout << current->fList->header->next->adi << endl;


	} while (current != takimListesi.trailer);

}







Takim* TakimListele::printList(string tAdi)// 2 tane print yollanilan degiskenin uzunluguna gore calismaktadir.
{
	if (empty())
	{
		cout << "List is empty !" << endl;
		return NULL;
	}

	Takim* takim = header->next;
	while (takim != trailer)
	{
		if (tAdi.length() <= 3)//eger takma ad yollaniyorsa yani uzunlugu 3ten kucukse takma ada gore istenen fonksiyon calisir.
		{
			if (!takim->takmaAd.compare(tAdi))//Gonderilen takma ada sahip takim oyuncularini bazi ozellikleriyle listeler.
			{
				cout << takim->adi << " - Teknikdirektor:" << takim->teknikDirektor << " - Perfomans Ortalama:" << takim->perfOrt << " - Yas Ortalama:" << takim->yasOrt << endl;

				cout << "Id |" << "\t\t Isim Soyisim \t\t |" << "\tYas |" << " \t Forma Numarasi |" << " Ortalama Performans |" << endl;
				Futbolcu* ftb = takim->fList->header->next;
				while (ftb != takim->fList->trailer)//Takimin tum oyuncularini yazdirir.
				{
					string adiSoyadi = ftb->adi + " " + ftb->soyadi;
					//buradaki compare adsoyad degiskenin boyutuna gore tab atar.
					cout << ftb->id << "\t" << adiSoyadi.substr(0, 25) << ((adiSoyadi.length() > 25) ? "..." : " ") << ((adiSoyadi.length() > 25) ? "\t\t" : (adiSoyadi.length() >= 15) ? "\t\t\t" : (adiSoyadi.length() >= 8) ? "\t\t\t\t" : "\t\t\t\t\t") << ftb->yas << "\t\t " << ftb->formaNo << "\t\t" << ftb->perf << endl;

					ftb = ftb->next;

				}
				return takim;
			}



		}
		else
		{
			if (!takim->adi.compare(tAdi))//eger gonderilen degisken bir takim adiysa asagidaki print calisacaktir. Takima ait bazi bilgiler yazdirilir.
			{
				cout << takim->adi << "-" << takim->teknikDirektor << "-" << takim->takmaAd << endl;
				return takim;
			}

		}




		takim = takim->next;

	}

	return takim;//takimi dondurur.
}

void main()
{
	cout << "NOT:" << endl;
	cout << "isimler soyimler txt dosyalarinda 300 satira askin veri oldugundan rand 300 e kadar deger uretmekte daha az verili txt dosyalarinda hata alinabilir." << endl;
	cout << "Program hic bir sekilde loopta kalmamaktadir print yaparken bekleme suresi bilgisayarinizin hizina gore degiskenlik gosterebilir." << endl << endl;

	takimOku();
	futbolculariOlustur(takimListesi.header->next);
	fiksturOlustur();
	//usteki fonksiyonlari prgoram baslarken calistirma sebebimiz takim ve futbolcular hazir gelmeli. Boylelikle fikstur olusturulur ve menuleri aktif bicimde kullana biliriz.

	_getch();
	//Menu Tasarimimiz
Menu:
	system("cls");
	cout << "1" << " Takimlari Listele" << endl;
	cout << "2" << " Takim Ara (Takim adina gore)" << endl;
	cout << "3" << " Yeni Takim Ekle" << endl;
	cout << "4" << " Mac Fikstur Menusu" << endl;
	cout << "5" << " Performans Guncelleme" << endl;
	cout << "6" << " Takim Oyuncularini Listele" << endl;
	cout << "7" << " Mac Arama" << endl;
	cout << "8" << " Lig Durum Tablosu" << endl;
	cout << "" << endl;

	cout << "0" << " Cikis" << endl;

	int secim = -1;
	string temp = "";
	cout << "Islem Seciniz..: ";
	cin >> secim;

	Takim takim;
	Takim* v;
	fstream dosya1("Takimlar.txt", ios::app); //ios::app  txt dosyasının devamına da işlem yapma imkani tanir 

	switch (secim)//switch ile menumuzu kontrol ediyoruz.
	{
	case 0:
		system("exit");
		break;
	case 1:
		system("cls");
		takimListesi.printList(0);
		cout << "\n\nAna Menu Bir Tusa Basiniz ... ";
		_getch();
		goto Menu;
		break;
	case 2:
		system("cls");
		cout << "Aranacak Takimin Adi ..: ";
		cin >> temp;
		takimListesi.printList(temp);
		cout << "\n\nAna Menu Bir Tusa Basiniz ... ";
		_getch();
		goto Menu;
		break;
	case 3:
		system("cls");

		cout << "Takimin Adi ..: ";
		cin >> temp;
		takim.adi = temp;
		cout << "Takimin Teknik Direktor Adi ..: ";
		cin >> temp;
		takim.teknikDirektor = temp;
		takim.takmaAd = takmaAd(takim.adi);
		takimListesi.addBack(takimListesi.count(), takim.adi, takim.teknikDirektor, takim.takmaAd);
		cin.ignore();
		futbolculariOlustur(takimListesi.trailer->prev);

		if (dosya1.is_open())
		{

			dosya1 << "\n" + takim.adi + "-" + takim.teknikDirektor;
			dosya1.close();
		}

		else
			cout << "Dosya acilamadi";


		cout << "\n\nAna Menu Bir Tusa Basiniz ... ";
		_getch();
		goto Menu;
		break;
	case 4:
		system("cls");
		cout << "1- Yeni Fikstur Olustur" << endl;
		cout << "2- Fikstur Goster" << endl;
		cout << "0- Ana Menu Icin Giriniz" << endl;
		temp = "";

		cin >> temp;
		if (temp == "1")
		{
			fiksturOlustur();
			cout << "Yeni Fikstur Basariyla Olusturuldu..." << endl;
		}
		else if (temp == "2")
			macFiksturu.maclistele();

		cout << "\n\nAna Menu Bir Tusa Basiniz ... ";
		_getch();
		goto Menu;
		break;

	case 5:
		system("cls");
		cout << "Futbolculari Aranacak Takimin Takma Adini Giriniz : ";
		cin >> temp;
		v = takimListesi.printList(temp);

		Futbolcu* futbolcu;
		cout << "Perfonasini Degistirmek Istediginiz Futbolcu Id Giriniz : ";

		do
		{
			cin >> temp;//id
			futbolcu = v->fList->printList(temp);

		} while (futbolcu == v->fList->trailer);

		cout << "Yeni Performans Degerini Giriniz : ";
		do
		{
			cin >> temp;
			futbolcu->perf = stoi(temp);
			v->perfOrt = v->fList->listPerfOrt();
		} while ((stoi(temp) < 30 && stoi(temp) > 100) && v->fList->listPerfOrt() < 60);



		cout << "\n\nAna Menu Bir Tusa Basiniz ... ";
		_getch();
		goto Menu;
		break;

	case 6:
		system("cls");
		cout << "Futbolculari Aranacak Takimin Takma Adini Giriniz : ";
		cin >> temp;
		takimListesi.printList(temp);
		cout << "\n\nAna Menu Bir Tusa Basiniz ... ";
		_getch();
		goto Menu;

		break;
	case 7:
		system("cls");
		cout << "1- MacId Ile Mac Arama" << endl;
		cout << "2- Takimin Maclarini Getir" << endl;
		cout << "3- Tum Maclari Getir" << endl;
		cout << "0- Ana Menu Icin Giriniz" << endl;
		temp = "";

		cin >> temp;
		if (temp == "1")
		{
			system("cls");
			cout << "MacId Giriniz :";
			int id;
			do
			{
				cin.clear();
				cin.ignore();
				cin >> id;
				if (!cin.fail())break;
				cout << "Lutfen Istenen Degeri Giriniz!" << endl;

			} while (cin.fail());//cin fail olduğunda çalışır int değer alana kadar döngü devam eder.
			(&macFiksturu)->printList(id);



		}
		else if (temp == "2")
		{

			system("cls");
			cout << "Arayacaginiz Takimi Giriniz :";
			cin.ignore();
			getline(cin, temp);

			(&macFiksturu)->printList(temp);
		}
		else if (temp == "3")
		{

			system("cls");
			macFiksturu.maclistele();
		}


		cout << "\n\nAna Menu Bir Tusa Basiniz ... ";
		_getch();
		goto Menu;
		break;


	case 8:
		system("cls");
		takimListesi.ordered();
		takimListesi.printList(1);
		cout << "\n\nAna Menu Bir Tusa Basiniz ... ";
		_getch();
		goto Menu;
		break;

	default:
		break;

	}

	system("pause");





}
#pragma region FUNC

string takmaAd(string TakimAdi)//Takma ad olusturan fonksiyon.
{
	int syc = 0;
	string takmaAd = "";
	Takim* current = takimListesi.header;

	while (TakimAdi.find(' ') != -1)//takim adinda bosluk bulanada kadar.
	{
		syc++;
		int s = TakimAdi.find(' ');

		string kelime = TakimAdi.substr(0, s);
		takmaAd += toupper(kelime[0]);

		TakimAdi = TakimAdi.substr(s + 1);// aradaki boşlukları silmek için +1 şeklinde kesilir
	}
	takmaAd += toupper(TakimAdi[0]);//son kelime icin toupper yapar.

	if (syc == 0)//tek kelimelik isimlerin ikinci harfini takma ad  için kullanır
	{
		takmaAd += toupper(TakimAdi[1]);
	}

	//Takma Adların Kıyaslanıp Aynı Takma Ada Sahiplerin Ayrıştırılmasını Sağlar(Eger ayni takma adi bulursa 3.harfi ekler.)
	do
	{
		if (current->takmaAd == takmaAd)
		{
			takmaAd += toupper(TakimAdi[2]);
			break;
		}
		else
			current = current->next;
	} while (current != takimListesi.trailer);

	// ilk harfleri alınmış takma adların  3 ten büyük olanlarını son 3 hanesini alarak takma ad oluşturur.
	if (takmaAd.length() > 3)
	{
		char tmp = takmaAd[0];
		takmaAd = takmaAd.substr((takmaAd.length() + 1) - 3);//son üç hanesi kalacak şekilde subs
		takmaAd = tmp + takmaAd;
	}

	return takmaAd;

}

void  fiksturOlustur() {

	macFiksturu.clear();
	int syc = 0;
	int a = takimListesi.count();
	for (int i = 1; i < a; i++) // Haftalar
	{

		Takim* t1 = takimListesi.header->next;
		Takim* t2 = takimListesi.trailer->prev;

		macFiksturu.addBack(syc, i, t1, t1->next);

		t1 = t1->next->next;
		syc++;

		while (t1 != takimListesi.trailer->prev && t1->next != takimListesi.trailer->prev) //Haftalarda oynanan maclar
		{

			macFiksturu.addBack(syc, i, t1, t2);


			if (t1->next == t2)
			{
				break;
			}
			t1 = t1->next;
			t2 = t2->prev;
			syc++;

		}

		takimListesi.listSwipe();
	}
	macFiksturu.LigTabloDegerleri();
}
#pragma endregion