#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<iterator>
#include<algorithm>
#include<math.h>
using namespace std;

typedef vector<double> LINE;
vector<vector<double> > KeyPoints;
vector<vector<int> > neigh;
vector<int> core;
vector<int> id;
vector<int> noise;
vector<int> border;
vector<int> visited;
vector<vector <int> >ith_cluster;
double sse_calc(vector<int>clust)
{
	vector<double> mean;
	double sum=0.0;
	int x=KeyPoints[0].size();
	for(int i=0;i<x;i++)
	{
		for(int j=0;j<clust.size();j++)
		{    
		    double a=(double)KeyPoints[clust.at(j)].at(i);
		   	sum+=a;
		}
		double size=clust.size();
		double m=sum/(size);
		sum=0.0;
		mean.push_back(m);
	}
	double sse=0;
	for(int i=0;i<x;i++)
	{
		for(int j=0;j<clust.size();j++)
		{
			sse+=pow((KeyPoints[clust.at(j)].at(i)-mean.at(i)),2);
		}
	}
	return sse;
}
int core_belong(int y)
{
	int ret=0;
	for(int i=0;i<core.size();i++)
	{
		if(y==core.at(i))
		{
			ret=1;
			break;
		}
			
	}
	return ret;
}
int noise_belong(int y)
{
	int ret=0;
	for(int i=0;i<noise.size();i++)
	{
		if(y==noise.at(i))
		{
			ret=1;
			break;
		}
			
	}
	return ret;
}
void density_connected(int cor,int cid)
{    
        int neigh_count=neigh.at(cor).size();
		for(int j = 0; j <neigh_count ; j++)
    	{   
    		int y=neigh.at(cor).at(j);
    		if(id[y]==0)
    		id.at(y)=cid;
    		for(int i=0;i<core.size();i++)
			{
				if(y==core.at(i) && visited[y]==0)
				{
					visited[y]=1;
					density_connected(y,cid);		
				}
			
			}

    	
		}
}
vector<int> neigh_calc(vector<double>keypoint, double eps)
{
	float dist,sum=0.0;
	vector<int> retKeys;
	
    for (int i = 0; i < KeyPoints.size(); i++)
	{	
    	vector <double> temp = KeyPoints.at(i);
    	for (int j = 0; j < temp.size(); j++)
    	{
        	sum+=pow((keypoint.at(j)-temp.at(j)),2);
        }
    	dist=sqrt(sum);
    	sum=0.0;
    	if(dist <= eps||dist==0.0f)
   	 	{
        	retKeys.push_back(i);
    	}
	}
	
return retKeys;
}


void DBSCAN_keypoints(double eps, int minPts)
{
	int cid=0;

	int noKeys = KeyPoints.size();
	for(int i = 0; i<noKeys; i++)
	{	
		id.push_back(0);
		visited.push_back(0);
		neigh.push_back(neigh_calc(KeyPoints.at(i),eps));
		
        if(neigh.at(i).size() < minPts)
          {
            
		  } 
        else
        {
			core.push_back(i);
		}
		
		
	}
	
    int corekeys=core.size();
    
	for(int i=0;i<corekeys;i++)
	{   
		if(id[core[i]]==0)
		{
			cid=cid+1;
			id[core[i]]=cid;
			density_connected(core[i],cid);
			
		}
	}
	noKeys = KeyPoints.size();
	vector<double> sses;
	for(int i=0;i<cid;i++)//cid is number of clusters
	{
		vector<int> clustement;
		int ncid=i+1;
		for(int j=0;j<noKeys;j++)
		{    
			if(id[j]==ncid)
				clustement.push_back(j);
		}	
		ith_cluster.push_back(clustement);	
	}
	
	if(cid!=0)
	{
	    cout<<"total clusters"<<ith_cluster.size()<<endl;
		for(int i=0;i<ith_cluster.size();i++)
		{	
			sses.push_back(sse_calc(ith_cluster[i]));
			cout<<"ELEMENT IN CLUSTER "<<i+1<<endl;
			vector<int> temp=ith_cluster.at(i);
			for(int j=0;j<temp.size();j++)
			{
				cout<<temp.at(j)<<",";
			}
			cout<<endl;
		}
		double total_sse=0;
		cout<<"SSE OF THE CLUSTERING IS"<<endl;
		for(int z=0;z<sses.size();z++)
		{
			total_sse+=sses.at(z);
		}
		cout<<total_sse<<endl;
	}
	else
	{
		cout<<"NO CLUSTER FORMED"<<endl;
	}

	for(int i=0;i<KeyPoints.size();i++)
	{
		if(id[i]==0)
		{
			noise.push_back(i);
			
		}
		
	}
	for(int i=0;i<KeyPoints.size();i++)
	{
		if(core_belong(i)!=true&&noise_belong(i)!=true)
		{
			border.push_back(i);
		}
	}
	cout<<"NOISE POINTS"<<endl;
	if(noise.size()>0)
	{
	
		for(int i=0;i<noise.size();i++)
		{
			cout<<noise[i]<<endl;
		
		}
	}
	else
		cout<<"NO NOISE POINTS"<<endl;
	cout<<"BORDER POINTS"<<endl;
	if(border.size()>0)
	{
	
		for(int i=0;i<border.size();i++)
		{	
			cout<<border[i]<<endl;
		}
	}
	else
		cout<<"NO BORDER POINTS"<<endl;
}
int main()
{
 
	ifstream indata;
	ofstream out;
	indata.open("temperature.csv", ios::app);
	string line;
	
	int pos;
	if(!indata.is_open())
	{
		cout << "Failed to open file" << endl;
		return 1;
	}
	int temp=1,i=0;
	while( getline(indata,line) )
	{
		LINE ln;
		while( (pos = line.find(',')) >= 0)
		{
			string field = line.substr(0,pos);
			line = line.substr(pos+1);
			double x=atof(field.c_str());
			ln.push_back(x);
		}
		double x=atof(line.c_str());
		ln.push_back(x);
		KeyPoints.push_back(ln);
	}
	double eps;
	int minPts;
	cout<<"ENTER THE EPSILON RADIUS"<<endl;
	cin>>eps;
	cout<<"ENTER THE MINMUM NUMBER OF POINTS YOU WANT"<<endl;
	cin>>minPts;
     DBSCAN_keypoints(eps, minPts);
	return 0;	
 } 

