//Navpreet Kaur. Student ID V0082334
//The objective of this program is to find minimum dominating set for a graph.
//This code is written on the algorithim designed by Dr. Wendy Myrvold.

#define MAIN 1
#include "header.h"
#define DEBUG 0
using namespace std;

int main()
{
	int G[NMAX][MMAX]; //array to store read graph.
	int dominated[MMAX]; //dominated array store the vertices which are dominated. 
    int dom_set[MMAX];  //dom_set store the partial dominating set of graph.
	int min_dom_set[MMAX]; //min_dom_set store the minimum dominating set found so far in graph. 
	int num_choice[NMAX]; //num_choice is the maximum choice for a vertex to be dominated if all white vertices are red.
    int n,m,n_dominated,min_size,size,delta,graph_number=0; //n_dominated is the dominated vertices of graph. delta is the maximum degree of vertex of graph.
	                                                       //min_size stores the size of minimum dominating set. size stores the size of partial dominating set.
    
	while(read_graph(&n,&m,G))
	{
	   graph_number++;
	   printf("\n %3d %3d \n",graph_number,n);
       find_dom_set(0, n, m, G, dominated,dom_set,min_dom_set,num_choice,&min_size,&delta,&size); // This function have logic of finding dominating set of graph.
       print_ver(1,&min_size,n,min_dom_set); // 1 status shows the best dominating set of graph.
	}
}
int read_graph(int *n, int *m, int G[NMAX][MMAX]) //This function is used to read graphs from text file.
{
 	int i, j, u, d;

    if (scanf("%d", n)!= 1) return(0);
    if(*n > NMAX)                       //If number of vertices is greater than NMAX message will be displayed on screen.
     {
     	printf("\n");
     	printf("Increase NMAX and recomplie");
     	return(0);
     }
    *m= (*n+31)/32;
    
    for (i=0; i < *n; i++)
    {
        for (j=0; j < *m; j++)
        {
            G[i][j]= 0;         //  Initialize the graph to have no edges.

        }
    }


    for (i=0; i < *n; i++)
    {
        if (scanf("%d", &d)!=1) return(0);   //  Read degree of graph.

        for (j=0; j < d; j++)
        {
            if (scanf("%d", &u)!=1) return(0);   //read vertices of graph.
            ADD_ELEMENT(G[i], u);               //add the read vertices of graph in array G
            ADD_ELEMENT(G[u], i);
        }
    }
    return(1);
}
//The function find_dom_set contains the logic of finding dominating set for graph.
void find_dom_set(int level, int n, int m, int G[NMAX][MMAX], int dominated[MMAX], int dom_set[MMAX],int min_dom_set[MMAX],int num_choice[NMAX],int *min_size,int *delta,int *size)
{
	int new_dominated[MMAX];   //array is used to find dominated vertices
    int i,u, j,d,del,n_dominated;
    float n_extra,undominated;   
     
	if (level == 0)         //when the level of recursion is 0 all the intialized process is done.
    {
    	n_dominated=0;
    	*min_size=n;
    	*delta=0;
	    n_extra=0;
	    undominated=0;
	    *size=0;
		 for(i=0;i<n;i++)
        {
        	d=set_size(n,G[i]);
        	if(d>*delta)
        	{
        		*delta=d;           //delta stores the maximum degree of vertex of graph.
            }
        }
        
        	
	    // Set diagonal entries of adjacency matrix to 1 to simplify logic.
          
           for (i=0; i < n; i++)
            ADD_ELEMENT(G[i], i);

        //Set the num_choice of graph. 
        for(i=0;i<n;i++)
        {
        	d=set_size(n,G[i]);
        	num_choice[i]=d;
        }
          // Initialize dominated and dom_set to have no vertices.
        for (j=0; j < m; j++)
        {
            dominated[j]=0;
            dom_set[j]=0;
        }
		
    }
    #if DEBUG
     printf("\n");
     printf("\n LEVEL %3d: Data structures.\n",level);
     printf("\n ENTERED NUM_CHOICE :");
     for(i=0;i<n;i++)
     printf("%d",num_choice[i]);
     printf("\n DOMINATING:");
     print_set(n,dominated);
     printf("\n DOM SET :");
     print_set(n,dom_set);
     printf("\n MIN_DOMINATE :");
     print_set(n,min_dom_set);
    #endif
     
     //If num_choice is zero the solution can not be completed so back track to the previous vertex and make it red. 
    for(i=0;i<n;i++)
    {
    	if(num_choice[i]==0)
    	{
    		#if DEBUG
               printf("\n Level %3d: Return because vertex %3d has no choices for being dominated.\n", level, i);
			#endif
			
   		return;
  
    	}
    }
    n_dominated=set_size(n,dominated);  //n_dominated stores the total dominated vertices of graph.
    undominated=(n-n_dominated);        //undominated stores the vertices of graph which are not dominated.
 	del=*delta+1;                       
 	n_extra=undominated/del;             //n_extra calculated the bound on graph.
 	n_extra=ceil(n_extra);               //taking upper bound on n_extra
 	#if DEBUG
 	printf("\n LEVEL %d :",level);
 	printf("N_DOMINATED %d",n_dominated);
    printf("\n MIN_SIZE %d",*min_size);
 	printf("\n UNDOMINATED %f",undominated);
 	printf("\n N_EXTRA %f",n_extra);
 	printf("\n SIZE_DOM_SET %d",*size);
 	printf("\n MIN_SIZE %d",*min_size);
 	#endif
 	if(n_extra+ (*size) >= *min_size)            //if n_extra + size is greater than the min_size found so far that vertex can not be red.
 	{
 		return;
 	}
     if(level==n)                    //if the recursion recahed the n level of graph.
     {
     	if(n_dominated==n)         //n_dominated equals n means are vertices are dominated.
     	{
		 	if(*size<*min_size)   //if size of dominating set found now is better than min_size i.e, size of dominating set found before .New dominating set is accepted.
		  	{
			for(i=0;i<m;i++)
	     	min_dom_set[i] = dom_set[i];     //copy the dom_set to min_dom_set.
	     	*min_size=*size;                 //copy the size into min_size.
	     	print_ver(0,min_size,n,min_dom_set); //the dominating set found now is better dominating set so print it with status 0.
	     	return;
		    }
	  
        }	
 	}
 	else if(undominated==0)       //If level is not n but all the vertices are dominated then check if new dom_set size is better than min_size found so far.
 	 {
 		if(*size<*min_size)
	  	{
		for(i=0;i<m;i++)
     	min_dom_set[i] = dom_set[i]; //copy dom_set to min_dom_set.
     	*min_size=*size;             //set size of min_size
     	print_ver(0,min_size,n,min_dom_set);  //the dominating set found now is better dominating set so print it with status 0.
     	return;
	    }
	  
 		
 	
 }
     
    
    u=level;   //Set u= level.

  for(i=0;i<n;i++)
   {
     if(IS_ELEMENT(G[u],i))
	 num_choice[i]=num_choice[i]-1;	 // Try vertex u as blue so decrementing num_choice of all neighbours v of u in graph G.

   }
   #if DEBUG
     printf("\n");
     printf("\n LEVEL %3d: \n",level);
     printf("\n NUM_CHOICE :");
     for(i=0;i<n;i++)
     printf("%d",num_choice[i]);
    #endif
// u is blue here .Call the routine recursively. 
find_dom_set(level+1,n, m, G, dominated,dom_set,min_dom_set,num_choice,min_size,delta,size);
 
 //num_choice of u is zero so incrementing num_choice of all neighbours v of u recorded in G.
 for(i=0;i<n;i++)
 {
 	if(IS_ELEMENT(G[u],i))
 	num_choice[i]=num_choice[i]+1;
 }
// Try vertex u as red .Add u to dom_set.
 ADD_ELEMENT(dom_set,u);
 *size=*size+1;
 //increase the size according to partial dominating set found so far.
 //new_dominated record the dominating set for vertex u.
   for (j=0; j < m; j++)
    {
        new_dominated[j]= dominated[j] | G[u][j];
    }
 

// u is red here.
find_dom_set(level+1,n, m, G, new_dominated,dom_set,min_dom_set,num_choice,min_size,delta,size);
DEL_ELEMENT(dom_set,u); //making red vertex white and deleteing it from dom_set found so far.
*size=*size-1;           //decrease the size accordingly.
return;
		 
}
int set_size(int n, int set[])  //this function calculate total bits set in array.
{
    int j, m, d;

    m= (n+31)/ 32;

    d=0;

    for (j=0; j < m; j++)
    {
       d+= POP_COUNT(set[j]);
    }
    return(d);
}


void print_set(int n, int set[])  // Prints a set.
{
   int i;

   for (i=0; i < n; i++)
       if (IS_ELEMENT(set, i))
           printf(" %3d", i);
           fflush(stdout);
   printf("\n");
}
int print_ver(int status, int *min_size,int n, int min_dom_set[]) //print the dominating set found so far.
{  
    
	
    printf("%1d %3d ", status, *min_size);
    print_set(n,min_dom_set);
    fflush(stdout);
}

