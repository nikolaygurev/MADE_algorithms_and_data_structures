#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include "Huffman.h"

using std::cout;
using std::endl;
using std::fstream;
using std::move;
using std::string;
using std::vector;


typedef unsigned char byte;
const int N_CHAR_VALUES = 256;
const int INT_SIZE = 4;


//struct IInputStream {
//    fstream fs;
//
//    explicit IInputStream(const string &filename) {
//        fs.open(filename, fstream::in);
//    }
//
//    // Возвращает false, если поток закончился
//    bool Read(byte &value) {
//        if (fs >> std::noskipws >> value) {
//            return true;
//        } else {
//            return false;
//        }
//    }
//
//    ~IInputStream() {
//        fs.close();
//        fs.clear();
//    }
//};
//
//
//struct IOutputStream {
//    fstream fs;
//
//    explicit IOutputStream(const string &filename) {
//        fs.open(filename, fstream::out | fstream::trunc);
//    }
//
//    void Write(byte value) {
//        fs << value;
//    }
//
//    ~IOutputStream() {
//        fs.close();
//        fs.clear();
//    }
//};


class BitsWriter {
private:
    vector<byte> buffer;
    byte accumulator = 0;
    int bits_count = 0;
public:
    void WriteBit(bool bit);

    void WriteByte(byte byte_);

    vector<byte> GetResult();
};


void BitsWriter::WriteBit(bool bit) {
    assert(bit == 0 | bit == 1);
    // Ставим бит в аккумулятор на нужное место
    // если bits_count = 3, то 1 превратится в 00001000
    accumulator |= (static_cast<byte>(bit) << bits_count);
    bits_count++;

    if (bits_count == 8) {
        bits_count = 0;
        buffer.push_back(accumulator);
        accumulator = 0;
    }
}


void BitsWriter::WriteByte(byte byte_) {
    if (bits_count == 0) {
        buffer.push_back(byte_);
    } else {
        accumulator |= (byte_ << bits_count);
        buffer.push_back(accumulator);
        accumulator = byte_ >> (8 - bits_count);
    }
}


vector<byte> BitsWriter::GetResult() {
    if (bits_count != 0) {
        // Добавляем в буффер аккумулятор, если в нём что-то есть
        buffer.push_back(accumulator);
    }
    buffer.push_back(static_cast<byte>(bits_count));
    return move(buffer);
}


class BitsReader {
private:
    vector<byte> buffer;
    int bits_count;
    int current_pos;
    size_t n_bits_left;
public:
    explicit BitsReader(vector<byte> &buffer_);

    bool ReadBit(bool &value);

    bool ReadByte(byte &value);
};


BitsReader::BitsReader(vector<byte> &buffer_) : bits_count(0), current_pos(0) {
    buffer = move(buffer_);

    int n_last_bits = (buffer.back() == 0) ? 8 : buffer.back();
    n_bits_left = (buffer.size() - 2) * 8 + n_last_bits;
}


bool BitsReader::ReadBit(bool &value) {
    if (n_bits_left <= 0) {
        return false;
    }

    bool bit = true;
    byte mask = static_cast<byte>(bit) << bits_count;
    mask &= buffer[current_pos];
    value = mask != 0;

    if (++bits_count == 8) {
        bits_count = 0;
        current_pos++;
    }
    n_bits_left--;

    return true;
}


bool BitsReader::ReadByte(byte &value) {
    if (n_bits_left <= 7) {
        return false;
    }

    if (bits_count == 0) {
        value = buffer[current_pos++];
    } else {
        byte accumulator = buffer[current_pos] >> bits_count;
        accumulator |= (buffer[++current_pos] << (8 - bits_count));
        value = accumulator;
    }
    n_bits_left -= 8;

    return true;
}


