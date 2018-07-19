#pragma once

#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

namespace math{

    template<class ...Args>
    struct count{
        enum{value = sizeof...(Args)};
    };

    template<class T>
    class polynomial_member{
        T val;
        unsigned short deg;
        char symbol;

    public:
        template<class U>
        polynomial_member(U val,unsigned short degree = 0,char symbol = 'x')
            : val(val),deg(degree),symbol(symbol){

        }
        T value()const{
            return val;
        }
        unsigned short degree()const{
            return deg;
        }
        bool is_negative() const{
            return val < 0;
        }
        bool is_null() const{
            return val == 0;
        }
        void set_symbol(char sym){
            symbol = sym;
        }

        friend std::ostream &operator<<(std::ostream &out,const polynomial_member &obj){
            if(!obj.is_null())
            {
                if(obj.deg != 0 && obj.deg != 1)
                    out << obj.val << obj.symbol << '^' << obj.deg;
                else if(obj.deg == 0)
                    out << obj.val;
                else
                    out << obj.val << obj.symbol;
            }
            return out;
        }
    };

    template<class T>
    class polynomial{
        std::vector<polynomial_member<T>>members;
        char symbol = 'x';
    public:
        polynomial() = default;
        template<class U,class ...Args>
        polynomial(U u,Args ...args){
           initialize(u,args...);
        }
        void set_symbol(char sym){
            std::for_each(members.begin(),members.end(),[=](polynomial_member<T> &elem){
                elem.set_symbol(sym);
            });
            symbol = sym;
        }

        polynomial& operator<<(const T &val){
            polynomial_member<T> member(val,
                                        members.size(),
                                        symbol);
            members.insert(members.begin(),member);
            return *this;
        }
        friend std::ostream &operator<<(std::ostream &out,const polynomial &obj){
           for(int size = obj.members.size(),i = 0;i < size;++i)
           {
               auto it = obj.members[i];
               (it.is_negative() ||  i == 0) ?  out << it : out << '+' << it;
           }
           return out;
        }
        T value(T val){
            T sum = std::accumulate(members.begin(),members.end(),0,
                [=](const T &init,const polynomial_member<T> &it){
                    return init + std::pow(val,it.degree()) * it.value();
                }
            );
            return sum;
        }

    private:
        template<class U,class ...Args>
        void initialize(U u,Args ...args){
            polynomial_member<T> member(u,
                                        members.size(),
                                        symbol);
            members.insert(members.begin(),member);
            initialize(args...);
        }
        template<class U>
        void initialize(U u){
            polynomial_member<T> member(u,
                                        members.size(),
                                        symbol);
            members.insert(members.begin(),member);
        }
    };
}
