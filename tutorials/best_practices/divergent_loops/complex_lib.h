#ifndef COMPLEXTYPE_H
#define COMPLEXTYPE_H
typedef float cType; 

class complexType {

private:
	cType real, imag;
public:
	complexType(cType iReal, cType iImag) {
		real = iReal;
		imag = iImag;
	}
	complexType() {
		real = 0;
		imag = 0;
	}
	cType getReal() {
		return real;
	}
	cType getImag() {
		return imag;
	}
	void setReal(cType iReal) {
		real = iReal;
	}
	void setImag(cType iImag) {
		imag = iImag;
	}
	complexType operator* (complexType B) {
		cType Breal = B.getReal();
		cType Bimag = B.getImag();

		cType resulta = real*Breal - imag*Bimag;
		cType resultb = real*Bimag + imag*Breal;
		complexType result(resulta, resultb);
		return result;
	}
	complexType operator+ (complexType B) {
		complexType ret;
		ret.setReal(real + B.getReal());
		ret.setImag(imag + B.getImag());
		return ret;
	}
};
#endif
