#include "stats.h"
#include <math.h>



stats::stats(PNG & im){

    for (int j = 0; j < im.height(); j++){
        sumRed.push_back(vector<long>());
        sumBlue.push_back(vector<long>());
        sumGreen.push_back(vector<long>());

        sumsqRed.push_back(vector<long>());
        sumsqBlue.push_back(vector<long>());
        sumsqGreen.push_back(vector<long>());
        for (int i = 0; i < im.width(); i++){
            if (j == 0 && i == 0){
                sumRed[j].push_back(im.getPixel(i, j) -> r);
                sumGreen[j].push_back(im.getPixel(i, j) -> g);
                sumBlue[j].push_back(im.getPixel(i, j) -> b);

                sumsqRed[j].push_back(pow(im.getPixel(i,j) -> r, 2));
                sumsqGreen[j].push_back(pow(im.getPixel(i,j) -> g, 2));
                sumsqBlue[j].push_back(pow(im.getPixel(i,j) -> b, 2));

            } else if (j == 0){
                sumRed[j].push_back(im.getPixel(i, j) -> r + sumRed[j][i-1]);
                sumGreen[j].push_back(im.getPixel(i, j) -> g + sumGreen[j][i-1]);
                sumBlue[j].push_back(im.getPixel(i, j) -> b + sumBlue[j][i-1]);

                sumsqRed[j].push_back(pow(im.getPixel(i, j) -> r, 2) + sumsqRed[j][i-1]);
                sumsqGreen[j].push_back(pow(im.getPixel(i, j) -> g, 2) + sumsqGreen[j][i-1]);
                sumsqBlue[j].push_back(pow(im.getPixel(i, j) -> b, 2) + sumsqBlue[j][i-1]);

            } else if (i == 0){
                sumRed[j].push_back(im.getPixel(i, j) -> r + sumRed[j-1][i]);
                sumGreen[j].push_back(im.getPixel(i, j) -> g + sumGreen[j-1][i]);
                sumBlue[j].push_back(im.getPixel(i, j) -> b + sumBlue[j-1][i]);

                sumsqRed[j].push_back(pow(im.getPixel(i, j) -> r, 2) + sumsqRed[j-1][i]);
                sumsqGreen[j].push_back(pow(im.getPixel(i, j) -> g, 2) + sumsqGreen[j-1][i]);
                sumsqBlue[j].push_back(pow(im.getPixel(i, j) -> b, 2) + sumsqBlue[j-1][i]);

            } else {
                sumRed[j].push_back(im.getPixel(i, j) -> r + sumRed[j-1][i] + sumRed[j][i-1] - sumRed[j-1][i-1]);
                sumGreen[j].push_back(im.getPixel(i, j) -> g + sumGreen[j-1][i] + sumGreen[j][i-1] - sumGreen[j-1][i-1]);
                sumBlue[j].push_back(im.getPixel(i, j) -> b + sumBlue[j-1][i] + sumBlue[j][i-1] - sumBlue[j-1][i-1]);

                sumsqRed[j].push_back(pow(im.getPixel(i, j) -> r, 2) + sumsqRed[j-1][i] + sumsqRed[j][i-1] - sumsqRed[j-1][i-1]);
                sumsqGreen[j].push_back(pow(im.getPixel(i, j) -> g, 2) + sumsqGreen[j-1][i] + sumsqGreen[j][i-1] - sumsqGreen[j-1][i-1]);
                sumsqBlue[j].push_back(pow(im.getPixel(i, j) -> b, 2) + sumsqBlue[j-1][i] + sumsqBlue[j][i-1] - sumsqBlue[j-1][i-1]);
            }
        }
    }
}

