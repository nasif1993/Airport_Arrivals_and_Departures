/*
Task: H4
Family Name: Mannan
Given Name: Nasif
Matriculation number: 3055949
Uni-Email: nasif.mannan@stud.uni-due.de
Course of Studies: ISE CSE
*/

#include <iostream>
#include <iomanip>
#include <string>

enum ZONES {ACT, CET, CST, EST, GMT, MSK, PST, UTC};
enum ARRIVALORDEPARTURE {Arrival, Departure};

using namespace std;
class LocalTime
{
private:
	int minutes;
	ZONES zone;
public:
	LocalTime() {
		minutes = -1;
		zone = CET;
	}
	LocalTime(int newHour, int newMinute, ZONES newZone = CET) {
		set_time(newHour, newMinute);
		zone = newZone;
	}
	void set_time(int newHour, int newMinute) {
		if (newHour < 0 || newHour >= 24 || newMinute < 0 || newMinute >= 60) {
			cout << "Out of range" << endl;
			minutes = -1;
		}
		else {
			minutes = newHour * 60 + newMinute;
		}
	}
	bool is_valid() {
		if (minutes == -1)
			return false;
		return true;
	}

	int get_hour() {
		if (is_valid())
			return minutes / 60;
		return -1;
	}

	int get_minute() {
		if (is_valid())
			return minutes % 60;
		return -1;
	}
	friend ostream& operator<<(ostream& os, const LocalTime& delayed);
};

ostream& operator<<(ostream& os, LocalTime& delayed)
{
	if (delayed.is_valid())
		os << string(11, ' ') << setfill('0') << setw(2) << delayed.get_hour() << ':' << setfill('0') << setw(2) << delayed.get_minute();
	else
		os << string(16, ' ');
	return os;
}

class Flight
{
private:
	ARRIVALORDEPARTURE arrivalOrDeparture;
	string code, destination, gate, checkIn, comment;
	LocalTime expected;
public:
	Flight(ARRIVALORDEPARTURE newAOR, string newCode, string newDest, string newGate, string newCheckIn);
	string get_code() { return code; }
	string get_destination() { return destination; }
	string get_gate() { return gate; }
	string get_checkIn(){ return checkIn; }
	string get_comment() { return comment; }
	LocalTime get_scheduled() { return expected; }
	void set_expected(LocalTime newExp){ expected.set_time(newExp.get_hour(), newExp.get_minute());}
	void set_comment(string c){ comment = c; }
	bool is_arrival(){ return arrivalOrDeparture == Arrival ? true : false; }
	bool is_departure(){ return arrivalOrDeparture == Departure ? true : false; }
	void print(LocalTime scheduled) {
		cout << setfill(' ') << setw(16) << code;
		cout << setfill(' ') << setw(20) << destination;
		cout << scheduled;
		cout << expected;
		cout << setfill(' ') << setw(16) << gate;
		cout << setfill(' ') << setw(16) << checkIn;
		cout << setfill(' ') << setw(16) << comment << endl;
	}
	~Flight();
};

Flight::Flight(ARRIVALORDEPARTURE newAOR, string newCode, string newDest, string newGate, string newCheckIn) : code(newCode), destination(newDest), gate(newGate), checkIn(newCheckIn), comment("") {
	arrivalOrDeparture = newAOR;
}
Flight::~Flight() { cout << "flight " << get_code() << " deleted" << endl; }

class Airport
{
private:
	string name;
	Flight* slot[24][60];
public:
	Airport(string newName): name(newName) {
		for (int i = 0; i < 24; i++)
			for (int j = 0; j < 60; j++)
				slot[i][j] = NULL;
	}

	~Airport() {
		for (int i = 0; i < 24; i++)
			for (int j = 0; j < 60; j++)
				if (slot[i][j])
					delete slot[i][j];
	}
	void schedule(LocalTime delayed, Flight* flight){ slot[delayed.get_hour()][delayed.get_minute()] = flight; }
	void comment(LocalTime delayed, string newComment) { if (slot[delayed.get_hour()][delayed.get_minute()]) slot[delayed.get_hour()][delayed.get_minute()]->set_comment(newComment); }
	void delay(LocalTime delayed, LocalTime expected) { if (slot[delayed.get_hour()][delayed.get_minute()]) slot[delayed.get_hour()][delayed.get_minute()]->set_expected(expected); }
	void print(ARRIVALORDEPARTURE newAOD) {
		if (newAOD == Arrival) {
			cout << name << " ARRIVALS" << endl;
			cout << string(name.length() + 9, '=') << endl;
		} else {
			cout << name << " DEPARTURES" << endl;
			cout << string(name.length() + 11, '=') << endl;
		}
		cout << setfill(' ') << setw(16) << "Flight";
		if (newAOD == Arrival) cout << setfill(' ') << setw(20) << "From";
		else cout << setfill(' ') << setw(20) << "To";
		cout << setfill(' ') << setw(19) << "Scheduled  ";
		cout << setfill(' ') << setw(13) << "Expected";
		cout << setfill(' ') << setw(16) << "Gate";
		cout << setfill(' ') << setw(16) << "Check-in";
		cout << setfill(' ') << setw(16) << "Comments" << endl;
		if (newAOD == Arrival) {
			for (int i = 0; i < 24; i++)
				for (int j = 0; j < 60; j++)
					if (slot[i][j] && slot[i][j]->is_arrival())
							slot[i][j]->print(LocalTime(i, j));
		}
		else {
			for (int i = 0; i < 24; i++)
				for (int j = 0; j < 60; j++)
					if (slot[i][j] && slot[i][j]->is_departure())
						slot[i][j]->print(LocalTime(i, j));
		}
		cout << endl;
	}
};

int main() {
	Airport ap("DUSSELDORF AIRPORT");
	Flight *flight1 = new Flight(Arrival, "LH 2010", "Munich", "A04", "");
	ap.schedule(LocalTime(12, 40), flight1);
	ap.delay(LocalTime(12, 40), LocalTime(13, 5));
	Flight *flight2 = new Flight(Arrival, "EW 9347", "Manchester", "B04", "");
	ap.schedule(LocalTime(14, 50), flight2);
	Flight *flight3 = new Flight(Departure, "AF 1307", "Paris", "B51", "192-194");
	ap.schedule(LocalTime(9, 10), flight3);
	ap.comment(LocalTime(9, 10), "departed");
	Flight *flight4 = new Flight(Departure, "SU 2537", "Moscow", "C31", "252-255");
	ap.schedule(LocalTime(10, 40), flight4);
	ap.comment(LocalTime(10, 40), "boarding");
	Flight *flight5 = new Flight(Departure, "EW 9466", "London-Heathrow", "B35", "151-170");
	ap.schedule(LocalTime(11, 15), flight5);
	Flight *flight6 = new Flight(Departure, "LH 2011", "Munich", "A40", "115-120");
	ap.schedule(LocalTime(13, 25), flight6);
	ap.comment(LocalTime(13, 25), "Code Sharing");
	Flight *flight7 = new Flight(Departure, "XQ 959", "Izmir", "C45", "240-242");
	ap.schedule(LocalTime(14, 55), flight7);
	ap.delay(LocalTime(14, 55), LocalTime(15, 20));
	ap.print(Arrival);
	ap.print(Departure);
}