vector<byte> IntToBytes(int int_number) {
    vector<byte> byte_array(INT_SIZE);

    for (int i = 0; i < INT_SIZE; i++) {
        byte_array[INT_SIZE - 1 - i] = (int_number >> (i * 8));
    }

    return byte_array;
}


int BytesToInt(const vector<byte> &byte_array) {
    assert(byte_array.size() == INT_SIZE);
    int int_number = 0;

    for (int i = 0; i < INT_SIZE; i++) {
        int mult = static_cast<int>(std::pow(2, 8 * (INT_SIZE - 1 - i)));
        int_number += (mult * byte_array[i]);
    }

    return int_number;
}


struct Node {
    byte value;  // Кодируемый символ
    string code;  // Битовый код символа
    long freq;  // Количество вхождений символа в последовательность
    Node *left;
    Node *right;

    // Оператор сравнения двух объектов типа Node
    bool operator()(const Node *first, const Node *second) const {
        return first->freq > second->freq;
    }

    // Вызывается при создании объекта для сравнения двух указателей на Node
    // в очереди с приоритетом
    Node() : value(0), code(""), freq(0), left(nullptr), right(nullptr) {}

    explicit Node(byte value_, long freq_,
                  Node *left_ = nullptr, Node *right_ = nullptr)
            : value(value_),
              code(""),
              freq(freq_),
              left(left_),
              right(right_) {}
};


Node *BuildTree(const vector<byte> &original_seq) {
    // Подсчитываем частоты символов во входной последовательности
    long frequencies[N_CHAR_VALUES] = {0};
    for (auto i: original_seq) {
        frequencies[i]++;
    }

    // Очередь с приоритетом из указателей на Node
    // Оператор сравнения определен в структуре Node
    std::priority_queue<Node *, vector<Node *>, Node> queue;

    // Добавляем в очередь с приоритетом узлы для всех символов, встретившихся
    // хотя бы один раз
    for (int i = 0; i < N_CHAR_VALUES; i++) {
        if (frequencies[i] > 0) {
            Node *new_node = new Node(i, frequencies[i]);
            queue.push(new_node);
        }
    }

    // На каждой итерации объединяем узлы двух самых редких символов до
    // тех пор, пока не останется один узел - это и есть дерево Хаффмана
    while (queue.size() > 1) {
        Node *node1 = queue.top();
        queue.pop();
        Node *node2 = queue.top();
        queue.pop();
        Node *new_node = new Node(0, node1->freq + node2->freq, node1, node2);
        queue.push(new_node);
    }
    Node *root = queue.top();
    queue.pop();

    return root;
}


void FillTree(Node *root, const string &code) {
    // Каждый узел дерева Хаффмана имеет или два дочерних узла, или ноль
    // Дерево содержит коды не более, чем 256 символов =>
    // В дереве может быть не более, чем 256 листьев =>
    // Максимальная глубина дерева - 256 =>
    // Максимальная глубина рекурсии - 255

    // Проверим, что узел имеет или два ребенка, или ноль
    assert((root->left == nullptr) == (root->right == nullptr));

    root->code = code;
    if (root->left != nullptr) {
        // Находимся не в листе
        FillTree(root->left, code + "0");
        FillTree(root->right, code + "1");
    }
}


void DeleteTree(Node *root) {
    if (root->left) {
        DeleteTree(root->left);
    }

    if (root->right) {
        DeleteTree(root->right);
    }

    delete root;
}


void FillCodeTable(Node *root, string codes[]) {
    // Проверим, что узел имеет или два ребенка, или ноль
    assert((root->left == nullptr) == (root->right == nullptr));

    if (root->left != nullptr) {
        // Находимся не в листе
        FillCodeTable(root->left, codes);
        FillCodeTable(root->right, codes);
    } else {
        // Находимся в листе
        codes[root->value] = root->code;
    }
}


