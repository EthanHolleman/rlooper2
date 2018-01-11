//
// Created by Robert Stolz on 6/27/17.
//

#include "gene.h"

//constructors
using namespace std;

void Gene::parse_header(){
    unsigned long pos = 0;
    string name, remaining;
    //extract gene name
    pos = header.find('=');
    name = header.substr(0,pos);
     //need to further parse the name
    pos = header.find(' ');
    name = name.substr(1,pos);
    remaining = header.substr(pos+1, header.length());
    pos = name.find(' ');
    name = name.substr(0,pos);
    gene_name = name;
    //extract chromosome name
    pos = remaining.find('=');
    remaining = remaining.substr(pos+1, remaining.length());
    pos = remaining.find(':');
    position.chromosome = remaining.substr(0,pos);
    //for (auto & c: position.chromosome) c = toupper(c); //C++11 string toUpper
    std::transform(position.chromosome.begin(), position.chromosome.end(), position.chromosome.begin(), ::tolower);
    std::transform(position.chromosome.end()-1, position.chromosome.end(), position.chromosome.end()-1, ::toupper);
    remaining = remaining.substr(pos+1, remaining.length());
    //extract the start and stop locations
    pos = remaining.find("-");
    position.start_pos = stoi(remaining.substr(0,pos));
    remaining = remaining.substr(pos+1, remaining.length());
    pos = remaining.find(" ");
    position.end_pos = stoi(remaining.substr(0,pos));
    remaining = remaining.substr(pos+1, remaining.length());
    pos = remaining.find("STRAND=");
    remaining = remaining.substr(pos+7, remaining.length());
    pos = remaining.find(" ");
    position.strand = remaining.substr(0,pos);

    return;
}
//constructors and destructors
Gene::Gene(){
    windower.set_sequence(sequence);
}

Gene::~Gene(){
    clear_structures();
}

//getters and setters
string Gene::getName(){
    return gene_name;
}

const string &Gene::getHeader() const {
    return header;
}

void Gene::setHeader(const string &header) {
    Gene::header = header;
}

const Loci &Gene::getPosition() const {
    return position;
}

void Gene::setPosition(const Loci &position) {
    Gene::position = position;
}

const vector<char, allocator<char>> &Gene::getSequence() const {
    return sequence;
}

vector<vector<Structure>*> Gene::getStructures(){
    return structures;
}

float Gene::compute_GC_skew(){

    if (sequence.size()){
        throw EmptyGeneException();
    }
    float Gs = 0.f;
    float Cs = 0.f;
    for (int i=0; i < sequence.size(); i++){
        if (sequence[i] == 'G'){
            Gs += 1.f;
        }
        else if (sequence[i] == 'C'){
            Cs += 1.f;
        }
    }
    return (Gs-Cs)/(Gs+Cs);
}

bool Gene::read_gene(ifstream& fastafile) { //need to test
    //initialize variables
    char c,p;
    //check that fstream is open and not eof, throw exceptions otherwise
    if (!fastafile.is_open()) {
        throw UnexpectedClosedFileException("Gene::read_gene");
    } else if (fastafile.eof()) {
        throw UnexpectedEOFException();
    }
    while (fastafile.get(c)) {
        //read the next character
        c = toupper(c);
        //if the character is the start of a header line
        if (c == '>') {
            //read until the end of the header line
            while (c != '\n') {
                header.push_back(c);
                c = toupper(fastafile.get());
            }
            parse_header();
        }
        else if (c == 'A' || c == 'T' || c == 'C' || c == 'G') {
            //save to sequence vector
            sequence.push_back(c);
        }
            //else if encountered some white space
        else if (c == '\n' || c == ' ' || c == '\t'){
            p = fastafile.peek();
            if (p == '>'){
                return false;
            }
            /*else if (p == EOF) {
                return true;
            }*/
            continue;
        }
            //else the charicter is unrecognized
        else {
            throw InvalidSequenceDataException(c);
        }
    }
    return true;
    //unexpected EOF
    //throw InvalidSequenceDataException();
}

void Gene::print_gene(){
    std::cout << header << std::endl;
    for (std::vector<char>::iterator it = sequence.begin(); it<sequence.end(); ++it){
        cout << *it;
    }
    std::cout << std::endl;
}

void Gene::compute_structures(Model &model){
    std::vector<Structure>* these_structures = new vector<Structure>;
    if (sequence.size() == 0){
        //throw exception
    }
    //initializing the iterators ensures that the intial comparison in next_window_from_all_windows is not problematic
    std::vector<char>::iterator start = sequence.begin(),stop=sequence.begin()+1;
    windower.reset_window();
    while (windower.has_next_window()){
        windower.next_window_from_all_windows(start,stop); //do I want the model to be able to decide it needs a different windowing scheme?
        Structure temp;
        //set the Loci of the structure using the gene's Loci
        temp.position.chromosome = position.chromosome;
        temp.position.strand = position.strand;
        temp.position.start_pos = position.start_pos + windower.get_current_start_offset();
        temp.position.end_pos = position.start_pos + windower.get_current_stop_offset();
        //pass the structure and window boundaries to the model
        model.compute_structure(start,stop,temp);
        //push the now computed structure onto these_structures
        these_structures->push_back(temp); //need to make sure the default copy constructor is working properly
    }
    //windower.print_current_window(); //DEBUG
    structures.push_back(these_structures);
}

void Gene::clear_structures(){
    for (auto it = structures.begin(); it != structures.end(); ++it ){
        delete *it;
    }
    structures.clear();
}

void Gene::complement_sequence(){
    for (int i = 0; i < sequence.size(); i++){
        if (sequence[i] == 'A')
            sequence[i] = 'T';
        else if (sequence[i] == 'T')
            sequence[i] = 'A';
        else if (sequence[i] == 'C')
            sequence[i] = 'G';
        else //sequence_data[i] == 'G'
            sequence[i] = 'C';
    }
}

void Gene::invert_sequence(){
    char temp;
    for (int i = 0; i < (sequence.size()/2); i++){
        temp = sequence[i];
        sequence[i] = sequence[sequence.size() - 1 - i];
        sequence[sequence.size() - 1 - i] = temp;
    }
}

int Gene::get_length(){
    return abs(position.end_pos - position.start_pos); //abs here or negative length exception?
}

void Gene::clear_sequence(){
    //delete sequence data
    sequence.clear();
}

