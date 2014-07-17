//
//  ImageCollection.h
//  ccmd
//
//  Created by Chris Rennick on 21/05/2013.
//
//

#ifndef __ccmd__ImageCollection__
#define __ccmd__ImageCollection__

#include "ccmd_sim.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>

#include <boost/thread.hpp>

class Hist3D;
class Vector3D;


class ImageCollection {
public:
    ImageCollection(const double binSize);
    ~ImageCollection();
    
    void addIon(const std::string& name, const Vector3D& r);
    void writeFiles(std::string const& basePath, Microscope_params& p) const;
private:
    typedef std::map<std::string, Hist3D*> Collection;
    Collection collection;
    double binSize;
    
    friend class ImageWorker;
};

class ImageWorker
{
public:
    ImageWorker(std::string const& basePath, ImageCollection::Collection::const_iterator const& it,
                Microscope_params& p);
    void join();
    
private:
    friend class ImageCollection;
    std::string basePath;
    ImageCollection::Collection::iterator it;
    boost::thread m_Thread;
    std::string fileName;
    Hist3D* pIonHist;
    Microscope_params& params;
    
    void generateAndSave();
    
    static double stopWatchTimer() {
        static clock_t start = clock();
        double time_elapsed;
        time_elapsed = ( std::clock() - start )/static_cast<double>(CLOCKS_PER_SEC);
        return time_elapsed;
    };
    
    static void printProgBar( int percent ){
        std::string bar;
        for(int i = 0; i < 50; i++){
            if( i < (percent/2)){
                bar.replace(i,1,"=");
            }else if( i == (percent/2)){
                bar.replace(i,1,">");
            }else{
                bar.replace(i,1," ");
            }
        }
        std::cout << '\r' << "[" << bar << "] ";
        std::cout.width( 3 );
        std::cout<< percent << "%     " << std::flush;
    }
};



#endif /* defined(__ccmd__ImageCollection__) */
