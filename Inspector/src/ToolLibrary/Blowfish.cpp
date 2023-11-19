// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

//Code from blowfish from ttarchext/bruce schnieder
#pragma warning(disable C4267 C4244 C4554 C4477)
#include "Blowfish.h"
#include <string.h>

#define N 16

void beo(Blowfish*, unsigned long* xl, unsigned long* xr);
void bdo(Blowfish*, unsigned long* xl, unsigned long* xr);
short io(Blowfish*, unsigned char key[], int keybytes);
void eo(Blowfish* c, unsigned long* data, int blocks);
void _do(Blowfish* c, unsigned long* data, int blocks);
void ko(Blowfish* c, unsigned char* key, int len);
void ben(Blowfish*, unsigned long* xl, unsigned long* xr);
void bdn(Blowfish*, unsigned long* xl, unsigned long* xr);
short in(Blowfish*, unsigned char key[], int keybytes);
void en(Blowfish* c, unsigned long* data, int blocks);
void dn(Blowfish* c, unsigned long* data, int blocks);
void kn(Blowfish* c, unsigned char* key, int len);

unsigned long bf(Blowfish* bc, unsigned long x)
{
	unsigned long a;
	unsigned long b;
	unsigned long c;
	unsigned long d;
	unsigned long y;

	d = x & 0x00FF;
	x >>= 8;
	c = x & 0x00FF;
	x >>= 8;
	b = x & 0x00FF;
	x >>= 8;
	a = x & 0x00FF;
	y = bc->S[0][a] + bc->S[1][b];
	y = y ^ bc->S[2][c];
	y = y + bc->S[3][d];

	return y;
}

void TelltaleToolLib_BlowfishEncrypt(unsigned char* data, unsigned int size, bool n, unsigned char* k) {
	Blowfish cipher;
	Blowfish* c = &cipher;
	if (n) {
		kn(c, k, strlen((char*)k));
		en(c, (unsigned long*)data, size / 8);
	}
	else {
		ko(c, k, strlen((char*)k));
		eo(c, (unsigned long*)data, size / 8);
	}
}

const char* TelltaleToolLib_GetRawBlowfishKey(const char* gameID) {
	for (int i = 0; i < KEY_COUNT; i++) {
		if (!_stricmp(sBlowfishKeys[i].game_id, gameID))
			return sBlowfishKeys[i].game_key;
	}
	return NULL;
}

bool TelltaleToolLib_DoesGameUseModifiedEncryption(const char* gameID) {
	for (int i = 0; i < KEY_COUNT; i++) {
		if (!_stricmp(sBlowfishKeys[i].game_id, gameID))
			return sBlowfishKeys[i].isNewEncryption;
	}
	return false;
}

void TelltaleToolLib_BlowfishDecrypt(unsigned char* data, unsigned int size, bool n, unsigned char* k) {
	Blowfish cipher;
	Blowfish* c = &cipher;
	if (n) {
		kn(c, k, strlen((char*)k));
		dn(c, (unsigned long*)data, size / 8);
	}
	else {
		ko(c, k, strlen((char*)k));
		_do(c, (unsigned long*)data, size / 8);
	}
}

void beo(Blowfish* bc, unsigned long* xl, unsigned long* xr)
{
	unsigned long  Xl;
	unsigned long  Xr;
	unsigned long  temp;
	short          i;

	Xl = *xl;
	Xr = *xr;

	for (i = 0; i < N; ++i)
	{
		Xl = Xl ^ bc->P[i];
		Xr = bf(bc, Xl) ^ Xr;

		temp = Xl;
		Xl = Xr;
		Xr = temp;
	}

	temp = Xl;
	Xl = Xr;
	Xr = temp;

	Xr = Xr ^ bc->P[N];
	Xl = Xl ^ bc->P[N + 1];

	*xl = Xl;
	*xr = Xr;
}

void bdo(Blowfish* bc, unsigned long* xl, unsigned long* xr)
{
	unsigned long  Xl;
	unsigned long  Xr;
	unsigned long  temp;
	short          i;

	Xl = *xl;
	Xr = *xr;

	for (i = N + 1; i > 1; --i)
	{
		Xl = Xl ^ bc->P[i];
		Xr = bf(bc, Xl) ^ Xr;

		/* Exchange Xl and Xr */
		temp = Xl;
		Xl = Xr;
		Xr = temp;
	}

	/* Exchange Xl and Xr */
	temp = Xl;
	Xl = Xr;
	Xr = temp;

	Xr = Xr ^ bc->P[1];
	Xl = Xl ^ bc->P[0];

	*xl = Xl;
	*xr = Xr;
}

