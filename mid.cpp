#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include <vector>
using namespace std;
class Note {
public:
	wstring symbol;
	wstring solfege;
	Note() {
		symbol = L'\0';
		solfege = L'\0';
	}
	wstring to_solfege(char& pitch) {
		switch (pitch)
		{
		case 'C':
			return L"Do";
		case 'D':
			return L"Re";
		case 'E':
			return L"Mi";
		case 'F':
			return L"Fa";
		case 'G':
			return L"So";
		case 'A':
			return L"La";
		case 'B':
			return L"Si";
		default:
			return L"";
		}
	}
	friend char& operator >>(char& pitch, Note& note) {
		note.solfege = note.to_solfege(pitch);
		return pitch;
	}
	friend wostream& operator <<(wostream& os, Note& note) {
		os << note.symbol;
		note.symbol = note.solfege;
		return os;
	}
};
class Note4 : public Note {
public:
	Note4() {
		symbol = L'♩';
	}
};
class Note8 : public Note {
public:
	Note8() {
		symbol = L'♪';
	}
};

class SubSection {
public:
	static int nbeat;
	vector<Note*>Beat;
	SubSection(int nbeat) {
		this->nbeat = nbeat;
	}
	~SubSection() {
		for (int i = 0; i < Beat.size(); i++)
			delete Beat.at(i);
		Beat.clear();
	}
	friend istream& operator >>(istream& is, SubSection& subsection) {
		float note_value = 0.0;
		for (float i = 0.0; i < subsection.nbeat; i += note_value) {
			char pitch, temp;
			is >> pitch >> temp;
			Note* note;
			if (temp == '4') {
				note = new Note4();
				note_value = 1.0;
			}
			else {
				note = new Note8();
				note_value = 0.5;
			}
			pitch >> *note;
			subsection.Beat.push_back(note);
		}
		return is;
	}
	friend wostream& operator <<(wostream& os, SubSection& subsection) {
		Note temp;
		for (int i = 0; i < subsection.Beat.size(); i++) {
			temp = *subsection.Beat.at(i);
			os << *subsection.Beat.at(i) << L" ";
		}
		os << ((temp.solfege == temp.symbol) ? L"  " : L"| ");
		return os;
	}
};
int SubSection::nbeat = 0;

class Score {
public:
	int nSection = 0;
	vector<SubSection*>sections;
	Score() {
		SetConsoleOutputCP(CP_UTF8);
		int error = _setmode(_fileno(stdout), _O_U8TEXT);
	}
	~Score() {
		for (int i = 0; i < nSection; i++)
			delete sections.at(i);
		sections.clear();
	}
	friend istream& operator >>(istream& is, Score& score) {
		int nbeat;
		is >> nbeat >> score.nSection;
		for (int i = 0; i < score.nSection; i++) {
			SubSection* s = new SubSection(nbeat);
			is >> *s;
			score.sections.push_back(s);
		}
		return is;
	}
	friend wostream& operator <<(wostream& os, Score& score) {
		for (int i = 0; i < score.nSection; i++) {
			os << *score.sections.at(i);
		}
		os << endl;
		for (int i = 0; i < score.nSection; i++) {
			os << *score.sections.at(i);
		}
		return os;
	}
};
int main()
{
	Score s;
	cin >> s;
	wcout << s << endl;
}