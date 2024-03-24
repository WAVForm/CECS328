#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;

class Matrix{
    public:
        vector<vector<int>> rowsp;

        int fin_ans(){
            int sum = 0;
            for (vector<int> row : rowsp){
                for (int each : row){
                    sum += each;
                }
            }
            return sum;
        }

        Matrix operator+(const Matrix& b){
            return mat_add(*this,b);
        }

        Matrix operator-(const Matrix& b){
            return mat_sub(*this,b);
        }

        Matrix operator*(const Matrix& b){
            return mat_mul(*this,b);
        }

        Matrix operator*(int c){
            return mat_mul_var(*this,c);
        }

        Matrix mat_strassen(Matrix mat_a, Matrix mat_b){
            if(mat_a.rowsp.size() <= 128){
                return mat_a * mat_b;
            }

            while(!mat_a.isPowerOfTwo()){
                for(int i=0; i<mat_a.rowsp.size(); i++){
                    mat_a.rowsp[i].push_back(0);
                }

                vector<int> zeroes = {};
                for(int i=0; i<mat_a.rowsp[0].size(); i++){
                    zeroes.push_back(0);
                }
                mat_a.rowsp.push_back(zeroes);
            }

            while(!mat_b.isPowerOfTwo()){
                for(int i=0; i<mat_b.rowsp.size(); i++){
                    mat_b.rowsp[i].push_back(0);
                }
                
                vector<int> zeroes = {};
                for(int i=0; i<mat_b.rowsp[0].size(); i++){
                    zeroes.push_back(0);
                }
                mat_b.rowsp.push_back(zeroes);
            }

            tuple<Matrix, Matrix, Matrix, Matrix> quads_a = split(mat_a); //a=0, b=1, c=2, d=3
            tuple<Matrix, Matrix, Matrix, Matrix> quads_b = split(mat_b); //e=0, f=1, g=2, h=3

            Matrix p1 = mat_strassen(get<0>(quads_a), get<1>(quads_b) - get<3>(quads_b));
            Matrix p2 = mat_strassen(get<0>(quads_a) + get<1>(quads_a), get<3>(quads_b));
            Matrix p3 = mat_strassen(get<2>(quads_a) + get<3>(quads_a), get<0>(quads_b));
            Matrix p4 = mat_strassen(get<3>(quads_a), get<2>(quads_b) - get<0>(quads_b));
            Matrix p5 = mat_strassen(get<0>(quads_a) + get<3>(quads_a), get<0>(quads_b) + get<3>(quads_b));
            Matrix p6 = mat_strassen(get<1>(quads_a) - get<3>(quads_a), get<2>(quads_b) + get<3>(quads_b));
            Matrix p7 = mat_strassen(get<0>(quads_a) - get<2>(quads_a), get<0>(quads_b) + get<1>(quads_b));

            Matrix c11 = (p5+p4-p2+p6);
            Matrix c12 = (p1+p2);
            Matrix c21 = (p3 + p4);           
            Matrix c22 = (p1+p5-p3-p7);

            Matrix mat_c = stitch(c11,c12,c21,c22);

            return mat_c;
        }

        Matrix(string txtFile){
            rowsp = text_to_list(file_read(txtFile));
        }

        string mat_str(){
            //for pretty printing
            string mat_str = "";
            for(vector<int> row : this->rowsp){
                for(int n: row){
                    mat_str += (to_string(n)+' ');
                }
                mat_str +=  '\n';
            }
            return mat_str;
        }

    private:
        
        Matrix(){
            rowsp = {};
        }

        bool isPowerOfTwo(){
            int n = rowsp.size();
            if (n==0){
                return false;
            }
            return(ceil(log2(n)) == floor(log2(n)));
        }

        string file_read(string fileName){
            //read file char by char, store into string and return it
            ifstream txtFile;
            txtFile.open(fileName);

            string buffer = "";

            char c;
            while(txtFile.get(c)){
                if(c != '\n'){
                    buffer += c;
                }
            }

            txtFile.close();
            return buffer;
        }

        string strip(string str){
            //go through a string, look for "}", "{", and " ", deleting them if found
            string temp = "";
            for(int i=0; i<str.size();i++){
                if(str[i] != '}' && str[i] != '{' && str[i] != ' '){
                    temp += str[i];
                }
            }
            return temp;
        }

