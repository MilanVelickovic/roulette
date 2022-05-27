#include "StandardUser.h"

double StandardUser::cashBack(double cash, double reward) {
	// over 10000
	if (cash >= 10000 && reward == 0) {
		return (double)(cash * 0.1);
	}
	return 0;
}