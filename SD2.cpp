#include <iostream>
#include <bitset>
using namespace std;

int NAND(int a, int b);
int OR(int a, int b);
int AND(int a, int b);
int XOR(int a, int b);
int NOR(int a, int b);
int NOT(int a);
int EQ(int a, int b);
int* decoder(int x, int y, int z);
void half_adder(int a, int b, int& sum, int& carry);
void full_adder(int a, int b, int carry, int& sum, int& carry_out);
bitset <8> adder(bitset <8> a, bitset <8> b);
bitset <8> atimtis(bitset <8> a, bitset <8> b);
bitset <8> shift(bitset <8> a, int c);
bitset <8> daugyba(bitset <8> a, bitset <8> b);
bitset <8> minus1(bitset <8> a);
bitset <8> comparison(bitset <8> a, bitset <8> b);
bitset <8> ENA(bitset <8> a, bitset <8> b);
bitset <8> ENB(bitset <8> a, bitset <8> b);
void alu(bitset <8> a, bitset <8> b, int s0, int s1, int s2, int ena, int enb, int c, int* p);
int main() {
	int s0 = 0; // 1 control
	int s1 = 1; // 2 control
	int s2 = 0; // 3 control
	int c = 0; // Shift. 0 - i kaire, 1 - i desine 
	int ena = 1; // enable/disable a
	int enb = 1; // enable/disable b
	int* p = decoder(s0, s1, s2); // decoder
	bitset<8> a("11111111");
	bitset<8> b("00000000");
	alu(a, b, s0, s1, s2, ena, enb, c, p);
	return 0;
}
int NAND(int a, int b) {
	if (a == 0 && b == 0) {
		return 1;
	}
	if (a == 0 && b == 1) {
		return 1;
	}
	if (a == 1 && b == 0) {
		return 1;
	}
	if (a == 1 && b == 1) {
		return 0;
	}
}
int OR(int a, int b) {
	return NAND(NAND(a, a), NAND(b, b));
}
int AND(int a, int b) {
	return NAND(NAND(a, b), NAND(a, b));
}
int XOR(int a, int b) {
	return NAND(NAND(a, NAND(a, b)), NAND(b, NAND(a, b)));
}
int NOR(int a, int b) {
	return NAND(a, b);
}
int NOT(int a) {
	return NAND(a, a);
}
int EQ(int a, int b) {
	return NOR(XOR(a,b),XOR(a,b));
}
int * decoder(int x, int y, int z) {
	int d[8];
	d[0] = AND(AND(NOT(x), NOT(y)), NOT(z));
	d[1] = AND(AND(NOT(x), NOT(y)), z);
	d[2] = AND(AND(NOT(x), y), NOT(z));
	d[3] = AND(AND(NOT(x), y), z);
	d[4] = AND(AND(x, NOT(y)), NOT(z));
	d[5] = AND(AND(x, NOT(y)), z);
	d[6] = AND(AND(x, y), NOT(z));
	d[7] = AND(AND(x, y), z);
	return d;
}
void half_adder(int a, int b, int& sum, int& carry) {
	sum = XOR(a, b);
	carry = AND(a, b);
}
void full_adder(int a, int b, int carry, int& sum, int& carry_out) {
	int carry1, carry2;
	half_adder(a, b, sum, carry1);
	half_adder(sum, carry, sum, carry2);
	carry_out = OR(carry1, carry2);
}
bitset <8> adder(bitset <8> a, bitset <8> b) {
	bitset <8> sum;
	int carry = 0;
	int carry_in , s,t;
	for (int i = 0; i < 8; i++) {
		carry_in = carry;
		full_adder(a[i], b[i], carry_in, s, carry);
		sum[i] = s;
	}
	return sum;
}
bitset <8> atimtis(bitset <8> a, bitset <8> b) {
	bitset <8> sub;
	bitset <8> vienas("00000001");
	int carry = 0;
	int carry_in, s;
	for (int i = 0; i < 8; i++) {
		carry_in = carry;
		full_adder(NOT(b[i]), vienas[i], carry_in, s, carry);
		sub[i] = s;
	} 
	return adder(a,sub);
}
bitset <8> shift(bitset <8> a, int c) {
	bitset <8> shifted;
	shifted[7] = AND(NOT(c), a[6]);
	shifted[6] = OR(AND(c, a[7]), AND(NOT(c), a[5]));
	shifted[5] = OR(AND(c, a[6]), AND(NOT(c), a[4]));
	shifted[4] = OR(AND(c, a[5]), AND(NOT(c), a[3]));
	shifted[3] = OR(AND(c, a[4]), AND(NOT(c), a[2]));
	shifted[2] = OR(AND(c, a[3]), AND(NOT(c), a[1]));
	shifted[1] = OR(AND(c, a[2]), AND(NOT(c), a[0]));
	shifted[0] = AND(c, a[1]);
	return shifted;
}
bitset <8> daugyba(bitset <8> a, bitset <8> b) {
	int c = 0;
	bitset <8> sand = a;
	bitset <8> temp = 0;
	for (int i = 0; i < 8; i++) {
		if (b[i] == 0) {
			sand = 0;
		}
		if (b[i] == 1) {
			sand = a;
		}
		a = shift(a, c);
		temp = adder(sand, temp);
	}
	
	return temp;
}
bitset <8> minus1(bitset <8> a) {
	bitset <8> temp;
	int t = 0;
	for (int i = 0; i < 8; i++) {	
		temp[i] = NOT(a[i]);
		if (temp[i] == 0) {
			t = i;
			break;
		}
	}
	for (int i = t+1; i < 8; i++) {
		temp[i] = a[i];
	}
	return temp;
}
bitset <8> comparison(bitset <8> a, bitset <8> b) {
	bitset <8> temp;
	bool f = false;
	for (int i = 0; i < 8; i++) {
		temp[i] = EQ(NOT(a[i]), b[i]);
		if (temp[i] == 1) {
			f = true;
			break;
		}
		if (temp[i] == 0) {
			f = false;
		}
	}
	cout << f;
	return f;
}
bitset <8> ENA(bitset <8> a, bitset <8> b) {
	bitset <8> temp;
	for (int i = 0; i < 8; i++) {
		temp[i] = AND(a[i], b[0]);
	}
	return temp;
}
bitset <8> ENB(bitset <8> a, bitset <8> b) {
	bitset <8> temp;
	for (int i = 0; i < 8; i++) {
		temp[i] = AND(a[i], b[0]);	
	}
	return temp;
}
void alu(bitset <8> a, bitset <8> b, int s0, int s1, int s2, int ena, int enb, int c, int *p) {
	a = ENA(a, ena);
	b = ENB(b, enb);
	cout << "A: " << a << endl;
	cout << "B: " << b << endl;
	p = decoder(s0, s1, s2);
	for (int i = 0; i < 8; i++) {
		if (*(p + 0) == 1) {
			cout << s0 << s1 << s2 << ". " << "A-B: " << atimtis(a, b) << endl;
			break;
		}
		if (*(p + 1) == 1) {
			cout << s0 << s1 << s2 << ". " << "A+B: " << adder(a, b) << endl;
			break;
		}
		if (*(p + 2) == 1) {
			cout << s0 << s1 << s2 << ". " << "A*B: " << daugyba(a, b) << endl;
			break;
		}
		if (*(p + 3) == 1) {
			cout << s0 << s1 << s2 << ". " << "A << 1: " << shift(a, c) << endl;
			break;
		}
		if (*(p + 4) == 1) {
			cout << s0 << s1 << s2 << ". " << "A-1: " << minus1(a) << endl;
			break;
		}
		if (*(p + 5) == 1) {
			cout << s0 << s1 << s2 << ". " << "B!=0: ";
			comparison(b, 0);
			cout << endl;
			break;
		}
		if (*(p + 6) == 1) {
			cout << "Ivestas registras ( " << s0 << s1 << s2 << " ) neatlieka jokios aritmetines operacijos! Iveskite kita ir bandykite dar karta." << endl;
			break;
		}
		if (*(p + 7) == 1) {
			cout << "Ivestas registras ( " << s0 << s1 << s2 << " ) neatlieka jokios aritmetines operacijos! Iveskite kita ir bandykite dar karta." << endl;
			break;
		}
	}

}