short io(Blowfish* bc, unsigned char key[], int keybytes)
{
	short          i;
	short          j;
	short          k;
	unsigned long  data;
	unsigned long  datal;
	unsigned long  datar;

	/* initialise p & s-boxes without file read */
	for (i = 0; i < N + 2; i++)
	{
		bc->P[i] = bfp[i];
	}
	for (i = 0; i < 256; i++)
	{
		bc->S[0][i] = ks0[i];
		bc->S[1][i] = ks1[i];
		bc->S[2][i] = ks2[i];
		bc->S[3][i] = ks3[i];
	}

	j = 0;
	for (i = 0; i < N + 2; ++i)
	{
		data = 0x00000000;
		for (k = 0; k < 4; ++k)
		{
			data = (data << 8) | key[j];
			j = j + 1;
			if (j >= keybytes)
			{
				j = 0;
			}
		}
		bc->P[i] = bc->P[i] ^ data;
	}

	datal = 0x00000000;
	datar = 0x00000000;

	for (i = 0; i < N + 2; i += 2)
	{
		beo(bc, &datal, &datar);

		bc->P[i] = datal;
		bc->P[i + 1] = datar;
	}

	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 256; j += 2)
		{

			beo(bc, &datal, &datar);

			bc->S[i][j] = datal;
			bc->S[i][j + 1] = datar;
		}
	}
	return 0;
}

void ko(Blowfish* c, unsigned char* k, int len)
{
	io(c, k, len);
}

void eo(Blowfish* c, unsigned long* data, int blocks)
{
	unsigned long* d;
	int i;

	d = data;
	for (i = 0; i < blocks; i++)
	{
		beo(c, d, d + 1);
		d += 2;
	}
}

void _do(Blowfish* c, unsigned long* data, int blocks)
{
	unsigned long* d;
	int i;

	d = data;
	for (i = 0; i < blocks; i++)
	{
		bdo(c, d, d + 1);
		d += 2;
	}
}

// modified functions for version 7

unsigned long bs(unsigned long num) {
	return(((num & 0xff000000) >> 24) |
		((num & 0x00ff0000) >> 8) |
		((num & 0x0000ff00) << 8) |
		((num & 0x000000ff) << 24));
}

void ben(Blowfish* bc, unsigned long* xl, unsigned long* xr)
{
	unsigned long  Xl;
	unsigned long  Xr;
	unsigned long  temp;
	short          i;

	Xl = *xl;
	Xr = *xr;

	for (i = 0; i < N; ++i)
	{
		switch (i) {   // version7
		case 1:  temp = bc->P[3];   break;
		case 2:  temp = bc->P[4];   break;
		case 3:  temp = bc->P[1];   break;
		case 4:  temp = bc->P[2];   break;
		default: temp = bc->P[i];   break;
		}
		Xl = Xl ^ temp;
		Xr = bf(bc, Xl) ^ Xr;

		temp = Xl;
		Xl = Xr;
		Xr = temp;
	}

	temp = Xl;
	Xl = Xr;
	Xr = temp;

	Xr = Xr ^ bc->P[N];
	Xl = Xl ^ bc->P[N + 1];

	*xl = Xl;
	*xr = Xr;
}

void bdn(Blowfish* bc, unsigned long* xl, unsigned long* xr)
{
	unsigned long  Xl;
	unsigned long  Xr;
	unsigned long  temp;
	short          i;

	Xl = *xl;
	Xr = *xr;

	for (i = N + 1; i > 1; --i)
	{
		switch (i) {   // version7
		case 4:  temp = bc->P[2];   break;
		case 3:  temp = bc->P[1];   break;
		case 2:  temp = bc->P[4];   break;
		default: temp = bc->P[i];   break;
		}
		Xl = Xl ^ temp;
		Xr = bf(bc, Xl) ^ Xr;

		/* Exchange Xl and Xr */
		temp = Xl;
		Xl = Xr;
		Xr = temp;
	}

	/* Exchange Xl and Xr */
	temp = Xl;
	Xl = Xr;
	Xr = temp;

	Xr = Xr ^ bc->P[3];  // version7
	Xl = Xl ^ bc->P[0];

	*xl = Xl;
	*xr = Xr;
}

