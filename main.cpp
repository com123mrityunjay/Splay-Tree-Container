#include<iostream>
#include <vector>
#include "splay.h"

using namespace std;

class Year
{
    
    public:
    Year():yyyy_(0) {}
    Year(int yyyy) : yyyy_(yyyy) { }
    bool operator<(const Year& rhs) const
    {
        return yyyy_<rhs.yyyy_;
    }
    bool operator>(const Year& rhs) const
    {
        return yyyy_>rhs.yyyy_;
    }
    bool operator==(const Year& rhs) const
    {
        return yyyy_==rhs.yyyy_;
    }
    bool operator!=(const Year& rhs) const
    {
        return !(*this==rhs);
    }

    friend ostream& operator<<(ostream& os, const Year& y);

    private:
    int yyyy_;

};

ostream& operator<<(ostream& os, const Year& y)
{
        os << y.yyyy_;
        return os;
}


int main()
{

    splay<Year> y;

    Year y1(2004);
    Year y2(2000);
    Year y3(2012);
    Year y4(2008);
    Year y5(2016);
    Year y6(2020);

    y.insert(y1);
    y.insert(y2);
    y.insert(y3);
    y.insert(y4);
    y.insert(y5);
    y.insert(y6);

    
    cout<<"\n";
    cout<<"Inorder traversal of y:  ";
    for(auto i = y.begin(); i!=y.end(); ++i) cout<<*i<<" "; cout<<"\n";

    cout<<"\n";

    cout<<"Inorder reverse traversal:  ";
    for(auto i = y.rbegin(); i!=y.rend(); ++i)
    {
        cout<<*i<<" ";
    }
    cout<<"\n";

    cout<<"\n";
    cout<<"Before searching for 2016 the root of the splay tree y is :  ";
    
    cout << y.base() << "\n";
    if(y.find(2016)!=y.end())
    {
        cout<<"\n";
        cout<<"Find Result: 2016 is present in the splay tree.\n";
    }
    else
    {
        cout<<"\n";
        cout<<"Find Result: 2016 is not present in the splay tree.\n";
    }

    cout<<"\n";
    cout<<"Inorder traversal after searching for 2016:  ";
    for(auto i = y.begin(); i!=y.end(); ++i)
    {
        cout<<*i<<" ";
    }

    cout<<"\n";

    cout<<"\n";
    cout<<"After searching for 2016 the root of the splay tree y is :  ";
    cout << y.base() << "\n";

    splay<Year> z;

    Year z1(2004);
    Year z2(2000);
    Year z3(2012);
    Year z4(2008);
    Year z5(2016);
    Year z6(2020);

    z.insert(z1);
    z.insert(z2);
    z.insert(z3);
    z.insert(z4);
    z.insert(z5);
    z.insert(z6);
	
    cout<<"\n";
    cout<<"Before deletion of 2008 the root of the splay tree z is :  ";
    cout << z.base() << "\n";
    z.erase(2008);
	
    cout<<"\n";
    cout<<"Deleted 2008 from z. "<<"\n";
    
    
    cout<<"\n";
    cout<<"Inorder traversal of z after deletion from z:  ";
    for(auto i = z.begin(); i!=z.end(); ++i) cout<<*i<<" "; cout<<"\n";
    cout<<"\n";
    cout<<"After deletion of 2008 the root of the splay tree z is :  ";
    cout << z.base() << "\n";
    cout<<"\n";
    cout<<"Inorder traversal of y after deletion from z:  ";
    for(auto i = y.begin(); i!=y.end(); ++i) cout<<*i<<" "; cout<<"\n";

    auto i = y.begin();
    auto j = i + 3;
    j += 2;
    cout<<"\n";
    cout << "Dereferencing the iterator after advancing it by 5 positions:  ";
    cout << *(j) << "\n";
    
    
    cout << boolalpha;
    
    cout<<"\n";
    cout<<"Is y empty? "<<y.empty()<<"\n";
    cout<<"Size of y: "<<y.size()<<"\n";

    y.clear();
    
    cout<<"\n";
    cout<<"y is cleared."<<"\n";

    cout<<"\n";
    cout<<"Is y empty? "<<y.empty()<<"\n";
    cout<<"Size of y: "<<y.size()<<"\n";

    cout<<"\n";
    cout<<"Is z empty? "<<z.empty()<<"\n";
    cout<<"Size of z: "<<z.size()<<"\n";

    
    

    return 0;
}