long stats::getSum(char channel, pair<int,int> ul, pair<int,int> lr){

    //std::cout << "Reached Sum 59" << std::endl;

    if(ul.first == 0 && ul.second == 0){
        if (channel == 'r') return sumRed[lr.second][lr.first];
        if (channel == 'g') return sumGreen[lr.second][lr.first];
        if (channel == 'b') return sumBlue[lr.second][lr.first];
    } else if (ul.second == 0){
        if (channel == 'r') return sumRed[lr.second][lr.first] - sumRed[lr.second][ul.first - 1];
        if (channel == 'g') return sumGreen[lr.second][lr.first] - sumGreen[lr.second][ul.first - 1];
        if (channel == 'b') return sumBlue[lr.second][lr.first] - sumBlue[lr.second][ul.first - 1];
    } else if (ul.first == 0){
        if (channel == 'r') return sumRed[lr.second][lr.first] - sumRed[ul.second - 1][lr.first];
        if (channel == 'g') return sumGreen[lr.second][lr.first] - sumGreen[ul.second - 1][lr.first];
        if (channel == 'b') return sumBlue[lr.second][lr.first] - sumBlue[ul.second - 1][lr.first];
    } else {
        if (channel == 'r') return sumRed[lr.second][lr.first] - sumRed[ul.second - 1][lr.first] - sumRed[lr.second][ul.first - 1] + sumRed[ul.second - 1][ul.first - 1];
        if (channel == 'g') return sumGreen[lr.second][lr.first] - sumGreen[ul.second - 1][lr.first] - sumGreen[lr.second][ul.first - 1] + sumGreen[ul.second - 1][ul.first - 1];
        if (channel == 'b') return sumBlue[lr.second][lr.first] - sumBlue[ul.second - 1][lr.first] - sumBlue[lr.second][ul.first - 1] + sumBlue[ul.second - 1][ul.first - 1];
    }

}

long stats::getSumSq(char channel, pair<int,int> ul, pair<int,int> lr){

    if(ul.first == 0 && ul.second == 0){
        if (channel == 'r') return sumsqRed[lr.second][lr.first];
        if (channel == 'g') return sumsqGreen[lr.second][lr.first];
        if (channel == 'b') return sumsqBlue[lr.second][lr.first];
    }  else if (ul.second == 0){
        if (channel == 'r') return sumsqRed[lr.second][lr.first] - sumsqRed[lr.second][ul.first - 1];
        if (channel == 'g') return sumsqGreen[lr.second][lr.first] - sumsqGreen[lr.second][ul.first - 1];
        if (channel == 'b') return sumsqBlue[lr.second][lr.first] - sumsqBlue[lr.second][ul.first - 1];
    } else if (ul.first == 0){
        if (channel == 'r') return sumsqRed[lr.second][lr.first] - sumsqRed[ul.second - 1][lr.first];
        if (channel == 'g') return sumsqGreen[lr.second][lr.first] - sumsqGreen[ul.second - 1][lr.first];
        if (channel == 'b') return sumsqBlue[lr.second][lr.first] - sumsqBlue[ul.second - 1][lr.first];
    } else {
        if (channel == 'r') return sumsqRed[lr.second][lr.first] - sumsqRed[ul.second - 1][lr.first] - sumsqRed[lr.second][ul.first - 1] + sumsqRed[ul.second - 1][ul.first - 1];
        if (channel == 'g') return sumsqGreen[lr.second][lr.first] - sumsqGreen[ul.second - 1][lr.first] - sumsqGreen[lr.second][ul.first - 1] + sumsqGreen[ul.second - 1][ul.first - 1];
        if (channel == 'b') return sumsqBlue[lr.second][lr.first] - sumsqBlue[ul.second - 1][lr.first] - sumsqBlue[lr.second][ul.first - 1] + sumsqBlue[ul.second - 1][ul.first - 1];
    }


}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
    return (lr.first + 1 - ul.first)*(lr.second + 1 - ul.second);
}

RGBAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
    //std::cout << "Reached AVG 112" << std::endl;
    long red = getSum('r', ul, lr)/rectArea(ul, lr);
    long blue = getSum('b', ul, lr)/rectArea(ul, lr);
    long green = getSum('g', ul, lr)/rectArea(ul, lr);
    return RGBAPixel(red, green, blue);
}

long stats::getScore(pair<int,int> ul, pair<int,int> lr){
    long redScore = getSumSq('r', ul, lr) - pow(getSum('r', ul, lr), 2)/rectArea(ul, lr);
    long blueScore = getSumSq('b', ul, lr) - pow(getSum('b', ul, lr), 2)/rectArea(ul, lr);
    long greenScore = getSumSq('g', ul, lr) - pow(getSum('g', ul, lr), 2)/rectArea(ul, lr);


    return redScore + greenScore + blueScore;
}
