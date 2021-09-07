#include "dongtai.h"
using namespace std;
template <typename T>
class My_vector{
public:
    int size;
    T *p;

    //构造函数
    My_vector():size(0),p(NULL)
    {}

    My_vector(int S){
        this -> size = S;
        this -> p = new T[size];
    }

    ~My_vector(){
        if(p!=NULL) delete []p;
    }

    My_vector(const My_vector& v){
        this -> size = v.size;
        this -> p = new T[this->size];
        memcpy(this->p, v.p, this->size*sizeof(T));
    }

    void clear(){
        delete[] p;
        p = NULL;
        this -> size = 0;
    }

    //在数组后面加一个元素，数组长自动加一
    void push_back(T data){
        this -> size += 1;
        T *p1 = new T[size];
        memcpy(p1, this->p, this->size*sizeof(T));
        delete[] p;
        this -> p = p1;
        p[size-1] = data;
    }

    //删除数组后面最后一个元素，数组长度自动减一
    void pop_back(){
        this -> size -= 1;
        T *p2 = new T[size];
        memcpy(p2, this->p, this->size*sizeof(T));
        delete[] p;
        this->p = p2;
    }

    T at(int num){
        if(num > this->size-1){
            cout << "访问越界" << endl;
            return 0;
        }

        else{
            return this->p[num-1];
        }
    }
};

int main(){
    My_vector<int> test2(10);
    for (int i=0;i<10;i++){
        test2.p[i] = i;
    }

    My_vector<int> test1(test2);
    cout << "This is the copy of vector: ";
    for(int i=0;i<test1.size;i++) cout << test1.p[i] << ' ';
    cout << endl << "The size is: " << test1.size << endl;

    test1.push_back(22);
    cout << "This is the push_back of vector: ";
    for(int i=0;i<test1.size;i++) cout << test1.p[i]<< ' ';
    cout << endl << "The size changes to: " << test1.size << endl;

    test1.pop_back();
    cout << "This is the pop_back of vector: ";
    for(int i=0;i<test1.size;i++) cout << test1.p[i] << ' ';
    cout << endl << "The size changes to: " << test1.size << endl;

    cout << "The fifth element of vector is: " << test1.at(5) << endl;

    test1.clear();
    cout << "After clear the size of vector is: " << test1.size << endl;
}