        vector<vector<int>> text_to_list(string txt){
            vector<vector<int>> rowsp = {};

            //Scan through entire string placing each char into a buffer string check for seperator "},", if found strip paranthesis and spaces from the buffer and append to rows
            vector<string> rows = {}; 
            string buffer = "";
            for(int i=0; i<txt.size(); i++){
                buffer += txt[i];
                if((txt[i] == ',' && txt[i-1] == '}') || (txt[i] == '}' && txt[i-1] == '}')){
                    buffer.pop_back();
                    buffer.pop_back();
                    rows.push_back(strip(buffer)+'\n');
                    buffer = "";
                }
            }

            //For each string in rows add the chars into buffer, look for comma seperator, if found type cast the buffer to an int and place the value into r, then push r to rowsp
            for(string each : rows){
                vector<int> r = {};
                string buffer = "";
                for(char c : each){
                    if(c == ',' || c == '\n'){
                        r.push_back(stoi(buffer));
                        buffer = "";
                    }
                    else{
                        buffer += c;
                    }

                }
                rowsp.push_back(r);
            }

            return rowsp;
        }

        vector<vector<int>> rowsp_to_colsp(vector<vector<int>> rowsp){
            vector<vector<int>> colsp = {};
            for(int j=0; j<rowsp[0].size(); j++){
                vector<int> col = {};
                for(int i=0; i<rowsp.size(); i++){
                    col.push_back(rowsp[i][j]);
                }
                colsp.push_back(col);
            }
            return colsp;
        }

        vector<vector<int>> colsp_to_rowsp(vector<vector<int>> colsp){
            vector<vector<int>> rowsp = {};
            for(int j=0; j<colsp[0].size(); j++){
                vector<int> row = {};
                for(int i=0; i<colsp.size(); i++){
                    row.push_back(colsp[i][j]);
                }
                rowsp.push_back(row);
            }
            return rowsp;
        }
        
        Matrix mat_add(Matrix a, Matrix b){
            if(a.rowsp.size() == b.rowsp.size()){
                Matrix sum_mat;
                for(int i=0; i<a.rowsp.size(); i++){
                    vector<int> sum_row = {};
                    for(int j=0; j<a.rowsp[i].size(); j++){
                        sum_row.push_back(a.rowsp[i][j] + b.rowsp[i][j]);
                    }
                    sum_mat.rowsp.push_back(sum_row);
                }
                return sum_mat; 
            }
        }

        Matrix mat_mul_var(Matrix a, int c){
            Matrix mul_array;
            for(int i=0; i<a.rowsp.size(); i++){
                vector<int> row = {};
                for(int j=0; j<a.rowsp[i].size(); j++){
                    row.push_back(a.rowsp[i][j] * c);
                }
                mul_array.rowsp.push_back(row);
            }
            return mul_array;
        }

        Matrix mat_sub(Matrix a, Matrix b){
            return mat_add(a, mat_mul_var(b, -1));
        }

        Matrix mat_mul(Matrix a, Matrix b){
            if(rowsp_to_colsp(a.rowsp).size() == b.rowsp.size()){
                vector<vector<int>> b_col = rowsp_to_colsp(b.rowsp);
                Matrix mul_array;
                for(vector<int> row : a.rowsp){
                    vector<int> r = {};
                    for(vector<int> col : b_col){
                        int sum = 0;
                        for(int i=0; i<row.size(); i++){
                            sum += row[i] * col[i];
                        }
                        r.push_back(sum);
                    }
                    mul_array.rowsp.push_back(r);
                }
                return mul_array;
            }
        }

        tuple<Matrix, Matrix, Matrix, Matrix> split(Matrix mat){
            int rowsize = mat.rowsp.size();
            int rowhalf = rowsize/2;
            int colsize = mat.rowsp[0].size();
            int colhalf = colsize/2;

            Matrix q11;
            for(int i=0; i<rowhalf; i++){
                vector<int> row;
                for(int j=0; j<colhalf; j++){
                    row.push_back(mat.rowsp[i][j]);
                }
                q11.rowsp.push_back(row);
            }

            Matrix q12;
            for(int i=0; i<rowhalf; i++){
                vector<int> row;
                for(int j=colhalf; j<colsize; j++){
                    row.push_back(mat.rowsp[i][j]);
                }
                q12.rowsp.push_back(row);
            }

            Matrix q21;
            for(int i=rowhalf; i<rowsize; i++){
                vector<int> row;
                for(int j=0; j<colhalf; j++){
                    row.push_back(mat.rowsp[i][j]);
                }
                q21.rowsp.push_back(row);
            }

            Matrix q22;
            for(int i=rowhalf; i<rowsize; i++){
                vector<int> row;
                for(int j=colhalf; j<colsize; j++){
                    row.push_back(mat.rowsp[i][j]);
                }
                q22.rowsp.push_back(row);
            }
            return {q11,q12,q21,q22};
        }

