#include "PremiumUser.h"

double PremiumUser::cashBack(double cash, double reward) {
	// over 7000
	if (cash >= 7000 && reward == 0) {
		return (double)(cash * 0.12);
	}
	return 0;
}