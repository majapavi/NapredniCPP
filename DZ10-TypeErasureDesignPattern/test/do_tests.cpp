#include "../myfunction.h"
#include "gtest/gtest.h"
#include <vector>
#include <string>

// Testiraj obične funkcije
int f0() { return 3; }
int f1(int x) { return 3*x; }
int f2(int x, int y) { return 3*x+y; }

TEST(Function, zero_v) {
     MyFunction<int()> mf(f0);
     EXPECT_EQ(mf(), 3);
}
TEST(Function, one_v) {
     MyFunction<int(int)> mf(f1);
     EXPECT_EQ(mf(1), 3);
}
TEST(Function, two_v) {
     MyFunction<int(int,int)> mf(f2);
     EXPECT_EQ(mf(1,2), 5);
}

// Testiraj funkcijske objekte
struct F0{ int operator()(){ return 3; } };
struct F1{ int operator()(int x){ return 3*x; } };
struct F2{ int operator()(int x, int y){ return 3*x+y; } };

TEST(FObject, zero_v) {
     F0 f;
     MyFunction<int()> mf(f);
     EXPECT_EQ(mf(), 3);
}
TEST(FObject, one_v) {
     F1 f;
     MyFunction<int(int)> mf(f);
     EXPECT_EQ(mf(1), 3);
}
TEST(FObject, two_v) {
     F2 f;
     MyFunction<int(int,int)> mf(f);
     EXPECT_EQ(mf(1,2), 5);
}

// Testiraj lambde
TEST(Lambda, zero_v) {
     auto f = [](){ return 3; };
     MyFunction<int()> mf(f);
     EXPECT_EQ(mf(), 3);
}
TEST(Lambda, one_v) {
     auto f = [](int x){ return 3*x; };
     MyFunction<int(int)> mf(f);
     EXPECT_EQ(mf(1), 3);
}
TEST(Lambda, two_v) {
     auto f = [](int x, int y){ return 3*x+y; };
     MyFunction<int(int,int)> mf(f);
     EXPECT_EQ(mf(1,2), 5);
}


// Konstruktor kopije
TEST(Copy, zero_v){
    F0 f;
    MyFunction<int()> mf(f);
    MyFunction<int()> mf1(mf);  // konstruktor kopije
    EXPECT_EQ(mf(), mf1());
}


TEST(Copy, one_v){
    F1 f;
    MyFunction<int(int)> mf(f);
    MyFunction<int(int)> mf1(mf);  // konstruktor kopije
    EXPECT_EQ(mf(2), mf1(2));
}

TEST(Copy, two_v){
    F2 f;
    MyFunction<int(int,int)> mf(f);
    MyFunction<int(int,int)> mf1(mf);  // konstruktor kopije
    EXPECT_EQ(mf(1,2), mf1(1,2));
}

// Konstruktor kopije premještanjem
TEST(CopyMove, zero_v){
    F0 f;
    MyFunction<int()> mf(f);
    MyFunction<int()> mf1(std::move(mf));  // konstruktor kopije premještanjem
    EXPECT_EQ(mf1(), 3);
}


TEST(CopyMove, one_v){
    F1 f;
    MyFunction<int(int)> mf(f);
    MyFunction<int(int)> mf1(std::move(mf));  // konstruktor kopije premještanjem
    EXPECT_EQ(mf1(2), 6);
}

TEST(CopyMove, two_v){
    F2 f;
    MyFunction<int(int,int)> mf(f);
    MyFunction<int(int,int)> mf1(std::move(mf));  // konstruktor kopije premještanjem
    EXPECT_EQ(mf1(1,2), 5);
}

// Operator pridruživanja
TEST(OP, zero_v){
    F0 f;
    MyFunction<int()> mf(f);
    MyFunction<int()> mf1(f0); 
    mf = mf1;  // OP
    EXPECT_EQ(mf(), mf1());
}

TEST(OP, one_v){
    F1 f;
    MyFunction<int(int)> mf(f);
    MyFunction<int(int)> mf1(f1); 
    mf = mf1;  // OP
    EXPECT_EQ(mf(6), mf1(6));
}

TEST(OP, two_v){
    F2 f;
    MyFunction<int(int,int)> mf(f);
    MyFunction<int(int,int)> mf1(f2); 
    mf = mf1;  // OP
    EXPECT_EQ(mf(6,5), mf1(6,5));
}

// Operator pridruživanja premještanjem
TEST(MoveOP, zero_v){
    F0 f;
    MyFunction<int()> mf(f);
    MyFunction<int()> mf1(f0); 
    mf = std::move(mf1);  // OP
    EXPECT_EQ(mf(), 3);
}

TEST(MoveOP, one_v){
    F1 f;
    MyFunction<int(int)> mf(f);
    MyFunction<int(int)> mf1(f1); 
    mf = std::move(mf1);  // OP
    EXPECT_EQ(mf(6), 18);
}

TEST(MoveOP, two_v){
    F2 f;
    MyFunction<int(int,int)> mf(f);
    MyFunction<int(int,int)> mf1(f2); 
    mf = std::move(mf1);  // OP
    EXPECT_EQ(mf(6,5), 23);
}

// Smještanje u vektor
TEST(Vect, one){
    std::vector< MyFunction<int(int)> > vec;
    vec.push_back(f1);
    F1 fo;
    vec.push_back(fo);
    auto f = [](int x){ return 3*x; };
    vec.push_back(f);
    EXPECT_EQ(vec[0](2), 6);
    EXPECT_EQ(vec[1](2), 6);
    EXPECT_EQ(vec[2](2), 6);
}


// Varijabla tipa reference
void f3(int x, double & y, std::string s) { y = y+1.0; }

TEST(VoidF, three){
     MyFunction<void(int,double &, std::string)> mf(f3);
     double y = 1.0;
     mf(1,y,"..");
     EXPECT_EQ(y,2.0);
}
