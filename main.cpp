/*************************************************************************
	> File Name: main.cpp
    > Author: 
	> Mail: 
    > Created Time: 2015年12月22日 星期二 16时59分25秒
 ************************************************************************/
#include <fstream>
#include<iostream>
#include <sstream>
#include<string>
#include <vector>
#include <shark/Algorithms/Trainers/CARTTrainer.h>
#include <shark/ObjectiveFunctions/Loss/ZeroOneLoss.h>
using namespace std;
using namespace shark;
typedef Data<RealVector> Set;
//if cant read we should exit the program 
void read_fail()
{
    cout<<"read_file_file"<<endl;
    exit(1);


}
//read the data and build the model
void read_file(const string &_path)
{   
    //init var
    string buffer;
    ifstream file(_path.c_str());
    if(!file)
    {
    read_fail();
    }
    int num_of_elements=0;
    vector<RealVector> input;
    vector<unsigned int> label_vec;
    int i;
    while(getline(file, buffer))
    {    i=1;
        num_of_elements++;
        istringstream stream(buffer);
        char sex;
        double _sex;
        //sex should be sa int type so we have a tranfer below
        int label;
        //label should be as int type
        double feature_value;
        stream>>sex;
        if(sex=='F')
        {
             _sex=0;
        }
        else if(label=='M')
        {
            _sex=1;
        }
        else if(label=='I')
        {
            _sex=2;
        }
        i++;
        RealVector feature_vec;
        feature_vec.push_back(_sex);
        stream.ignore(buffer.size(), ',');
        while(i<=8)
        {
            stream>>feature_value;
           feature_vec.push_back(feature_value);
            stream.ignore(buffer.size(), ',');
            i++;
        }

        stream>>label;
        
        label_vec.push_back(label);
        stream.ignore(buffer.size(), ',');
        input.push_back(feature_vec);
    }

        //build CART model 
        ClassificationDataset data_set=createLabeledDataFromRange(input,label_vec);
        //get test set   
    ClassificationDataset data_test =splitAtElement(data_set,num_of_elements*3/4);
        //train
        CARTTrainer trainer;
        CARTClassifier<RealVector>model;
        trainer.train(model,data_set);

        ZeroOneLoss<unsigned int, RealVector> loss;
        //test
        Data<RealVector> prediction = model(data_set.inputs());
        
        //print
        cout << "CART on training set accuracy: " << 1. - loss.eval(data_set.labels(), prediction) << endl;
        prediction = model(data_test.inputs());

        cout << "CART on test set accuracy:     " << 1. - loss.eval(data_test.labels(), prediction) << endl;
        int diff_1=0;
        int diff_2=0;
        int diff_3=0;
        int len=1000;
        for(int i=0;i<len;i++)
        
        {
            int age=0;
            for(int j=0;j<30;j++)
            {
            if (prediction.element(i)[j]==1)
                {
                    age=j;
                    cout<<"age="<<j<<endl;
                    break;
                }

            }
            if(age<=data_test.element(i).label+1&&age>=data_test.element(i).label-1)
            diff_1++;
            if(age<=data_test.element(i).label+2&&age>=data_test.element(i).label-2)
            diff_2++;
            if(age<=data_test.element(i).label+3&&age>=data_test.element(i).label-3)
            diff_3++;


        }
        cout<<num_of_elements*3/4;
        cout << "CART on test set accuracy:     " <<diff_1*1.0/(len);
        cout << "CART on test set accuracy:     " <<diff_2*1.0/(len);
        cout << "CART on test set accuracy:     " <<diff_3*1.0/(len);
}
int main()
{
    
    string path="abalone.data";
    read_file(path);

    return 0;
}
