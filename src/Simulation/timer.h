

// Wee class to make sure only one repeat is logging
class baseTimer {
public:
	baseTimer();
	virtual void logger(int step) = 0;
};


class nullTimer : public baseTimer {
public:

	nullTimer();

	void logger(int step) override;

};


class activeTimer : public baseTimer {
public:

	activeTimer();

	void logger(int step) override;

private:

	int stepper = 0;

};