void EncodeTree(Node *root, vector<byte> &encoded_tree) {
    // Проверим, что узел имеет или два ребенка, или ноль
    assert((root->left == nullptr) == (root->right == nullptr));

    if (root->left != nullptr) {
        // Находимся не в листе
        encoded_tree.push_back(0);
        EncodeTree(root->left, encoded_tree);
        EncodeTree(root->right, encoded_tree);
    } else {
        // Находимся в листе
        encoded_tree.push_back(1);
        encoded_tree.push_back(root->value);
    }
}


void DecodeTree(Node *root, vector<byte> &encoded_tree) {
    byte value = encoded_tree.back();
    encoded_tree.pop_back();

    if (value == 1) {
        // Если попали в лист, считываем ещё одно значение и записываем в лист
        value = encoded_tree.back();
        encoded_tree.pop_back();
        root->value = value;

        // Возвращаемся на уровень выше
        return;

    } else {
        // Если попали не в лист, строим левого ребенка
        assert(value == 0);
        Node *left_child = new Node();
        left_child->code = root->code + "0";
        root->left = left_child;
        DecodeTree(root->left, encoded_tree);

        // Не возвращаемся на уровень выше
    }

    // Строим правого ребенка
    Node *right_child = new Node();
    right_child->code = root->code + "1";
    root->right = right_child;
    DecodeTree(root->right, encoded_tree);
}


void MakeEncode(IInputStream &original, BitsWriter &bits_writer) {
    // Чтение оригинального файла
    vector<byte> original_seq;
    byte value = ' ';
    while (original.Read(value)) {
        original_seq.push_back(value);
    }

    // Построение дерева Хаффмана
    Node *root = BuildTree(original_seq);
    FillTree(root, "");

    // Заполнение таблицы кодирования
    string codes[256];
    FillCodeTable(root, codes);

    // Кодирование дерева
    vector<byte> encoded_tree;
    EncodeTree(root, encoded_tree);

    // Удаление дерева
    DeleteTree(root);

    // Кодирование размера дерева
    // Запись закодированного размера дерева
    for (auto i: IntToBytes(encoded_tree.size())) {
        bits_writer.WriteByte(i);
    }

    // Запись закодированного дерева
    for (auto i : encoded_tree) {
        bits_writer.WriteByte(i);
    }

    // Кодирование оригинального файла
    // Запись закодированного файла
    for (auto i : original_seq) {
        for (auto j : codes[i]) {
            assert((j == '0') || (j == '1'));
            bool bit = j == '1';
            bits_writer.WriteBit(bit);
        }
    }
}


void MakeDecode(BitsReader &bits_reader, BitsWriter &bits_writer) {
    // Чтение закодированного размера дерева
    // Декодирование закодированного размера дерева
    vector<byte> encoded_tree_size;
    byte value = ' ';
    for (int i = 0; i < INT_SIZE; i++) {
        bits_reader.ReadByte(value);
        encoded_tree_size.push_back(value);
    }
    int tree_size = BytesToInt(encoded_tree_size);

    // Чтение закодированного дерева
    vector<byte> encoded_tree;
    for (int i = 0; i < tree_size; i++) {
        bits_reader.ReadByte(value);
        encoded_tree.push_back(value);
    }

    // Декодирование закодированного дерева
    std::reverse(std::begin(encoded_tree), std::end(encoded_tree));
    Node *root = new Node();
    DecodeTree(root, encoded_tree);

    // Чтение закодированного файла
    vector<bool> compressed_seq;
    bool bit = true;
    while (bits_reader.ReadBit(bit)) {
        compressed_seq.push_back(bit);
    }

    // Декодирование закодированного файла
    // Запись декодированного файла
    Node *temp_root = root;
    for (auto j : compressed_seq) {
        temp_root = (!j) ? temp_root->left : temp_root->right;

        // Проверим, что узел имеет или два ребенка, или ноль
        assert((temp_root->left == nullptr) == (temp_root->right == nullptr));

        if (temp_root->left == nullptr) {
            // Находимся в листе
            bits_writer.WriteByte(temp_root->value);
            // Переходим в корень
            temp_root = root;
        }
    }

    // Удаление дерева
    DeleteTree(root);
}