short in(Blowfish* bc, unsigned char key[], int keybytes)
{
	short          i;
	short          j;
	short          k;
	unsigned long  data;
	unsigned long  datal;
	unsigned long  datar;

	/* initialise p & s-boxes without file read */
	for (i = 0; i < N + 2; i++)
	{
		bc->P[i] = bfp[i];
	}
	for (i = 0; i < 256; i++)
	{
		bc->S[0][i] = ks0[i];
		bc->S[1][i] = ks1[i];
		bc->S[2][i] = ks2[i];
		bc->S[3][i] = ks3[i];
	}
	bc->S[0][118] = bs(bc->S[0][118]);   // version7

	j = 0;
	for (i = 0; i < N + 2; ++i)
	{
		data = 0x00000000;
		for (k = 0; k < 4; ++k)
		{
			data = (data << 8) | key[j];
			j = j + 1;
			if (j >= keybytes)
			{
				j = 0;
			}
		}
		bc->P[i] = bc->P[i] ^ data;
	}

	datal = 0x00000000;
	datar = 0x00000000;

	for (i = 0; i < N + 2; i += 2)
	{
		beo(bc, &datal, &datar);

		bc->P[i] = datal;
		bc->P[i + 1] = datar;
	}

	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 256; j += 2)
		{
			beo(bc, &datal, &datar);

			bc->S[i][j] = datal;
			bc->S[i][j + 1] = datar;
		}
	}

	return 0;
}

void kn(Blowfish* c, unsigned char* k, int len)
{
	in(c, k, len);
}

void en(Blowfish* c, unsigned long* data, int blocks)
{
	unsigned long* d;
	int i;

	d = data;
	for (i = 0; i < blocks; i++)
	{
		ben(c, d, d + 1);
		d += 2;
	}
}

void dn(Blowfish* c, unsigned long* data, int blocks)
{
	unsigned long* d;
	int i;

	d = data;
	for (i = 0; i < blocks; i++)
	{
		bdn(c, d, d + 1);
		d += 2;
	}
}

unsigned int sSetKeyIndex = DEFAULT_BLOWFISH_GAME_KEY;
//Blowfish* spBlowfish;

/*

void Blowfish::Shutdown() {
	if (!spBlowfish)return;
	delete spBlowfish;
	spBlowfish = 0;
}

void Blowfish::Init(const char* password, bool _New) {
	this->PA[0] = 608135816;
	this->PA[1] = -2052912941;
	this->PA[2] = 320440878;
	this->PA[3] = 57701188;
	this->PA[4] = -1542899678;
	this->PA[5] = 698298832;
	this->PA[6] = 137296536;
	this->PA[7] = -330404727;
	this->PA[8] = 1160258022;
	this->PA[9] = 953160567;
	this->PA[10] = -1101764913;
	this->PA[11] = 887688300;
	this->PA[12] = -1062458953;
	this->PA[13] = -914599715;
	this->PA[14] = 1065670069;
	this->PA[15] = -1253635817;
	this->PA[16] = -1843997223;
	this->PA[17] = -1988494565;
	for (int i = 0; i < 256; i++) {
		this->SB[0][i] = ks0[i];
		this->SB[1][i] = ks1[i];
		this->SB[2][i] = ks2[i];
		this->SB[3][i] = ks3[i];
	}
	this->Gen_Subkeys(password);
	mbInitialized = true;
}

void Blowfish::Gen_Subkeys(const char* Passwd) {
	int len = 0;
	while (Passwd[len]) {
		len++;
	}
	if (len) {
		short j = 0, i = 0,k = 0, N = 16;
		unsigned long data,datal,datar;//32bits

		for (i = 0; i < N + 2; ++i) {
			data = 0x00000000;
			for (k = 0; k < 4; ++k) {
				data = (data << 8) | Passwd[j];
				j = j + 1;
				if (j >= len)j = 0;
			}
			PA[i] = PA[i] ^ data;
		}
		datal = 0;
		datar = 0;
		for (i = 0; i < N + 2; i += 2) {
			BF_En(&datal, &datar);
			PA[i] = datal;
			PA[i + 1] = datar;
		}
		for (i = 0; i < 4; ++i) {
			for (j = 0; j < 256; j += 2) {
				BF_En(&datal, &datar);
				SB[i][j] = datal;
				SB[i][j + 1] = datar;
			}
		}
	}
}

Blowfish::Blowfish(const char* password) : Blowfish() {
	Init(password);
}

Blowfish::Blowfish() : mbInitialized(false){}

Blowfish* Blowfish::Get() {
	return spBlowfish;
}

void Blowfish::Initialize(const BlowfishKey* key) {
	if (!spBlowfish) {
		spBlowfish = new Blowfish();
		spBlowfish->Init(key->game_key);
	}
}
*/