        Matrix stitch(Matrix q11, Matrix q12, Matrix q21, Matrix q22){
            Matrix h1 = q11;
            Matrix h2 = q21;

            for(int i=0; i<h1.rowsp.size();i++){
                for(int each : q12.rowsp[i]){
                    h1.rowsp[i].push_back(each);
                }
            }
            for(int i=0; i<h2.rowsp.size();i++){
                for(int each : q22.rowsp[i]){
                    h2.rowsp[i].push_back(each);
                }
            }

            for(vector<int> row : h2.rowsp){
                h1.rowsp.push_back(row);
            }

            return h1;
        }
};

int main(){
    time_t start, end; 

    cout << "------#4------" << endl;

    Matrix fourA = Matrix("probs/4a.txt");
    Matrix fourB = Matrix("probs/4b.txt");

    double four_time[2];
    time(&start);
    int four_strassen = fourA.mat_strassen(fourA,fourB).fin_ans();
    time(&end);
    four_time[0] = double(end-start);
    time(&start);
    int four_regular= (fourA*fourB).fin_ans();
    time(&end);
    four_time[1] = double(end-start);

    cout << "Regular: " << four_regular << "| time: " << four_time[1] <<
    endl << "Strass: " << four_strassen << "| time: " << four_time[0] << endl;

    
    cout << "------#6------" << endl;

    Matrix sixA = Matrix("probs/6a-1.txt");
    Matrix sixB = Matrix("probs/6b-1.txt");

    double six_time[2];
    time(&start);
    int six_strassen = sixA.mat_strassen(sixA,sixB).fin_ans();
    time(&end);
    six_time[0] = double(end-start);
    time(&start);
    int six_regular= (sixA*sixB).fin_ans();
    time(&end);
    six_time[1] = double(end-start);

    cout << "Regular: " << six_regular << "| time: " << six_time[1] <<
    endl << "Strass: " << six_strassen << "| time: " << six_time[0] << endl;


    cout << "------#7------" << endl;

    Matrix sevenA = Matrix("probs/7a-1.txt");
    Matrix sevenB = Matrix("probs/7b-2.txt");

    double seven_time[2];
    time(&start);
    int seven_strassen = sevenA.mat_strassen(sevenA,sevenB).fin_ans();
    time(&end);
    seven_time[0] = double(end-start);
    time(&start);
    int seven_regular= (sevenA*sevenB).fin_ans();
    time(&end);
    seven_time[1] = double(end-start);

    cout << "Regular: " << seven_regular << "| time: " << seven_time[1] <<
    endl << "Strass: " << seven_strassen << "| time: " << seven_time[0] << endl;


    cout << "------#6------" << endl;

    Matrix eightA = Matrix("probs/8a.txt");
    Matrix eightB = Matrix("probs/8b.txt");

    double eight_time[2];
    time(&start);
    int eight_strassen = eightA.mat_strassen(eightA,eightB).fin_ans();
    time(&end);
    eight_time[0] = double(end-start);
    time(&start);
    int eight_regular= (eightA*eightB).fin_ans();
    time(&end);
    eight_time[1] = double(end-start);

    cout << "Regular: " << eight_regular << "| time: " << eight_time[1] <<
    endl << "Strass: " << eight_strassen << "| time: " << eight_time[0] << endl;


    cout << "------#9------" << endl;

    Matrix nineA = Matrix("probs/9a.txt");
    Matrix nineB = Matrix("probs/9b.txt");

    double nine_time[2];
    time(&start);
    int nine_strassen = nineA.mat_strassen(nineA,nineB).fin_ans();
    time(&end);
    nine_time[0] = double(end-start);
    time(&start);
    int nine_regular= (nineA*nineB).fin_ans();
    time(&end);
    nine_time[1] = double(end-start);

    cout << "Regular: " << nine_regular << "| time: " << nine_time[1] <<
    endl << "Strass: " << nine_strassen << "| time: " << nine_time[0] << endl;


    cout << "------#10------" << endl;

    Matrix tenA = Matrix("probs/10a.txt");
    Matrix tenB = Matrix("probs/10b.txt");

    double ten_time[2];
    time(&start);
    int ten_strassen = tenA.mat_strassen(tenA,tenB).fin_ans();
    time(&end);
    ten_time[0] = double(end-start);
    time(&start);
    int ten_regular= (tenA*tenB).fin_ans();
    time(&end);
    ten_time[1] = double(end-start);

    cout << "Regular: " << ten_regular << "| time: " << ten_time[1] <<
    endl << "Strass: " << ten_strassen << "| time: " << ten_time[0] << endl;

    return 0;
}