void Encode(IInputStream &original, IOutputStream &compressed) {
    BitsWriter bits_writer;
    MakeEncode(original, bits_writer);

    vector<byte> result = move(bits_writer.GetResult());
    for (auto i : result) {
        compressed.Write(i);
    }

}


void Decode(IInputStream &compressed, IOutputStream &original) {
    vector<byte> compressed_seq;
    byte value = ' ';
    while (compressed.Read(value)) {
        compressed_seq.push_back(value);
    }

    BitsReader bits_reader(compressed_seq);
    BitsWriter bits_writer;
    MakeDecode(bits_reader, bits_writer);

    vector<byte> result = move(bits_writer.GetResult());
    result.pop_back();
    for (auto i : result) {
        original.Write(i);
    }
}


//int main() {
//    const string file_folder = "/Users/n.guryev/CLionProjects/Hometask_9/";
//    const string in_file = file_folder + "in_new.txt";
//    const string compressed_file = file_folder + "compressed.bin";
//    const string out_file = file_folder + "out.txt";
//
//    // При выходе из блока будет вызван деструктор IInputStream и IOutputStream
//    {
//        IInputStream encode_input(in_file);
//        IOutputStream encode_output(compressed_file);
//        Encode(encode_input, encode_output);
//    }
//
//    // При выходе из блока будет вызван деструктор IInputStream и IOutputStream
//    {
//        IInputStream decode_input(compressed_file);
//        IOutputStream decode_output(out_file);
//        Decode(decode_input, decode_output);
//    }
//
//    return 0;
//}


//int main() {
//    BitsWriter bits_writer;
//
//    bits_writer.WriteBit(true);
//    bits_writer.WriteBit(true);
//    bits_writer.WriteBit(false);
//    bits_writer.WriteBit(false);
//    bits_writer.WriteBit(true);
//    bits_writer.WriteBit(true);
//    bits_writer.WriteBit(true);
//    bits_writer.WriteByte(0xFF);
//    bits_writer.WriteBit(false);
//    bits_writer.WriteBit(true);
//    bits_writer.WriteBit(false);
//    bits_writer.WriteBit(false);
//    bits_writer.WriteBit(false);
//    bits_writer.WriteBit(false);
//    bits_writer.WriteByte(0xFE);
//    bits_writer.WriteBit(false);
//    bits_writer.WriteBit(true);
//    vector<byte> result = move(bits_writer.GetResult());
//
//    BitsReader bits_reader((result));
//
//    cout << bits_reader.ReadBit() << endl;
//    cout << bits_reader.ReadBit() << endl;
//    cout << bits_reader.ReadBit() << endl;
//    cout << bits_reader.ReadBit() << endl;
//    cout << bits_reader.ReadBit() << endl;
//    cout << bits_reader.ReadBit() << endl;
//    cout << bits_reader.ReadBit() << endl;
//    cout << int(bits_reader.ReadByte()) << endl;
//    cout << bits_reader.ReadBit() << endl;
//    cout << bits_reader.ReadBit() << endl;
//    cout << bits_reader.ReadBit() << endl;
//    cout << bits_reader.ReadBit() << endl;
//    cout << bits_reader.ReadBit() << endl;
//    cout << bits_reader.ReadBit() << endl;
//    cout << int(bits_reader.ReadByte()) << endl;
//    cout << bits_reader.ReadBit() << endl;
//    cout << bits_reader.ReadBit();
//
////    cout << int(result[4]) << endl << endl;
////    cout << int(result.back()) << endl << endl;
////
////    for (byte byte_ : result) {
////        for (int i = 0; i < 8; i++) {
////            cout << ((byte_ >> i) & 1);
////        }
////        cout << " ";
////    }
//
//    return 0;
//}
