//
//  main.cpp
//  DBSCAN
//
//  Created by Tony SHE on 30/4/15.
//  Copyright (c) 2015 Tony Sheh. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <math.h>
#include <queue>
#include <vector>
#include "stdlib.h"
#include"time.h"
using namespace std;
struct Point{
    double x;
    double y;
    int lable;  // -1 unvisited, 0 noise, >0 cluster index
};

double euclidean_distance(Point a, Point b)
{
    double x = a.x-b.x;
    double y = a.y-b.y;
    return sqrt(x*x+y*y);
}

// get neighborhood of point p and add it to neighborhood queue
int region_query( vector<Point> &dataset, int p, queue<int> &neighborhood, double eps)
{
	//int count = 0;
    for (int i = 0; i < dataset.size(); i++) {
		//cout << "regin_query" << count++ << endl;
        if(i!=p){
            int dist = euclidean_distance(dataset[p],dataset[i]);
            if ( dist< eps) {
                neighborhood.push(i);
            }
        }
    }
    return (int)neighborhood.size();
}

// expand cluster formed by p, which works in a way of bfs.
bool expand_cluster( vector<Point> &dataset, int p, int c, double eps, int min_pts){
    queue<int> neighbor_pts;
    dataset[p].lable = c;

    region_query(dataset, p, neighbor_pts, eps);

    while (!neighbor_pts.empty()) {

        int neighbor = neighbor_pts.front();
        queue<int> neighbor_pts1;
        region_query(dataset, neighbor, neighbor_pts1, eps);

        if(neighbor_pts1.size()>=min_pts-1)
        {
            while (!neighbor_pts1.empty()) {

                int pt = neighbor_pts1.front();
                if(dataset[pt].lable ==-1){
                    neighbor_pts.push(pt);
                }
                neighbor_pts1.pop();
            }
        }
        dataset[neighbor].lable = c;
        neighbor_pts.pop();

    }
    return  true;
}

// doing dbscan, given radius and minimum number of neigborhoods.
int dbscan(vector<Point> &dataset, double eps, int min_pts)
{
    int c = 0;  // cluster lable
	//int count = 0;
    for (int p = 0; p<dataset.size(); p++) {
        queue<int> neighborhood;
		//cout << count++ << endl;
        region_query(dataset, p, neighborhood, eps);

        if (neighborhood.size()+1 < min_pts) {
            // mark as noise
            dataset[p].lable = 0;
        }else
        {

            if(dataset[p].lable==-1){
                 c++;
                expand_cluster(dataset, p,c, eps, min_pts);
            }
        }
    }
    return c;

}

int main(int argc, const char * argv[]) {
	clock_t start, finish;
	double duration;
	start= clock();
    vector<Point> dataset;
    ifstream in("./3spiral.txt");
    int i=0;
    string line;
    if(in){
        float x , y;
        while (in>>x>>y) {
            Point p;
            p.x = x;
            p.y = y;
            p.lable = -1;
            dataset.push_back(p);
        }
    }else{
        cout<<"No such file"<<endl;
    }

   int c =  dbscan(dataset,2, 3);
    cout<<" （x, y）  cluster"<<endl;
    for (int i = 0; i<dataset.size(); i++) {
        cout<<"("<<dataset[i].x<<","<<dataset[i].y<<") "<<dataset[i].lable<<endl;
    }
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << duration<<"s"<<endl;
	system("PAUSE ");
    return 0;
}
