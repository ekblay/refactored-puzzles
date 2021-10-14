//#This comes from 3.1.4-new
#include<mpi.h>
#include<iostream>
#include<fstream>
#include<stdio.h>
#include<string>
#include<stdlib.h>
#include<math.h>
#include"dynarray1.cpp"
#define S_DEBUG 1
#include<unistd.h>
using namespace std;
mList<int> b;
mList<int> newb,temp1,h,PRESENT,d,states;
mList<char *> labels,transition,buf2; char buf[500],temp[299];
int CC;
int D;
int acountx,countx,ccc=0,k=0,i=0,change,change1;
int p,q,Z,B,X,jj,t,m,n,M,nn;
int indexx;
float r;
int main(int argc, char *argv[])

{	

	int numprocs,rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;
	char greeting[MPI_MAX_PROCESSOR_NAME + 80];
	
       char identity[50];
      gethostname(identity,50);
       cout<<endl<<"This is from process "<<rank<<", executing on "<<identity<<"."<<endl;
if(rank==0)//If rank=0 means the server or master will execute the code in this loop. The server sends data to
                    //the other processors.
	{  
	double start_time = MPI_Wtime();
	cout<<"start time"<<start_time<<endl;
	  // cout<<" This is from rank 0";
	               countx=0;
			acountx=0;	
			for(int bbb=0;bbb<20000;bbb++)
			{
				states[bbb]=-1;
			}  
			//ifstream inFile("par8.txt");
			ifstream inFile(argv[argc-1]);
			while(inFile)
			{       inFile.getline(buf,500,'\n');
					// If reading the file is over, then it calculates the block numbers of the 	
					// states, the total number of states, and their transitions and the input 	
					// symbols etc.
					if(!inFile||buf[0]==' '||buf[0]==NULL||buf==NULL)
		   			{
               			        	//assigning block number 2 for non final states.
						 for(int kk=0;kk<countx;kk++)
						{
						    for(jj=0;jj<ccc;jj++)
				                    { 
					             if(kk==d[jj])//d[jj] was calculated while reading the input..There
								  //block number 1 was assigned for final states!	
						     break;
				                    }
				                    if(jj==ccc)
						    b[kk]=2;
						 }
					
					//storing the block numbers of all the states in a temporary array 
					for(int L=0;L<countx;L++)
				         {
				          temp1[L]=b[L];
				         }
				//cout<<"total number of states"<<countx<<endl<<endl;
				//cout<<"Total number of input symbols is"<<acountx<<endl;
				p=ceil((double)log((double)countx)/(double)log((double)2));
				//	p = p*2;
				q=ceil(float(countx)/float(p));
				//cout<<"logn is"<<p<<endl;
				//cout<<"n/logn is"<<q<<endl;//This indicates the numnber of processors required.
					}	
	       			else//If reading the file is not over
					{
					
		    for(indexx=k;indexx<k+3;indexx++)
		    {
			     
				if(indexx==k)//reading the first token
			     {
				strcpy(buf2[indexx],strtok(buf," "));
				strcpy(transition[indexx],buf2[indexx]);
				
				//strcat(transition[indexx],'\0');
				if(strcmp(buf2[indexx],"(START)")!=0)
				{
				    if(states[atoi(buf2[indexx])]==-1)
				    {
				       //storing the states in an array
					states[atoi(buf2[indexx])]=atoi(buf2[indexx]);
				       	countx++;
					
				    }
						

				}
			       }
				else if(indexx==k+1)//Reading the second token					
				{
			           strcpy(buf2[indexx],strtok(NULL," "));
			           strcpy(transition[indexx],buf2[indexx]);
				   if((strcmp(buf2[indexx],"|-")!=0)&&(strcmp(buf2[indexx],"-|")!=0))
				   {
				      if(acountx==0)
				      {
				         strcpy(labels[0],buf2[indexx]);
				         acountx++;
				      }
				      else
				      {
				         for(t=0;t<acountx;t++)
					 {
					   if(strcmp(labels[t],buf2[indexx])==0)
				           break;
					 }
					 if(t==acountx)
					 {
			  			//storing the labels in an array
						strcpy(labels[acountx],buf2[indexx]);                                  
			  			acountx++;
				         }
				      }
				   }

				}
				else//Reading the third token
				{
				     strcpy(buf2[indexx],strtok(NULL," "));
				     strcpy(transition[indexx],buf2[indexx]);
				    
					if(strcmp(buf2[indexx],"(FINAL)")!=0)
				     {
					 if(states[atoi(buf2[indexx])]==-1)
					 {
					    //storing the states in the array
					    states[atoi(buf2[indexx])]=atoi(buf2[indexx]);
					    countx++;
					 } 
							
				     }
				     else
				     {
					//If the third token is "(FINAL)", then assign the corresponding state(first  
					// token) as a final state and assign a block number 1.
					b[atoi(buf2[k])]=1;
					d[ccc]=atoi(buf2[k]);
					ccc++;
				      }
				}
		 			 

			}//end for
		}//end elsei
		  i++;
		  k=k+3;	
		}
			inFile.close();	
		       cout<<"P value "<<p<<endl;
			cout<<"q value "<<q<<endl;
			cout<<"count value "<<countx<<endl;
			cout<<"acount value "<<acountx<<endl;			
			
			
			

int ib[countx*10];

//since we cannot sent dynamically declared arrays from server to other nodes, it is converted into a fixed sized array
for(int k=0;k<countx;k++)
{
   ib[k]=b[k];
}

			//sending the required data to all the nodes	
			for(int j=1;j<q+1;j++)
			{
			MPI_Send(&countx,1,MPI_INT,j,1,MPI_COMM_WORLD);
		 	MPI_Send(&acountx,1,MPI_INT,j,2,MPI_COMM_WORLD);
			MPI_Send(&indexx,1,MPI_INT,j,3,MPI_COMM_WORLD);
			MPI_Send(&p,1,MPI_INT,j,4,MPI_COMM_WORLD);
			MPI_Send(&q,1,MPI_INT,j,5,MPI_COMM_WORLD);
			MPI_Send(&ib[0],countx,MPI_INT,j,17,MPI_COMM_WORLD);
			char cclabels[acountx*10];
			char ctransition[indexx*10];
			for(int l=0;l<acountx;l++)
				{
				strcpy(&cclabels[l*10],labels[l]);	
				///cout<<"cclabel "<<&cclabels[l*10];
				}
			MPI_Send(&cclabels[0],acountx*10,MPI_CHAR,j,7,MPI_COMM_WORLD);	 					for(int ll=0;ll<indexx;ll++)
				{
				strcpy(&ctransition[ll*10],transition[ll]);	
				///cout<<"cclabel "<<&cclabels[l*10];
				
				}
			MPI_Send(&ctransition[0],indexx*10,MPI_CHAR,j,8,MPI_COMM_WORLD);
			}
do
{
for(D=0;D<acountx;D++)
{
  //cout<<"error here"; 
for(int H=1;H<q+1;H++) 
{ 
MPI_Send(&ib[0],countx,MPI_INT,H,6,MPI_COMM_WORLD);//sends the block numbers of all the states
}
int ih[countx*10];
for(int u=1;u<q+1;u++)
		
		{		      
 MPI_Recv(&ih[(u-1)*p],p,MPI_INT,u,9,MPI_COMM_WORLD, &status);//Receives the hash values from all the states.

                }

/*
for(int uu=1;uu<q+1;uu++)
		
		{		
			for(int uuuu=(uu-1)*p;uuuu<uu*p;uuuu++)
			{
				if(uuuu<countx)		
				{
					//cout<<"h[ "<<uuuu<<" ]is "<<ih[uuuu]<<endl;
				}
			}
	
		}
*/
for(int UU=1;UU<q+1;UU++)

{
		MPI_Send(&ih[0],countx,MPI_INT,UU,10,MPI_COMM_WORLD);//sending all the hash values to all nodes
}
 //finding the biggest hash value.
		int V=0;
		for(int m=0;m<countx;m++)
		{
		   if(ih[m]>V)
		   V=ih[m];
		}

		B=(V+(countx-(V%countx)))/countx;
		for(int j=1;j<q+1;j++)
			{
				MPI_Send(&B,1,MPI_INT,j,11,MPI_COMM_WORLD);//sending the biggest hash value

			}
			


int inum[q*10];
			for(int P=1;P<q+1;P++)
			{
			//receiving the information of number of 1's in each processor range 
			MPI_Recv(&inum[P],1,MPI_INT,P,12,MPI_COMM_WORLD, &status);
											
			}

			int is[q*10];	
			is[0]=0;
			for(int O=1;O<q+1;O++)
			{
			is[O]=is[O-1]+inum[O];
			MPI_Send(&is[O-1],1,MPI_INT,O,13,MPI_COMM_WORLD);//sending the partial sums value
			}
			
int inewb[q*B*p*10];
for(int Q=1;Q<q+1;Q++)
{
	MPI_Recv(&inewb[((Q-1)*B*p)+1],B*p,MPI_INT,Q,14,MPI_COMM_WORLD, &status);//Receiving the newblock numbers	
	
}
/*
for(int i=1;i<q*B*p+1;i++)
{
  //cout<<"inewb[ "<<i<<" ] is "<<inewb[i]<<endl;
} 
*/
for(int kkkk=1;kkkk<q+1;kkkk++)
{
	MPI_Send(&inewb[1],q*B*p,MPI_INT,kkkk,15,MPI_COMM_WORLD);//sending all the newblock numbers to all the nodes
}
//int iib[countx*10];
for(int G=1;G<q+1;G++)
{
MPI_Recv(&ib[(G-1)*p],p,MPI_INT,G,16,MPI_COMM_WORLD,& status);//Receiving updated block numbers
}
for(int II=0;II<countx;II++)
{
	
//	cout<<"Ib["<<II<<"] ="<<ib[II]<<endl;
}
}//end of D

//cheking whether the received block numbers are same as the original block numbers(temp1)or not
change=0;
	
		for(int i=0;i<countx;i++)
		 {
			if(temp1[i]!=ib[i])
			{
			change=1;
			}
		  temp1[i]=ib[i];//if they are not same, temp1 is updated with the newblock numbers
		}
	   

}//end of do	 
     while(change==1);



//cout<<endl<<endl;
	 
	 //printing the resultant block numbers
int f[countx];	
//for(int i=0;i<countx;i++)
	 //cout<<"\t b["<<i<<" ]  =  "<<temp1[i]<<endl;
	 //getch();
	 //printing the equivalent states
        	 
for(int y=0;y<countx;y++)
	  {
	   

	    //cout<<"entered the for loop"<<endl;
	    int nn;
	   f[0]=y;
		nn=1;
		for(int j=y+1;j<countx;j++)
		 {
		   if(temp1[y]==temp1[j])
		   {
				f[nn]=j;
				nn=nn+1;
		   }
		 }
		 if(nn>1)
		 {
		 for(int k=0;k<nn;k++)
		 {
		  //cout<<f[k]<<", ";
		 }
		 //cout<<" "<<"are equivalent states"<<endl;
                

		 //cout<<"printing the resultant automata"<<endl;
		 for(int e=1;e<nn;e++)
		 {
		    for(int k=3;k<indexx;k=k+3)
		   {
			int ss= atoi(transition[k]);
			if(f[e]==ss)
			strcpy(transition[k],"-1");
		   }
		 }
			for(int g=1;g<nn;g++)
		 {
		   for(int k=2;k<indexx;k=k+3)
		  {
			if(strcmp(transition[k],"(FINAL)")!=0)
			{
		        if(atoi(transition[k])==f[g])
			//itoa(f[0],transition[k],10);
              			//strcpy(transition[k],itoa(f[0]));
				sprintf(transition[k],"%d",f[0]);			
                         }
		  }
		 
 		  }

		 }//end of if
	   }
	   
//printing
//cout<<"This is at printing"<<endl;
int gg=0;
	   for(int ff=0;ff<indexx;ff++)
	   {
	   if(ff>2)
		   {
		   if(strcmp(transition[ff],"-1")!=0 &&strcmp(transition[ff-1],"-1")!=0 &&strcmp(transition[ff-2],"-1")!=0)
		   {
			if((ff%3)==0)
		   //cout<<endl<<"\t \t";
		   cout<<"\n";
			if(ff%3!=1&&strcmp(transition[ff],"(FINAL)")!=0)
			cout<<atoi(transition[ff]);
			else
			cout<<transition[ff];
		   }
		   cout<<" ";
		   }
		   else
		   {
                   		   

		   if(ff%3!=0)
		   cout<<" ";
		//else
                 //cout<<"\t \t";	
		if(ff!=2)		   
		cout<<transition[ff];
		else
                cout<<atoi(transition[ff]);  
}
	   }

double end_time = MPI_Wtime();
double diff = end_time - start_time;
cout<<"time taken to calculate for rank "<<rank<<"is "<<diff<<endl;
//cout<<endl;
}//end of rank0
	else//if rank not equal to 0
	{
                	 
	  cout<<"This is at rank "<<rank<<endl; 
			int dcountx,dacountx,dindexx,dp,dq;        
                	
			//Receiving the required data from rank0


			MPI_Recv(&dcountx,1,MPI_INT,0,1,MPI_COMM_WORLD, &status);
			//	cout<<" total number of states"<<dcountx<<endl;           
			MPI_Recv(&dacountx,1,MPI_INT,0,2,MPI_COMM_WORLD, &status);
			//cout<<" total number of labels"<<dacountx<<endl;
			MPI_Recv(&dindexx,1,MPI_INT,0,3,MPI_COMM_WORLD, &status);
			//cout<<"index value is"<<dindexx<<endl;
			MPI_Recv(&dp,1,MPI_INT,0,4,MPI_COMM_WORLD, &status);
			int cb[dcountx];
			
			
			int dstates[dp*10];
			
			int db[dcountx*10];
			MPI_Recv(&dq,1,MPI_INT,0,5,MPI_COMM_WORLD, &status);
			MPI_Recv(&cb[0],dcountx,MPI_INT,0,17,MPI_COMM_WORLD, &status);
			for(int pppp=(rank-1)*dp;pppp<rank*dp;pppp++)
			{
				if(pppp<dcountx)		
				{
						
				     	dstates[pppp]=pppp;
					//cout<<"dstates["<<pppp<<" ]"<<dstates[pppp]<<"\t";
				}
			}
			
			char dlabels[dacountx*10];
			char dtransition[dindexx*10];
			MPI_Recv(&dlabels[0],dacountx*10, MPI_CHAR, 0, 7, MPI_COMM_WORLD, &status);
			MPI_Recv(&dtransition[0],dindexx*10, MPI_CHAR, 0, 8, MPI_COMM_WORLD, &status);
			for(int l=0;l<dacountx;l++)
					{
					for(int z=0;z<10;z++)
					{
					labels[l][z]=dlabels[l*10+z];
					}
					}			

			/*for(int ffff=0;ffff<dacountx;ffff++)
			  {cout<<" label[ "<<ffff<<" ] is "<<labels[ffff][0]<<endl;}*/
			for(int ll=0;ll<dindexx;ll++)
					{
					for(int zz=0;zz<10;zz++)
					{
					transition[ll][zz]=dtransition[ll*10+zz];
					}
					}

			/* for(int gggg=0;gggg<dindexx;gggg++)
			   cout<<" transition[ "<<gggg<<" ] is "<<transition[gggg]<<endl;*/
			
do
{
for(CC=0;CC<dacountx;CC++)
{
	MPI_Recv(&db[0],dcountx,MPI_INT,0,6,MPI_COMM_WORLD, &status);
 		
 			for(int S=0;S<dcountx;S++)
                        {
			b[S]=db[S];
			//	cout<<"b[ "<<S<<"] "<<b[S]<<endl;
			}
			
		for(int m=(rank-1)*dp;m<rank*dp;m++)
		{
                  int b1,b2;
		  	if(m<dcountx)
		  {
			int RRR=0;
			for(int k=3;k<dindexx;k=k+3)
			{
	           //The following if condition checks whether the particular state has a transition with a particular
				 //input symbol....
				
                  if(atoi(transition[k])==dstates[m] && strcmp(transition[k+1],labels[CC])==0)			
				{
			  	//cout<<"labels at rank"<<rank<<"is"<<labels[0];
					//finding the block numbers if there is a transition 
			  b1=b[m];
			  b2=b[atoi(transition[k+2])];
			
			//finding the hash values
			h[m]=b1*(dcountx+1)+b2;
			
			//cout<<"hash of"<<m<<"is"<<h[m]<<"and rank is"<<rank<<"\t";
			 RRR++;
			 
			}
			
			}
			  //finding the block numbers if there is no transition from the particular state with a 
                          //particular input symbol.
			if(RRR==0)
			{
			 b1=b[m];
			 b2=(dcountx+1);
			 //finding the hash values
			 h[m]=b1*(dcountx+1)+b2;
			 //cout<<"hash of"<<m<<"is"<<h[m]<<"\t";
			 }//end of if
		  }//end of if
 }//end of for m


int dh[dcountx*10];

for(int mmmm=(rank-1)*dp;mmmm<rank*dp;mmmm++)
{
if(mmmm<dcountx)		
{
dh[mmmm]=h[mmmm];	
}
}
MPI_Send(&dh[(rank-1)*dp],dp,MPI_INT,0,9,MPI_COMM_WORLD);//sending the calculated hash values to rank0
MPI_Recv(&dh[0],dcountx,MPI_INT,0,10,MPI_COMM_WORLD, &status);//receiving the hash values of other nodes also!
 for(int M=0;M<dcountx;M++)
{
	//cout<<"dh["<<M<<" ] ="<<dh[M]<<"\t";
}
			
int dB;
MPI_Recv(&dB,1,MPI_INT,0,11,MPI_COMM_WORLD, &status);
//cout<<"K value is "<<dB;

for(int f=((rank-1)*dB*dp)+1;f<(rank*dB*dp)+1;f++)//Initializing the present array
		
{
		PRESENT[f]=0;	
		for(int M=0;M<dcountx;M++)  
		{
		 	//cout<<" h["<<M<<"is ]"<<dh{M]<<"and f is "<<f<<endl;
			if(f==dh[M])
		 	{
		 		PRESENT[f]=1;
	   			//cout<<"present["<<f<<"]"<<PRESENT[f]<<"at rank"<<rank<<endl;
		 		break;
		 	}
		 
		 }
		 		
		 
	      }
		//finding the number of 1's in each processor's range and sending the value to rank0
		int num[10];
                 num[rank]=0;
		 for(int g=((rank-1)*dB*dp)+1;g<(rank*dB*dp)+1;g++)
		 {
		 	if(PRESENT[g]==1)
			{
			//cout<<"present in the loop[ "<<g<<" ] = "<<PRESENT[g]<<"at rank"<<rank<<"\t";
			}
			num[rank]=num[rank]+PRESENT[g];		   
		 }
			//cout<<"num["<<rank<<"] is "<<num[rank]<<endl<<endl;
			MPI_Send(&num[rank],1,MPI_INT,0,12,MPI_COMM_WORLD);
			
			//Receving the partial sums
			//int A=0;	
			int s[10];
			//int anewb[dB*dp];
			MPI_Recv(&s[rank-1],1,MPI_INT,0,13,MPI_COMM_WORLD, &status);
			num[rank]=s[rank-1];
		
			//finding the new block numbers

			for(int o=((rank-1)*dB*dp)+1;o<(rank*dB*dp)+1;o++)
		 {
		    num[rank]=num[rank]+PRESENT[o];
		    if(PRESENT[o]==1)
		   {
			newb[o]=num[rank];
		    //    cout<<"newb[ "<<o<<" ] is "<<newb[o]<<endl<<endl; 
			//   anewb[A]=o;
			 //A++;	
		   }
		else
		newb[o]=0; 
		}
		
			//Sending the newb value to rank0

			int dnewb[dq*dB*dp*10];
	for(int kkkk=((rank-1)*dB*dp)+1;kkkk<(rank*dB*dp)+1;kkkk++)
	{
		dnewb[kkkk]=newb[kkkk];
	}
/*for(int KKKK=((rank-1)*dB*dp+1);KKKK<(rank*dB*dp)+1;KKKK++)
{
	cout<<"dnewb[ "<<KKKK<<"] is "<<dnewb[KKKK]<<endl;
}*/
 
MPI_Send(&dnewb[((rank-1)*dB*dp)+1],dB*dp,MPI_INT,0,14,MPI_COMM_WORLD);
		 
MPI_Recv(&dnewb[1],dq*dB*dp,MPI_INT,0,15,MPI_COMM_WORLD, &status);//receiving all the newb values from rank0		
for(int KKKK=1;KKKK<(dq*dB*dp)+1;KKKK++)
{
	//cout<<"dnewb[ "<<KKKK<<"] is "<<dnewb[KKKK]<<endl;
}
	

	//Updatinig the new block numbers
		
		for(int E=(rank-1)*dp;E<rank*dp;E++)
		{
			if(E<dcountx)
			{
		
			b[E]=dnewb[dh[E]];
			//cout<<" b["<<E<<"] is "<<b[E]<<"for input symbol"<<labels[CC]<<endl;
			}
		}
    
int ddb[dcountx*10];
for(int k=(rank-1)*dp;k<rank*dp;k++)
{
if(k<dcountx)   
{
ddb[k]=b[k];
//cout<<"ddb[ "<<k<<"] ="<<ddb[k]<<endl;
}
}

        MPI_Send(&ddb[(rank-1)*dp],dp,MPI_INT,0,16,MPI_COMM_WORLD);//sending the block numbers to rank0

		

}

                     //checking whether the block numbers are same as the previous one or not
change1=0;
	
		for(int i=0;i<dcountx;i++)
		 {
			if(cb[i]!=b[i])
			{
			change1=1;
			}
		  cb[i]=b[i];
		}
//cout<<"hai";
}//end of do

    while(change1==1);
//}
//int ddb1[dcountx];
//for(int k=(rank-1)*dp;k<rank*dp;k++)
//{
//if(k<dcountx)   
//{
//ddb1[k]=b[k];
//cout<<"block_number_of[ "<<k<<"] ="<<ddb1[k]<<"  (calculated by process "<<rank<<")"<<endl;
 //}
 //}


MPI_Finalize();        
						
	}

return 0;

}
//end of main..
