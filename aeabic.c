
void __aeabi_unwind_cpp_pr0() {}

void __aeabi_unwind_cpp_pr1() {}

unsigned int __aeabi_uidiv(unsigned int num, unsigned int den)
{
    unsigned int quot, qbit;
    
    if (den > num || den == 0)
	return 0;

    quot = 0;
    qbit = 1;
    
    while ((int)den >= 0) {
	den <<= 1;
	qbit <<= 1;
    }

    while (qbit) {
	if (den <= num) {
	    num -= den;
	    quot += qbit;
	}

	den >>= 1;
	qbit >>= 1;
    }
    
    return quot;
}
