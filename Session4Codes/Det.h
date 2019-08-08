#ifndef DET_H
#define DET_H

// VERSION 2 Corrected time function type

#include "TRandom.h"
#include <iostream>

//.L Det.h+
// gInterpreter->GenerateDictionary("vector<Track&gt","Track.h;vector");
// gInterpreter->GenerateDictionary("myclass","myheader.h");


// Calibration parameters
// Wanted to use a class static, but conflicts with trees and simple dictionary use
std::vector<std::vector<double>> gCalPar;
std::vector<double> gTheta;
double gBeta;

void SetCal(unsigned int chan,double p0,double p1,double p2=0,double p3=0){
    while(gCalPar.size()<=chan)gCalPar.push_back(std::vector<double>(4,0));
    gCalPar[chan][0]=p0;
    gCalPar[chan][1]=p1;
    gCalPar[chan][2]=p2;
    gCalPar[chan][3]=p3;
} 

void SetTheta(unsigned int chan,double t){
    while(gTheta.size()<=chan)gTheta.push_back(0);
    gTheta[chan]=t;
} 


class Hit {
    
private:
    unsigned short int ID;
    long TimeStamp;
    int Charge;
    mutable double Energy;//!<!   
    mutable bool Eset; 
    
    double Calibrate(unsigned int id, int charge) const{
        if(id>=gCalPar.size())return 0;
        charge+=gRandom->Uniform();
        double cal_chg = gCalPar[id][0];
        for(size_t i = 1; i < gCalPar[id].size(); i++) {
            cal_chg += gCalPar[id][i] * pow((charge), i);
        }
        return cal_chg;
    }    
    
    
public:
    Hit(int id=0,long t=0,int c=0){
        ID=id;
        TimeStamp=t;
        Charge=c;
        Eset=false;
    };
    void Copy(Hit& rhs) const
    {
        rhs.TimeStamp=TimeStamp;
        rhs.Charge=Charge;
        rhs.Energy=Energy;
        rhs.ID=ID;
        rhs.Eset=false;
    }
    Hit(const Hit& rhs){
        rhs.Copy(*this);
    }
    ~Hit(){};

    int GetCharge() const { return Charge;}
    long GetTimeStamp() const { return TimeStamp;}
    long GetTime() const { return TimeStamp;}
    unsigned short int GetID() const { return ID;}

    double GetEnergy() const {
        if(Eset)return Energy;
        Energy=Calibrate(ID,Charge);
        Eset=true;
        return Energy;
    }

    double GetDopplerEnergy() {
        double gammma = 1/(sqrt(1-pow(gBeta,2)));
        return GetEnergy()*gammma *(1 - gBeta*TMath::Cos(gTheta[ID]));
    }
    
   ClassDef(Hit, 1)
};



class Event {
private:
   std::vector< Hit > Hits;
    
public:
   Event(){};
   Event(Event& rhs){
       rhs.Hits=Hits;
       }
   ~Event(){};
   
   int Size(){return Hits.size();}
   
   void AddHit(Hit& hit){
       Hits.push_back(hit);
    }
    Hit* GetHit(int i){
        if(i<Size())return &Hits[i];
        std::cout<<"Hit out of range"<<std::endl;
        return 0;
    }
    
    void Clear(){Hits.clear();}

   ClassDef(Event, 1)
};

#endif
