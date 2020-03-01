#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Интерфейсы, которые видны студентам

// Определение типа byte
typedef unsigned char byte;

// Входной поток
struct IInputStream {
	// Возвращает false, если поток закончился
	virtual bool Read(byte& value) = 0;
};

// Выходной поток
struct IOutputStream {
	virtual void Write(byte value) = 0;
};

// Метод архивирует данные из потока original
void Encode(IInputStream& original, IOutputStream& compressed);
// Метод восстанавливает оригинальные данные
void Decode(IInputStream& compressed, IOutputStream& original);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Входной поток
class CInputStream : public IInputStream {
public:
	CInputStream(const vector<byte>& _body) : index(0), body(_body) {}
		
	virtual bool Read(byte& value);

private:
	unsigned int index;
	const vector<unsigned char>& body;
};

bool CInputStream::Read(byte& value)
{
	if (index >= body.size()) {
		return false;
	}

	value = body[index++];
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Выходной поток
class COutputStream : public IOutputStream {
public:
	COutputStream(vector<byte>& _body) : body(_body) { body.clear(); }
	virtual void Write(byte value) { body.push_back(value); }

private:
	vector<byte>& body;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Основное приложение

// Считываем все файлы, что перечислены во входе
void fillInputs(vector< vector<byte> >& inputs)
{
	inputs.clear();

	string currentFileName;
	int a = 0;
	while (getline( cin, currentFileName )) {
		if (currentFileName.empty()) {
			continue;
		}

		// Есть еще один файл, который следует закодировать
		inputs.push_back(vector<byte>());

		ifstream file;
		file.open(currentFileName.c_str());

		byte value;
		while (file >> value) {
			(*inputs.rbegin()).push_back(value);
		}

		a++;
		if (a >= 1){
		    break;
		}
	}
}

bool isEqual(const vector<byte>& v1, const vector<byte>& v2)
{
	if (v1.size() != v2.size()) {
		return false;
	}

	for( unsigned int i = 0; i < v1.size(); i++ ) {
		if( v1[i] != v2[i] ) {
			return false;
		}
	}
	
	return true;
}

int calculateSize(const vector< vector<byte> >& array)
{
	int result = 0;
	for ( unsigned int i  = 0; i < array.size(); i++ ) {
		const vector<byte>& data = array[i];
		result += data.size(); 
	}

	return result;
}

int main()
{
	// Получаем данные, которые нужно закодировать
	vector< vector<byte> > input;

	fillInputs(input);

	// Сжимаем данные
	vector< vector<byte> > compressed;
	compressed.resize(input.size());
	for (unsigned int i = 0; i < input.size(); i++) {
		CInputStream iStream(input[i]);
		COutputStream oStream(compressed[i]);
		Encode(iStream, oStream);
	}

	// Распаковываем сжатые данные и проверяем, что они совпадают с оригиналом
	for (unsigned int i = 0; i < input.size(); i++) {
		vector<byte> output;
		CInputStream iStream(compressed[i]);
		COutputStream oStream(output);
		Decode(iStream, oStream);
		if (!isEqual(input[i], output)) {
			cout << -1;
			return 0;
		}
	}

	// Вычисляем степень сжатия
	cout << (100. * calculateSize(compressed) / calculateSize(input));

	return 0;
}
